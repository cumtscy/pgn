#include <iskin.h>
#include <map>
#include <max.h>
#include <MeshNormalSpec.h>
#include <modstack.h>
#include <PGN/Assets/PNM.h>
#include <string>
#include <TextureMapIndexConstants.h>
#include <vector>
#include "About.h"
#include "Utilities.h"

class UV
{
public:
	float u, v;

	UV()
	{
	}

	UV(float u, float v)
		: u(u), v(v)
	{
	}

	UV(Point3& p)
	{
		u = p.x;
		v = p.y;
	}

	bool operator !=(UV& uv)
	{
		return uv.u != u || uv.v != v;
	}
};

class MeshExporter : public SceneExport, public ITreeEnumProc
{
public:
	static const int maxNumVerts = 30000;
	static const int maxNumIndices = 30000;

	Point3* pos;
	Point3* normals;
	unsigned char *numAssignedBones;
	unsigned char (*boneIndices)[4];
	float (*boneWeights)[4];
	unsigned char *mtlIndices;
	UV* tc0;
	UV* tc1;
	int numVerts;

	std::map<Texmap*, std::vector<unsigned short>> vertexIndexArrays;

	std::map<std::wstring, unsigned char> boneIdx;
	static const int maxNumBones = 128;
	Matrix3 offsetMats[maxNumBones];
	Quat defaultRot[maxNumBones];
	Point4 defaultPos[maxNumBones];
	unsigned char parentIndices[maxNumBones];

	std::map<Mtl*, unsigned char> mtlIdx;

	bool exportNormal;
	bool exportMtl;
	bool exportTC0;
	bool exportTC1;

	enum {boneIndexingPass, exportPass} pass;

	MeshExporter();
	~MeshExporter();

	// SceneExport
	virtual int  ExtCount () { return 1; }
	virtual const MCHAR *  Ext (int n) { return L"pnm"; }
	virtual const MCHAR *  LongDesc () { return L"PGN Mesh File"; }
	virtual const MCHAR *  ShortDesc () { return L"PGN Mesh File"; }
	virtual const MCHAR *  AuthorName () { return about.authorName.c_str(); }
	virtual const MCHAR *  CopyrightMessage () { return about.copyright.c_str(); }
	virtual const MCHAR *  OtherMessage1 () { return L"OtherMessage1"; }
	virtual const MCHAR *  OtherMessage2 () { return L"OtherMessage2"; }
	virtual unsigned int  Version () { return about.version; }
	virtual void  ShowAbout (HWND hWnd) { /*MessageBox(hWnd, about.aboutText.c_str(), about.aboutCaption.c_str(), MB_OK);*/ }
	virtual int  DoExport (const MCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options);

	// ITreeEnumProc
	virtual int  callback (INode *node);
};

MeshExporter::MeshExporter()
{
	pos = new Point3[maxNumVerts];
	normals = new Point3[maxNumVerts];
	numAssignedBones = new unsigned char[maxNumVerts];
	boneIndices = new unsigned char[maxNumVerts][4];
	boneWeights = new float[maxNumVerts][4];
	mtlIndices = new unsigned char[maxNumVerts];
	tc0 = new UV[maxNumVerts];
	tc1 = new UV[maxNumVerts];
}

MeshExporter::~MeshExporter()
{
	delete[] pos;
	delete[] normals;
	delete[] numAssignedBones;
	delete[] boneIndices;
	delete[] boneWeights;
	delete[] mtlIndices;
	delete[] tc0;
	delete[] tc1;
}

// http://dl3d.free.fr/phpBB2/viewtopic.php?t=1166
Point3 getNormal(Mesh* mesh, int faceIdx, int corner)
{
	// THIS IS WHAT YOU NEED IF SOMEONE HAS USED THE EDIT NORMAL MODIFIER
	MeshNormalSpec* normalSpec = mesh->GetSpecifiedNormals();

	if(normalSpec && normalSpec->GetNumFaces() && normalSpec->GetNumNormals())
		return normalSpec->Normal(normalSpec->Face(faceIdx).GetNormalID(corner)).Normalize();

	Face* face = &mesh->faces[faceIdx];
	int vertIdx = face->v[corner];
	RVertex *rVert = mesh->getRVertPtr(vertIdx);

	if(rVert)
	{
		unsigned rFlags = rVert->rFlags;

		// check if the normal is specified ...
		if(rFlags & SPECIFIED_NORMAL)
			return rVert->rn.getNormal();

		// ... otherwise, check for a smoothing group
		int normalCount = rFlags & NORCT_MASK;

		if(normalCount == 1)
			return rVert->rn.getNormal();

		unsigned smGroup = face->smGroup;

		for(int i = 0; i < normalCount; i++)
			if(rVert->ern[i].getSmGroup() & smGroup)
				return rVert->ern[i].getNormal();
	}

	return mesh->getFaceNormal(faceIdx);
}

Mtl* getMtl(INode *node, Mesh* mesh, int faceIdx)
{
	Mtl* mtl = node->GetMtl();

	if(!mtl)
		return 0;

	if(mtl->ClassID() == Class_ID(MULTI_CLASS_ID, 0))
		return mtl->GetSubMtl(mesh->getFaceMtlIndex(faceIdx));
	else
		return mtl;
}

int  MeshExporter::callback (INode *node)
{
	switch(pass)
	{
	case boneIndexingPass:

		if(isBone(node))
		{
			unsigned char i = (unsigned char)boneIdx.size();
			boneIdx[node->GetName()] = i;
			Matrix3 tm = node->GetNodeTM(0);
			Matrix3 offsetMat = Inverse(tm);
			float _xm[4][3] =
			{
				1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
			};
			offsetMats[i] = XFormMat(Matrix3(_xm), offsetMat);
			Matrix3 C = node->GetNodeTM(0);
			Matrix3 P = node->GetParentTM(0);
			Matrix3 L = C * Inverse(P);
			Quat q(L);
			defaultRot[i] = Quat(q.x, q.z, q.y, q.w);
			Point3 trans = L.GetTrans();
			defaultPos[i] = Point4(trans.x, trans.z, trans.y, 0.0f);
			parentIndices[i] = boneIdx[node->GetParentNode()->GetName()];
			return TREE_CONTINUE;
		}
		return TREE_IGNORECHILDREN;

	case exportPass:

		if(node->Selected())
		{
			Object* obj = node->EvalWorldState(0).obj;
			if(obj->SuperClassID() == GEOMOBJECT_CLASS_ID && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
			{
				ISkin* skin = 0; // 为了避免警告才初始化了一下
				ISkinContextData* skinContex = 0;
				if(node->GetObjectRef()->SuperClassID() == GEN_DERIVOB_CLASS_ID)
				{
					IDerivedObject* derObj = (IDerivedObject*)node->GetObjectRef();
					for(int i = 0; i < derObj->NumModifiers(); i++)
					{
						Modifier* mod = derObj->GetModifier(i);
						if(mod->ClassID() == SKIN_CLASSID)
						{
							skin = (ISkin*)mod->GetInterface(I_SKIN);
							skinContex = skin->GetContextInterface(node);
							break;
						}
					}
				}

				TriObject *triObj = (TriObject*)obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID,0));
				Mesh *mesh = &triObj->mesh;
				mesh->checkNormals(TRUE);
				int numFaces = mesh->numFaces;
				Face* faces = mesh->faces;
				Point3* verts = mesh->verts;
				TVFace* tvFaces0 = mesh->mapFaces(1);
				Point3* tVerts0 = mesh->mapVerts(1);
				TVFace* tvFaces1 = mesh->mapFaces(2);
				Point3* tVerts1 = mesh->mapVerts(2);

				Matrix3 worldMat = node->GetObjTMAfterWSM(0);

				for(int faceIdx = 0; faceIdx < numFaces; faceIdx++)
				{
					Face* face = &faces[faceIdx];
					TVFace* tvFace0 = tvFaces0 ? &tvFaces0[faceIdx] : 0;
					TVFace* tvFace1 = tvFaces1 ? &tvFaces1[faceIdx] : 0;

					for(int corner = 0; corner < 3; corner++)
					{
						int vertIdx = face->v[corner];
						Point3 vert = verts[vertIdx] * worldMat;
						Point3 normal = VectorTransform(worldMat, getNormal(mesh, faceIdx, corner));
						Mtl* mtl = getMtl(node, mesh, faceIdx);
						unsigned char mtlIdx = this->mtlIdx.insert(std::make_pair(mtl, (unsigned char)this->mtlIdx.size())).first->second;
						UV tv0 = tvFace0 ? tVerts0[tvFace0->t[corner]] : UV(0,0);
						UV tv1 = tvFace1 ? tVerts1[tvFace1->t[corner]] : UV(0,0);

						Texmap* diffuseMap = mtl ? mtl->GetSubTexmap(ID_DI) : 0;
						auto& indices = vertexIndexArrays[diffuseMap];

						int i;

						for(i = 0; i < numVerts; i++)
						{
							if (vert != pos[i]) continue;

							if (exportNormal && normal != normals[i]) continue;

							if (exportMtl && mtlIdx != mtlIndices[i]) continue;

							if (exportTC0 && tv0 != tc0[i]) continue;

							if (exportTC1 && tv1 != tc1[i]) continue;

							break;
						}

						if(i == numVerts)
						{
							pos[i] = vert;

							if(exportNormal)
								normals[i] = normal;

							if(exportMtl)
								mtlIndices[i] = mtlIdx;

							if(exportTC0 && tVerts0)
								tc0[i] = tv0;

							if(exportTC1 && tVerts1)
								tc1[i] = tv1;

							if (boneIdx.size() > 1)
							{
								int numAssignedBones = skinContex ? skinContex->GetNumAssignedBones(vertIdx) : 0;
								if (numAssignedBones > 4) numAssignedBones = 4;

								if (numAssignedBones > 0)
								{
									this->numAssignedBones[i] = numAssignedBones;
									for (int j = 0; j < numAssignedBones; j++)
									{
										INode* bone = skin->GetBone(skinContex->GetAssignedBone(vertIdx, j));
										boneIndices[i][j] = boneIdx[bone->GetName()];
										boneWeights[i][j] = skinContex->GetBoneWeight(vertIdx, j);
									}
								}
								else
								{
									this->numAssignedBones[i] = 1;
									boneIndices[i][0] = 0;
									boneWeights[i][0] = 1.0f;
								}
							}

							numVerts++;
						}

						indices.push_back(i);
					}
				}
			}
			return TREE_CONTINUE;
		}
		return TREE_IGNORECHILDREN;
	};

	return TREE_IGNORECHILDREN; // 不会执行到这 只为避免编译警告
}

int  MeshExporter::DoExport (const MCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
{
	boneIdx.clear();
	boneIdx[i->GetRootNode()->GetName()] = 0;
	pass = boneIndexingPass;

	if(ei->theScene->EnumTree(this))
		return IMPEXP_FAIL;

	numVerts = 0;
	vertexIndexArrays.clear();
	mtlIdx.clear();

	exportNormal = true;
	exportMtl = true;
	exportTC0 = true;
	exportTC1 = false;

	pass = exportPass;

	if(ei->theScene->EnumTree(this))
		return IMPEXP_FAIL;

	for(int i = 0; i < numVerts; i++)
	{
		pos[i] = Point3(pos[i].x, pos[i].z, pos[i].y);
		normals[i] = Point3(normals[i].x, normals[i].z, normals[i].y);
	}

	pgn::PNMHeader header;

	FILE *f;
	_wfopen_s(&f, name, L"wb");
	fseek(f, sizeof(header), SEEK_SET);

	int offset = sizeof(header);
	int size;

	header.numVerts = numVerts;

	header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_POSITION] = offset;
	size = sizeof(pos[0]) * numVerts;
	fwrite(pos, size, 1, f);
	offset += size;

	if(exportNormal)
	{
		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_NORMAL] = offset;
		size = sizeof(normals[0]) * numVerts;
		fwrite(normals, size, 1, f);
		offset += size;
	}
	else
	{
		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_NORMAL] = -1;
	}

	if(boneIdx.size())
	{
		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_NUM_ASSIGNED_BONES] = offset;
		size = sizeof(numAssignedBones[0]) * numVerts;
		fwrite(numAssignedBones, size, 1, f);
		offset += size;

		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_BONE_INDICES] = offset;
		size = sizeof(boneIndices[0]) * numVerts;
		fwrite(boneIndices, size, 1, f);
		offset += size;

		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_BONE_WEIGHTS] = offset;
		size = sizeof(boneWeights[0]) * numVerts;
		fwrite(boneWeights, size, 1, f);
		offset += size;
	}
	else
	{
		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_NUM_ASSIGNED_BONES] = -1;
		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_BONE_INDICES] = -1;
		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_BONE_WEIGHTS] = -1;
	}

	if(exportMtl)
	{
		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_MATERIAL_INDEX] = offset;
		size = sizeof(mtlIndices[0]) * numVerts;
		fwrite(mtlIndices, size, 1, f);
		offset += size;
	}
	else
	{
		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_MATERIAL_INDEX] = -1;
	}

	if(exportTC0)
	{
		for (int i = 0; i < numVerts; i++)
			tc0[i].v = 1.0f - tc0[i].v;

		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_TEX_COORD_0] = offset;
		size = sizeof(tc0[0]) * numVerts;
		fwrite(tc0, size, 1, f);
		offset += size;
	}
	else
	{
		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_TEX_COORD_0] = -1;
	}

	if(exportTC1)
	{
		for (int i = 0; i < numVerts; i++)
			tc1[i].v = 1.0f - tc1[i].v;

		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_TEX_COORD_1] = offset;
		size = sizeof(tc1[0]) * numVerts;
		fwrite(tc1, size, 1, f);
		offset += size;
	}
	else
	{
		header.attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_TEX_COORD_1] = -1;
	}

	header.indicesChunkOffset = offset;
	header.numSubsets = (int)vertexIndexArrays.size();
	auto it = vertexIndexArrays.begin();
	for (int i = 0; i < vertexIndexArrays.size(); i++)
	{
		auto& indices = it->second;
		header.numIndices[i] = (int)indices.size();
		size = sizeof(indices[0]) * (int)indices.size();
		fwrite(&indices[0], size, 1, f);
		offset += size;
		it++;
	}

	if (boneIdx.size() > 1)
	{
		int numBones = (int)boneIdx.size();
		header.numBones = numBones;

		int _offset = align(offset, 16);
		size = _offset - offset;
		static char padding[15];
		if(size) fwrite(padding, size, 1, f);
		offset += size;

		header.offsetMatsChunkOffset = offset;
		size = 16 * 3 * numBones;
		for (int i = 0; i < numBones; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Point4 col = offsetMats[i].GetColumn(j);
				fwrite(&col.x, 16, 1, f);
			}
		}
		offset += size;

		header.defaultRotChunkOffset = offset;
		size = sizeof(defaultRot[0]) * numBones;
		fwrite(defaultRot, size, 1, f);
		offset += size;

		header.defaultPosChunkOffset = offset;
		size = sizeof(defaultPos[0]) * numBones;
		fwrite(defaultPos, size, 1, f);
		offset += size;

		header.parentIndicesChunkOffset = offset;
		size = sizeof(parentIndices[0]) * numBones;
		fwrite(parentIndices, size, 1, f);
		offset += size;
	}
	else
	{
		header.numBones = 0;
		header.offsetMatsChunkOffset = -1;
		header.defaultRotChunkOffset = -1;
		header.defaultPosChunkOffset = -1;
		header.parentIndicesChunkOffset = -1;
	}

	if (exportMtl)
	{
		struct _Material
		{
			float Ks;
			float shininess;
		};

		_Material* mtls = new _Material[mtlIdx.size()];

		for(auto it = mtlIdx.begin(); it != mtlIdx.end(); it++)
		{
			_Material* _mtl = &mtls[it->second];
			Mtl* mtl = it->first;
			if(mtl)
			{
				_mtl->Ks = mtl->GetShinStr() * 100 / 100;
				_mtl->shininess = mtl->GetShininess() * 100;
			}
			else
			{
				_mtl->Ks = 0.0f;
				_mtl->shininess = 10.0f;
			}
		}

		header.materialChunkOffset = offset;
		size = sizeof(mtls[0]) * (int)mtlIdx.size();
		fwrite(mtls, size, 1, f);
		header.materialChunkSize = size;

		delete[] mtls;
	}
	else
	{
		header.materialChunkOffset = -1;
		header.materialChunkSize = 0;
	}

	fseek(f, 0, SEEK_SET);
	fwrite(&header, sizeof(header), 1, f);
	fclose(f);

	return IMPEXP_SUCCESS;
}

class MeshExporterDesc : public ClassDesc
{
	virtual int  IsPublic () { return TRUE; }
	virtual void *  Create (BOOL loading) { return new MeshExporter; }
	virtual const MCHAR *  ClassName () { return L"PNM Exporter"; }
	virtual SClass_ID  SuperClassID () { return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID  ClassID () { return Class_ID(0x4751736c, 0x606550cf); }
	virtual const MCHAR *  Category () { return L""; }
} _meshExporterDesc;

ClassDesc* meshExporterDesc = &_meshExporterDesc;

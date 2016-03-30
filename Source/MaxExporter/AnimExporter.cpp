#include <CS/BipedApi.H>
#include <CS/BIPEXP.H>
#include <map>
#include <max.h>
#include <PGN/Assets/PNA.h>
#include <vector>
#include "About.h"
#include "Utilities.h"

struct BoneAnimation
{
	std::map<TimeValue, Quat> rotKeys;
	std::map<TimeValue, Point4> posKeys;
};

class AnimExporter : public SceneExport, public ITreeEnumProc
{
public:
	std::vector<BoneAnimation> boneAnims;

	AnimExporter();
	~AnimExporter();

	// SceneExport
	virtual int  ExtCount () { return 1; }
	virtual const MCHAR *  Ext (int n) { return L"pna"; }
	virtual const MCHAR *  LongDesc () { return L"PGN Animation File"; }
	virtual const MCHAR *  ShortDesc () { return L"PGN Animation File"; }
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

AnimExporter::AnimExporter()
{
}

AnimExporter::~AnimExporter()
{
}

int  AnimExporter::callback(INode *node)
{
	if (!isBone(node))
		return TREE_IGNORECHILDREN;

	BoneAnimation boneAnim;
	Tab<TimeValue> rotKeyTimes, posKeyTimes;

	Control* tmc = node->GetTMController();
	Class_ID cid = tmc->ClassID();
	INode* parent = node->GetParentNode();

	if (cid == Class_ID(PRS_CONTROL_CLASS_ID, 0))
	{
		Control* rc = tmc->GetRotationController();
		if (rc->ClassID() == Class_ID(EULER_CONTROL_CLASS_ID, 0))
		{
			Control* xc = rc->GetXController();
			Control* yc = rc->GetYController();
			Control* zc = rc->GetZController();
			xc->GetKeyTimes(rotKeyTimes, FOREVER, KEYAT_ROTATION);
			yc->GetKeyTimes(rotKeyTimes, FOREVER, KEYAT_ROTATION);
			zc->GetKeyTimes(rotKeyTimes, FOREVER, KEYAT_ROTATION);
		}
		else
		{
			tmc->GetKeyTimes(rotKeyTimes, FOREVER, KEYAT_ROTATION);
		}

		tmc->GetKeyTimes(posKeyTimes, FOREVER, KEYAT_POSITION);
	}
	else if (cid == BIPBODY_CONTROL_CLASS_ID/* || cid == FOOTPRINT_CLASS_ID*/)
	{
		IBipMaster *bip = (IBipMaster*)tmc->GetInterface(I_BIPMASTER);
		Control *biph = bip->GetHorizontalControl();
		Control *bipv = bip->GetVerticalControl();
		Control *bipr = bip->GetTurnControl();
		biph->GetKeyTimes(rotKeyTimes, FOREVER, KEYAT_ROTATION);
		bipv->GetKeyTimes(rotKeyTimes, FOREVER, KEYAT_ROTATION);
		bipr->GetKeyTimes(rotKeyTimes, FOREVER, KEYAT_ROTATION);
		biph->GetKeyTimes(posKeyTimes, FOREVER, KEYAT_POSITION);
		bipv->GetKeyTimes(posKeyTimes, FOREVER, KEYAT_POSITION);
		bipr->GetKeyTimes(posKeyTimes, FOREVER, KEYAT_POSITION);
	}
	else if (cid == BIPSLAVE_CONTROL_CLASS_ID)
	{
		tmc->GetKeyTimes(rotKeyTimes, FOREVER, KEYAT_ROTATION);
		tmc->GetKeyTimes(posKeyTimes, FOREVER, KEYAT_POSITION);
	}

	for (int i = 0; i < rotKeyTimes.Count(); i++)
	{
		int time = rotKeyTimes[i];
		Matrix3 C = node->GetNodeTM(time);
		Matrix3 P = node->GetParentTM(time);
		Matrix3 L = C * Inverse(P);
		Quat q(L);
		boneAnim.rotKeys[time] = Quat(q.x, q.z, q.y, q.w);
	}

	for (int i = 0; i < posKeyTimes.Count(); i++)
	{
		int time = posKeyTimes[i];
		Matrix3 C = node->GetNodeTM(time);
		Matrix3 P = node->GetParentTM(time);
		Matrix3 L = C * Inverse(P);
		Point3 trans = L.GetTrans();
		boneAnim.posKeys[time] = Point4(trans.x, trans.z, trans.y, 0.0f);
	}

	boneAnims.push_back(boneAnim);

	return TREE_CONTINUE;
}

int  AnimExporter::DoExport (const MCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
{
	boneAnims.clear();
	boneAnims.emplace_back();

	if(ei->theScene->EnumTree(this))
		return IMPEXP_FAIL;

	pgn::PNAHeader header;
	header.numBones = (int)boneAnims.size();
	int sizeHeader = sizeof(header)-sizeof(header.boneAnimations[0]) + sizeof(header.boneAnimations[0]) * (int)boneAnims.size();

	FILE *f;
	_wfopen_s(&f, name, L"wb");
	fseek(f, sizeHeader, SEEK_SET);

	int offset = sizeHeader;
	int size;

	std::vector<pgn::PNAHeader::BoneAnimation> headerKeyArrays;
	headerKeyArrays.resize(boneAnims.size());

	for (int i = 0; i < boneAnims.size(); i++)
	{
		int numRotKeys = (int)boneAnims[i].rotKeys.size();
		headerKeyArrays[i].numRotKeys = numRotKeys;

		headerKeyArrays[i].rotKeyTimesChunkOffset = offset;
		for (auto it = boneAnims[i].rotKeys.begin(); it != boneAnims[i].rotKeys.end(); it++)
		{
			int time = it->first;
			size = sizeof(time);
			fwrite(&time, size, 1, f);
			offset += size;
		}

		int numPosKeys = (int)boneAnims[i].posKeys.size();
		headerKeyArrays[i].numPosKeys = numPosKeys;

		headerKeyArrays[i].posKeyTimesChunkOffset = offset;
		for (auto it = boneAnims[i].posKeys.begin(); it != boneAnims[i].posKeys.end(); it++)
		{
			int time = it->first;
			size = sizeof(time);
			fwrite(&time, size, 1, f);
			offset += size;
		}

		int _offset = align(offset, 16);
		size = _offset - offset;
		static char padding[15];
		if (size) fwrite(padding, size, 1, f);
		offset += size;

		headerKeyArrays[i].rotKeysChunkOffset = offset;
		for (auto it = boneAnims[i].rotKeys.begin(); it != boneAnims[i].rotKeys.end(); it++)
		{
			Quat& quat = it->second;
			size = sizeof(quat);
			fwrite(&quat, size, 1, f);
			offset += size;
		}

		headerKeyArrays[i].posKeysChunkOffset = offset;
		for (auto it = boneAnims[i].posKeys.begin(); it != boneAnims[i].posKeys.end(); it++)
		{
			Point4& pos = it->second;
			size = sizeof(pos);
			fwrite(&pos, size, 1, f);
			offset += size;
		}
	}

	fseek(f, 0, SEEK_SET);
	fwrite(&header, offsetof(pgn::PNAHeader, boneAnimations), 1, f);
	fwrite(&headerKeyArrays[0], sizeof(header.boneAnimations[0]) * boneAnims.size(), 1, f);
	fclose(f);

	return IMPEXP_SUCCESS;
}

class AnimExporterDesc : public ClassDesc
{
	virtual int  IsPublic () { return TRUE; }
	virtual void *  Create (BOOL loading) { return new AnimExporter; }
	virtual const MCHAR *  ClassName () { return L"PNA Exporter"; }
	virtual SClass_ID  SuperClassID () { return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID  ClassID() { return Class_ID(0x3d522859, 0x56e05f66); }
	virtual const MCHAR *  Category () { return L""; }
} _animExporterDesc;

ClassDesc* animExporterDesc = &_animExporterDesc;

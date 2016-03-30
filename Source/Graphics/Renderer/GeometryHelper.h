
namespace pgn {
	class RenderingSystem;
	struct VertexAttribDesc;
}

class Geometry;

class GeometryHelper
{
	pgn::RenderingSystem* rs;
public:
	GeometryHelper(pgn::RenderingSystem* rs);
	void createGeometry(Geometry* geom, int numAttribs, pgn::VertexAttribDesc attribDescs[], int numVerts, int numSubsets, int numIndices[], void* attribs[] = 0, void* indices = 0);
	void destroyGeometry(Geometry* geom);
};

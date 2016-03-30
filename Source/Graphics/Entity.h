#include <PGN/Graphics/Entity.h>

namespace pgn {
	struct Float4x3;
	class Pool;
}

class Model;

class Entity : public pgn::Entity
{
public:
	pgn::Pool* pool;
	Model* model;
	pgn::Skeleton* skel;
	pgn::Float4x3* boneMats;

	Entity(pgn::Pool* pool);
	virtual void dispose();
	virtual void _free();
	virtual void setModel(pgn::Model* model);
	virtual void setSkeleton(pgn::Skeleton* skel);
	virtual void setPose(pgn::Float4x3* boneMats);
};

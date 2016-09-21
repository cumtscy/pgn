#pragma once

namespace pgn {
	class Pool;
}

class Light
{
public:
	pgn::Pool* pool;

	Light(pgn::Pool* pool);
};

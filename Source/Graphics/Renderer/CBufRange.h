#pragma once

namespace pgn {
	class Buffer;
}

struct CBufRange
{
	pgn::Buffer*	buf;
	int				offset;
	int				size;
};

#pragma once
namespace pgn {

#ifdef __SNC__
#pragma diag_suppress=1700
#endif

class Interface
{
public:
	virtual void dispose() = 0;
	virtual void _free() = 0;
	virtual void destroy()
	{
		dispose();
		_free();
	}
};

}

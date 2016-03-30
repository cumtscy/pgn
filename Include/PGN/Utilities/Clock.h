#pragma once
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"
namespace pgn {

class Clock : public Interface
{
public:
	// 创建一个时钟
	// 时钟周期初始化为cycleNumer/cycleDenom秒
	static PGN_DLL_INTERFACE Clock* create(long long cycleNumer = 1, long long cycleDenom = 1000);

	// 返回自首次调用以来过去的时钟周期个数
	virtual long long getTickCount() = 0;

	// 设置时钟周期为numer/denom秒
	virtual void setCycle(long long numer, long long denom) = 0;

	// 获得时钟周期
	virtual void getCycle(long long* numer, long long* denom) = 0;

	// 重置时钟
	// 再次调用getTickCount()重新开始计数
	virtual void reset() = 0;
};

}

#define PGN_DLL_EXPORT
#include <PGN/Platform/Thread/Signal.h>
#undef PGN_DLL_EXPORT

#include <kernel.h>
#include <PGN/Common/debug_new.h>

class Signal : public pgn::Signal
{
	SceUID event;

public:
	Signal()
	{
		event = sceKernelCreateEventFlag("", SCE_KERNEL_EVF_ATTR_TH_FIFO | SCE_KERNEL_EVF_ATTR_MULTI, 0x00, 0);
	}

	virtual void dispose()
	{
		sceKernelDeleteEventFlag(event);
	}

	virtual void _free()
	{
		delete this;
	}

	virtual void raise()
	{
		sceKernelSetEventFlag(event, 0x01);
	}

	virtual void wait()
	{
		sceKernelWaitEventFlag(event, 0x01, SCE_KERNEL_EVF_WAITMODE_AND, 0, 0);
	}
};

pgn::Signal* pgn::Signal::create()
{
	return debug_new::Signal;
}

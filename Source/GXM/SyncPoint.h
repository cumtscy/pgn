#include <gxm/structs.h>
#include <PGN/RenderingSystem/SyncPoint.h>

class SyncPoint : public pgn::SyncPoint
{
public:
	RenderingSystem* rs;
	SceGxmNotification notification;

	SyncPoint(RenderingSystem* rs);
	virtual void dispose();
	virtual void _free();
};

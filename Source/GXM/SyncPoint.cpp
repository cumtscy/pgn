#include <PGN/Common/debug_new.h>
#include "RenderingSystem.h"
#include "SyncPoint.h"

SyncPoint::SyncPoint(RenderingSystem* rs)
{
	this->rs = rs;

	notification.address = rs->notificationAddrFreeList.back();
	notification.value = 1;

	rs->notificationAddrFreeList.pop_back();
}

void SyncPoint::dispose()
{
	rs->notificationAddrFreeList.push_back(notification.address);
}

pgn::SyncPoint* RenderingSystem::createSyncPoint()
{
	return debug_new SyncPoint(this);
}

void SyncPoint::_free()
{
	delete this;
}

bool RenderingSystem::checkSyncPoint(pgn::SyncPoint* _sync)
{
	SyncPoint* sync = (SyncPoint*)_sync;

	bool r = *sync->notification.address == sync->notification.value;

	if (r)
		*sync->notification.address = 0;

	return r;
}

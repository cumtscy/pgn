#include <CS/BIPEXP.H>
#include <max.h>
#include "Utilities.h"

bool isBone(INode *node)
{
	Object* obj = node->GetObjectRef();
	if (obj->ClassID() == BONE_OBJ_CLASSID || obj->ClassID() == SKELOBJ_CLASS_ID)
	{
		Control *c = node->GetTMController();
		return (c->ClassID() == Class_ID(PRS_CONTROL_CLASS_ID, 0) ||
				c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||
				c->ClassID() == BIPBODY_CONTROL_CLASS_ID ||
				c->ClassID() == FOOTPRINT_CLASS_ID);
	}
	return false;
}

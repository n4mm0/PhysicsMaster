#pragma once
#include "TypeList.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"

namespace ColliderType
{
	typedef TYPELIST_3(BoxCollider, SphereCollider, PlaneCollider) ColliderTypeList;
	typedef TL::Length<ColliderTypeList> ColliderTypeListLength;
}

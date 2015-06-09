#pragma once
#include "TypeList.h"
//#include "BoxCollider.h"
//#include "SphereCollider.h"
//#include "PlaneCollider.h"
class BoxCollider;
class SphereCollider;
class PlaneCollider;

namespace ColliderType
{

	typedef TYPELIST_3(BoxCollider, SphereCollider, PlaneCollider) ColliderTypeList;
	typedef TL::Length<ColliderTypeList> ColliderTypeListSize;

	enum TypeIndex
	{
		BOX = TL::IndexOf<ColliderTypeList, BoxCollider>::value,
		SPHERE = TL::IndexOf<ColliderTypeList, SphereCollider>::value,
		PLANE = TL::IndexOf<ColliderTypeList, PlaneCollider>::value
	};
s}

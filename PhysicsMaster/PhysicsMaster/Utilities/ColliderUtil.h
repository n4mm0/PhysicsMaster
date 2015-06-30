#pragma once
#include "TypeList.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"


namespace ColliderType
{

	typedef TYPELIST_3(BoxCollider, SphereCollider, PlaneCollider) ColliderTypeList;
	typedef TL::Length<ColliderTypeList> ColliderTypeListLength;

	/*No more needed
	enum TypeIndex
	{
		BOX = TL::IndexOf<ColliderTypeList, BoxCollider>::value,
		SPHERE = TL::IndexOf<ColliderTypeList, SphereCollider>::value,
		PLANE = TL::IndexOf<ColliderTypeList, PlaneCollider>::value
	};*/
}
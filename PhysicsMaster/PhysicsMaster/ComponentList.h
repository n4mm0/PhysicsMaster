#pragma once
#include "Utilities\TypeList.h"
class Transform;
class RigidBody;
class Collider;

namespace Components
{
	typedef TYPELIST_3(Transform, RigidBody, Collider) ComponentList;
	typedef TL::Length<ComponentList> ComponentListLength;
}
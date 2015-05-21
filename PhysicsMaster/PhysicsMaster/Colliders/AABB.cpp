#include "AABB.h"
#include <iostream>

//CONSTRUCTOR
AABB::AABB()
{
	min = Point3(0, 0, 0);
	max = Point3(0, 0, 0);
	empty = true;
}

AABB::~AABB()
{
	this->clear();
}

//MEMBER FUNCTION
void AABB::clear()
{
	this->empty = true;
	this->myBody = nullptr;
}

void AABB::add(Point3 point)
{
	if (isEmpty())
		max = min = point;
	else
	{
		if (point.getX()<min.getX())
			min.setX(point.getX());
		if (point.getY()<min.getY())
			min.setY(point.getY());
		if (point.getZ()<min.getZ())
			min.setZ(point.getZ());

		if (point.getX()>max.getX())
			max.setX(point.getX());
		if (point.getY()>max.getY())
			max.setY(point.getY());
		if (point.getZ()>max.getZ())
			max.setZ(point.getZ());

	}
	this->empty = false;
}

bool AABB::isEmpty() const
{
	return empty;
}

Point3 AABB::center() const
{
	return (max + min)*0.5;
}

Point3 AABB::maxPoint() const
{
	return max;
}

Point3 AABB::minPoint() const
{
	return min;
}

void AABB::Collide(Collider& i_Collider)
{
	i_Collider.Collide(*this);
}

void AABB::Collide(AABB& i_Collider)
{
	std::cout << "Collision BoxBox! FIRE!" << std::endl;
}

void AABB::Collide(SphereCollider& i_Collider)
{
	std::cout << "Collision BoxSphere! FIRE!" << std::endl;
}
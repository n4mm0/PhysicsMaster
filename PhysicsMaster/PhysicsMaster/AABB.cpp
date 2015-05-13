#include "AABB.h"

//CONSTRUCTOR
AABB::AABB()
{
	min = Point3(0, 0, 0);
	max = Point3(0, 0, 0);
	empty = true;
}

//MEMBER FUNCTION
void AABB::clear()
{
	this->empty = true;
}

void AABB::add(Point3 point)
{
	if (isEmpty())
		max = min = point;
	else
	{
		if (point.x()<min.x())
			min.x() = point.x();
		if (point.y()<min.y())
			min.y() = point.y();
		if (point.z()<min.z())
			min.z() = point.z();

		if (point.x()>max.x())
			max.x() = point.x();
		if (point.y()>max.y())
			max.y() = point.y();
		if (point.z()>max.z())
			max.z() = point.z();
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
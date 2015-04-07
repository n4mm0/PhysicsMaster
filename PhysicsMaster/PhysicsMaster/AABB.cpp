#include "AABB.h"

//COSTRUTTORE
AABB::AABB()
{
   min = point3(0,0,0);
   max = point3(0,0,0);
   empty = true;
}

//METODI
void AABB::clear()
{
    this->empty = true;
}

void AABB::add(point3 p)
{
    if(isEmpty())
        max = min = p;
    else
    {
        if(p.x()<min.x())
            min.x() = p.x();
        if(p.y()<min.y())
            min.y() = p.y();
        if(p.z()<min.z())
            min.z() = p.z();

        if(p.x()>max.x())
            max.x() = p.x();
        if(p.y()>max.y())
            max.y() = p.y();
        if(p.z()>max.z())
            max.z() = p.z();
    }
    this->empty = false;
}

bool AABB::isEmpty() const
{
    return empty;
}

point3 AABB::center() const
{
    return (max+min)*0.5;
}

point3 AABB::maxP() const
{
    return max;
}

point3 AABB::minP() const
{
    return min;
}


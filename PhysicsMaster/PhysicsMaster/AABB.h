#include "Vector3.h"

class AABB
{
private:
	//MEMBER
	Point3 min;
	Point3 max;
	bool empty;

public:
	//CONSTRUCTOR
	AABB();

	//MEMBER FUNCTION
	void clear();
	void add(Point3 p);
	bool isEmpty() const;
	Point3 center() const;
	Point3 minPoint() const;
	Point3 maxPoint() const;
};
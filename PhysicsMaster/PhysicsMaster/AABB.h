#include "Vector3.h"
#include "Collider.h"

class AABB : public Collider
{
private:
	//MEMBER
	Point3 min;
	Point3 max;
	bool empty;

	void Collide(AABB& i_Collider);

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
	void Collide(Collider& i_Collider);
};
#pragma once

typedef float scalar;

class Vector3
{
private:
    //MEMBER
    scalar vector[3];

public:
	//STATIC MEMBER
	static const Vector3 Zero;
	static const enum clamping { MAX = 0, MIN = 1 };
    
	//CONSTRUCTORS
	Vector3();
	Vector3(scalar x, scalar y, scalar z);
	Vector3(const Vector3 &other);

	//DESTRUCTOR
	~Vector3();

    //MEMBER FUNCTION
	scalar getX() const;
	scalar getY() const;
	scalar getZ() const;
	void setX(scalar x);
	void setY(scalar y);
	void setZ(scalar z);
	void set(scalar x, scalar y, scalar z);
	scalar dot(const Vector3 &other) const;
	Vector3 cross(const Vector3 &other) const;
	scalar magnitude() const;
	scalar sqrMagnitude() const;
	void normalize();
	bool isZero() const;
	scalar angle(const Vector3 &other);
	scalar distance(const Vector3 &other);
	Vector3 lerp(const Vector3 &other, float t);
	void rotationX(const int degree);
	void rotationY(const int degree);
	void rotationZ(const int degree);

	//OPERATORS
	Vector3 operator-() const;
	Vector3& operator =(const Vector3 &other);
	Vector3& operator +=(const Vector3& other);
	Vector3& operator -=(const Vector3& other);
	Vector3& operator *=(const Vector3& other);
	Vector3& operator *=(const scalar k);
	Vector3& operator /=(const scalar k);
	bool operator ==(Vector3 const &other) const;
	bool operator !=(Vector3 const &other) const;
	scalar& operator[](int index);
	const scalar& operator[](int index) const;
};

typedef Vector3 Point3;
Vector3 operator+(const Vector3 &first,const Vector3& second);
Vector3 operator-(const Vector3 &first, const Vector3& second);
Vector3 operator*(const Vector3 &vec,const scalar k);
Vector3 operator*(const scalar k, const Vector3& vec);
Vector3 operator/(const Vector3 &vec,const  scalar k);
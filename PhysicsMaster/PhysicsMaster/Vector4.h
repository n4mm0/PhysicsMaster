#pragma once

typedef float scalar;

class Vector4
{
private:
    //MEMBER
    scalar vector[4];

public:
	//STATIC MEMBER
	static const Vector4 vectorZero;
	static const Vector4 pointZero;

	//CONSTRUCTORS
	Vector4();
	Vector4(scalar x, scalar y, scalar z, scalar w);
	Vector4(const Vector4 &other);

	//DESTRUCTOR
	~Vector4();

	//MEMBER FUNCTION
	scalar getX() const;
	scalar getY() const;
	scalar getZ() const;
	scalar getW() const;
	void setX(scalar x);
	void setY(scalar y);
	void setZ(scalar z);
	void setW(scalar w);
	void set(scalar x, scalar y, scalar z, scalar w);
	scalar dot(const Vector4 &other) const;
	scalar magnitude() const;
	scalar sqrMagnitude() const;
	void normalize();
	bool isZero() const;
	scalar angle(const Vector4 &other);				//ONLY FOR VECTOR
	scalar distance(const Vector4 &other);
	Vector4 lerp(const Vector4 &other, float t);
	void rotationX(const int degree);				//ONLY FOR VECTOR
	void rotationY(const int degree);				//ONLY FOR VECTOR
	void rotationZ(const int degree);				//ONLY FOR VECTOR

	//OPERATORS
	Vector4 operator +(const Vector4 &other) const;
	Vector4 operator -(const Vector4 &other) const;
	Vector4 operator *(const scalar k) const;
	Vector4 operator /(const scalar k) const;
	Vector4 operator -() const;
	Vector4& operator =(const Vector4 &other);
	Vector4& operator +=(const Vector4& other);
	Vector4& operator -=(const Vector4& other);
	Vector4& operator *=(const scalar k);
	Vector4& operator /=(const scalar k);
	bool operator ==(Vector4 const &other) const;
	bool operator !=(Vector4 const &other) const;
};
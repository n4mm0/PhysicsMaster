#pragma once
#include "Vector4.h"

class Matrix4x4
{
private:
	//MEMBER
	scalar matrix[16];

public:
	//CONSTRUCTORS
	Matrix4x4();
	Matrix4x4(Vector4 v0, Vector4 v1, Vector4 v2, Vector4 p0);
	Matrix4x4(const Matrix4x4 &other);

	//DESTRUCTOR
	~Matrix4x4();

	//MEMBER FUNCTION
	Vector4 getRow(int row) const;
	Vector4 getColumn(int column) const;
	scalar getElementAt(int row, int column) const;
	void setRow(int row, const Vector4 &vector);
	void setColumn(int column, const Vector4 &vector);
	void setElementAt(int row, int column, scalar value);
	void transpose();
	bool isIdentity() const;

	//OPERATORS
	Vector4 operator *(const Vector4 &vector) const;
	Matrix4x4 operator *(const Matrix4x4 &other) const;

	//STATIC MEMBER FUNCTION
	static Matrix4x4 identity();
	static Matrix4x4 translation(scalar dx, scalar dy, scalar dz);
	static Matrix4x4 rotationX(scalar degree);
	static Matrix4x4 rotationY(scalar degree);
	static Matrix4x4 rotationZ(scalar degree);
	static Matrix4x4 scaling(scalar sx, scalar sy, scalar sz);
	static Matrix4x4 uniformScaling(scalar scale);
};
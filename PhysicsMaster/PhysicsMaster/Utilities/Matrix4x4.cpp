#define _USE_MATH_DEFINES

#include <cmath>
#include "Matrix4x4.h"

//CONSTRUCTORS
Matrix4x4::Matrix4x4()
{
	for (int index = 0; index < 16; ++index)
		matrix[index] = 0;

	matrix[15] = 1;
}

Matrix4x4::Matrix4x4(Vector4 v0, Vector4 v1, Vector4 v2, Vector4 p0)
{
	matrix[0] = v0.getX(); matrix[4] = v1.getX(); matrix[8] = v2.getX();  matrix[12] = p0.getX();
	matrix[1] = v0.getY(); matrix[5] = v1.getY(); matrix[9] = v2.getY();  matrix[13] = p0.getY();
	matrix[2] = v0.getZ(); matrix[6] = v1.getZ(); matrix[10] = v2.getZ(); matrix[14] = p0.getZ();
	matrix[3] = v0.getW(); matrix[7] = v1.getW(); matrix[11] = v2.getW(); matrix[15] = p0.getW();
}

Matrix4x4::Matrix4x4(const Matrix4x4 &other)
{
	for (int index = 0; index < 16; ++index)
		matrix[index] = other.getElementAt((index % 4), (index / 4));
}

//DESTRUCTOR
Matrix4x4::~Matrix4x4()
{
}

//MEMBER FUNCTION
Vector4 Matrix4x4::getRow(int row) const
{
	return Vector4(matrix[row], matrix[row + 4], matrix[row + 8], matrix[row + 12]);
}

Vector4 Matrix4x4::getColumn(int column) const
{
	return Vector4(matrix[column * 4], matrix[column * 4 + 1], matrix[column * 4 + 2], matrix[column * 4 + 3]);
}

scalar Matrix4x4::getElementAt(int row, int column) const
{
	return matrix[row + column * 4];
}

void Matrix4x4::setRow(int row, const Vector4 &vector)
{
	matrix[row] = vector.getX();
	matrix[row + 4] = vector.getY();
	matrix[row + 8] = vector.getZ();
	matrix[row + 12] = vector.getW();
}

void Matrix4x4::setColumn(int column, const Vector4 &vector)
{
	matrix[column * 4] = vector.getX();
	matrix[column * 4 + 1] = vector.getY();
	matrix[column * 4 + 2] = vector.getZ();
	matrix[column * 4 + 3] = vector.getW();
}

void Matrix4x4::setElementAt(int row, int column, scalar value)
{
	matrix[row + column * 4] = value;
}

void Matrix4x4::transpose()
{
	Matrix4x4 tmp = *this;
	for (int index = 0; index < 4; ++index)
		setColumn(index, tmp.getRow(index));
}

bool Matrix4x4::isIdentity() const
{

	return	getRow(0) == Vector4(1, 0, 0, 0) &&
			getRow(1) == Vector4(0, 1, 0, 0) &&
			getRow(2) == Vector4(0, 0, 1, 0) &&
			getRow(3) == Vector4(0, 0, 0, 1);
}

//OPERATORS
Vector4 Matrix4x4::operator *(const Vector4 &vector) const
{
	scalar x = getRow(0).dot(vector);
	scalar y = getRow(1).dot(vector);
	scalar z = getRow(2).dot(vector);
	scalar w = getRow(3).dot(vector);

	return Vector4(x, y, z, w);
}

Matrix4x4 Matrix4x4::operator *(const Matrix4x4 &other) const
{
	Matrix4x4 result;
	for (int index = 0; index < 16; ++index)
	{
		scalar element = getRow(index % 4).dot(other.getColumn(index / 4));
		result.setElementAt(index % 4, index / 4, element);
	}
	return result;
}

//STATIC MEMBER FUNCTION
Matrix4x4 Matrix4x4::identity()
{
	Vector4 v0(1, 0, 0, 0);
	Vector4 v1(0, 1, 0, 0);
	Vector4 v2(0, 0, 1, 0);
	Vector4 p0(0, 0, 0, 1);

	return Matrix4x4(v0, v1, v2, p0);
}

Matrix4x4 Matrix4x4::translation(scalar dx, scalar dy, scalar dz)
{
	Vector4 v0(1, 0, 0, 0);
	Vector4 v1(0, 1, 0, 0);
	Vector4 v2(0, 0, 1, 0);
	Vector4 p0(dx, dy, dz, 1);

	return Matrix4x4(v0, v1, v2, p0);
}

Matrix4x4 Matrix4x4::rotationX(scalar degree)
{
	float rad = degree * M_PI / 180.0f;

	scalar cosine = cos(rad);
	scalar sine = sin(rad);

	Vector4 v0(1, 0, 0, 0);
	Vector4 v1(0, cosine, sine, 0);
	Vector4 v2(0, -sine, cosine, 0);
	Vector4 p0(0, 0, 0, 1);

	return Matrix4x4(v0, v1, v2, p0);
}

Matrix4x4 Matrix4x4::rotationY(scalar degree)
{
	float rad = degree * M_PI / 180.0f;

	scalar cosine = cos(rad);
	scalar sine = sin(rad);

	Vector4 v0(cosine, 0, -sine, 0);
	Vector4 v1(0, 1, 0, 0);
	Vector4 v2(sine, 0, cosine, 0);
	Vector4 p0(0, 0, 0, 1);

	return Matrix4x4(v0, v1, v2, p0);
}

Matrix4x4 Matrix4x4::rotationZ(scalar degree)
{
	float rad = degree * M_PI / 180.0f;

	scalar cosine = cos(rad);
	scalar sine = sin(rad);

	Vector4 v0(cosine, sine, 0, 0);
	Vector4 v1(-sine, cosine, 0, 0);
	Vector4 v2(0, 0, 1, 0);
	Vector4 p0(0, 0, 0, 1);

	return Matrix4x4(v0, v1, v2, p0);
}

Matrix4x4 Matrix4x4::scaling(scalar sx, scalar sy, scalar sz)
{
	Vector4 v0(sx, 0, 0, 0);
	Vector4 v1(0, sy, 0, 0);
	Vector4 v2(0, 0, sz, 0);
	Vector4 p0(0, 0, 0, 1);

	return Matrix4x4(v0, v1, v2, p0);
}

Matrix4x4 Matrix4x4::uniformScaling(scalar scale)
{
	return scaling(scale, scale, scale);
}
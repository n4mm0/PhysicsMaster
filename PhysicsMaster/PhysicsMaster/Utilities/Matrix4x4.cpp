#define _USE_MATH_DEFINES

#include <cmath>
#include "Matrix4x4.h"

//CONSTRUCTORS
Matrix4x4::Matrix4x4()
{
	for (int index = 0; index < 16; ++index)
		matrix[index / 4][index % 4] = 0;

	matrix[3][3] = 1;
}

Matrix4x4::Matrix4x4(Vector4 v0, Vector4 v1, Vector4 v2, Vector4 p0)
{
	matrix[0][0] = v0.getX(); matrix[0][1] = v1.getX(); matrix[0][2] = v2.getX(); matrix[0][3] = p0.getX();
	matrix[1][0] = v0.getY(); matrix[1][1] = v1.getY(); matrix[1][2] = v2.getY(); matrix[1][3] = p0.getY();
	matrix[2][0] = v0.getZ(); matrix[2][1] = v1.getZ(); matrix[2][2] = v2.getZ(); matrix[2][3] = p0.getZ();
	matrix[3][0] = v0.getW(); matrix[3][1] = v1.getW(); matrix[3][2] = v2.getW(); matrix[3][3] = p0.getW();	
}

Matrix4x4::Matrix4x4(const Matrix4x4 &other)
{
	for (int index = 0; index < 16; ++index)
		matrix[index / 4][index % 4] = other.getElementAt((index / 4), (index % 4));
}

//DESTRUCTOR
Matrix4x4::~Matrix4x4()
{
}

//MEMBER FUNCTION
Vector4 Matrix4x4::getRow(int row) const
{
	return Vector4(matrix[row][0], matrix[row][1], matrix[row][2], matrix[row][3]);
}

Vector4 Matrix4x4::getColumn(int column) const
{
	return Vector4(matrix[0][column], matrix[1][column], matrix[2][column], matrix[3][column]);
}

scalar Matrix4x4::getElementAt(int row, int column) const
{
	return matrix[row][column];
}

void Matrix4x4::setRow(int row, const Vector4 &vector)
{
	matrix[row][0] = vector.getX();
	matrix[row][1] = vector.getY();
	matrix[row][2] = vector.getZ();
	matrix[row][3] = vector.getW();
}

void Matrix4x4::setColumn(int column, const Vector4 &vector)
{
	matrix[0][column] = vector.getX();
	matrix[1][column] = vector.getY();
	matrix[2][column] = vector.getZ();
	matrix[3][column] = vector.getW();
}

void Matrix4x4::setElementAt(int row, int column, scalar value)
{
	matrix[row][column] = value;
}

Matrix4x4 Matrix4x4::inverse()
{
	int determinant = this->determinant();
	if (determinant != 0.0)
	{
		Matrix4x4 tmpMatrix = coFactor();
		for (int index = 0; index < 16; ++index)
		{
			int element = tmpMatrix.getElementAt((index / 4), (index % 4));
			matrix[index / 4][index % 4] = element / determinant;
		}
	}
	return *this;
}

Matrix4x4 Matrix4x4::transpose()
{
	for (int index = 0; index < 16; ++index)
	{
		scalar elementTmp = matrix[index / 4][index % 4];
		matrix[index / 4][index % 4] = matrix[index % 4][index / 4];
		matrix[index % 4][index / 4] = elementTmp;
	}
	return *this;
}

bool Matrix4x4::isIdentity(const Matrix4x4 &other) const
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
	Matrix4x4 result = *this;
	return result *= other;
}

Matrix4x4& Matrix4x4::operator *=(const Matrix4x4 &other)
{
	for (int index = 0; index < 16; ++index)
	{
		scalar element = this->getRow(index / 4).dot(other.getColumn(index % 4));
		matrix[index / 4][index % 4] = element;
	}

	return *this;
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
	Vector4 v2(0,-sine, cosine, 0);
	Vector4 p0(0, 0, 0, 1);
	
	return Matrix4x4(v0, v1, v2, p0);
}

Matrix4x4 Matrix4x4::rotationY(scalar degree)
{
	float rad = degree * M_PI / 180.0f;

	scalar cosine = cos(rad);
	scalar sine = sin(rad);

	Vector4 v0(cosine, 0,-sine, 0);
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

	Vector4 v0( cosine, sine, 0, 0);
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

Matrix4x4 Matrix4x4::coFactor() const
{
	Matrix4x4 tmpMatrix;

	for (int index = 0; index < 16; ++index)
	{
		int element =	matrix[((index / 4) + 1) % 3][((index % 4) + 1) % 3] * 
						matrix[((index / 4) + 2) % 3][((index % 4) + 2) % 3] - 
						matrix[((index / 4) + 1) % 3][((index % 4) + 2) % 3] * 
						matrix[((index / 4) + 2) % 3][((index % 4) + 1) % 3];

		tmpMatrix.setElementAt(index / 4, index % 4, element);
	}
	return tmpMatrix;
}

double Matrix4x4::determinant() const
{
	return	(matrix[0][0] * matrix[1][1] * matrix[2][2]) + 
			(matrix[0][1] * matrix[1][2] * matrix[2][0]) + 
			(matrix[0][2] * matrix[1][0] * matrix[2][1]) -
			(matrix[0][0] * matrix[1][2] * matrix[2][1]) - 
			(matrix[0][1] * matrix[1][0] * matrix[2][2]) - 
			(matrix[0][2] * matrix[1][1] * matrix[2][0]);
}
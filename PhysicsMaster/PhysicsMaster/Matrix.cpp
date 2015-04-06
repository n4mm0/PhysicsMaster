#include "Matrix.h"

void MatrixOp::RotateToWorldSpace(Matrix<3, 3>& Matrix, const Vector3& vector, Vector3& result)
{
	float x = Matrix.GetElementAt(0) * vector.GetX() + Matrix.GetElementAt(3) * vector.GetY() + Matrix.GetElementAt(6) * vector.GetZ();
	float y = Matrix.GetElementAt(1) * vector.GetX() + Matrix.GetElementAt(4) * vector.GetY() + Matrix.GetElementAt(7) * vector.GetZ();
	float z = Matrix.GetElementAt(2) * vector.GetX() + Matrix.GetElementAt(5) * vector.GetY() + Matrix.GetElementAt(8) * vector.GetZ();
	result.SetX(x);
	result.SetY(y);
	result.SetZ(z);
}

void MatrixOp::RotateToObjectSpace(Matrix<3, 3>& Matrix, const Vector3& vector, Vector3& result)
{
	float x = Matrix.GetElementAt(0) * vector.GetX() + Matrix.GetElementAt(1) * vector.GetY() + Matrix.GetElementAt(2) * vector.GetZ();
	float y = Matrix.GetElementAt(3) * vector.GetX() + Matrix.GetElementAt(4) * vector.GetY() + Matrix.GetElementAt(5) * vector.GetZ();
	float z = Matrix.GetElementAt(6) * vector.GetX() + Matrix.GetElementAt(7) * vector.GetY() + Matrix.GetElementAt(8) * vector.GetZ();
	result.SetX(x);
	result.SetY(y);
	result.SetZ(z);
}

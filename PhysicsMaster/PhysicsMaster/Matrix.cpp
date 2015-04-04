#include "Matrix.h"

template<int Row, int Col>
Matrix::Matrix(){
	for (int i = 0; i < Row*Col; ++i)
	{
		_matrix[i] = 0;
	}
}

template<int Row, int Col>
Matrix::Matrix(const Matrix<Row, Col>& other)
{
	for (int i = 0; i < Row*Col; ++i)
	{
		_matrix[i] = other.GetElementAt(i);
	}
}

template<int Row, int Col>
Matrix::Matrix<Row, Col>& operator=(const Matrix<Row, Col> other)
{
	if (this != &other)
	{
		for (int i = 0; i < Row*Col; i++)
			_matrix[i] = other.GetElementAt(i);
	}
	return *this;
}

template<int Row, int Col>
float Matrix::GetElementAt(int i, int j) const
{
	return _matrix[i*Col + j];
};

template<int Row, int Col>
float Matrix::GetElementAt(int i) const
{
	return _matrix[i];
};

template<int Row, int Col>
void Matrix::SetElementAt(int i, float val)
{
	_matrix[i] = val;
};

template<int Row, int Col>
void Matrix::SetElementAt(int i, int j, float val)
{
	_matrix[i*Col + j] = val;
};

template<int Row, int Col>
void Matrix::SetRow(int Nrow, float* value)
{
	for (int i = 0; i < Col; ++i)
	{
		_matrix[Nrow*Col + i] = value[i];
	}
};

template<int Row, int Col, int OtherDim>
float Matrix::MultiplyRowCol(int Nrow, int Ncol, const Matrix<Col, OtherDim>& secondMatrix) const
{
	float result = 0;
	const float* row = GetRow(Nrow);
	const float* col = secondMatrix.GetCol(Ncol);
	for (int i = 0; i < Col; ++i)
	{
		result += row[i] * col[i*Col];
	}
	return result;
};

template<int Row, int Col, int secondMatrixCol>
MatrixOp::MultiplyMatrix(const Matrix<Row, Col>& first, const Matrix<Col, secondMatrixCol>& second, Matrix<Row, secondMatrixCol>& result)
{
	float rowR[secondMatrixCol];
	for (int i = 0; i < Row; ++i)
	{
		for (int j = 0; j < secondMatrixCol; ++j)
		{
			rowR[j] = first.MultiplyRowCol(i, j, second);
		}
		result.SetRow(i, rowR);
	}
}

template<int Row, int Col>
MatrixOp::RotateToWorldSpace(Matrix<Row, Col>& Matrix, const Vector3& vector, Vector3& result)
{
	float x = Matrix.GetElementAt(0) * vector.getX() + Matrix.GetElementAt(3) * vector.getY() + Matrix.GetElementAt(6) * vector.getZ();
	float y = Matrix.GetElementAt(1) * vector.getX() + Matrix.GetElementAt(4) * vector.getY() + Matrix.GetElementAt(7) * vector.getZ();
	float z = Matrix.GetElementAt(2) * vector.getX() + Matrix.GetElementAt(5) * vector.getY() + Matrix.GetElementAt(8) * vector.getZ();
	result.SetX(x);
	result.SetY(y);
	result.SetZ(z);
}

template<int Row, int Col>
MatrixOp::RotateToObjectSpace(Matrix<Row, Col>& Matrix, const Vector3& vector, Vector3& result)
{
	float x = Matrix.GetElementAt(0) * vector.getX() + Matrix.GetElementAt(1) * vector.getY() + Matrix.GetElementAt(2) * vector.getZ();
	float y = Matrix.GetElementAt(3) * vector.getX() + Matrix.GetElementAt(4) * vector.getY() + Matrix.GetElementAt(5) * vector.getZ();
	float z = Matrix.GetElementAt(6) * vector.getX() + Matrix.GetElementAt(7) * vector.getY() + Matrix.GetElementAt(8) * vector.getZ();
	result.SetX(x);
	result.SetY(y);
	result.SetZ(z);
}
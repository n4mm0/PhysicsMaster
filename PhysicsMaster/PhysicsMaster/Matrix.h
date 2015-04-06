#pragma once
#include <math.h>
#include "Vector3.h"
//Debug Only
#include <iostream>

template<int Row, int Col>
class Matrix
{
	template<int, int>
	friend class Matrix;			// We need this for calling Matrix in non-member functions
public:

	Matrix()
	{
		for (int i = 0; i < Row*Col; ++i)
		{
			_matrix[i] = 0;
		}
	}

	Matrix(const Matrix<Row, Col>& other)
	{
		for (int i = 0; i < Row*Col; ++i)
		{
			_matrix[i] = other.GetElementAt(i);
		}
	}

	Matrix<Row, Col>& operator=(const Matrix<Row, Col> other)
	{
		if (this != &other)
		{
			for (int i = 0; i < Row*Col; i++)
				_matrix[i] = other.GetElementAt(i);
		}
		return *this;
	}

	template<int OtherDim>
	float MultiplyRowCol(int Nrow, int Ncol, const Matrix<Col, OtherDim>& secondMatrix) const
	{
		float result = 0;
		const float* row = GetRow(Nrow);
		const float* col = secondMatrix.GetCol(Ncol);
		for (int i = 0; i < Col; ++i)
		{
			result += row[i] * col[i*Col];
		}
		return result;
	}

	// GETTERS
	float GetElementAt(int i, int j) const
	{
		return _matrix[i*Col + j];
	}

	float GetElementAt(int i) const
	{
		return _matrix[i];
	}

	// SETTERS
	void SetElementAt(int i, float val)
	{
		_matrix[i] = val;
	}

	void SetElementAt(int i, int j, float val)
	{
		_matrix[i*Col + j] = val;
	}

	void SetRow(int Nrow, float* value)
	{
		for (int i = 0; i < Col; ++i)
		{
			_matrix[Nrow*Col + i] = value[i];
		}
	}

private:

	const float* GetRow(int i) const
	{
		return &_matrix[i*Col];
	}

	const float* GetCol(int i)  const
	{
		return &_matrix[i];
	}

	float _matrix[Row*Col];
};

namespace MatrixOp 
{
	template<int Row, int Col, int secondMatrixCol>
	void MultiplyMatrix(const Matrix<Row, Col>& first, const Matrix<Col, secondMatrixCol>& second, Matrix<Row, secondMatrixCol>& result)
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

	//NOTE: Only works for 3x3 matrixes.
	void RotateToWorldSpace(Matrix<3, 3>& Matrix, const Vector3& vector, Vector3& result);

	//NOTE: Only works for 3x3 matrixes
	void RotateToObjectSpace(Matrix<3, 3>& Matrix, const Vector3& vector, Vector3& result);
}
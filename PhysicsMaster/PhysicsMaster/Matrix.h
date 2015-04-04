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

	Matrix();
	Matrix(const Matrix<Row, Col>& other);
	Matrix<Row, Col>& operator=(const Matrix<Row, Col> other);
	template<int OtherDim>
	float MultiplyRowCol(int Nrow, int Ncol, const Matrix<Col, OtherDim>& secondMatrix) const;

	// GETTERS
	float GetElementAt(int i, int j) const;
	float GetElementAt(int i) const;
	// SETTERS
	void SetElementAt(int i, float val);
	void SetElementAt(int i, int j, float val);
	void SetRow(int Nrow, float* value);

private:

	const float* GetRow(int i) const;
	const float* GetCol(int i)  const;

	float _matrix[Row*Col];
};

namespace MatrixOp 
{
	template<int Row, int Col, int secondMatrixCol>
	void MultiplyMatrix(const Matrix<Row, Col>& first, const Matrix<Col, secondMatrixCol>& second, Matrix<Row, secondMatrixCol>& result);

	template<int Row, int Col>
	void RotateToWorldSpace(Matrix<Row, Col>& Matrix, const Vector3& vector, Vector3& result);

	template<int Row, int Col>
	void RotateToObjectSpace(Matrix<Row, Col>& Matrix, const Vector3& vector, Vector3& result);
}
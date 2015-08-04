#ifndef MATRIX_H
#define MATRIX_H

#include <sstream>

class Matrix {
	unsigned int nRows;
	unsigned int nColumns;
	double **matrix;
	void initiateIdentity();
	void initiateZero();
	void tearDown();
public:
	Matrix();
	explicit Matrix (unsigned int nRows);
	Matrix(unsigned int nRows, unsigned int nColumns);
	Matrix(const Matrix &other);
	~Matrix();
	bool isValid() const;
	unsigned int getNRows() const;
	unsigned int getNColumns() const;
	double getElement(unsigned int row, unsigned int column) const;
	void setElement(unsigned int row, unsigned int column, double value);
	std::string toString() const;
	friend std::ostream& operator <<(std::ostream &out, const Matrix &rhs);
	Matrix& operator=(const Matrix &rhs);
	Matrix& operator+=(const Matrix &rhs);
	Matrix& operator-=(const Matrix &rhs);
	Matrix& operator*=(const Matrix &rhs);
	Matrix operator+(const Matrix &rhs) const;
	Matrix operator-(const Matrix &rhs) const;
	Matrix operator*(const Matrix &rhs) const;
};

#endif

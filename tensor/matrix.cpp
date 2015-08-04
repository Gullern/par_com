#include <iostream>
#include "matrix.h"

using namespace std;

Matrix::Matrix() : nRows(0), nColumns(0), matrix(nullptr) {

}

Matrix::Matrix (unsigned int nRowsValue) : Matrix(nRowsValue, nRowsValue) {

}

Matrix::Matrix(unsigned int nRowsValue, unsigned int nColumnsValue) : nRows(nRowsValue), nColumns(nColumnsValue), matrix(nullptr) {
	initiateZero();
}

Matrix::Matrix(const Matrix &other) : Matrix() {
	*this = other;
}

Matrix::~Matrix() {
	this->tearDown();
}

void Matrix::initiateIdentity() {
	matrix = new double*[nRows];
	for (unsigned int i = 0; i < nRows; i++) {
		matrix[i] = new double[nColumns];
		for (unsigned int j = 0; j < nColumns; j++) {
			matrix[i][j] = (i == j);
		}
	}
}

void Matrix::initiateZero() {
	matrix = new double*[nRows];
	for (unsigned int i = 0; i < nRows; i++) {
		matrix[i] = new double[nColumns];
		for (unsigned int j = 0; j < nColumns; j++) {
			matrix[i][j] = 0;
		}
	}
}

void Matrix::tearDown() {
	for (unsigned int i = 0; i < nRows; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;

	nRows = 0;
	nColumns = 0;
	matrix = nullptr;
}

bool Matrix::isValid() const {
	return matrix != nullptr;
}

unsigned int Matrix::getNRows() const {
	return nRows;
}

unsigned int Matrix::getNColumns() const {
	return nColumns;
}

double Matrix::getElement(unsigned int row, unsigned int column) const {
	if (row >= nRows) {
		cout << "Invalid row: ";
		if (! isValid()) { cout << "Matrix empty!\n"; }
		else { cout << row << " of " << nRows - 1 << "!\n"; }
		throw 1;
	}
	if (column >= nColumns) {
		cout << "Invalid column: ";
		if (! isValid()) { cout << "Matrix empty!\n"; }
		else { cout << column << " of " << nColumns - 1 << "!\n"; }
		throw 1;
	}

	return this->matrix[row][column];
}

void Matrix::setElement(unsigned int row, unsigned int column, double value) {
	if (row >= nRows) {
		cout << "Invalid row: ";
		if (! isValid()) { cout << "Matrix empty!\n"; }
		else { cout << row << " of " << nRows - 1 << "!\n"; }
		throw 1;
	}
	if (column >= nColumns) {
		cout << "Invalid column: ";
		if (! isValid()) { cout << "Matrix empty!\n"; }
		else { cout << column << " of " << nColumns - 1 << "!\n"; }
		throw 1;
	}

	this->matrix[row][column] = value;
}

string Matrix::toString() const {
	if (! isValid()) {
		return "Empty matrix";
	}

	string output = "";
	stringstream ss;
	for (unsigned int i = 0; i < nRows; i++) {
		for (unsigned int j = 0; j < nColumns; j++) {
			ss << matrix[i][j];
			output += ss.str();
			ss.str("");
			if (j < nColumns - 1) {
				output += ", ";
			}
		}
		output += "\n";
	}
	return output;
}

ostream& operator <<(ostream &out, const Matrix &rhs) {
	out << rhs.toString() << endl;
	return out;
}

Matrix& Matrix::operator=(const Matrix &rhs) {
	if (isValid() && (this->nRows != rhs.nRows || this->nColumns != rhs.nColumns)) {
		tearDown();
	}

	if (! rhs.isValid()) {
		return *this;
	}

	this->nRows = rhs.nRows;
	this->nColumns = rhs.nColumns;

	initiateZero();

	for (unsigned int i = 0; i < nRows; i++) {
		for (unsigned int j = 0; j < nColumns; j++) {
			this->matrix[i][j] = rhs.matrix[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator+=(const Matrix &rhs) {
	if (this->nRows != rhs.nRows || this->nColumns != rhs.nColumns) {
		cout << "Unequal dimensions: "
				<< this->nRows << "x" << this->nColumns << " vs. "
				<< rhs.nRows << "x" << rhs.nColumns << "!\n";
		throw 1;
	}

	for (unsigned int i = 0; i < nRows; i++) {
		for (unsigned int j = 0; j < nColumns; j++) {
			this->matrix[i][j] += rhs.matrix[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix &rhs) {
	if (this->nRows != rhs.nRows || this->nColumns != rhs.nColumns) {
		cout << "Unequal dimensions: "
				<< this->nRows << "x" << this->nColumns << " vs. "
				<< rhs.nRows << "x" << rhs.nColumns << "!\n";
		throw 1;
	}

	for (unsigned int i = 0; i < nRows; i++) {
		for (unsigned int j = 0; j < nColumns; j++) {
			this->matrix[i][j] -= rhs.matrix[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator*=(const Matrix &rhs) {
	if (this->nColumns != rhs.nRows) {
		cout << "Matrix multiplication undefined: "
				<< this->nRows << "x" << this->nColumns << " * "
				<< rhs.nRows << "x" << rhs.nColumns << "!\n";
		throw 1;
	}

	unsigned int rows = this->nRows;
	unsigned int columns = rhs.nColumns;
	unsigned int iterations = this->nColumns;

	Matrix result(rows, columns);

	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < columns; j++) {
			double element = 0;
			for (unsigned int k = 0; k < iterations; k++) {
				element += matrix[i][k] * rhs.matrix[k][j];
			}
			result.matrix[i][j] = element;
		}
	}
	return *this = result;
}

Matrix Matrix::operator+(const Matrix &rhs) const {
	return Matrix(*this) += rhs;
}

Matrix Matrix::operator-(const Matrix &rhs) const {
	return Matrix(*this) -= rhs;
}

Matrix Matrix::operator*(const Matrix &rhs) const {
	return Matrix(*this) *= rhs;
}

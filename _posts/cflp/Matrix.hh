/* Matrix.hh
 *
 * Created by Lau,KwanYuen on 2018/12/24.
 * Copyright© 2018年 Lau,KwanYuen. All rights reserved.
 */

#ifndef _MATRIX_HH_
#define _MATRIX_HH_

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#define DIM 3		// default value of row and col

template <typename T>
class Matrix {
public:
	Matrix(const int = DIM, const int = DIM, const T = T());
	Matrix(const Matrix<T>&);
	Matrix<T>& operator=(const Matrix<T>&);
	~Matrix() = default;
	template <typename t>
	Matrix(const std::vector<std::vector<t>>&);

	int getRow() const;
	int getCol() const;
	std::vector<T>& at(const int);
	std::vector<T>& operator[](const int);

	Matrix<T> inverse() const;
	Matrix<T> transpose() const;

	template <typename t>
	Matrix<T> time(const t) const;
	template <typename t>
	Matrix<T> time(const Matrix<t>&) const;
	template <typename t>
	Matrix<T> dot(const Matrix<t>&) const;
	template <typename t>
	Matrix<T> operator*(const Matrix<t>&) const;
	template <typename t>
	Matrix<T> operator+(const Matrix<t>&) const;
	template <typename t>
	Matrix<T> operator-(const Matrix<t>&) const;

	static Matrix<T> diagonal(int = DIM);

	template <typename t>
	friend std::ostream& operator<<(std::ostream&, const Matrix<t>&);
	template <typename t>
	friend std::ostream& operator<<(std::ostream&, const std::vector<t>&);

private:
	int row, col;
	std::vector<std::vector<T>> data;
};

#include <Matrix.cc>

#endif
#pragma once
#include <initializer_list>

template<class T = double>
class Matrix
{
private:
	T** _matrix;
	int _width;
	int _height;

	void Init(const int& width, const int& height);
	void Destroy();
	void Copy(const Matrix& other);
public:
	Matrix() :_matrix(nullptr), _width(0), _height(0) {}
	Matrix(const int& width, const int& height) :_width(width), _height(height) { Init(width, height); }
	Matrix(const Matrix<T>& other);
	Matrix(const std::initializer_list<std::initializer_list<T>>& lst);

	~Matrix();

	Matrix<T>& operator=(const Matrix& other);

	Matrix<T> operator+(const Matrix& other) const;
	Matrix<T>& operator+=(const Matrix& other);
	Matrix<T> operator-(const Matrix& other) const;
	Matrix<T>& operator-=(const Matrix& other);
	Matrix<T> operator*(const Matrix& other) const;
	Matrix<T>& operator*=(const Matrix& other);
	T* operator[](const int& index);

	friend std::ostream& operator<<(ostream& os, Matrix m);

	//transposes array
	Matrix<T>& Transpose();

	/// <summary>
	/// Creates new transposed array from acutal one
	/// </summary>
	/// <returns></returns>
	Matrix<T> CreateTransposed() const;


	int GetRows() const { return _height; }
	int GetCols() const { return _width; }

	bool IsError() { return _width < 0 || _height < 0; }
};

#include "Matrix.h"

template <class T>
void Matrix<T>::Init(const int& width, const int& height)
{
	_matrix = new T*[height];
	for (int x = 0; x < height; x++)
		_matrix[x] = new T[width];
}

template<class T>
void Matrix<T>::Destroy()
{
	for (int x = 0; x < _height; x++)
		delete[] _matrix[x];
	delete[] _matrix;

	_matrix = nullptr;
}

template<class T>
inline void Matrix<T>::Copy(const Matrix& other)
{
	for (int x = 0; x < _height; x++)
		for (int y = 0; y < _width; y++)
			_matrix[x][y] = other._matrix[x][y];
}

template<class T>
Matrix<T>::Matrix(const Matrix& other)
{
	_width = other._width;
	_height = other._height;

	Init(_width, _height);

	//copy memory
	Copy(other);
}

template<class T>
inline Matrix<T>::Matrix(const std::initializer_list<std::initializer_list<T>>& lst)
{
	_height = lst.size();
	_width = lst.begin()->size();

	Init(_width, _height);
	int x = 0, y = 0;
	for (auto& l : lst)
	{
		for (auto& ly : l)
		{
			_matrix[x][y] = ly;
			y++;
		}
		x++;
		y = 0;
	}
}

template <class T>
Matrix<T>::~Matrix()
{
	Destroy();
}

template<class T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other)
{
	//self assignemnt guard
	if (this == &other)
		return *this;

	Destroy();

	_width = other._width;
	_height = other._height;

	Init(_width, _height);
	Copy(other);
}

template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix& other) const
{
	if (_width != other._width || _height != other._height)
		return Matrix<T>();

	Matrix<T> output = Matrix<T>(_width, _height);

	for (int x = 0; x < _height; x++)
		for (int y = 0; y < _width; y++)
			output[x][y] = _matrix[x][y] + other._matrix[x][y];

	return output;
}

template<class T>
Matrix<T>& Matrix<T>::operator+=(const Matrix& other)
{
	if (_width != other._width || _height != other._height)
		return *this;

	for (int x = 0; x < _height; x++)
		for (int y = 0; y < _width; y++)
			_matrix[x][y] += other._matrix[x][y];

	return *this;
}

template<class T>
Matrix<T> Matrix<T>::operator-(const Matrix& other) const
{
	if (_width != other._width || _height != other._height)
		return Matrix<T>();

	Matrix<T> output = Matrix<T>(_width, _height);

	for (int x = 0; x < _height; x++)
		for (int y = 0; y < _width; y++)
			output[x][y] = _matrix[x][y] - other._matrix[x][y];

	return output;
}

template<class T>
Matrix<T>& Matrix<T>::operator-=(const Matrix& other)
{
	if (_width != other._width || _height != other._height)
		return Matrix<T>();

	for (int x = 0; x < _height; x++)
		for (int y = 0; y < _width; y++)
			_matrix[x][y] -= other._matrix[x][y];

	return *this;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const Matrix& other) const
{
	//multiplication is only allowed for matrices with the same row count
	if (_width != other._height)
		return Matrix<T>();

	Matrix<T> output(_width, other._height);


	int row = 0, col = 0;

	for (int x = 0; x < other._height; x++)
	{
		for (int y = 0; y < _width; y++)
		{
			int add = 0;
			for (int z = 0; z < other._height; z++)
				add += _matrix[x][z] * other._matrix[y][z];
			output[x][y] = add;
		}

	}

	return output;
}

template<class T>
Matrix<T>& Matrix<T>::operator*=(const Matrix& other)
{
	return *this * other;
}

template<class T>
T* Matrix<T>::operator[](const int& index)
{
	return _matrix[index];
}

template<class T>
Matrix<T>& Matrix<T>::Transpose()
{
	*this = CreateTransposed();

	return *this;
}

template<class T>
Matrix<T> Matrix<T>::CreateTransposed() const
{
	Matrix<T> transpose = Matrix<T>(_height, _width);

	for (int x = 0; x < _height; x++)
		for (int y = 0; y < _width; y++)
			transpose[y][x] = _matrix[x][y];

	return transpose;
}


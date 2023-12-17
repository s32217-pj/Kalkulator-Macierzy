#pragma once
#include <initializer_list>
#include <iostream>
#include <iomanip>

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

	/// <summary>
	/// Changes dimensions of matrix, if dimmensions are smaller, data is lost
	/// If Dimensions are higher, zeroes are added
	/// </summary>
	/// <returns>Matrix with changed dimensions</returns>
	Matrix<T>& ChangeDimensions(const int& new_width, const int& new_height);

	/// <summary>
	/// Creates copy of a matrix with changed dimensions, if dimmensions are smaller, data is lost
	/// If Dimensions are higher, zeroes are added 
	/// </summary>
	/// <returns>Copy of a matrix with changed dimensions</returns>
	Matrix<T> ChangeDimensionsCopy(const int& new_width, const int& new_height);

	//transposes matrix
	Matrix<T>& Transpose();

	/// <summary>
	/// Creates new transposed matrix from acutal one
	/// </summary>
	/// <returns></returns>
	Matrix<T> CreateTransposed() const;


	int GetRows() const { return _height; }
	int GetCols() const { return _width; }

	bool IsError() const { return _width <= 0 || _height <= 0; }

	//output stream overload
	friend std::ostream& operator<<(std::ostream& os, Matrix<T>& m)
	{
		for (int x = 0; x < m.GetRows(); x++)
		{
			for (int y = 0; y < m.GetCols(); y++)
			{
				os << std::setw(3);
				os << m[x][y];
			}
			os << "\n";
		}

		return os;
	}
};

template <class T>
void Matrix<T>::Init(const int& width, const int& height)
{
	_matrix = new T*[height];
	for (int x = 0; x < height; x++)
		_matrix[x] = new T[width]{ 0 }; //create filled with zeroes

	_width = width;
	_height = height;
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
inline Matrix<T>& Matrix<T>::ChangeDimensions(const int& new_width, const int& new_height)
{
	if (new_width <= 0 || new_height <= 0)
		return *this; //do nothing

	Matrix<T> copy = *this;
	Destroy();
	Init(new_width, new_height);

	for (int x = 0; x < copy.GetRows() && x < new_height; x++)
		for (int y = 0; y < copy.GetCols() && y < new_width; y++)
			(*this)[x][y] = copy[x][y];

	return *this;
}

template<class T>
inline Matrix<T> Matrix<T>::ChangeDimensionsCopy(const int& new_width, const int& new_height)
{
	if (new_width <= 0 || new_height <= 0)
		return Matrix(); //do nothing

	Matrix<T> copy = Matrix(new_width, new_height);

	for (int x = 0; x < new_height; x++)
		for (int y = 0; y < new_width; y++)
			copy[x][y] = (*this)[x][y];

	return copy;
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


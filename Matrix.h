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

	//support Iterators
	class Iterator;
	Iterator begin() { return Iterator(&_matrix[0][0]); }
	Iterator end() { return Iterator(&_matrix[_height][_width-1]); }



	//transposes matrix
	Matrix<T>& Transpose();

	/// <summary>
	/// Creates new transposed matrix from acutal one
	/// </summary>
	/// <returns></returns>
	Matrix<T> CreateTransposed() const;


	int GetRows() const { return _height; }
	int GetCols() const { return _width; }

	bool IsError() const { return _width < 0 || _height < 0; }

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

template<class T>
class Matrix<T>::Iterator
{
private:
	T* m_iterator;
public:

	using value_type = T;
	using reference = T&;
	using pointer = T*;
	using Iterator_category = std::random_access_iterator_tag;
	using difference_type = ptrdiff_t;

	//przepraszam za skopiowanie tego wszystkiego, ale nie chcialo mi sie tego po prostu pisac samemu od poczatku
	//nie jest to pierwszy raz kiedy pisze taki Iterator

	constexpr Iterator(T* iter = nullptr) : m_iterator{ iter } {}

	constexpr bool operator==(const Iterator& other) const noexcept { return m_iterator == other.m_iterator; }
	constexpr bool operator!=(const Iterator& other) const noexcept { return m_iterator != other.m_iterator; }
	constexpr reference operator*() const noexcept { return *m_iterator; }
	constexpr pointer operator->() const noexcept { return m_iterator; }
	constexpr Iterator& operator++() noexcept { ++m_iterator; return *this; }
	constexpr Iterator operator++(int) noexcept { Iterator tmp(*this); ++(*this); return tmp; }
	constexpr Iterator& operator--() noexcept { --m_iterator; return *this; }
	constexpr Iterator operator--(int) noexcept { Iterator tmp(*this); --(*this); return tmp; }
	constexpr Iterator& operator+=(const difference_type other) noexcept { m_iterator += other; return *this; }
	constexpr Iterator& operator-=(const difference_type other) noexcept { m_iterator -= other; return *this; }
	constexpr Iterator operator+(const difference_type other) const noexcept { return Iterator(m_iterator + other); }
	constexpr Iterator operator-(const difference_type other) const noexcept { return Iterator(m_iterator - other); }
	constexpr Iterator operator+(const Iterator& other) const noexcept { return Iterator(*this + other.m_iterator); }
	constexpr difference_type operator-(const Iterator& other) const noexcept { return std::distance(m_iterator, other.m_iterator); }
	constexpr reference operator[](std::size_t index) const { return m_iterator[index]; }
	constexpr bool operator<(const Iterator& other) const noexcept { return m_iterator < other.m_iterator; }
	constexpr bool operator>(const Iterator& other) const noexcept { return m_iterator > other.m_iterator; }
	constexpr bool operator<=(const Iterator& other) const noexcept { return m_iterator <= other.m_iterator; }
	constexpr bool operator>=(const Iterator& other) const noexcept { return m_iterator >= other.m_iterator; }
};

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


#include "Matrix.h"

template <typename T>
class GridGraph {
public:
	GridGraph(int size, const T& val): _size(size), _data(size, size, val) {}

private:
	Matrix<T> _data;
	int _size;
};

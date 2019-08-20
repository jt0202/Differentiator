#pragma once
#include <vector>
#include <memory>


template<class T>
class SmartPointerVector
{
private:
	std::vector<std::shared_ptr<T>> vector;

public:
	SmartPointerVector();

	T* at(int pos);
};



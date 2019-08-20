#pragma once
#include <vector>
#include <memory>

/*
This class is an adapter for the usuage of smart pointers. It provides functions as a regular std::vector
, but instead of performing then on the smart pointer(here a shared_ptr) at performs then at the actual
pointer inside the smart pointer. It is not a subclass of std::vector, because I only want to use the 
methods that I have changed and not by accident use wrong object. 
*/

template<typename T>
class SmartPointerVector
{
private:
	std::vector<std::shared_ptr<T>> vector;

public:
	T* at(size_t pos);

	void push_back(T* value);

	size_t size();

	auto begin() noexcept;

	auto end() noexcept;

	std::vector<std::shared_ptr<T>> getVector();
};

template<typename T>
T* SmartPointerVector<T>::at(size_t pos)
{
	return vector.at(pos).get();
}

template<typename T>
void SmartPointerVector<T>::push_back(T* value)
{
	vector.emplace_back().reset(value);
}

template<typename T>
size_t SmartPointerVector<T>::size()
{
	return vector.size();
}

template<typename T>
auto SmartPointerVector<T>::begin() noexcept
{
	return vector.begin();
}

template<typename T>
auto SmartPointerVector<T>::end() noexcept
{
	return vector.end();
}

template<typename T>
std::vector<std::shared_ptr<T>> SmartPointerVector<T>::getVector()
{
	return vector;
}

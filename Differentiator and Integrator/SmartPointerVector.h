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
	T* at(size_t pos) const;

	void push_back(T* value);

	size_t size() const;

	auto begin() const;

	auto end() const;

	std::vector<std::shared_ptr<T>> getVector() const;

	void replace(T* value, int pos);

	void erase(int pos);
};

template<typename T>
T* SmartPointerVector<T>::at(size_t pos) const
{
	return vector.at(pos).get();
}

template<typename T>
void SmartPointerVector<T>::push_back(T* value)
{
	vector.emplace_back().reset(value);
}

template<typename T>
size_t SmartPointerVector<T>::size() const
{
	return vector.size();
}

template<typename T>
auto SmartPointerVector<T>::begin() const
{
	return vector.begin();
}

template<typename T>
auto SmartPointerVector<T>::end() const
{
	return vector.end();
}

template<typename T>
std::vector<std::shared_ptr<T>> SmartPointerVector<T>::getVector() const
{
	return vector;
}

template<typename T>
void SmartPointerVector<T>::replace(T* value, int pos)
{
	if (pos < vector.size() && pos >= 0)
	{
		if (vector[pos].get() == value)
		{
			return;
		}

		vector[pos].reset(value);
	}
}

template<typename T>
void SmartPointerVector<T>::erase(int pos)
{
	if (pos < vector.size() && pos >= 0)
	{
		vector.erase(vector.begin() + pos);
	}
}
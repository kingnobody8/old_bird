#pragma once
#include <string>
using namespace std;

template<typename Param>
struct Base
{
	virtual void Foo(Param) = 0;
};

template<typename Param>
struct Child : public Base < Param >
{
	virtual void Foo(Param p)
	{
	}
};

template<typename T>
struct ReturnValue { };

// Specialization for type int. Implements 'T generate()'
// for T == int.
template<>
struct ReturnValue<int> 
{
	static int value() { return 17; }
};

// Specialization for type float. Implements 'T generate()'
// for T == float.
template<>
struct ReturnValue<float>
{
	static float value() { return 3.14159f; }
};

// Specialization for type std::string. Implements
// 'T generate()' for T == std::string.
template<>
struct ReturnValue<std::string> 
{
	static std::string value() { return "foo"; }
};

struct ReturnType 
{
	template<typename T>
	operator T() 
	{
		return ReturnValue<T>::value();
	}
};
ReturnType generate() {
	return ReturnType();
}

//template<>
//struct Child<void> : public Base<void>
//{
//	virtual void Foo(void)
//	{
//
//	}
//};
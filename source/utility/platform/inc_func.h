#pragma once
#include "platform/platform.h"
#include <functional>

//NOTE this used to include the boost library but now instead just uses std::functional
//just keep that in mind

//#ifdef WIN
//#include <boost/function.hpp>
//#include <boost/bind.hpp>
//#elif MAC
//#elif IOS_SIM
//#include <boost/function.hpp>
//#include <boost/bind.hpp>
//#elif IOS_DEV
//#include <boost/function.hpp>
//#include <boost/bind.hpp>
//#endif

#define BIND0(that, func) (std::bind(func, that))
#define BIND1(that, func) (std::bind(func, that, std::placeholders::_1))
#define BIND2(that, func) (std::bind(func, that, std::placeholders::_1, std::placeholders::_2))
#define BIND3(that, func) (std::bind(func, that, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
#define BIND4(that, func) (std::bind(func, that, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))

/*
-Examples of Boost Function Pointers-

boost::function<ReturnType(Param)> variable;

//Function
void Foo(void)
{
}

//Functor
class Foo
{
	void operator () (void);
}

//Lamba
[captures_go_here](void)
{
}

//Member Function
class Foo
{
	void Foo_func(void);
}
*/
//// --------------------------------------------------------------------------------------------- //
//// LuaBind example
////
//// This example project for Microsoft(tm) Visual C++ 2008(tm) users
//// compiles out-of-the-box. Precompiled binaries can be found in the
//// "References" directory in case you wish to use them for other
//// projects. Pay attention to the runtime library (multithreaded dll)
//// setting and the different library files being used for debug and release
//// builds when you do so!
//// 注意要多线程
//// --------------------------------------------------------------------------------------------- //
//
//// Include the lua headers (the extern "C" is a requirement because we're
//// using C++ and lua has been compiled as C code)
//
// Some other C++ headers we're most likely going to use
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
//#include <iostream>
#include <string>

#define luac_c
#define LUA_CORE

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

// This is the only header we need to include for LuaBind to work
#include "luabind/luabind.hpp"

// Some other C++ headers we're most likely going to use
#include <iostream>
//#include <string>

// We don't want to write std:: every time we're displaying some debug output
using namespace std;

// --------------------------------------------------------------------------------------------- //

// Put your testing code here...

// 这个里面一共有X个示例调用luabind注册C++的Function && class to lua

// 一个C++函数
void print_hello(int number) {
	cout << "hello world " << number << endl;
}

// 一个C++类
class NumberPrinter {
public:
	NumberPrinter(int number) :
	  m_number(number) {}

	  void print() {
		  cout << m_number << endl;
	  }

private:
	int m_number;
};

// 一个模板类
template<typename T>
struct Point {
	Point(T X, T Y) :
X(X), Y(Y) {}
T X, Y;
};

//一个模板的模板类
template<typename T>
struct Box {
	Box(Point<T> UpperLeft, Point<T> LowerRight) :
UpperLeft(UpperLeft), LowerRight(LowerRight) {}
Point<T> UpperLeft, LowerRight;
};

// 资源管理类
struct ResourceManager {
	ResourceManager() :
m_ResourceCount(0) {}

void loadResource(const string &sFilename) {
	++m_ResourceCount;
}
size_t getResourceCount() const {
	return m_ResourceCount;
}
size_t m_ResourceCount;
};


//注册c++的东西到lua里去都是用lua::module lua::def 的
//关键看的就是这个 module def的写法
//注册函数：luabind::module(L) [ luabind::def("print_hello", print_hello)]; 
//	估计是注册了一个lua函数
//注册类  ：luabind::module(L) [luabind::class_<NumberPrinter>("NumberPrinter") .def(luabind::constructor<int>()).def("print", &NumberPrinter::print)];
//	估计是

int main() {

	//初创建一个lua_state
	lua_State *myLuaState = lua_open();

	// Connect LuaBind to this lua state
	luabind::open(myLuaState);

	//2//注册C++函数到lua
	//void print_hello(int number) {cout << "hello world " << number << endl;} 要放全局 
	luabind::module(myLuaState) [
		luabind::def("print_hello", print_hello),

			luabind::class_<NumberPrinter>("NumberPrinter")
			.def(luabind::constructor<int>())
			.def("print", &NumberPrinter::print),

			luabind::class_<Point<float> >("Point")
			.def(luabind::constructor<float, float>())
			.def_readwrite("X", &Point<float>::X)
			.def_readwrite("Y", &Point<float>::Y),

			luabind::class_<Box<float> >("Box")
			.def(luabind::constructor<Point<float>, Point<float> >())
			.def_readwrite("UpperLeft", &Box<float>::UpperLeft)
			.def_readwrite("LowerRight", &Box<float>::LowerRight)
	];

	luaL_dostring(
		myLuaState,

		"function add(first, second)\n"
		"  return first + second\n"
		"end\n"

		"print_hello(123)\n"

		"Print2000 = NumberPrinter(2000)\n"
		"Print2000:print()\n"	

		"MyBox = Box(Point(10, 20), Point(30, 40))\n"
		"MyBox.UpperLeft.X = MyBox.LowerRight.Y\n"
		);


	cout << "Result: "<< luabind::call_function<int>(myLuaState, "add", 2, 3) << endl;  


	lua_close(myLuaState);
	getchar();
	return 0;


}


#include <iostream>
#include <string>
#include <list>
#include <tuple>

using namespace std;


// test object

class MyObj{
public:
	MyObj(){
		cout << "constructed MyObj\n";
	}
	MyObj(int a_):a(a_){
		cout << "constructed MyObj with a=" << a << "\n";
	}
	MyObj(int a_, int b_):a(a_), b(b_){
		cout << "constructed MyObj with a=" << a << ", b=" << b << "\n";
	}
	int a = 0;
	int b = 0;
	~MyObj(){
		cout << "MyObj(" << a << ", " << b << ") destroyed.\n";
	}
};


// TODO: shared and weak mechanics, copy and move stuff, const stuff


// types of handles

template<typename CONT>
class Holder{
public:
	CONT value;
	Holder(CONT value_):value(value_){}
	CONT& operator->(){return value;}
	CONT* operator *(){return &value;}
};

// TODO: we also need allowPlain functionality everywhere and an extra template parameter everywhere
template<typename CONT>
class Plain{
public:
	CONT* target;
	Plain(CONT* target_):target(target_){}
	CONT& operator->(){return *target;}
	CONT* operator *(){return target;}
};

template<typename CONT>
class Owner{
public:
	CONT* target;
	Owner(CONT* target_):target(target_){}
	~Owner(){delete target;}
	CONT& operator->(){return *target;}
	CONT* operator *(){return target;}
};

template<typename CONT>
class Weak{
public:
	CONT* target;
	Weak(CONT* target_):target(target_){}
	CONT& operator->(){return *target;}
	CONT* operator *(){return target;}
};

template<typename CONT>
class Shared{
public:
	CONT* target;
	Shared(CONT* target_):target(target_){}
	CONT& operator->(){return *target;}
	CONT* operator *(){return target;}
};



// pointable options for a container

class AllowWeak{
public:
	enum{ALLOWWEAKPTR = true};
	int weakCnt = 0; // this doesn't work like that but you know what I mean
};
class DenyWeak{
public:
	enum{ALLOWWEAKPTR = false};
};


class AllowRaw{
public:
	enum{ALLOWRAWPTR = true};
};
class DenyRaw{
public:
	enum{ALLOWRAWPTR = false};
};


// life options for a container and its object

class Value{ // container lives directly on stack, directly as a member of its handle
public:
	enum{ISREFCNT = false};
};

class Persistent{ // container lives on heap and has to be deleted manually
public:
	enum{ISREFCNT = false};
};

class Scoped{ // container lives on heap and is deleted when handle (pointer) goes out of scope
public:
	enum{ISREFCNT = false};
};

class RefCounted{ // container lives on heap and is deleted when all shared references to it are gone
public:
	enum{ISREFCNT = true};
	int useCnt = 0;
};


// TODO: cant remember this so I'll program with the defines and then replace them in the end
#define VARIADIC typename ... ARGS
#define VARARGS ARGS&&... args
#define EXPAND ARGS...
#define FORWARD std::forward<ARGS>(args)...


template<
	typename T,
	typename LIFE,
	typename WEAK,
	typename RAW
>
class Container:
	public LIFE,
	public WEAK,
	public RAW
{
public:
	T obj;

	Container(T obj_):obj(obj_){}

	auto getRawPtr(){
		static_assert(this->ALLOWRAWPTR, "raw pointer to object denied");
		return &obj;
	}

	auto getWeakPtr(){
		static_assert(this->ALLOWWEAKPTR, "weak pointer to object denied");
		return Weak(this);
	}

	auto getSharedPtr(){
		static_assert(this->ISREFCNT, "shared pointer to object denied");
		return Shared(this);
	}

	~Container(){
		cout << "container destroyed\n";
	}

	T* operator->(){return &obj;}
};




template<typename T, typename LIFE, typename WEAK, typename RAW>
class Factory{};

template<typename T, typename WEAK, typename RAW>
class Factory<T,Value,WEAK,RAW>{
public:
	template<VARIADIC>
	static auto create(VARARGS){
		return Holder( Container<T,Value,WEAK,RAW>( T(FORWARD) ) );
	}
};

template<typename T, typename WEAK, typename RAW>
class Factory<T,Persistent,WEAK,RAW>{
public:
	template<VARIADIC>
	static auto create(VARARGS){
		return Plain( new Container<T,Persistent,WEAK,RAW>( T(FORWARD) ) );
	}
};

template<typename T, typename WEAK, typename RAW>
class Factory<T,Scoped,WEAK,RAW>{
public:
	template<VARIADIC>
	static auto create(VARARGS){
		return Owner( new Container<T,Scoped,WEAK,RAW>( T(FORWARD) ) );
	}
};

template<typename T, typename WEAK, typename RAW>
class Factory<T,RefCounted,WEAK,RAW>{
public:
	template<VARIADIC>
	static auto create(VARARGS){
		return Shared( new Container<T,RefCounted,WEAK,RAW>( T(FORWARD) ) );
	}
};


int main()
{

	auto h10 = Factory<MyObj, Value, AllowRaw, AllowWeak>::create(1);
	auto h11 = (*h10)->getRawPtr();
	auto h12 = (*h10)->getWeakPtr();
	//auto h13 = (*h10)->getSharedPtr(); // compiler error

	cout << h10->b << endl;
	h11->b = 1;
	cout << h10->b << endl;
	h12->b = 2;
	cout << h10->b << endl;

	auto h20 = Factory<MyObj, Value, DenyRaw, DenyWeak>::create(2);
	//auto h21 = (*h20)->getRawPtr(); // compiler error
	//auto h22 = (*h20)->getWeakPtr(); // compiler error
	//auto h23 = (*h20)->getSharedPtr(); // compiler error

	auto h30 = Factory<MyObj, Scoped, AllowRaw, AllowWeak>::create(3);
	h30->b = 7;
	cout << h30->b << endl;

	auto h40 = Factory<MyObj, RefCounted, AllowRaw, AllowWeak>::create(4);
	auto h41 = (*h40)->getWeakPtr();
	auto h42 = (*h41)->getSharedPtr(); // shared pointer from weak pointer! how cool is that!?
	h42->b = 9;
	cout << h40->b << endl;

	return 0;
}

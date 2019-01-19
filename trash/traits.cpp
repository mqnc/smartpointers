
#include <iostream>
#include <string>

using namespace std;


class RawPointable{
public:
	RawPointable* getRawPtr(){
		cout << "handing out dangerous pointer\n";
		return this;
	}
};

class WeakPointable{
public:
	WeakPointable* getWeakPtr(){
		cout << "handing out tracked pointer\n";
		// TODO return tracked pointer that turns zero when obj is destroyed
		return this;
	}
	~WeakPointable(){
		cout << "nulling all references to this object\n";
		// TODO actually do this
	}
};

class Scoped{
public:
	template<class T>
	void exitScope(T obj){
		cout << "freeing memory of scoped object\n";
		delete obj;
	}
};

class Persistent{
public:
	template<class T>
	void exitScope(T obj){
		cout << "not freeing memory of persistent object\n";
	}
};

class Sharable{
	int refCount=0;
	// many todos
};


template<class... TRAITS>
class MyObj:
	public TRAITS...
{
public:
	int m = 1338;
	auto* operator->(){return this;}
};

template<class OBJECT>
class Handler{
public:
	OBJECT* obj;

	Handler(/*vararg*/){
		obj = new OBJECT(/*vararg*/);
	}

	~Handler(){
		obj->exitScope(obj); // not sure how kosher this is
	}

	auto* operator->(){return obj;}
};


template<class ... TRAITS>
MyObj<TRAITS...> process(MyObj<TRAITS...> o){return o;}
// not sure how to do this with multiple arguments, probably needs nested typelists

int main()
{

	MyObj<RawPointable, WeakPointable> so;

	so = process(so);

	auto ho = Handler<MyObj<RawPointable, WeakPointable, Sharable, Scoped>>();

	auto* dpso = so->getRawPtr();
	auto* tpso = so->getWeakPtr();

	so->m = 5;
	ho->m = so->m;

	cout << ho->m;

	return 0;
}

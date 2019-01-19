
#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;

// test object

#include "noisy.h"
#include "registry.h"

Registry weakRegistry;

// TODO cant remember this so I'll program with the defines and then replace them in the end
#define VARIADIC typename ... ARGS
#define VARARGS ARGS&&... args
#define EXPAND ARGS...
#define FORWARD std::forward<ARGS>(args)...

template<typename T>
class UniquePtr;

template<typename T>
class SharedPtr;

template<typename T>
class WeakPtr;

#include "target.h"
#include "pointers.h"

int main(void){

	auto uni = Target<Unique<Noisy>>::create("Uniquebert");

	auto stack = Target<Stack<Noisy>>::create("Stackbert");

	auto stackRef = stack.weakRef();

	auto share = Target<Shared<Noisy>>::create("Sharebert");

	auto shareRef = share->weakRef();

	return 0;
}

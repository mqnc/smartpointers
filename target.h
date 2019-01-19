
template<typename LIFE>
class Target;

template<typename T>
class Stack{
public:
	typedef T TYPE;

	RegEntry<Target<Stack<T>>>* pEntry = nullptr;

	template<VARIADIC>
	static auto create(VARARGS){
		return Target<Stack<T>>(FORWARD);
	}

	auto weakRef(){
		if(nullptr == this->pEntry){
			pEntry = weakRegistry.enter(
				reinterpret_cast< Target<Stack<T>>* >(this)
			);
		}
		return WeakPtr(pEntry);
	}

	~Stack(){
		if(nullptr != this->pEntry){
			this->pEntry->target = nullptr;
			if( 0 == this->pEntry->refCount ){
				weakRegistry.remove(this->pEntry);
			}
		}
	}
};

template<typename T>
class Unique{
public:
	typedef T TYPE;

	template<VARIADIC>
	static auto create(VARARGS){
		return UniquePtr<T>(FORWARD);
	}
};

template<typename T>
class Shared{
public:
	typedef T TYPE;

	size_t refCount = 1;
	RegEntry<Target<Shared<T>>>* pEntry = nullptr;

	template<VARIADIC>
	static auto create(VARARGS){
		return SharedPtr<T>( new Target<Shared<T>>(FORWARD) );
	}

	auto share(){
		refCount++;
		return SharedPtr<T>( this );
	}

	auto weakRef(){
		if(nullptr == this->pEntry){
			pEntry = weakRegistry.enter(
				reinterpret_cast< Target<Shared<T>>* >(this)
			);
		}
		return WeakPtr(pEntry);
	}

	~Shared(){
		if(nullptr != this->pEntry){
			this->pEntry->target = nullptr;
			if( 0 == this->pEntry->refCount ){
				weakRegistry.remove(this->pEntry);
			}
		}
	}
};

template<typename LIFE>
class Target:public LIFE{
public:
	typename LIFE::TYPE obj;

	template<VARIADIC>
	Target(VARARGS):obj(FORWARD){}

	auto operator->(){return &obj;}
};

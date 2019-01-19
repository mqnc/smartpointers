
template<typename T>
class UniquePtr{
public:
	Target<Unique<T>>* target;

	template<VARIADIC>
	UniquePtr(VARARGS):
		target(new Target<Unique<T>>(FORWARD))
	{};

	~UniquePtr(){
		delete target;
	}

	auto operator->(){return &(*target);}
};

template<typename T>
class SharedPtr{
public:
	Target<Shared<T>>* target;

	SharedPtr(Target<Shared<T>>* target_):
		target(target_)
	{
		cout << target->obj.name << " shared pointer count increased to " << target->refCount << "\n"; // REMOVE ME
	};

	~SharedPtr(){
		target->refCount--;
		cout << target->obj.name << " shared pointer count dropped to " << target->refCount << "\n"; // REMOVE ME
		if(0 == target->refCount){
			delete target;
		}
	}

	auto operator->(){return &(*target);}
};

template<typename LIFE>
class WeakPtr{
public:
	RegEntry<Target<LIFE>>* entry;

	WeakPtr(RegEntry<Target<LIFE>>* entry_):
		entry(entry_)
	{
		entry->refCount++;
		cout << entry->target->obj.name << " weak pointer count increased to " << entry->refCount << "\n"; // REMOVE ME
	}

	~WeakPtr(){
		entry->refCount--;
		if( entry->target != nullptr ){  // REMOVE ME
			cout << entry->target->obj.name << " weak pointer count dropped to " << entry->refCount << "\n";
		}
		else{
			cout << "deleted object's weak pointer count dropped to " << entry->refCount << "\n";
		}

		if(nullptr == entry->target && 0 == entry->refCount){
			weakRegistry.remove(entry);
		}
	}

	auto operator->(){return &(*entry);}
};

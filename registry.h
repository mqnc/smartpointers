
const size_t CHUNKSIZE = 8;

template <class T>
struct RegEntry{
	// TODO turn target and pNextFree into a union
	T* target = nullptr;
	RegEntry<void>* pNextFree = nullptr;
	size_t refCount = 0;

	auto operator->(){return &(*target);}
};

class RegChunk{
public:
	RegEntry<void> slots[CHUNKSIZE];

	// initialize registry chunk: each element points to the next, the last is 0
	RegChunk(){
		for(RegEntry<void>* ps=slots; ps<slots+CHUNKSIZE-1; ps++){
			ps->pNextFree = ps+1;
		}
		slots[CHUNKSIZE-1].target = nullptr;
	}
};

class Registry{
public:
	RegEntry<void>* pNextFree = nullptr;

	template<class T>
	auto enter(T* target){
		if(pNextFree == nullptr){
			RegChunk* chunk = new RegChunk;
			pNextFree = &(chunk->slots[0]);
			cout << "new chunk\n"; // REMOVE ME
		}
		cout << "new registry entry\n"; // REMOVE ME
		RegEntry<T>* result = reinterpret_cast<RegEntry<T>*>(pNextFree);
		pNextFree = result->pNextFree;
		result->target = target;
		return result;
	}

	template<class T>
	void remove(RegEntry<T>* entry){
		cout << "freeing weak pointer registry slot\n"; // REMOVE ME
		entry->target = nullptr;
		entry->refCount = 0;
		entry->pNextFree = pNextFree;
		pNextFree = reinterpret_cast<RegEntry<void>*>(entry);
	}
};

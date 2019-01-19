class Noisy{
public:
	string name = "(noname)";

	Noisy(){
		std::cout << "constructed without name\n";
	}

	Noisy(string name_):name(name_){
		std::cout << "constructed " << name << "\n";
	}

	Noisy(const Noisy& other):name(other.name){
		std::cout << "copy-constructed from " << name << "\n";
	}

	Noisy(Noisy&& other):name(other.name){
		other.name = "ex-" + other.name;
		std::cout << "move-constructed from " << name << "\n";
	}

	Noisy& operator=(Noisy&& other){
		name = other.name;
		other.name = "ex-" + other.name;
		std::cout << "move-assigned from " << name << "\n";
		return *this;
	}

	Noisy& operator=(Noisy& other){
		name = other.name;
		std::cout << "copy-assigned from " << name << "\n";
		return *this;
	}

	~Noisy(){
		std::cout << "destructed " << name << "\n";
	}
};

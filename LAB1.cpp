#include <iostream>
#include <list>
#include <chrono>
#include <random>
#include <cassert>

using namespace std::chrono;
using namespace std;

class orderedSet
{
public:
	//Дії з множиною
	void Insert(double value);
	void Delete(double value);
	bool Search(double value) const;
	void Clear();

	//Дії над множинами
	static orderedSet Intersection(const orderedSet& a, const orderedSet& b);
	static orderedSet Union(const orderedSet& a, const orderedSet& b);
	bool IsSubset(const orderedSet& other) const;
	static orderedSet SumDifference(const orderedSet& a, const orderedSet& b);
	static orderedSet SetDifference(const orderedSet& a, const orderedSet& b);

	//Вивід множини
	void Print() const;

private:
	list<double> elements;
};

//Дії з множиною(определение) 
void orderedSet::Insert(double value)
{
	auto it = elements.begin();
	while (it != elements.end()) {
		if (*it == value) return;
		if (*it > value) break;
		++it;
	}
	elements.insert(it, value);

};

bool orderedSet::Search(double value) const
{
	for (const double& elem : elements) {
		if (elem == value) return true;
		if (elem > value) break;
	};

	return false;
};


void orderedSet::Print() const
{	
	cout << "{ ";
	for (const double& elem : elements) {
		cout << elem << " ";
	}
	cout << "}" << endl;
};

void orderedSet::Delete(double value)
{
	size_t before = elements.size();
	elements.remove(value);               
	if (elements.size() == before) {      
		cout << value << " not found\n\n"; 
	}
};

void orderedSet::Clear() {
	elements.clear();
};

//Дії над множинами(определение)
orderedSet orderedSet::Intersection(const orderedSet& a, const orderedSet& b)
{
	orderedSet result;
	auto itA = a.elements.begin();
	auto itB = b.elements.begin();

	while (itA != a.elements.end() && itB != b.elements.end()) {
		if (*itA == *itB) {
			result.Insert(*itA);
			++itA;
			++itB;
		}
		else if (*itA < *itB) {
			++itA;
		}
		else {
			++itB;
		}
	}
	return result;
};

orderedSet orderedSet::Union(const orderedSet& a, const orderedSet& b)
{
	orderedSet result;
	auto itA = a.elements.begin();
	auto itB = b.elements.begin();

	while (itA != a.elements.end() || itB != b.elements.end()) {
		if (itB == b.elements.end() || (itA != a.elements.end() && *itA < *itB)) {
			result.elements.push_back(*itA++);
		}
		else if (itA == a.elements.end() || *itB < *itA) {
			result.elements.push_back(*itB++);
		}
		else {
			result.elements.push_back(*itA);
			++itA;
			++itB;
		}
	}
	return result;
};

bool orderedSet::IsSubset(const orderedSet& other) const
{
	auto itA = elements.begin();
	auto itB = other.elements.begin();

	while (itA != elements.end()) {
		if (itB == other.elements.end()) return false;
		if (*itA == *itB) {
			++itA;
			++itB;
		}
		else if (*itA > *itB) {
			++itB;
		}
		else {
			return false;
		}
	}
	return true;
};

orderedSet orderedSet::SetDifference(const orderedSet& a, const orderedSet& b)
{
	orderedSet result;
	auto itA = a.elements.begin();
	auto itB = b.elements.begin();

	while (itA != a.elements.end()) {
		if (itB == b.elements.end() || *itA < *itB) {
			result.elements.push_back(*itA++);
		}
		else if (*itA == *itB) {
			++itA;
			++itB;
		}
		else {
			++itB;
		}
	}
	return result;
};

orderedSet orderedSet::SumDifference(const orderedSet& a, const orderedSet& b)
{
	orderedSet result;
	auto itA = a.elements.begin();
	auto itB = b.elements.begin();

	while (itA != a.elements.end() || itB != b.elements.end()) {
		if (itB == b.elements.end() || (itA != a.elements.end() && *itA < *itB)) {
			result.elements.push_back(*itA++);
		}
		else if (itA == a.elements.end() || *itB < *itA) {
			result.elements.push_back(*itB++);
		}
		else {
			++itA;
			++itB;
		}
	}
	return result;
};

double measureSearchTime(int setSize, int trials) {
	orderedSet s;
	default_random_engine gen;
	uniform_real_distribution<double> dist(0.0, 99999.0);

	for (int i = 0; i < setSize; ++i)
		s.Insert(dist(gen));

	double totalTime = 0.0;

	for (int i = 0; i < trials; ++i) {
		double val = dist(gen);
		auto start = high_resolution_clock::now();
		s.Search(val);
		auto end = high_resolution_clock::now();
		totalTime += duration<double, std::micro>(end - start).count();
	}

	return totalTime / trials;
};

void runSearchBenchmark() {
	cout << "Size\t\tTime\n";
	for (int size = 100; size <= 1000; size += 100) {
		double avgTime = measureSearchTime(size, 1000);
		cout << size << "\t\t" << avgTime << "\n";
	}
};

double measureUnionTime(int setSize, int trials) {
	default_random_engine gen;
	uniform_real_distribution<double> dist(0.0, 99999.0);

	double totalTime = 0.0;

	for (int i = 0; i < trials; ++i) {
		orderedSet a, b;
		for (int j = 0; j < setSize; ++j) {
			a.Insert(dist(gen));
			b.Insert(dist(gen));
		}

		auto start = high_resolution_clock::now();
		orderedSet::Union(a, b);
		auto end = high_resolution_clock::now();
		totalTime += duration<double, std::micro>(end - start).count();
	}

	return totalTime / trials;
};

void runUnionBenchmark() {
	cout << "Size\t\tTime\n";
	for (int size = 100; size <= 1000; size += 100) {
		double avgTime = measureUnionTime(size, 100);
		cout << size << "\t\t" << avgTime << "\n";
	}
};

void runTests() {
	orderedSet s;
	s.Insert(5.0);
	s.Insert(3.0);
	s.Insert(7.0);

	assert(s.Search(5.0));
	assert(s.Search(3.0));
	assert(!s.Search(10.0));

	s.Delete(3.0);
	assert(!s.Search(3.0));

	orderedSet a, b;
	a.Insert(1.0); a.Insert(2.0); a.Insert(3.0);
	b.Insert(3.0); b.Insert(4.0);

	orderedSet u = orderedSet::Union(a, b);
	assert(u.Search(1.0));
	assert(u.Search(4.0));
	assert(u.Search(3.0));

	orderedSet i = orderedSet::Intersection(a, b);
	assert(i.Search(3.0));
	assert(!i.Search(1.0));

	cout << "All tests passed.\n";
}

int main()
{	runTests();
	orderedSet set1, set2;

	// Заповнення множин випадковими числами
	default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	uniform_real_distribution<double> distribution(0.0, 99999.0);
	for (int i = 0; i < 10; ++i) {
		set1.Insert(distribution(generator));
	}
	for (int i = 0; i < 10; ++i) {
		set2.Insert(distribution(generator));
	}
	
	// Вивід множин
	cout << "Set 1: ";
	set1.Print();
	cout << "Set 2: ";
	set2.Print();

	// Перевірка операцій з множиною
	double felem = distribution(generator);
	cout << felem<< " in Set 1: " << (set1.Search(felem) ? "Yes" : "No") << "\n\n";

	felem = distribution(generator);
	cout << felem << " in Set 2: " << (set2.Search(felem) ? "Yes" : "No") << "\n\n";

	felem = distribution(generator);
	set1.Delete(felem);
	set1.Print();

	felem = distribution(generator);
	set2.Delete(felem);
	set2.Print();


	orderedSet UnionSet = orderedSet::Union(set1, set2);
	cout << "Union: ";
	UnionSet.Print();

	orderedSet IntersectionSet = orderedSet::Intersection(set1, set2);
	cout << "Intersection: ";
	IntersectionSet.Print();

	orderedSet SetDifference = orderedSet::SetDifference(set1, set2);
	cout << "Set Difference (Set1 - Set2): ";
	SetDifference.Print();

	orderedSet SumDifference = orderedSet::SumDifference(set1, set2);
	cout << "Sum Difference: ";
	SumDifference.Print();

	// Перевірка на підмножину
	cout << "Set 1 is subset of Set 2: " << (set1.IsSubset(set2) ? "Yes" : "No") << "\n";

	set1.Clear();
	cout << "Set 1: ";
	set1.Print();

	set2.Clear();
	cout << "Set 2: ";
	set2.Print();

	cout << "Search Benchmark (microseconds):\n";
	runSearchBenchmark();
	cout << "\nUnion Benchmark (microseconds):\n";
	runUnionBenchmark();
	

	return 0;
}


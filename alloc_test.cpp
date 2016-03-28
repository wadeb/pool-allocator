#include "Memory_pool.h"

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

//#define NDEBUG

struct Record {
public:
	const string name;
	const string phone_no;
	int age;

	// Initialize the Record.
	Record(const string &name_, const string &phone_no_, int age_)
		: name(name_), phone_no(phone_no_), age(age_) {}

	// Overloaded memory operators.
	void *operator new(size_t size)
		{ assert(size); return allocator.alloc(); }
	void operator delete(void *returned, size_t size)
		{ allocator.free(returned); }

private:
	// Memory allocator for the Record class.
	static Memory_pool<Record> allocator;

};

//
// Output operator for Record struct.
//
ostream &operator<<(ostream &os, Record &rec)
{
	os << rec.name << " " << rec.phnone_no << " " << rec.age << endl;
	return os;
}

// Initialize the allocator.
Record::allocator = Memory_pool<Record>();

int main()
{
	Record *ptr = new Record("Wade", "989-277-1010", 24);
	cout << *ptr;
	delete ptr;

	return 0;
}

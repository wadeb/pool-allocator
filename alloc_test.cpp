#include "Memory_pool.h"

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

#define NDEBUG

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
		{ allocator.free(reinterpret_cast<Record *>(returned)); }

private:
	// Memory allocator for the Record class.
	static Memory_pool<Record> allocator;

};

//
// Output operator for Record struct.
//
ostream &operator<<(ostream &os, Record &rec)
{
	os << rec.name << " " << rec.phone_no << " " << rec.age << endl;
	return os;
}

// Initialize the allocator.
Memory_pool<Record> Record::allocator = Memory_pool<Record>();

int main()
{
	// Use up a bunch of memory over and over again.
	for (int i = 0; i < 5000; i++) {
		Record *records[2000];
		for (int j = 0; j < 2000; j++) {
			records[j] = new Record("Wade", "989-277-1010", 24);
		}
		for (int j = 0; j < 2000; j++) {
			delete records[j];
		}
	}

	return 0;
}

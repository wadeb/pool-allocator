#include <iostream>
#include <string>

using namespace std;

struct Record {
	const string name;
	const string phone_no;
	int age;

	// Initialize the Record.
	Record(const string &name_, const string &phone_no_, int age_)
		: name(name_), phone_no(phone_no_), age(age_) {}

};

//
// Output operator for Record struct.
//
ostream &operator<<(ostream &os, Record &rec)
{
	os << rec.name << " " << rec.phone_no << " " << rec.age << endl;
	return os;
}

int main()
{
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

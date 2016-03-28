#ifndef MEMORY_POOL
#define MEMORY_POOL

//
// A pool memory allocator for fixed-size objects.
// Great for use with data or any data type that might
// have a lot of allocations and deallocations throughout
// the lifetime of the program.
// Expansions will take significantly longer than regular allocations,
// so if working in a realtime environment make sure to allocate
// a sufficient amount at startup.
//
// Wade Bonkowski - 3-37-2016
//

#include <cstdlib>

// Amount to expand the free list by if it is empty.
const size_t EXPANSION_SIZE = 1000;

//
// Templated fixed-size pool memory allocator.
//
template <typename T>
class Memory_pool {
public:
	// Creates the initial pool of memory used for allocations.
	Memory_pool(size_t amount = EXPANSION_SIZE);

	// Frees up all of the memory that is in the pool.
	~Memory_pool();

	// Allocate and element from the free list.
	inline T *alloc();

	// Places elem back into the free list, effectively free'ing it.
	inline void free(T *elem);

private:
	// The next element on the free list.
	// NOTE: Absolutely must be the first thing laid out in memory
	//       as part of an object of this class.
	Memory_pool<T> *next;

	// Expand the free list by the specified amount.
	void expand_free_list(size_t amount = EXPANSION_SIZE);

};

//
// Create the initial list of available memory with the amount of objects
// specified.
//
template <typename T>
Memory_pool<T>::Memory_pool(size_t amount)
{
	expand_free_list(amount);
}

//
// Walk the free list and delete any memory that is there.
//
template <typename T>
Memory_pool<T>::~Memory_pool()
{
	for (Memory_pool<T> *ptr = next; ptr != nullptr; ptr = next) {
		next = next->next;
		// Cast back so destructors aren't called.
		delete[] reinterpret_cast<char *>(ptr);
	}
}

//
// Allocate a T element from the free list.
//
template <typename T>
T *Memory_pool<T>::alloc()
{
	if (!next) {
		expand_free_list();
	}

	Memory_pool<T> *ptr = next;
	next = next->next;

	return reinterpret_cast<T*>(ptr);
}

//
// Return an element to the free list.
//
template <typename T>
void Memory_pool<T>::free(T *elem)
{
	Memory_pool<T> *returned = reinterpret_cast<Memory_pool<T> *>(elem);
	returned->next = next;
	next = returned;
}

//
// Create and add amount elements to the free list.
//
template <typename T>
void Memory_pool<T>::expand_free_list(size_t amount)
{
	// The allocated objects must be at least as big as a
	// pointer so they can be linked together.
	size_t size = (sizeof(T) > sizeof(Memory_pool<T> *))?
			sizeof(T) : sizeof(Memory_pool<T> *);

	// Create the amount of pieces needed and add them to the free list.
	Memory_pool<T> *runner = reinterpret_cast<Memory_pool<T> *>(new char[size]);
	next = runner;
	for (size_t i = 0; i < amount; i++) {
		runner->next = reinterpret_cast<Memory_pool<T> *>(new char[size]);
		runner = runner->next;
	}

	runner->next = nullptr;
}

#endif // MEMORY_POOL

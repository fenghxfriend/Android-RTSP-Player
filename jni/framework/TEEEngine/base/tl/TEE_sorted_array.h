#ifndef TL_FILE_SORTED_ARRAY_HPP
#define TL_FILE_SORTED_ARRAY_HPP

#include "TEE_algorithm.h"
#include "TEE_array.h"

template <class T, class ALLOCATOR = allocator_default<T> >
class sorted_array : public array<T, ALLOCATOR>
{
	typedef array<T, ALLOCATOR> parent;
	
	int insert(const T& item, typename parent::range r) { dbg_break(); return 0; }
	int set_size(int new_size) { dbg_break(); return 0; }
		
public:
	typedef plain_range_sorted<T> range;

	int add(const T& item)
	{
		return parent::insert(item, partition_binary(all(), item));
	}

	int add_unsorted(const T& item)
	{
		return parent::add(item);
	}
	
	void sort_range()
	{
		sort(all());
	}

	range all() { return range(parent::list, parent::list+parent::num_elements); }
};

#endif

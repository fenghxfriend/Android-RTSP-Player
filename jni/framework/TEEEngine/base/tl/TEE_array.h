#ifndef TL_FILE_ARRAY_HPP
#define TL_FILE_ARRAY_HPP

#include "TEE_range.h"
#include "TEE_allocator.h"

template <class T, class ALLOCATOR = allocator_default<T> >
class array : private ALLOCATOR
{
	void init()
	{
		list = 0x0;
		clear();
	}
	
public:
	typedef plain_range<T> range;
    
	array()
	{
		init();
	}

	array(const array &other)
	{
		init();
		set_size(other.size());
		for(int i = 0; i < size(); i++)
			(*this)[i] = other[i];
	}

	~array()
	{
		ALLOCATOR::free_array(list);
		list = 0x0;
	}

	void delete_all()
	{
		for(int i = 0; i < size(); i++)
			delete list[i];
		clear();
	}

	void clear()
	{
		ALLOCATOR::free_array(list);
		list_size = 1;
		list = ALLOCATOR::alloc_array(list_size);
		num_elements = 0;
	}

	int size() const
	{
		return num_elements;
	}

	void remove_index_fast(int index)
	{
		list[index] = list[num_elements-1];
		set_size(size()-1);
	}

	void remove_fast(const T& item)
	{
		for(int i = 0; i < size(); i++)
			if(list[i] == item)
			{
				remove_index_fast(i);
				return;
			}
	}

	void remove_index(int index)
	{
		for(int i = index+1; i < num_elements; i++)
			list[i-1] = list[i];
		
		set_size(size()-1);
	}

	bool remove(const T& item)
	{
		for(int i = 0; i < size(); i++)
			if(list[i] == item)
			{
				remove_index(i);
				return true;
			}
		return false;
	}

	int add(const T& item)
	{
		incsize();
		set_size(size()+1);
		list[num_elements-1] = item;
		return num_elements-1;
	}

	int insert(const T& item, range r)
	{
		if(r.empty())
			return add(item);
			
		int index = (int)(&r.front()-list);
		incsize();
		set_size(size()+1);
		
		for(int i = num_elements-1; i > index; i--)
			list[i] = list[i-1];

		list[index] = item;
		
		return num_elements-1;
	}

	T& operator[] (int index)
	{
		return list[index];
	}

	const T& operator[] (int index) const
	{
		return list[index];
	}

	T *base_ptr()
	{
		return list;
	}

	const T *base_ptr() const
	{
		return list;
	}

	void set_size(int new_size)
	{
		if(list_size < new_size)
			alloc(new_size);
		num_elements = new_size;
	}

	void hint_size(int hint)
	{
		if(num_elements < hint)
			alloc(hint);
	}

	int optimize()
	{
		int before = memusage();
		alloc(num_elements);
		return before - memusage();
	}

	int memusage()
	{
		return sizeof(array) + sizeof(T)*list_size;
	}

	array &operator = (const array &other)
	{
		set_size(other.size());
		for(int i = 0; i < size(); i++)
			(*this)[i] = other[i];
		return *this;
	}

	range all() { return range(list, list+num_elements); }
protected:

	void incsize()
	{
		if(num_elements == list_size)
		{
			if(list_size < 2)
				alloc(list_size+1);
			else
				alloc(list_size+list_size/2);
		}		
	}

	void alloc(int new_len)
	{
		list_size = new_len;
		T *new_list = ALLOCATOR::alloc_array(list_size);
		
		int end = num_elements < list_size ? num_elements : list_size;
		for(int i = 0; i < end; i++)
			new_list[i] = list[i];
		
		ALLOCATOR::free_array(list);

		num_elements = num_elements < list_size ? num_elements : list_size;
		list = new_list;
	}

	T *list;
	int list_size;
	int num_elements;
};

#endif

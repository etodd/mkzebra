#pragma once

#include <cmath>
#include <cstdlib>
#include <cstring>

#include "types.h"
#include "vi_assert.h"

namespace VI
{

#define ARRAY_GROWTH_FACTOR 1.5
#define ARRAY_INITIAL_RESERVATION 1

template <typename T, s32 size>
struct StaticArray
{
	union
	{
		char _nil[size * sizeof(T)];
		T data[size];
	};
	s32 length;

	StaticArray()
		: _nil(), length()
	{
	}

	StaticArray(s32 l)
		: _nil(), length(l)
	{
	}

	const StaticArray& operator=(const StaticArray& other)
	{
		memcpy(_nil, other._nil, sizeof(_nil));
		length = other.length;
		return *this;
	}

	StaticArray(const StaticArray& other)
		: length(other.length)
	{
		memcpy(_nil, other._nil, sizeof(_nil));
	}

	~StaticArray()
	{
	}

	inline void resize(s32 l)
	{
		vi_assert(l <= size);
		length = l;
	}

	inline const T& operator [] (s32 i) const
	{
		vi_assert(i >= 0 && i < length);
		return data[i];
	}

	inline T& operator [] (s32 i)
	{
		vi_assert(i >= 0 && i < length);
		return data[i];
	}

	void remove(s32 i)
	{
		vi_assert(i >= 0 && i < length);
		if (i != length - 1)
			data[i] = data[length - 1];
		length--;
	}

	void remove_ordered(s32 i)
	{
		vi_assert(i >= 0 && i < length);
		memmove(&data[i + 1], &data[i], sizeof(T) * (length - (i + 1)));
		length--;
	}

	T* insert(s32 i, T& t)
	{
		vi_assert(i >= 0 && i <= length);
		length++;
		vi_assert(length <= size);
		memmove(&data[i], &data[i + 1], sizeof(T) * (length - i));
		data[i] = t;
		return &data[i];
	}

	T* insert(s32 i)
	{
		vi_assert(i >= 0 && i <= length);
		length++;
		vi_assert(length <= size);
		memmove(&data[i], &data[i + 1], sizeof(T) * (length - i));
		return &data[i];
	}

	T* add()
	{
		length++;
		vi_assert(length <= size);
		return &data[length - 1];
	}

	T* add(const T& t)
	{
		T* p = add();
		*p = t;
		return p;
	}
};

template <typename T>
struct Array
{
	T* data;
	s32 length;
	s32 reserved;

	Array(s32 reserve_count = 0, s32 length = 0)
		: length(length)
	{
		vi_assert(reserve_count >= 0 && length >= 0 && length <= reserve_count);
		reserved = 0;
		if (reserve_count > 0)
			reserve(reserve_count);
		else
			data = 0;
	}

	~Array()
	{
		if (data)
			free(data);
	}

	inline const T& operator [] (s32 i) const
	{
		vi_assert(i >= 0 && i < length);
		return *(data + i);
	}

	inline T& operator [] (s32 i)
	{
		vi_assert(i >= 0 && i < length);
		return *(data + i);
	}

	void reserve(s32 size)
	{
		vi_assert(size >= 0);
		if (size > reserved)
		{
			s32 next_size = (u32)pow(ARRAY_GROWTH_FACTOR, (s32)(log(size) / log(ARRAY_GROWTH_FACTOR)) + 1);
			if (!reserved)
			{
				next_size = next_size > ARRAY_INITIAL_RESERVATION ? next_size : ARRAY_INITIAL_RESERVATION;
				data = (T*)calloc(next_size, sizeof(T));
			}
			else
			{
				data = (T*)realloc(data, next_size * sizeof(T));
				memset((void*)&data[reserved], 0, (next_size - reserved) * sizeof(T));
			}
			reserved = next_size;
		}
	}

	void remove(s32 i)
	{
		vi_assert(i >= 0 && i < length);
		if (i != length - 1)
			data[i] = data[length - 1];
		length--;
	}

	void remove_ordered(s32 i)
	{
		vi_assert(i >= 0 && i < length);
		memmove(&data[i + 1], &data[i], sizeof(T) * (length - (i + 1)));
		length--;
	}

	T* insert(s32 i, T& t)
	{
		vi_assert(i >= 0 && i <= length);
		reserve(++length);
		memmove(&data[i], &data[i + 1], sizeof(T) * (length - i));
		data[i] = t;
		return &data[i];
	}

	T* insert(s32 i)
	{
		vi_assert(i >= 0 && i <= length);
		reserve(++length);
		memmove(&data[i], &data[i + 1], sizeof(T) * (length - i));
		return &data[i];
	}

	void resize(s32 i)
	{
		reserve(i);
		length = i;
	}

	T* add()
	{
		reserve(++length);
		return &data[length - 1];
	}

	T* add(const T& t)
	{
		T* p = add();
		*p = t;
		return p;
	}
};

}

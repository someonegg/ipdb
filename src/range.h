/*
 *  File   : range.h
 *  Author : Jiang Wangsheng
 *  Date   : 2012/04/17 14:21:23
 *  Brief  :
 *
 *  $Id: $
 */
#ifndef __RANGE_H__
#define __RANGE_H__

#include <utility>
#include <list>

#include "misc/common_mp.h"


// Range will be automatically merged

template <typename T>
class RangeManager
{
public:
	// [first,last]
	typedef std::pair<T, T> Range;
	typedef std::list<Range> Ranges;

	Ranges ranges;

	void add(T first, T last)
	{
		ASSERT(first <= last);

		Range rn(first, last);
		while ( !__tryAdd(rn) );
	}

private:
	// true ok
	// false __tryAdd again
	bool __tryAdd(Range &rn)
	{
		typename Ranges::iterator itor = ranges.begin();
		for ( ; itor != ranges.end(); ++itor )
		{
			if ( rn.second < itor->first )
			{
				ranges.insert(itor, rn);
				return true;
			}

			if ( rn.first < itor->first &&
				rn.second >= itor->first )
			{
				rn.second = std::max(rn.second, itor->second);
				ranges.erase(itor);
				return false;
			}

			if ( rn.first >= itor->first &&
				rn.first <= itor->second )
			{
				if ( rn.second <= itor->second )
				{
					return true;
				}

				rn.first = itor->first;
				rn.second = std::max(rn.second, itor->second);
				ranges.erase(itor);
				return false;
			}
		}

		ASSERT(itor == ranges.end());
		ranges.insert(itor, rn);
		return true;
	}
};


template <typename T, typename D>
class RangeSimpleManager
{
public:
	// [first,last]
	struct Range
	{
		T first;
		T second;
		D data;
	};
	typedef std::list<Range> Ranges;

	Ranges ranges;

	void add(T first, T last, D data)
	{
		ASSERT(first <= last);

		Range rn = {first, last, data};

		typename Ranges::iterator itor = ranges.begin();
		for ( ; itor != ranges.end(); ++itor )
		{
			if ( itor->first > rn.first )
			{
				ranges.insert(itor, rn);
				return;
			}
		}

		ASSERT(itor == ranges.end());
		ranges.insert(itor, rn);
	}
};

#endif // !__RANGE_H__

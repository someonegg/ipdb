/*
 *  File   : tstring.h
 *  Author : Jiang Wangsheng
 *  Date   : 2012/04/16 09:34:23
 *  Brief  :
 *
 *  $Id: $
 */
#ifndef __TSTRING_H__
#define __TSTRING_H__

#include <string>


typedef const char* LPCSTR;

typedef unsigned short UDCHAR;
typedef UDCHAR* LPUSTR;
typedef const UDCHAR* LPCUSTR;

namespace std
{
	typedef basic_string<UDCHAR, char_traits<UDCHAR>,
	allocator<UDCHAR> > ustring;
}

inline
int ustringLen(LPCUSTR str)
{
	int len = 0;

	while ( *str++ != 0 ) ++len;

	return len;
}

inline
bool equalUString(LPCUSTR str1, LPCUSTR str2)
{
	int len1 = ustringLen(str1);
	int len2 = ustringLen(str2);

	if ( len1 != len2 )
	{
		return false;
	}

	for ( int i = 0; i < len1; ++i )
	{
		if ( str1[i] != str2[i] )
		{
			return false;
		}
	}

	return true;
}

inline
bool equalNUString(LPCUSTR masterStr, LPCUSTR subStr, int n)
{
	int lenMaster = ustringLen(masterStr);
	int lenSub = ustringLen(subStr);

	if ( lenMaster < n || lenSub < n )
		return false;

	for ( int i = 0; i < n; i++ )
	{
		if ( masterStr[i] != subStr[i] )
			return false;
	}

	return true;
}

inline
bool strCat(LPUSTR buf, LPCUSTR str1, LPCUSTR str2)
{
	int len1 = ustringLen(str1);
	int len2 = ustringLen(str2);

	int pos = 0;
	for ( int i = 0; i < len1; i++ )
	{
		buf[pos++] = str1[i];
	}

	for ( int i = 0; i < len2; i++ )
	{
		buf[pos++] = str2[i];
	}

	buf[pos] = L'\0';

	return true;
}

inline
bool isStrInStr(LPCUSTR masterStr, LPCUSTR subStr)
{
	int lenMaster = ustringLen(masterStr);
	int lenSub = ustringLen(subStr);

	if ( masterStr < subStr )
		return false;

	if ( masterStr == subStr && equalUString(masterStr, subStr) )
		return true;

	int CompareLen = lenMaster - lenSub + 1;
	for ( int i = 0; i < CompareLen; i++ )
	{
		if ( equalNUString(&masterStr[0], subStr, lenSub) )
			return true;
	}

	return false;
}


#endif /* __TSTRING_H__ */

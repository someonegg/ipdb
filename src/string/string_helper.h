/*
 *  File   : string_helper.h
 *  Author : Jiang Wangsheng
 *  Date   : 2012/04/17 09:34:23
 *  Brief  :
 *
 *  $Id: $
 */
#ifndef __STRING_HELPER_H__
#define __STRING_HELPER_H__

#include "tstring.h"
#include "ConvertUTF.h"


inline
bool str2ustr(LPCSTR src, std::ustring &dst)
{
	size_t srcLen = strlen(src);
	if ( srcLen == 0 )
	{
		return true;
	}

	UDCHAR* dstBuf = (UDCHAR*)calloc((srcLen + 1), sizeof(UDCHAR));

	const UTF8* inUTF8 = (const UTF8*)src;
	UTF16* outUTF16 = (UTF16*)dstBuf;
	ConversionResult r = ConvertUTF8toUTF16(&inUTF8, inUTF8 + srcLen, &outUTF16, outUTF16 + srcLen, strictConversion);
	if ( r != conversionOK )
	{
		return false;
	}

	dst = dstBuf;
	free(dstBuf);

	return true;
}

template <class TStr, class TVec>
void Split(
	const TStr &str,
	const TStr &match,
	TVec &res,
	bool fullMatch = false
	)
{
	typedef typename TStr::size_type (TStr::*find_t)(
		const TStr&,
		typename TStr::size_type ) const;

	typename TStr::size_type start = 0, skip = 1;
	find_t pfind = &TStr::find_first_of;

	if ( fullMatch )
	{
		// use the whole match string as a key
		// instead of individual characters
		// skip might be 0. see search loop comments
		skip = match.length();
		pfind = &TStr::find;
	}

	while ( start != TStr::npos )
	{
		// get a complete range [start..end)
		typename TStr::size_type end = (str.*pfind)(match, start);

		// null strings always match in string::find, but
		// a skip of 0 causes infinite loops. pretend that
		// no tokens were found and extract the whole string
		if (skip == 0) end = TStr::npos;

		TStr token = str.substr(start, end - start);

		// extract the token and add it to the result list
		res.push_back(token);

		// start the next range
		if ( (start = end) != TStr::npos ) start += skip;
	}
}

template <class TStr>
TStr TrimStr(const TStr &str)
{
	typename TStr::value_type blanks[3] = {' ', '\t', 0};

	typename TStr::size_type f = str.find_first_not_of(blanks);
	typename TStr::size_type l = str.find_last_not_of(blanks);

	if ( f == TStr::npos ) return TStr();

	return str.substr(f, l - f + 1);
}

template <class TStr, class TVec>
class StrTrimVec
{
public:
	TVec v;

	void push_back(TStr e)
	{
		e = TrimStr(e);
		v.push_back(e);
	}
};

#endif // !__STRING_HELPER_H__

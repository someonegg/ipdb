/*
 *  File   : stream_interface.h
 *  Author : Jiang Wangsheng
 *  Date   : 2012/04/16 09:34:23
 *  Brief  :
 *
 *  $Id: $
 */
#ifndef __STREAM_INTERFACE_H__
#define __STREAM_INTERFACE_H__


namespace stream_helper {

class OutputStream
{
public:
	virtual bool write(const void* buffer, unsigned int cb) = 0;
	virtual bool flush() = 0;
};

class InputStream
{
public:
	virtual bool read(void* buffer, unsigned int cb, unsigned int *readedBytes) = 0;
	virtual bool eof() = 0;
};

template <class TStr>
inline
void ReadLine(InputStream &is, TStr &line)
{
	for ( ; ; )
	{
		typename TStr::value_type ch;

		unsigned int readed = 0;
		if ( !is.read(&ch, sizeof(ch), &readed) ||
			readed != sizeof(ch) )
		{
			return;
		}

		if ( ch == '\n' ) break;
		if ( ch == '\r' ) continue;

		line.append(1, ch);
	}
}

}


#endif /* __STREAM_INTERFACE_H__ */

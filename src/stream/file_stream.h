/*
 *  File   : file_stream.h
 *  Author : Jiang Wangsheng
 *  Date   : 2012/04/16 09:34:23
 *  Brief  :
 *
 *  $Id: $
 */
#ifndef __STREAM_FILE_STREAM_H__
#define __STREAM_FILE_STREAM_H__

#include "stream_interface.h"
#include <fstream>


namespace stream_helper {

class FileOutputStream : public OutputStream
{
	std::ofstream m_file;

public:
	FileOutputStream() {}

	~FileOutputStream() {
		m_file.flush();
		m_file.close();
	}

	bool open(const char* fileName) {
		m_file.open(fileName, std::ios::binary);
		return !(!m_file);
	}

	void close()
	{
		m_file.close();
	}

	bool write(const void* buffer, unsigned int cb)	{
		m_file.write( (char*)buffer, cb );
		return m_file.good();
	}

	bool flush() {
		m_file.flush();
		return m_file.good();
	}
};

class FileInputStream : public InputStream
{
	std::ifstream m_file;

public:
	FileInputStream() {}

	~FileInputStream() {
		m_file.close();
	}

	bool open(const char* fileName) {
		m_file.open(fileName, std::ios::binary);
		return !(!m_file);
	}

	void close()
	{
		m_file.close();
	}

	bool read(void* buffer, unsigned int cb, unsigned int* readed) {
		m_file.read( (char*)buffer, cb );
		unsigned int __readed = m_file.gcount();
		if ( readed )
			*readed = __readed;
		return __readed > 0 ? true : false;
	}

	bool eof(){
		return m_file.eof();
	}

};

}


#endif /* __STREAM_FILE_STREAM_H__ */

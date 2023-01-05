/*
 *  File   : help_function.h
 *  Author : Jiang Wangsheng
 *  Date   : 2012/04/16 09:34:23
 *  Brief  :
 *
 *  $Id: $
 */
#ifndef __HELP_FUCTION_H__
#define __HELP_FUCTION_H__

#include "../stream/file_stream.h"
#include "../string/tstring.h"


inline
bool copyStream(stream_helper::InputStream &inS, stream_helper::OutputStream &outS)
{
	char buffer[1024];

	for ( ; ; )
	{
		unsigned int readed = 0;

		inS.read(buffer, 1024, &readed);

		if ( readed > 0 )
		{
			if ( !outS.write(buffer, readed) )
			{
				return false;
			}
		}

		if ( readed != 1024 )
		{
			break;
		}
	}

	return true;
}

inline
bool copyFile(LPCSTR srcFile, LPCSTR dstFile)
{
	stream_helper::FileInputStream inFile;
	if ( !inFile.open(srcFile) )
	{
		return false;
	}

	stream_helper::FileOutputStream outFile;
	if ( !outFile.open(dstFile) )
	{
		return false;
	}

	return copyStream(inFile, outFile);
}

inline
bool copyDir(LPCSTR srcDir, LPCSTR dstDir)
{
	std::string copyCmd;
	copyCmd = "cp -r";

	copyCmd.append( " " );

	copyCmd.append( "\"" );
	copyCmd.append( srcDir );
	copyCmd.append( "\"" );

	copyCmd.append( " " );

	copyCmd.append( "\"" );
	copyCmd.append( dstDir );
	copyCmd.append( "\"" );

	int ret = system( copyCmd.c_str() );

	return true;
}

inline
bool removeDir(LPCSTR dir)
{
	std::string removeCmd;
	removeCmd = "rm -r";

	removeCmd.append( " " );

	removeCmd.append( "\"" );
	removeCmd.append( dir );
	removeCmd.append( "\"" );

	int ret = system( removeCmd.c_str() );

	return true;
}


#endif /* __HELP_FUCTION_H__ */

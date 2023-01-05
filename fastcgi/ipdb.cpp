/***************************************************************************
* Copyright (C) 2007 Eddie Carle [eddie@erctech.org]                       *
*                                                                          *
* This file is part of fastcgi++.                                          *
*                                                                          *
* fastcgi++ is free software: you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as  published   *
* by the Free Software Foundation, either version 3 of the License, or (at *
* your option) any later version.                                          *
*                                                                          *
* fastcgi++ is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     *
* License for more details.                                                *
*                                                                          *
* You should have received a copy of the GNU Lesser General Public License *
* along with fastcgi++.  If not, see <http://www.gnu.org/licenses/>.       *
****************************************************************************/

#include <fstream>
#include <sstream>
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <fastcgi++/request.hpp>
#include <fastcgi++/manager.hpp>

#include <ipdb.h>

// I like to have an independent error log file to keep track of exceptions while debugging.
// You might want a different filename. I just picked this because everything has access there.
void error_log(const char* msg)
{
	using namespace std;
	using namespace boost;
	static ofstream error;
	if(!error.is_open())
	{
		error.open("/tmp/cgi_ipdb_log", ios_base::out | ios_base::app);
		error.imbue(locale(error.getloc(), new posix_time::time_facet()));
	}

	error << '[' << posix_time::second_clock::local_time() << "] " << msg << endl;
}

IPDBH g_dbh = NULL;

const wchar_t* COUNTRY_NAME[] = {L"未知",L"中国"};
int COUNTRY_NAME_COUNT = sizeof(COUNTRY_NAME) / sizeof(COUNTRY_NAME[0]);
const wchar_t* CNISP_NAME[] = {L"未知",L"电信",L"网通",L"铁通",L"移动",L"教育网"};
int CNISP_NAME_COUNT = sizeof(CNISP_NAME) / sizeof(CNISP_NAME[0]);
const wchar_t* CNLOC_NAME[] = {L"未知",L"北京",L"天津",L"河北",L"山西",L"内蒙古",L"辽宁",L"吉林",L"黑龙江",L"上海",L"江苏",L"浙江",L"安徽",L"福建",L"江西",L"山东",L"河南",L"湖北",L"湖南",L"广东",L"广西",L"海南",L"重庆",L"四川",L"贵州",L"云南",L"西藏",L"陕西",L"甘肃",L"青海",L"宁夏",L"新疆",L"香港",L"澳门",L"台湾"};
int CNLOC_NAME_COUNT = sizeof(CNLOC_NAME) / sizeof(CNLOC_NAME[0]);

// Let's make our request handling class. It must do the following:
// 1) Be derived from Fastcgipp::Request
// 2) Define the virtual response() member function from Fastcgipp::Request()

// First things first let's decide on what kind of character set we will use.
// Since we want to be able to echo all languages we will use unicode. The way this
// library handles unicode might be different than some are used to but it is done
// the way it is supposed to be. All internal characters are wide. In this case UTF-32.
// This way we don't have to mess around with variable size characters in our program.
// A string with 10 wchar_ts is ten characters long. Not up in the air as it is with UTF-8.
// Anyway, moving right along, the streams will code convert all the UTF-32 data to UTF-8
// before it is sent out to the client. This way we get the best of both worlds.
//
// So, whenever we are going to use UTF-8, our template parameter for Fastcgipp::Request<charT>
// should be wchar_t. Keep in mind that this suddendly makes
// everything wide character and utf compatible. Including HTTP header data (cookies, urls, yada-yada).

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

class S2UIVec
{
public:
	std::vector<unsigned int> v;

	void push_back(std::wstring e)
	{
		std::wistringstream wstrm(e);
  		unsigned int _e;
  		wstrm >> _e;
		v.push_back(_e);
	}
};

class IPDB: public Fastcgipp::Request<wchar_t>
{
	bool response()
	{
		using namespace Fastcgipp;

		out << "Content-Type: text/html; charset=utf-8\r\n\r\n";

		out << "<html><head><meta http-equiv='Content-Type' content='text/html; charset=utf-8' />";
		out << "<title>IP Query</title></head><body>";

		out << "<div id='ips'><table border='1'>";

		for(Http::Environment<wchar_t>::Gets::const_iterator it=environment().gets.begin(); it!=environment().gets.end(); ++it)
		{
			if ( it->first.compare(0, 2, L"ip") == 0 )
			{
				S2UIVec ips;
				std::wstring dot = L".";
				Split(it->second, dot, ips);
				if ( ips.v.size() == 4 )
				{
					out << "<tr>";

					unsigned int ip = (ips.v[0] << 24) + (ips.v[1] << 16) + (ips.v[2] << 8) + ips.v[3];
					IPINFO ipi = {0};
					ipdb_query(g_dbh, ip, &ipi);

					if ( ipi.country >= COUNTRY_NAME_COUNT )
					{
						ipi.isp = COUNTRY_Unknown;
					}
					if ( ipi.isp >= CNISP_NAME_COUNT )
					{
						ipi.isp = CNISP_Unknown;
					}
					if ( ipi.loc >= CNLOC_NAME_COUNT )
					{
						ipi.loc = CNLOC_Unknown;
					}

					out << "<td class='ip'>" << it->second << "</td>";
					out << "<td class='country'>" << COUNTRY_NAME[ipi.country] << "</td>";
					out << "<td class='isp'>" << CNISP_NAME[ipi.isp] << "</td>";
					out << "<td class='location'>" << CNLOC_NAME[ipi.loc] << "</td>";

					out << "</tr>";
				}
			}
		}

		out << "</table></div>";

		out << "</body></html>";

		// Always return true if you are done. This will let apache know we are done
		// and the manager will destroy the request and free it's resources.
		// Return false if you are not finished but want to relinquish control and
		// allow other requests to operate. You might do this after an SQL query
		// while waiting for a reply. Passing messages to requests through the
		// manager is possible but beyond the scope of this example.
		return true;
	}
};

// The main function is easy to set up
int main(int argc, char* argv[])
{
	if ( argc < 2 )
	{
		error_log( "Error : IPDB not set" );
		return 1;
	}

	g_dbh = ipdb_init(argv[1]);
	if ( g_dbh == NULL )
	{
		error_log( "Error : IPDB init error" );
		return 2;
	}

	try
	{
		// First we make a Fastcgipp::Manager object, with our request handling class
		// as a template parameter.
		Fastcgipp::Manager<IPDB> fcgi;
		// Now just call the object handler function. It will sleep quietly when there
		// are no requests and efficiently manage them when there are many.
		fcgi.handler();
	}
	catch(std::exception& e)
	{
		error_log(e.what());
	}
}


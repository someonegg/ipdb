// test app package
//

#include "misc/common_mp.h"
#include "misc/help_fuction.h"

#include "stream/file_stream.h"
using namespace stream_helper;

#include "string/string_helper.h"

#include "ipdb_desc.h"
#include "range.h"
#include "ipdb.h"

#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <iostream>

class S2UIVec
{
public:
	std::vector<uint32_t> v;

	void push_back(std::string e)
	{
		e = TrimStr(e);
		uint32_t _e = (uint32_t)atoi(e.c_str());
		v.push_back(_e);
	}
};

class LocationMap
{
	typedef std::map<std::string, uint32_t> LocationInfo;
	LocationInfo m_locationInfos;

public:
	void init(InputStream &slocation)
	{
		std::string sem = ";";

		for ( ; !slocation.eof(); )
		{
			std::string line;

			ReadLine(slocation, line);

			if ( line.empty() || line[0] == '#' )
			{
				continue;
			}

			StrTrimVec<std::string, std::vector<std::string> > lns;
			Split(line, sem, lns);

			if ( lns.v.size() < 2 )
			{
				continue;
			}

			std::string locKey = lns.v[1];
			std::transform(locKey.begin(), locKey.end(), locKey.begin(), ::toupper);
			uint32_t locVal = (uint32_t)atoi(lns.v[0].c_str());

			m_locationInfos[locKey] = locVal;
		}
	}

	uint32_t get(std::string locKey)
	{
		std::transform(locKey.begin(), locKey.end(), locKey.begin(), ::toupper);
		if ( m_locationInfos.find(locKey) != m_locationInfos.end() )
		{
			return m_locationInfos[locKey];
		}

		return 0;
	}
};

class GroupMaker
{
	uint32_t m_ispid;
	LocationMap &m_locationMap;
	InputStream &m_siplist;
	OutputStream &m_sgroup;

	typedef std::map<uint32_t, RangeManager<uint32_t> > LocationRanges;
	LocationRanges m_locationRanges;

public:
	GroupMaker(int ispid, LocationMap &locationMap, InputStream &fiplist, OutputStream &fgroup)
		: m_ispid(ispid)
		, m_locationMap(locationMap)
		, m_siplist(fiplist)
		, m_sgroup(fgroup)
	{

	}

	bool process()
	{
		std::string sem = ";";
		std::string dot = ".";

		for ( ; !m_siplist.eof(); )
		{
			std::string line;

			ReadLine(m_siplist, line);

			if ( line.empty() || line[0] == '#' )
			{
				continue;
			}

			StrTrimVec<std::string, std::vector<std::string> > lns;
			Split(line, sem, lns);

			if ( lns.v.size() < 2 )
			{
				continue;
			}

			S2UIVec ip1s, ip2s;
			Split(lns.v[0], dot, ip1s);
			Split(lns.v[1], dot, ip2s);

			if ( ip1s.v.size() != 4 ||
				(ip2s.v.size() != 4  && ip2s.v.size() != 1) )
			{
				continue;
			}

			uint32_t ip1 = ip1s.v[0] * 0x1000000 + ip1s.v[1] * 0x10000 + ip1s.v[2] * 0x100 + ip1s.v[3];
			uint32_t ip2 = 0;
			if ( ip2s.v.size() == 4 )
			{
				ip2 = ip2s.v[0] * 0x1000000 + ip2s.v[1] * 0x10000 + ip2s.v[2] * 0x100 + ip2s.v[3];
			}
			else
			{
				ASSERT( ip2s.v.size() == 1 );
				if ( ip2s.v[0] >= 32 )
					continue;
				uint32_t mask = 0xFFFFFFFF;
				ip2 = ip1 + (mask >> ip2s.v[0]);
			}

			uint32_t location = 0;
			if ( lns.v.size() >= 3 )
			{
				location = m_locationMap.get(lns.v[2]);
			}

			if ( location != 0 )
				m_locationRanges[0].add(ip1, ip2);

			m_locationRanges[location].add(ip1, ip2);
		}

		return true;
	}

	bool output()
	{
		RangeSimpleManager<uint32_t, uint32_t> whole;

		for ( LocationRanges::iterator i = m_locationRanges.begin();
			i != m_locationRanges.end(); ++i )
		{
			uint32_t location = i->first;
			RangeManager<uint32_t> &lrs = i->second;

			for ( RangeManager<uint32_t>::Ranges::iterator j = lrs.ranges.begin();
				j != lrs.ranges.end(); ++j )
			{
				whole.add(j->first, j->second, location);
			}
		}

		if ( whole.ranges.empty() )
		{
			return false;
		}

		size_t bufSize = sizeof(IPINFO_GROUP) + (whole.ranges.size() - 1) * sizeof(IPINFO_ITEM);
		PIPINFO_GROUP group = (PIPINFO_GROUP)calloc(bufSize, 1);

		group->ispid = m_ispid;
		group->count = whole.ranges.size();

		size_t max_last = 0;
		size_t item_idx = 0;
		for ( RangeSimpleManager<uint32_t, uint32_t>::Ranges::iterator i = whole.ranges.begin();
			i != whole.ranges.end(); ++i )
		{
			RangeSimpleManager<uint32_t, uint32_t>::Range &rg = *i;
			IPINFO_ITEM &info = group->items[item_idx];
			info.first = rg.first;
			info.second = rg.second;
			info.location = rg.data;

			if ( rg.second >= max_last )
			{
				info.idx_sup = item_idx;
				max_last = rg.second;
			}
			else
			{
				ASSERT( item_idx > 0 );
				for ( size_t j = item_idx - 1; j >= 0; )
				{
					if ( group->items[j].second > rg.second )
					{
						info.idx_sup = j;
						break;
					}
					else
					{
						ASSERT( group->items[j].idx_sup < j );
						j = group->items[j].idx_sup;
					}
				}
			}

			++item_idx;
		}

		bool f = m_sgroup.write(group, bufSize) && m_sgroup.flush();

		free(group);

		return f;
	}
};


void usage(char* prog)
{
	printf( "usage : %s --group ispid location_file iplist_file group_file\n", prog );
	printf( "        %s --ipdb dbid country group1_file group2_file ... ipdb_file\n", prog );
	printf( "        %s --query ipdb_file\n", prog );
}

int main(int argc, char* argv[])
{
	if ( argc < 2 )
	{
		usage(argv[0]);
		return 1;
	}

	if ( strcmp(argv[1], "--group") == 0 )
	{
		if ( argc < 6 )
		{
			usage(argv[0]);
			return 1;
		}

		uint32_t ispid = (uint32_t)atoi(argv[2]);
		LPCSTR location = argv[3];
		LPCSTR iplist = argv[4];
		LPCSTR group = argv[5];

		LocationMap locationMap;
		{
			FileInputStream flocation;
			if ( !flocation.open(location) )
			{
				printf( "Error : Cannot open loc file\n" );
				return 2;
			}
			locationMap.init(flocation);
		}

		FileInputStream fiplist;
		if ( !fiplist.open(iplist) )
		{
			printf( "Error : Cannot open iplist file\n" );
			return 2;
		}

		FileOutputStream fgroup;
		if ( !fgroup.open(group) )
		{
			printf( "Error : Cannot create group file\n" );
			return 2;
		}

		GroupMaker maker(ispid, locationMap, fiplist, fgroup);
		if ( !(maker.process() && maker.output()) )
		{
			printf( "Error : Make group file failed\n" );
			return 3;
		}

		return 0;
	}
	else if ( strcmp(argv[1], "--ipdb") == 0 )
	{
		if ( argc < 6 )
		{
			usage(argv[0]);
			return 1;
		}

		uint64_t dbid = (uint32_t)atoll(argv[2]);
		uint32_t country = (uint32_t)atoi(argv[3]);

		FileOutputStream fipdb;
		if ( !fipdb.open(argv[argc - 1]) )
		{
			printf( "Error : Cannot create ipdb file\n" );
			return 2;
		}

		int groupCount = argc - 5;
		ASSERT(groupCount >  0);

		size_t ipdbSize = sizeof(IPDB) + (groupCount - 1) * sizeof(uint32_t);
		PIPDB ipdb = (PIPDB)calloc(ipdbSize, 1);

		ipdb->dbid = dbid;
		ipdb->country = country;
		ipdb->count = groupCount;

		FileWrapper* pfs = new FileWrapper[groupCount];

		uint32_t group_off = ipdbSize;

		for ( int i = 0; i < groupCount; ++i )
		{
			FileWrapper &pf = pfs[i];
			pf = fopen(argv[4 + i], "rb");
			if ( pf == 0 )
			{
				printf( "Error : Cannot open group file [%d]\n", i + 1 );
				return 2;
			}

			IPINFO_GROUP group;
			if ( fread(&group, sizeof(group), 1, pf) != 1 )
			{
				printf( "Error : Group file [%d] format\n", i + 1 );
				return 2;
			}

			int64_t sz;
			if ( fseeko(pf, 0, SEEK_END) != 0 ||
				(sz = ftello(pf)) == -1 ||
				fseeko(pf, 0, SEEK_SET) != 0 ||
				sz > 100 * 1024 * 1024 ) // > 100M
			{
				printf( "Error : Group file [%d] format\n", i + 1 );
				return 2;
			}

			if ( sz != sizeof(IPINFO_GROUP) + (group.count - 1) * sizeof(IPINFO_ITEM) )
			{
				printf( "Error : Group file [%d] format\n", i + 1 );
				return 2;
			}

			ipdb->group_off[i] = group_off;
			group_off += (uint32_t)sz;
		}

		if ( !fipdb.write(ipdb, ipdbSize) )
		{
			printf( "Error : Make ipdb file failed\n" );
			return 3;
		}

		for ( int i = 0; i < groupCount; ++i )
		{
			FileWrapper &pf = pfs[i];

			char buffer[1024];

			for ( ; ; )
			{
				unsigned int readed = fread(buffer, 1, 1024, pf);

				if ( readed > 0 )
				{
					if ( !fipdb.write(buffer, readed) )
					{
						printf( "Error : Make ipdb file failed\n" );
						return 3;
					}
				}

				if ( readed != 1024 )
				{
					break;
				}
			}
		}

		delete [] pfs;
		free(ipdb);

		return 0;
	}
	else if ( strcmp(argv[1], "--query") == 0 )
	{
		if ( argc < 3 )
		{
			usage(argv[0]);
			return 1;
		}

		LPCSTR ipdb = argv[2];
		IPDBH dbh = ipdb_init(ipdb);
		if ( dbh == NULL )
		{
			printf( "Error : IPDB init error\n" );
			return 2;
		}

		LPCSTR COUNTRY_NAME[] = {"未知","中国"};
		int COUNTRY_NAME_COUNT = sizeof(COUNTRY_NAME) / sizeof(COUNTRY_NAME[0]);
		LPCSTR CNISP_NAME[] = {"未知","电信","网通","铁通","移动","教育网"};
		int CNISP_NAME_COUNT = sizeof(CNISP_NAME) / sizeof(CNISP_NAME[0]);
		LPCSTR CNLOC_NAME[] = {"未知","北京","天津","河北","山西","内蒙古","辽宁","吉林","黑龙江","上海","江苏","浙江","安徽","福建","江西","山东","河南","湖北","湖南","广东","广西","海南","重庆","四川","贵州","云南","西藏","陕西","甘肃","青海","宁夏","新疆","香港","澳门","台湾"};
		int CNLOC_NAME_COUNT = sizeof(CNLOC_NAME) / sizeof(CNLOC_NAME[0]);

		for ( std::string str; getline(std::cin, str);  )
		{
			uint32_t ip = 0;
			S2UIVec ips;
			std::string dot = ".";
			Split(str, dot, ips);
			if ( ips.v.size() != 4 )
			{
				printf( "Error : IP format error\n" );
				continue;
			}
			ip = ips.v[0] * 0x1000000 + ips.v[1] * 0x10000 + ips.v[2] * 0x100 + ips.v[3];

			IPINFO ipi = {0};
			ipdb_query(dbh, ip, &ipi);
			if ( ipi.country >= COUNTRY_NAME_COUNT )
			{
				ASSERT(0);
				ipi.isp = COUNTRY_Unknown;
			}
			if ( ipi.isp >= CNISP_NAME_COUNT )
			{
				ASSERT(0);
				ipi.isp = CNISP_Unknown;
			}
			if ( ipi.loc >= CNLOC_NAME_COUNT )
			{
				ASSERT(0);
				ipi.loc = CNLOC_Unknown;
			}

			printf( "%s:%s:%s:%s\n", str.c_str(), COUNTRY_NAME[ipi.country], CNISP_NAME[ipi.isp], CNLOC_NAME[ipi.loc] );
		}

		ipdb_term(dbh);
		return 0;
	}
	else
	{
		usage(argv[0]);
		return 1;
	}
}

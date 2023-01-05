/*
 *  File   : ipdb.cpp
 *  Author : Jiang Wangsheng
 *  Date   : 2012/4/20 16:57:27
 *  Brief  :
 *
 *  $Id: $
 */

#include "ipdb.h"
#include "ipdb_desc.h"

#include "misc/common_mp.h"


class IPDBManager
{
	PIPDB m_ipdb;

public:
	IPDBManager()
	{
		m_ipdb = NULL;
	}

	~IPDBManager()
	{
		if ( m_ipdb != NULL )
		{
			free(m_ipdb);
			m_ipdb = NULL;
		}
	}


	bool init(PIPDB ipdb, size_t sz)
	{
		if ( ipdb->count <= 0 )
		{
			return false;
		}

		size_t _sz = sizeof(IPDB) + (ipdb->count - 1) * sizeof(uint32_t);

		for ( uint32_t i = 0; i < ipdb->count; ++i )
		{
			PIPINFO_GROUP group = (PIPINFO_GROUP)
				((char*)ipdb + ipdb->group_off[i]);
			if ( group->count <= 0 )
			{
				return false;
			}

			_sz += sizeof(IPINFO_GROUP) +
				(group->count - 1) * sizeof(IPINFO_ITEM);
		}

		if ( _sz != sz )
		{
			return false;
		}

		m_ipdb = ipdb;
		return true;
	}

	bool query(unsigned int ip, IPINFO &ipi)
	{
		ipi.country = COUNTRY_Unknown;

		for ( uint32_t i = 0; i < m_ipdb->count; ++i )
		{
			PIPINFO_GROUP group = (PIPINFO_GROUP)
				((char*)m_ipdb + m_ipdb->group_off[i]);

			if ( __query(group, ip, ipi) )
			{
				ipi.country = m_ipdb->country;
				return true;
			}
		}

		return false;
	}

private:
	bool __query(PIPINFO_GROUP group, uint32_t ip, IPINFO &ipi)
	{
		ASSERT( group->count > 0 );

		int ibeg = 0, iend = group->count - 1;
		int inow = (ibeg + iend) / 2, inex = inow + 1;

		for ( ; inex <= iend; inow = (ibeg + iend) / 2, inex = inow + 1 )
		{
			size_t now = group->items[inow].first;
			size_t nex = group->items[inex].first;

			if ( now <= ip && nex >= ip )
			{
				break;
			}

			if ( now > ip )
			{
				iend = inow;
				continue;
			}

			if ( nex < ip )
			{
				ibeg = inex;
				continue;
			}
		}

		while ( inex <= iend )
		{
			size_t nex = group->items[inex].first;
			if ( nex == ip )
			{
				inow = inex;
				inex = inow + 1;
				continue;
			}
			break;
		}

		int icur = inow;
		for ( ; ; )
		{
			IPINFO_ITEM &now = group->items[icur];
			if ( now.first <= ip  && now.second >= ip )
			{
				ipi.isp = group->ispid;
				ipi.loc = now.location;
				return true;
			}

			if ( now.idx_sup < (uint32_t)icur)
			{
				icur = now.idx_sup;
				continue;
			}

			break;
		}

		return false;
	}
};

// -------------------------------------------------------------------------

IPDBH ipdb_init(const char* db_path)
{
	ASSERT( db_path != NULL );

	FileWrapper pf;
	pf = fopen(db_path, "rb");
	if ( pf == 0 )
	{
		return NULL;
	}

	int64_t sz;
	if ( fseeko(pf, 0, SEEK_END) != 0 ||
		(sz = ftello(pf)) == -1 ||
		fseeko(pf, 0, SEEK_SET) != 0 ||
		sz > 100 * 1024 * 1024 )
	{
		return NULL;
	}

	void* buffer = malloc((size_t)sz);
	if ( buffer == NULL )
	{
		return NULL;
	}

	if ( fread(buffer, 1, (size_t)sz, pf) != sz )
	{
		free(buffer);
		return NULL;
	}


	IPDBManager* manager = new IPDBManager;
	if ( manager == NULL )
	{
		free(buffer);
		return NULL;
	}

	if ( !manager->init((PIPDB)buffer, (size_t)sz) )
	{
		delete manager;
		free(buffer);
		return NULL;
	}

	return (IPDBH)manager;
}

void ipdb_term(IPDBH db)
{
	ASSERT( db != NULL );

	IPDBManager* manager = (IPDBManager*)db;
	delete manager;
}

int ipdb_query(IPDBH db, unsigned int ip, PIPINFO ipi)
{
	ASSERT( db != NULL );
	ASSERT( ipi != NULL );

	IPDBManager* manager = (IPDBManager*)db;
	return manager->query(ip, *ipi) ? 0 : -1;
}

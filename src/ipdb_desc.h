/*
 *  File   : ipdb_desc.h
 *  Author : Jiang Wangsheng
 *  Date   : 2012/04/17 09:34:23
 *  Brief  :
 *
 *  $Id: $
 */
#ifndef __IPDB_DESC_H__
#define __IPDB_DESC_H__

#include "misc/common_mp.h"

#pragma pack(push, 1)

// IP range
typedef struct _IPINFO_ITEM
{
	// [first, second]
	uint32_t first;
	uint32_t second;

	// prev range idx contain me
	// idx_sup <= idx_me
	uint32_t idx_sup;

	// location & 0xFF ==> province
	uint32_t  location;

} IPINFO_ITEM, *PIPINFO_ITEM;


// ISP
typedef struct _IPINFO_GROUP
{
	uint32_t ispid;
	uint32_t count;

	// items[count]
	IPINFO_ITEM items[1];

} IPINFO_GROUP, *PIPINFO_GROUP;


// IPDB
typedef struct _IPDB
{
	uint64_t dbid;

	uint32_t country;
	uint32_t count;

	// groups[count]
	// &db + group_off[i] ==> PIPINFO_GROUP
	uint32_t group_off[1];

} IPDB, *PIPDB;

#pragma pack(pop)

/*
 1 中国 cn

 1 电信 cnt
 2 网通 cnc
 3 铁通 ctt
 4 移动 cmcc
 5 教育 edu

 1 北京
 2 天津
 3 河北
 4 山西
 5 内蒙古
 6 辽宁
 7 吉林
 8 黑龙江
 9 上海
 10 江苏
 11 浙江
 12 安徽
 13 福建
 14 江西
 15 山东
 16 河南
 17 湖北
 18 湖南
 19 广东
 20 广西
 21 海南
 22 重庆
 23 四川
 24 贵州
 25 云南
 26 西藏
 27 陕西
 28 甘肃
 29 青海
 30 宁夏
 31 新疆
 32 香港
 33 澳门
 34 台湾
*/

#endif // !__IPDB_DESC_H__

/*
 *  File   : ipdb.h
 *  Author : Jiang Wangsheng
 *  Date   : 2012/4/20 16:57:23
 *  Brief  :
 *
 *  $Id: $
 */
#ifndef __IPDB_H__
#define __IPDB_H__

#ifdef __cplusplus
extern "C" {
#endif


#pragma pack(push, 1)

enum COUNTRY
{
	COUNTRY_Unknown = 0,
	COUNTRY_CHINA = 1, // 中国
};

// 运营商信息
enum CNISP
{
	CNISP_Unknown = 0,
	CNISP_CNT  = 1, // 电信
	CNISP_CNC  = 2, // 网通
	CNISP_CTT  = 3, // 铁通
	CNISP_CMCC = 4, // 移动
	CNISP_EDU  = 5, // 教育
};

// 地域信息
enum CNLOC
{
	CNLOC_Unknown = 0,
	CNLOC_BJ = 1,  // 北京
	CNLOC_TJ = 2,  // 天津
	CNLOC_HE = 3,  // 河北
	CNLOC_SX = 4,  // 山西
	CNLOC_NM = 5,  // 内蒙古
	CNLOC_LN = 6,  // 辽宁
	CNLOC_JL = 7,  // 吉林
	CNLOC_HL = 8,  // 黑龙江
	CNLOC_SH = 9,  // 上海
	CNLOC_JS = 10, // 江苏
	CNLOC_ZJ = 11, // 浙江
	CNLOC_AH = 12, // 安徽
	CNLOC_FJ = 13, // 福建
	CNLOC_JX = 14, // 江西
	CNLOC_SD = 15, // 山东
	CNLOC_HA = 16, // 河南
	CNLOC_HB = 17, // 湖北
	CNLOC_HN = 18, // 湖南
	CNLOC_GD = 19, // 广东
	CNLOC_GX = 20, // 广西
	CNLOC_HI = 21, // 海南
	CNLOC_CQ = 22, // 重庆
	CNLOC_SC = 23, // 四川
	CNLOC_GZ = 24, // 贵州
	CNLOC_YN = 25, // 云南
	CNLOC_XZ = 26, // 西藏
	CNLOC_SN = 27, // 陕西
	CNLOC_GS = 28, // 甘肃
	CNLOC_QH = 29, // 青海
	CNLOC_NX = 30, // 宁夏
	CNLOC_XJ = 31, // 新疆
	CNLOC_HK = 32, // 香港
	CNLOC_MC = 33, // 澳门
	CNLOC_TW = 34, // 台湾
};

// LPCSTR COUNTRY_NAME[] = {"未知","中国"};
// int COUNTRY_NAME_COUNT = sizeof(COUNTRY_NAME) / sizeof(COUNTRY_NAME[0]);
// LPCSTR CNISP_NAME[] = {"未知","电信","网通","铁通","移动","教育网"};
// int CNISP_NAME_COUNT = sizeof(CNISP_NAME) / sizeof(CNISP_NAME[0]);
// LPCSTR CNLOC_NAME[] = {"未知","北京","天津","河北","山西","内蒙古","辽宁","吉林","黑龙江","上海","江苏","浙江","安徽","福建","江西","山东","河南","湖北","湖南","广东","广西","海南","重庆","四川","贵州","云南","西藏","陕西","甘肃","青海","宁夏","新疆","香港","澳门","台湾"};
// int CNLOC_NAME_COUNT = sizeof(CNLOC_NAME) / sizeof(CNLOC_NAME[0]);

typedef struct _IPINFO
{
	int country;
	int isp;
	int loc;
} IPINFO, *PIPINFO;

#pragma pack(pop)


typedef void* IPDBH;


IPDBH ipdb_init(const char* db_path);

void ipdb_term(IPDBH db);


// only ipv4
// return 0 found, other not found , 
// thread-safe function , mutiple thread share the same handle
int ipdb_query(IPDBH db, unsigned int ip, PIPINFO ipi);


#ifdef __cplusplus
}
#endif

#endif /* __IPDB_H__ */

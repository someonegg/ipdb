#!/bin/bash
# ./gen_ipdb.sh [-q|-s|-m] datadir

bindir=$(cd "$(dirname "$0")"; pwd)
query_rest_time=2

exec_whle=1
exec_qury=0
exec_sort=0
exec_make=0

if [ $1 = "-q" ]; then
	shift
	exec_whle=0
	exec_qury=1
fi
if [ $1 = "-s" ]; then
        shift
        exec_whle=0
        exec_sort=1
fi
if [ $1 = "-m" ]; then
        shift
        exec_whle=0
        exec_make=1
fi

if [ -n $1 ]; then
    cd $1
fi

if [ $exec_whle = 1 ] || [ $exec_qury = 1 ]; then
rm -rf isp && mkdir isp
fi
if [ $exec_whle = 1 ] || [ $exec_sort = 1 ]; then
rm -rf ip && mkdir ip
rm -rf loc && mkdir loc
fi
if [ $exec_whle = 1 ] || [ $exec_make = 1 ]; then
rm -rf grup && mkdir grup
fi


query_isp() {
	whois3 -h whois.apnic.net -T inetnum -i ml $1 | sed -n -e '/[^ \t]/{H;$!d;}' -e 'x;/inetnum:/p;'
	sleep $query_rest_time
	whois3 -h whois.apnic.net -T inetnum,route -i mb $1 | sed -n -e '/[^ \t]/{H;$!d;}' -e 'x;/inetnum:/p;/route:/p;'
	sleep $query_rest_time
}

query_isp_ex() {
	for prn in BJ TJ HE SX NM LN JL HL SH JS ZJ AH FJ JX SD HA HB HN GD GX HI CQ SC GZ YN XZ SN SHAANXI GS QH NX NINGXIA XJ XINJIANG HK; do
		whois3 -h whois.apnic.net -T inetnum -i ml $1"-"${prn} | sed -n -e '/[^ \t]/{H;$!d;}' -e 'x;/inetnum:/p;'
		sleep $query_rest_time
		whois3 -h whois.apnic.net -T inetnum,route -i mb $1"-"${prn} | sed -n -e '/[^ \t]/{H;$!d;}' -e 'x;/inetnum:/p;/route:/p;'
		sleep $query_rest_time
	done
}

pick_ip_nn() {
sed -n -e '/^inetnum:/{s/^inetnum:[ \t]*\([0-9\.]*\)[ \t]*-[ \t]*\([0-9\.]*\)[ \t]*/\1;\2/;h;d;}' \
       -e '/^netname:/{s/^netname:[ \t]*\([^ \t].*[^ \t]\)[ \t]*/\1/;H;d;}' \
       -e '/^route:/{s/^route:[ \t]*\([0-9\.]*\)[ \t]*\/[ \t]*\([0-9\.]*\)[ \t]*/\1;\2;\1/;h;d;}' \
       -e '/^descr:/{s/^descr:[ \t]*\([^ \t].*[^ \t]\)[ \t]*/\1/;H;d;}' \
       -e '/[^ \t]/{$!d;}' \
       -e 'x;s/\n/;/g;p;' $1
}


query_abandon_ip() {
	whois3 -h whois.apnic.net -T inetnum -i mb MAINT-CNNIC-AP | sed -n -e '/[^ \t]/{H;$!d;}' -e 'x;/inetnum:/p;' >> isp/cnnic
	sleep $query_rest_time
	whois3 -h whois.apnic.net -T inetnum -i mb MAINT-NET-AP | sed -n -e '/[^ \t]/{H;$!d;}' -e 'x;/country:[ \t]*CN/p;' | sed -n -e '/[^ \t]/{H;$!d;}' -e 'x;/inetnum:/p;' >> isp/cnnic
	sleep $query_rest_time
	cat isp/cnnic | sed -n -e '/[^ \t]/{H;$!d;}' -e 'x;/netname:[ \t]*CHINANET/p;/MAINT-CHINANET/p;' >> isp/cnt
	cat isp/cnnic | sed -n -e '/[^ \t]/{H;$!d;}' -e 'x;/netname:[ \t]*UNICOM/p;/netname:[ \t]*CNCGROUP/p;/netname:[ \t]*SANXIN/p;/MAINT-CNCGROUP-RR/p;' >> isp/cnc
	cat isp/cnnic | sed -n -e '/[^ \t]/{H;$!d;}' -e 'x;/netname:[ \t]*CRTC/p;/netname:[ \t]*CTTNET/p;/netname:[ \t]*RAR237/p;/netname:[ \t]*Etelnet/p;/netname:[ \t]*Forest-Eternal/p;/MAINT-NORTHSTAR-APNIC/p;' >> isp/ctt
	cat isp/cnnic | sed -n -e '/[^ \t]/{H;$!d;}' -e 'x;/netname:[ \t]*CMCC/p;' >> isp/cmcc
	cat isp/cnnic | sed -n -e '/[^ \t]/{H;$!d;}' -e 'x;/netname:[ \t]*CERNET/p;/netname:[ \t]*BJENET/p;' >> isp/edu
}


query_whole_cn() {
	wget http://ftp.apnic.net/apnic/stats/apnic/delegated-apnic-latest -O isp/apnic >> /dev/null 2>&1
}

sort_whole_cn() {
grep 'apnic|CN|ipv4|' isp/apnic | cut -f 4,5 -d'|'| sed -e 's/|/ /g' | while read ip cnt
do
mask=$(bc << MASK_EOF | tail -1
pow=32;
define log2(x) {
if (x<=1) return (pow);
pow--;
return(log2(x/2));
}
log2($cnt)
MASK_EOF
)
	echo "$ip;$mask" >> ip/cn_whole
done

echo "" > loc/cn_whole
}


if [ $exec_whle = 1 ] || [ $exec_qury = 1 ]; then

# ISP顶级
query_isp MAINT-CHINANET >> isp/cnt
query_isp_ex MAINT-CHINANET >> isp/cnt
query_isp MAINT-CN-CHINANET >> isp/cnt
query_isp_ex MAINT-CN-CHINANET >> isp/cnt
query_isp MAINT-IP-WWF >> isp/cnt
query_isp MAINT-CNCGROUP >> isp/cnc
query_isp_ex MAINT-CNCGROUP >> isp/cnc
query_isp MAINT-CNCGROUP-RR >> isp/cnc
query_isp MAINT-CN-CRTC >> isp/ctt
query_isp MAINT-NORTHSTAR-APNIC >> isp/ctt
query_isp MAINT-CN-CMCC >> isp/cmcc
query_isp MAINT-CERNET-AP >> isp/edu
query_isp MAINT-BJENET-CNNIC-AP >> isp/edu

query_abandon_ip
query_whole_cn

fi


if [ $exec_whle = 1 ] || [ $exec_sort = 1 ]; then

# 提取IP地址
pick_ip_nn isp/cnt >> ip/cnt
pick_ip_nn isp/cnc >> ip/cnc
pick_ip_nn isp/ctt >> ip/ctt
pick_ip_nn isp/cmcc >> ip/cmcc
pick_ip_nn isp/edu >> ip/edu
cat cmcc.txt       >> ip/cmcc

# 生成LOCATION信息
${bindir}"/cnt_loc.sh" ip/cnt > loc/cnt
${bindir}"/cnc_loc.sh" ip/cnc > loc/cnc
${bindir}"/ctt_loc.sh" ip/ctt > loc/ctt
${bindir}"/cmcc_loc.sh" ip/cmcc > loc/cmcc
${bindir}"/edu_loc.sh" ip/edu > loc/edu

sort_whole_cn

fi


if [ $exec_whle = 1 ] || [ $exec_make = 1 ]; then

ipdbmaker=${bindir}"/ipdbmaker"
$ipdbmaker --group 1 loc/cnt ip/cnt grup/cnt
$ipdbmaker --group 2 loc/cnc ip/cnc grup/cnc
$ipdbmaker --group 3 loc/ctt ip/ctt grup/ctt
$ipdbmaker --group 4 loc/cmcc ip/cmcc grup/cmcc
$ipdbmaker --group 5 loc/edu ip/edu grup/edu
$ipdbmaker --group 0 loc/cn_whole ip/cn_whole grup/cn_whole

# 此顺序即查询顺序，一般情况请确保cn_whole在最后
dbid=$(date +"%s")
$ipdbmaker --ipdb $dbid 1 grup/cnt grup/cnc grup/ctt grup/cmcc grup/edu grup/cn_whole ipdb_cnt
$ipdbmaker --ipdb $dbid 1 grup/cnc grup/ctt grup/cnt grup/cmcc grup/edu grup/cn_whole ipdb_cnc

fi


# 1 中国 cn

# 1 电信 cnt
# 2 网通 cnc
# 3 铁通 ctt
# 4 移动 cmcc
# 5 教育 edu


# 1;北京
# 2;天津
# 3;河北
# 4;山西
# 5;内蒙古
# 6;辽宁
# 7;吉林
# 8;黑龙江
# 9;上海
# 10;江苏
# 11;浙江
# 12;安徽
# 13;福建
# 14;江西
# 15;山东
# 16;河南
# 17;湖北
# 18;湖南
# 19;广东
# 20;广西
# 21;海南
# 22;重庆
# 23;四川
# 24;贵州
# 25;云南
# 26;西藏
# 27;陕西
# 28;甘肃
# 29;青海
# 30;宁夏
# 31;新疆
# 32;香港
# 33;澳门
# 34;台湾


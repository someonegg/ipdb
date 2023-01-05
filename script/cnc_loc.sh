#!/bin/bash
#

cat $1 | grep -i "beijing\?[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "1;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "tianjing\?[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "2;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "hebei[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "3;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "shang\?xi[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "4;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "[ln]eimen[^ \t]*[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "5;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "[ln]iao[ln]ing\?[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "6;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "ji[ln]ing\?[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "7;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "hei[ln]on[^ \t]*[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "8;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "shang\?hai[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "9;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "jiang\?su[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "10;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "zhejiang\?[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "11;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "anhui[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "12;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "fujian[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "13;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "jiang\?xi[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "14;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "shandong\?[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "15;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "he[ln]an[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "16;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "hubei[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "17;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "hu[ln]an[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "18;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "guang\?dong\?[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "19;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "guang\?xi[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "20;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "hai[ln]an[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "21;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "chong\?qing\?[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "22;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "sichuan[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "23;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "guizhou[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "24;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "yun[ln]an[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "25;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "xizang\?[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "26;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "shaanxi[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "27;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "shanxi(sn)[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "27;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "shannxi[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "27;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "gansu[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "28;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "qing\?hai[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "29;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "[ln]ing\?xia[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "30;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq
cat $1 | grep -i "xing\?jiang\?[ \t]*provin[^ \t]*[ \t]*network" | awk -F\; '{ print "31;" $3 }' | grep '^[^ \t]\{1,30\}$' | sort | uniq

idx=1
for prn in BJ TJ HE SX NM LN JL HL SH JS ZJ AH FJ JX SD HA HB HN GD GX HI CQ SC GZ YN XZ SN GS QH NX XJ HK; do
	echo "${idx};UNICOM-${prn}"
	echo "${idx};CNCGROUP-${prn}"
	idx=$((idx+1))
done


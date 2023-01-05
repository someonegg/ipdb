#!/bin/bash
# test_ipdb.sh [-c|-a] datadir

bindir=$(cd "$(dirname "$0")"; pwd)

test_whle=1
test_coverage=0
test_accuracy=0

if [ $1 = "-c" ]; then
	shift
	test_whle=0
	test_coverage=1
fi
if [ $1 = "-a" ]; then
	shift
	test_whle=0
	test_accuracy=1
fi

if [ -n $1 ]; then
    cd $1
fi


rm -rf test && mkdir test


if [ $test_whle = 1 ] || [ $test_coverage = 1 ]; then

db_host="125.89.78.3"
db_user="dlrs"
db_pass="3335688"
db_name="download_report"

mysql="mysql"

sql_op_s() {
	${mysql} -h${db_host} -u${db_user} -p${db_pass} --skip-column-names << SQL_ENTRY_TAG_S
use ${db_name};
$1;
exit
SQL_ENTRY_TAG_S
}


today=$(date +%Y.%m.%d)

sql_op_s "select ip from report where record_time>'${today}-10' limit 20000" | sort | uniq > test/iptop
${bindir}"/ipdbmaker" --query ipdb_cnt < test/iptop > test/ipinfo
cat test/ipinfo | grep -v '教育' | grep '中国:.*:未知' > test/ipunknown

total=$(cat test/ipinfo | wc -l)
foreign=$(cat test/ipinfo | grep '未知:未知:未知' | wc -l)
unknown1=$(cat test/ipinfo | grep '中国:未知' | wc -l)
unknown2=$(cat test/ipinfo | grep '中国:.*:未知' | wc -l)

echo "Total       : ${total}"
echo "Foreign     : ${foreign}"
echo "Unknown ISP : ${unknown1}"
echo "Unknown LOC : ${unknown2}"


fi


if [ $test_whle = 1 ] || [ $test_accuracy = 1 ]; then

ip_count=0

query_ip138() {
	tmp_file="/tmp/test_ipdb_ip138"
	rm $tmp_file
	wget "http://wap.ip138.com/ip.asp?ip=$1" -O $tmp_file >> /dev/null 2>&1
	cat $tmp_file
}

while [ $ip_count -lt 100 ]; do

	ip=$(($RANDOM%256)).$(($RANDOM%256)).$(($RANDOM%256)).$(($RANDOM%256))

	ipinfo=$(${bindir}"/ipdbmaker" --query ipdb_cnt << IP_ENTRY_TAG_S
${ip}
IP_ENTRY_TAG_S
)

	ipinfo=$(echo ${ipinfo} | grep '中国:')
	if [ -z $ipinfo ]; then
		continue
	fi

	isp=$(echo $ipinfo | awk -F: '{ print $3 }')
	loc=$(echo $ipinfo | awk -F: '{ print $4 }')

	ipinfo_138=$(query_ip138 $ip | sed -n -e 's/^.*<b>查询结果：\(.*\)<\/b>.*$/\1/p')

	echo $ip",\""$loc" "$isp"\",\""$ipinfo_138"\"" >> test/accuracy

	sleep 1s
	ip_count=$((ip_count+1))

done

fi


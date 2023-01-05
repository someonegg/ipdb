#!/usr/bin/python
# must use this encoding and save to utf8 boom format
# -*- coding=utf-8 -*-
# read extracted qqwarp.dat to destination file 

import sys
import os

key_word_list=["移动" , "移动(全省通用)" , "移动机房"]
key_loc_word=['北京' , '天津' , '重庆', '上海']
key_p_word=['省']

key_loc_list=[
 "北京;CMNET-beijing" ,                
 "天津;CMNET-tianjin" ,
 "河北;CMNET-hebei"   ,
 "山西;CMNET-shangxi" ,
 "内蒙;CMNET-neimeng"     ,
 "辽宁;CMNET-liaoning" ,
 "吉林;CMNET-jilin",
 "黑龙江;CMNET-heilongjiang",
 "上海;CMNET-shanghai",
 "江苏;CMNET-jiangsu",
 "浙江;CMNET-zhejiang",
 "安徽;CMNET-anhui",
 "福建;CMNET-fujian",
 "江西;CMNET-jiangxi",
 "山东;CMNET-shandong",
 "河南;CMNET-henan",
 "湖北;CMNET-hubei",
 "湖南;CMNET-hunan",
 "广东;CMNET-guangdong",
 "广西;CMNET-guangxi",
 "海南;CMNET-hainan",
 "重庆;CMNET-chongqing",
 "四川;CMNET-sichuan",
 "贵州;CMNET-guizhou",
 "云南;CMNET-yunnan",
 "西藏;CMNET-xizang",
 "山西;CMNET-shanxi",
 "甘肃;CMNET-gansu",
 "青海;CMNET-qinghai",
 "宁夏;CMNET-ningxia",
 "新疆;CMNET-xinjiang"   
]

if __name__ == '__main__':	
	with open( sys.argv[1] , 'r') as f:		
		key_word_list=[kk.decode('utf-8').encode('gbk') for kk in key_word_list]
		key_loc_word=[kk.decode('utf-8').encode('gbk') for kk in key_loc_word]
		key_p_word=[kk.decode('utf-8').encode('gbk') for kk in key_p_word]
		f_first=f_last=['111','111','111']
		location_map={}
		for ii in key_loc_list:
			kv_list=ii.split(';')
			k=kv_list[0].decode('utf-8').encode('gbk')
			v=kv_list[1].decode('utf-8').encode('gbk')
			location_map[k]=v			
			
		for line in f.readlines():		
			#print line			
			field=line.strip().split()
			f_valid = 0			
			
			if len(field) >= 4:
				f3=field[3].strip()				
				for k in key_word_list:
					if k == f3: f_valid=1 ; break
				if f_valid:
					#print "valid"
					f0=field[0].strip()
					f1=field[1].strip()
					f2=field[2].strip()
					
					#print f0,f1,f2
					
					f_loc="111"
					p_index=f2.find(key_p_word[0])
					if p_index <> -1:
						f_loc=f2[:p_index]
					else:
						for kk in key_loc_word:
							p_index=f2.find(kk)
							if p_index <> -1:
								f_loc=kk
								break
								
					if f_loc <> '111':
						f2=f_loc
						if f2 <> f_last[2]:
							ip_start=f_first[0]
							ip_end=f_last[1]
							loc=f_last[2]
							##output the result
							if f_first[0] <> '111':
								if location_map.has_key(loc):
									print "%s;%s;%s" %(ip_start,ip_end,location_map[loc])	
							f_first=f_last=[f0,f1,f2]
						else:
							f_last=[f0,f1,f2]
							
							
					
					
						
				
			
	
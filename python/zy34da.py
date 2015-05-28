# -*- coding: utf-8 -*-
"""
Created on Thu May 28 22:42:10 2015

@author: 瑾
"""

import math
count = 0
n = int(raw_input())
shuzi = 0
if n >= 2:
    for shuzi in range(2,n+1):
        for yinzi in range(2,int(math.sqrt(shuzi) + 1)):
            if shuzi % yinzi == 0:
                break
        else:
            sushu = shuzi
            sushu_tmp = sushu
            weishu = 0
            while sushu_tmp != 0:
                weishu += 1
                sushu_tmp /= 10
            if weishu == 1:
                count += 1
            elif weishu >= 2:
                i = 1
                while i < weishu:
                    shuzi_xunhuan = int(sushu/(10**i)+(sushu %(10**i) )* (10 ** (weishu - i)))
                    i += 1
                    for yinzi2 in range(2,int(math.sqrt(shuzi_xunhuan) + 1)):
                        if shuzi_xunhuan % yinzi2 == 0 :
                            i = weishu + 1
                            break
                if i == weishu:
                    count +=1

    print count
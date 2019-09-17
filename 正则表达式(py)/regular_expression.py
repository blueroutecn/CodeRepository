#-*- coding: gbk -*-


'''正则表达式 '''

import re


## ^      匹配字符串的开始
## $      匹配字符串的结尾
## ()     匹配子表达式的开始和结束的位置
## []     匹配中括号表达式

## \s     匹配任何空白字符（等价[\f\n\r\t\v]），包括空格、制表符、换页符等
## \S     匹配任何非空白字符（等价[^\f\n\r\t\v]）
## \d     匹配任意数字（等价[0-9]）
## \D     匹配任意非数字字符
## \b     匹配一个单词的边界，即字与空格间的位置
## \B     匹配非单词边界

## \t     匹配一个制表符
## \v     匹配一个垂直制表符
## \r     匹配一个回车符
## \n     匹配一个换行符
## \f     匹配一个换页符

## x*     匹配0次或者多次 x 字符，如zo*可以匹配z, zo, zoo...
## x+     匹配1次或者多次 x 字符, 如zo+可以匹配zo, zoo..., 不能匹配z.
## x?     匹配1次或者0 次 x 字符, 如do(es)?只能匹配do和does.
## x{n}   匹配n次x字符，    如o{2}只能匹配oo.
## x{n,}  匹配n次以上x字符，如o{2,}可以匹配oo, ooo, oooo...
## x{n,m} 匹配 x 字符至少 n 至 m 次，如o{2,4}只能匹配oo, ooo, oooo.
## (a|b|c)要么匹配 a，要么匹配 b，要么匹配 c
## [abc]  匹配其中任何一个字符
## [^abc] 匹配任何非abc字符
## [a-z]  匹配a-z内的任意字符
## [0-9]  匹配0-9内任意数字
## .      匹配任意单字符（除\n,\r）


# ^[a-z0-9_-]{3,15}$， 匹配用户名，要求长度在3-15之间，只能包含字符/数字/下划线/连接字符.


astr="A13fhg,.567ghy7DG87!'gty78"
m1=re.compile(r"\d\d")              #取一个两位数
print(m1.search(astr,2).group())    #从第二个字符开始取一个两位数
print(m1.findall(astr,3,12))        #从第3个字符开始，到第12个字符截止，取一个两位数 
# 运行结果：
#56
#['56']    


m2=re.compile(r"a",re.I)    #取出字符串中的“a”，不区分大小写
print(m2.findall(astr)) 
#运行结果：
#['A']


astr="A13fhg,.567ghy7DG87!'gty78" 
print(re.sub("7","9",astr))    #用"9"，替换字符串中的"7"
#运行结果：
#A13fhg,.569ghy9DG128!'gty98
 
 
m4=re.compile(r"\d+")       #编译字符串中所有数字
print(m4.sub("",astr))      #用空格替换字符串中所有数字
print(m4.sub("",astr,2))    #用空格替换字符串中数字，替换2次
#运行结果：
#Afhg,.ghyDG!'gty 
#Afhg,.ghy7DG87!'gty78
 
 
def f1(m):
    return m.group().upper()       #upper()：将字符串中的小写字母转为大写字母。
print(re.sub(r"[a-z]",f1,astr))    #用大写字母替换所有字符串中的小写字母
#运行结果：
#A13FHG,.567GHY7DG87!'GTY78


astr="A13fhg,.567ghy7DG87!'gty78" 
print(astr.split("13"))          #以数字"13"分割字符串 
print(re.split("\d{2}",astr))    #以两位数字分割字符串 
print(re.split("\d+",astr))    #以所有数字分割字符串
#运行结果：
#['A', "fhg,.567ghy7DG87!'gty78"] 
#['A', 'fhg,.', '7ghy7DG', "!'gty", ''] 
#['A', 'fhg,.', 'ghy', 'DG', "!'gty", '']


m3=re.compile(r"\d+")          #先预编译，再分割
print(m3.split(astr))
print(m3.split(astr,3))        #以所有数字分割字符串，分割三次
#运行结果：
#['A', 'fhg,.', 'ghy', 'DG', "!'gty", ''] 
#['A', 'fhg,.', 'ghy', "DG87!'gty78"]


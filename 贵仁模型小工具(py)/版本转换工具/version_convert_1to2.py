#-*- coding: gbk -*-

'''读取文本文件内容 '''
'''将贵仁模型1.0版一维模型输入文件，转为2.0版输入文件'''


'''
文件操作--------------------- 
os.getcwd()         #返回当前工作路径
os.path.isabs()     #判断是否是绝对路径
os.path.isfile()    #检验给出的路径是否是一个文件
os.path.isdir()     #检验给出的路径是否是一个目录
os.sep              #文件的路径分隔符 如:在windows上是 '\'  而在Linux上是 '/'

os.walk()  #文件遍历
for root, dirs, files in os.walk(file_dir):
    print('root_dir:', root)         # 当前目录路径
    print('sub_dirs:', dirs)         # 当前路径下所有子目录
    print('files:', files)           # 当前路径下所有非目录子文件

os.path.splitext()  #分离扩展名
os.system()         #运行shell命令:
os.path.exists()    #检验给出的路径是否真地存:
os.linesep          #给出当前平台使用的行终止符('\r\n', '\n', '\r')
os.chmod(file)      #修改文件权限与时间戳
os.exit()           #终止当前进程

目录操作--------------------- 
os.makedirs("file1/file2/file3")     #创建文件夹目录. 创建多级目录
os.mkdir("file")                     #创建文件夹. 但是上级目录必须存在
os.copy("oldfile","newfile")         #oldfile只能是文件夹，newfile可以是文件，也可以是目标目录
os.rename("oldname","newname")       #重命名文件（目录），文件或目录都是使用这条命令
os.removedirs()                      #删除多级目录, 要求必须是空的
os.rmdir("dir")	                     #只能删除空目录  
os.listdir()                         #返回指定目录下的所有文件和目录名
os.path.split(path)                  #返回一个路径的目录名和文件名   
os.path.dirname()                    #获取路径名
os.path.basename()                   #获取文件名
os.path.getsize(filename)            #获取文件大小
os.chdir("path")                     #换路径

文件读写操作--------------------- 
os.remove()         #删除文件 或空文件夹
os.stat()           #获取文件属性

#写文件,当文件不存在时,就直接创建此文件
with open("abc.txt",mode="w",encoding="utf-8") as f:

关于open 模式：
w      以写方式打开，
a      以追加模式打开 (从 EOF 开始, 必要时创建新文件)
r      以读写模式打开
w+     以读写模式打开 (参见 w )
a+     以读写模式打开 (参见 a )
rb     以二进制读模式打开
wb     以二进制写模式打开   (参见 w )
ab     以二进制追加模式打开 (参见 a )
rb+    以二进制读写模式打开 (参见 r+ )
wb+    以二进制读写模式打开 (参见 w+ )
ab+    以二进制读写模式打开 (参见 a+ )
--------------------- 
'''


import os
import linecache
from itertools import islice
import matplotlib.pyplot as plt


#\，字符串和代码换行，但有时会有问题（路径不识别）
path = r"F:\\抚州项目(验收)\\项目建模数据\\陈华工作交接\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\Topo\\HD.inp"

# 判断路径是否存在
message = '%s'
if os.path.exists(path):
    message = 'OK, file or folder exists "%s".'
else:
    message = 'NO, file or folder doesnt exists "%s".'
#print(message%path)        

# 创建目录
#os.path.makedirs(path)
# 创建文件
#open(path,"w")

# 判断是文件还是文件夹
message = '%s'
if os.path.isfile(path):
    message = 'YES,  is file "%s".'
elif os.path.isdir(path):
    message = 'Yes,  is folder "%s".'
#print(message%path)


shape = {}
reach = {}
reachCount = list()
reachPosition = list()


# 获取文件中[reach]的位置
with open(path,"r") as file:
    count, num =1, 1
    for line in islice(file,1,None):
        count += 1        
        title = line.strip().split()        
        if title[0] == "[reach]":
            reachPosition.append(count)  
            num+=1
        else:
            continue
#print(reachPosition)



# 获取文件中各reach的断面总数
for it in reachPosition:
    # linecache.getline(file, position)，读取文件中特定行
    arr = linecache.getline(path, it+1).strip()
    reachCount.append(int(arr))
#print(reachCount)


index = 0
for position in reachPosition:     # 定位到每个reach所在行
    count = reachCount[index]
    index += 1           # 为获得当前reach所在数组位置

    reachid = "reach" + str(index)
    reachlist = list()

    tmpP = position+2
    it = 0
    while it < count:    # 读取每个河道数据
        line = linecache.getline(path, tmpP).strip().split()
        shapeid = "shape" + line[1]
        shapelist = list()
        it += 1          # 为while判断计数

        reachlist.append("cross" + line[1])

        it2 = 0
        while it2 < int(line[0]):  # 读取每个断面形状数据
            tmpP += 1
            it2 += 1     # 为while判断计数
            data = linecache.getline(path,tmpP).strip().split()
            shapelist.extend([float(data[0]),float(data[1])])            
        shape[shapeid] = shapelist # 生成shape字典
        tmpP += 1        # 这里要跳到下一行
    reach[reachid] = reachlist     # 生成reach字典    
#print(shape.keys())        


# 输出模型[Reach]部分        
with open(r"C:\Users\gr\Desktop\result.txt","w") as result:
    for key, value in reach.items():
        result.write(key+"\t")

        count = 0
        for it in value:
            count += 1
            result.write(it + "   ")
            if count > 10:
                count = 0
                result.write("\n" + key + "\t")
        result.write("\n")


# 输出模型[Shape]部分
with open(r"C:\Users\gr\Desktop\crossShape.inp", "w") as shapefile:
    shapefile.write("[Shape]\n")
    
    for key, value in shape.items():
        num,count = 0, len(value)
        shapefile.write("X1" + "\t" + key + "\t" + str(count/2) + "\n")
        while num < (count-1):
            shapefile.write("GR\t" + str(value[num]) +"\t" + str(value[num + 1]) + "\n")
            num += 2








'''
正则表达式--------------------- 
## ^      匹配字符串的开始。
## $      匹配字符串的结尾。
## \b     匹配一个单词的边界。
## \d     匹配任意数字。
## \D     匹配任意非数字字符。
## x?     匹配一个可选的 x 字符 (换言之，它匹配1次或者0次x字符)。
## x*     匹配0次或者多次 x 字符。
## x+     匹配1次或者多次 x 字符。
## x{n,m} 匹配 x 字符，至少 n 次，至多 m 次。
## (a|b|c)要么匹配 a，要么匹配 b，要么匹配 c。
## (x)    一般情况下表示一个记忆组 (remembered group)。
## 正则表达式中的点号通常意味着 “匹配任意单字符”

提取字符串中数字---------------------
数字的形式一般是：整数、小数、整数加小数。
所以一般形式是： ——.——, 表达式是"\d+.?\d*",
\d+   匹配1次或者多次数字，注意这里不要写成*，因为即便是小数，小数点之前也得有一个数字；
\.?   这个是匹配小数点的，可能有，也可能没有；
\d*   这个是匹配小数点之后的数字的，所以是0个或者多个；
--------------------- 
'''


import re


coordination = r"F:\\抚州项目(验收)\\项目建模数据\\陈华工作交接\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\Topo\\CrossSectionLocation.inp"
distance = r"F:\\抚州项目(验收)\\项目建模数据\\陈华工作交接\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\Topo\\dst.inp"

coorLines = list()
disLines = list()
# 获取坐标和断面间距数据
with open(coordination, "r") as coor, open(distance, "r") as dis:  # 同时打开多个文件
    coorLines = coor.readlines()
    disLines = dis.readlines()


keys= list()
values = list()
# 输出模型[Cross]部分
with open(r"C:\Users\gr\Desktop\result2.txt","w") as result:
    for key, value in shape.items():
        tmp = re.findall("\d+",key)           # 提取字符串中整数
        keys.append(tmp[0])                   # 保存断面id中的数字        
        values.append(value)


    for i,j in zip(coorLines, disLines):      # 同时循环多个列表
        if i == "" and j == "":    continue   # 检查是否有异常
        elif i == "" or j == "":   raise Exception("line donot match in dis and coor")
        coor = i.strip().split()
        dist = j.strip().split()        
        if coor[0] != dist[0]:     raise Exception("line donot match in dis and coor")

        # 写入断面id、x坐标、y坐标
        result.write("corss" + coor[0] + "\t")
        result.write(coor[1] + "\t" + coor[2] + "\t")

        # 写入断面最低点
        index = keys.index(coor[0])
        tmpList = [x for x in values[index] if x%2 != 0]  # 列表生成器，提取断面高程数据
        result.write(str(min(tmpList)) + "\t")            # mn(list)，获取列表最小值

        # 写入断面间距、断面形状id
        result.write(dist[1])
        result.write("\tshape" + coor[0] + "\n")
        


junction = r"F:\\抚州项目(验收)\\项目建模数据\\陈华工作交接\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\Topo\\Junction.inp"

# 输出模型[Junction]部分
with open(junction,"r") as jun, open(r"C:\Users\gr\Desktop\result3.txt","w") as result:
    index,count = 0, 0
    for it in jun.readlines():
        index += 1
        line = it.strip().split(" ")
        
        if line[0] == "[JunctionsNum]":
            arr = linecache.getline(junction, index + 1).strip()
            count = int(arr[0]) # 汊点断面数量
            continue
        elif line[0] == "[Junctions]":
            while count > 0:
                arr = linecache.getline(junction, index + count).strip().split()          
                count -= 1      # 从最后一行数据开始向上读取汊点断面id
                
                result.write("junction" + arr[0] + "\t" + arr[1] + "\t")
                tmp = ["cross"+x for x in arr[2:]]        # 列表切片，左闭右开          
                for p in tmp:
                    result.write(p + "\t")
                result.write("\n")
                    
                    
bc = r"F:\\抚州项目(验收)\\项目建模数据\\陈华工作交接\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\BC\\bcCrossSections.inp"

# 输出模型[Boundary]部分
with open(bc,"r") as bcfile, open(r"C:\Users\gr\Desktop\result4.txt","w") as result:
    bcLines = bcfile.readlines()
    count = int(linecache.getline(bc, 2).strip())  # linecache.getline(file, num)中，行数从1开始

    num = 0
    while num < count:
        line = linecache.getline(bc, 4 + num).strip().split()
        result.write("bc" + line[0] + "\t" + "cross" + line[1] + "\t")

        Type = int(line[2])
        if   Type == 1: typeid = "WL"
        elif Type == 2: typeid = "FLOW"
        elif Type == 3: typeid = "OPEN"
        else: raise Exception("bc type error")
        result.write(typeid + "\t" + "ts" + line[0] + "\n")
        num += 1
      
            

bcTime = r"F:\\抚州项目(验收)\\项目建模数据\\陈华工作交接\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\BC\\bcTimeSeries.inp"

#输出模型[TimeSeries]部分
timeList = dict()
timeLen = int(linecache.getline(bcTime, 2).strip())
timeCount = len(linecache.getline(bcTime, 4).strip().split())
for i in range(1, timeCount):
    timeList["ts" + str(i)] = []          # 自动生成对应数量的列表变量（通过字典键值对）
num = 0
while num < timeLen:
    line = linecache.getline(bcTime, 4+num).strip().split() # split(), 默认以空格为分隔符，此时不用考虑中间空格数量
    for i in range(1, timeCount):         # 自动加载对应的列表数据（全过程不用手动定数量）
        timeList["ts" + str(i)].extend([line[0], line[i]])  # 字典中值为列表时的操作
    num += 1

with open(r"C:\Users\gr\Desktop\timeSeries.inp", "w") as result:
    #result.write("[TimeSeries]\n")
    for keys, values in timeList.items(): # 通过字典键值对，输出数据
        num, count = 0, len(values)
        while num < (count-1):
            if num == 0:
                result.write(keys + "\t" + "1" + "\t")
            else:
                result.write(keys + "\t\t")
            result.write(values[num] + "\t" + values[num + 1] + "\n")
            num += 2


psource = r"F:\\抚州项目(验收)\\项目建模数据\\陈华工作交接\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\Setting\\PSource.inp"

# 输出模型[Inflow]部分
with open(psource, "r") as ps, \
     open(r"C:\Users\gr\Desktop\timeSeries.inp", "a") as ts, \
     open(r"C:\Users\gr\Desktop\result5.txt", "w") as result: # 同时打开多个文件
    psPosition = list()
    index = 0
    for line in ps.readlines():
        index += 1
        if line.strip() == "[source]":
            psPosition.append(index)

    result.write("[Inflow]\n")
    for i in range(1, len(psPosition)+1):
        psid = linecache.getline(psource, psPosition[i-1]+2).strip().split()
        result.write("cross" + psid[1] + "\tFLOW\t" + "ts" + str(timeCount+i-1) + "\tcms\n")
        
    for i in range(1, len(psPosition)+1):  # 向时间序列文件尾追写数据，"a"模式
        timeid = "ts" + str(timeCount + i -1)
        key = linecache.getline(psource, psPosition[i-1]+4).strip().split()
        val = linecache.getline(psource, psPosition[i-1]+3).strip().split()
        for k,v in zip(key, val):
            ts.write(timeid + "\t\t")       # 少写"1"
            ts.write(k + "\t" + v + "\n")




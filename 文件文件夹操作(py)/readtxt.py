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







'''遍历当前文件夹下所有文件，不包括子文件夹文件 '''

path = r"C:\Users\gr\Desktop\data"
time = list()
value = list()

# 遍历文件夹中当前级的所有文件，读取特定类型的文件中数据
for pathes,folders,files in os.walk(path):           # os.walk(path), 返回路径、所有当前级文件夹、当前级所有文件
    for f in files:
        fname,ftype = os.path.splitext(f)            # os.path.splitext(file), 返回文件名、文件后缀
        
        if ftype == ".dat":                          # 判断文件类型
            with open(path + "\\" + f,"r") as data:
                
                for line in data.readlines():        # file.readlines(), 返回文件所有行
                    arr = line.strip().split(",")
                    
                    if arr[0] == "time(hr)":
                        time.append(float(arr[1]))           
                        continue
                    elif arr[0] == "cross2_1":
                        value.append(float(arr[2]))
                        break
print(len(time))
print(len(value))


# 对读取的数据作折线图
plt.plot(time,value,label="example", linewidth=3, color="r") # 横轴数组、纵轴数组、表名、线宽，线颜色
plt.xlabel("time")      # 横轴名
plt.ylabel("value")     # 纵轴名
plt.legend()            # 作图
plt.show()              # 展示










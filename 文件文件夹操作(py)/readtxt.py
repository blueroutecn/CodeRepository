#-*- coding: gbk -*-

'''��ȡ�ı��ļ����� '''
'''������ģ��1.0��һάģ�������ļ���תΪ2.0�������ļ�'''


'''
�ļ�����--------------------- 
os.getcwd()         #���ص�ǰ����·��
os.path.isabs()     #�ж��Ƿ��Ǿ���·��
os.path.isfile()    #���������·���Ƿ���һ���ļ�
os.path.isdir()     #���������·���Ƿ���һ��Ŀ¼
os.sep              #�ļ���·���ָ��� ��:��windows���� '\'  ����Linux���� '/'

os.walk()  #�ļ�����
for root, dirs, files in os.walk(file_dir):
    print('root_dir:', root)         # ��ǰĿ¼·��
    print('sub_dirs:', dirs)         # ��ǰ·����������Ŀ¼
    print('files:', files)           # ��ǰ·�������з�Ŀ¼���ļ�

os.path.splitext()  #������չ��
os.system()         #����shell����:
os.path.exists()    #���������·���Ƿ���ش�:
os.linesep          #������ǰƽ̨ʹ�õ�����ֹ��('\r\n', '\n', '\r')
os.chmod(file)      #�޸��ļ�Ȩ����ʱ���
os.exit()           #��ֹ��ǰ����

Ŀ¼����--------------------- 
os.makedirs("file1/file2/file3")     #�����ļ���Ŀ¼. �����༶Ŀ¼
os.mkdir("file")                     #�����ļ���. �����ϼ�Ŀ¼�������
os.copy("oldfile","newfile")         #oldfileֻ�����ļ��У�newfile�������ļ���Ҳ������Ŀ��Ŀ¼
os.rename("oldname","newname")       #�������ļ���Ŀ¼�����ļ���Ŀ¼����ʹ����������
os.removedirs()                      #ɾ���༶Ŀ¼, Ҫ������ǿյ�
os.rmdir("dir")	                     #ֻ��ɾ����Ŀ¼  
os.listdir()                         #����ָ��Ŀ¼�µ������ļ���Ŀ¼��
os.path.split(path)                  #����һ��·����Ŀ¼�����ļ���   
os.path.dirname()                    #��ȡ·����
os.path.basename()                   #��ȡ�ļ���
os.path.getsize(filename)            #��ȡ�ļ���С
os.chdir("path")                     #��·��

�ļ���д����--------------------- 
os.remove()         #ɾ���ļ� ����ļ���
os.stat()           #��ȡ�ļ�����

#д�ļ�,���ļ�������ʱ,��ֱ�Ӵ������ļ�
with open("abc.txt",mode="w",encoding="utf-8") as f:

����open ģʽ��
w      ��д��ʽ�򿪣�
a      ��׷��ģʽ�� (�� EOF ��ʼ, ��Ҫʱ�������ļ�)
r      �Զ�дģʽ��
w+     �Զ�дģʽ�� (�μ� w )
a+     �Զ�дģʽ�� (�μ� a )
rb     �Զ����ƶ�ģʽ��
wb     �Զ�����дģʽ��   (�μ� w )
ab     �Զ�����׷��ģʽ�� (�μ� a )
rb+    �Զ����ƶ�дģʽ�� (�μ� r+ )
wb+    �Զ����ƶ�дģʽ�� (�μ� w+ )
ab+    �Զ����ƶ�дģʽ�� (�μ� a+ )
--------------------- 
'''


import os
import linecache
from itertools import islice
import matplotlib.pyplot as plt


#\���ַ����ʹ��뻻�У�����ʱ�������⣨·����ʶ��
path = r"F:\\������Ŀ(����)\\��Ŀ��ģ����\\�»���������\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\Topo\\HD.inp"

# �ж�·���Ƿ����
message = '%s'
if os.path.exists(path):
    message = 'OK, file or folder exists "%s".'
else:
    message = 'NO, file or folder doesnt exists "%s".'
#print(message%path)        

# ����Ŀ¼
#os.path.makedirs(path)
# �����ļ�
#open(path,"w")

# �ж����ļ������ļ���
message = '%s'
if os.path.isfile(path):
    message = 'YES,  is file "%s".'
elif os.path.isdir(path):
    message = 'Yes,  is folder "%s".'







'''������ǰ�ļ����������ļ������������ļ����ļ� '''

path = r"C:\Users\gr\Desktop\data"
time = list()
value = list()

# �����ļ����е�ǰ���������ļ�����ȡ�ض����͵��ļ�������
for pathes,folders,files in os.walk(path):           # os.walk(path), ����·�������е�ǰ���ļ��С���ǰ�������ļ�
    for f in files:
        fname,ftype = os.path.splitext(f)            # os.path.splitext(file), �����ļ������ļ���׺
        
        if ftype == ".dat":                          # �ж��ļ�����
            with open(path + "\\" + f,"r") as data:
                
                for line in data.readlines():        # file.readlines(), �����ļ�������
                    arr = line.strip().split(",")
                    
                    if arr[0] == "time(hr)":
                        time.append(float(arr[1]))           
                        continue
                    elif arr[0] == "cross2_1":
                        value.append(float(arr[2]))
                        break
print(len(time))
print(len(value))


# �Զ�ȡ������������ͼ
plt.plot(time,value,label="example", linewidth=3, color="r") # �������顢�������顢�������߿�����ɫ
plt.xlabel("time")      # ������
plt.ylabel("value")     # ������
plt.legend()            # ��ͼ
plt.show()              # չʾ










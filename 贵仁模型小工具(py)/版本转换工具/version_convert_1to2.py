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
#print(message%path)


shape = {}
reach = {}
reachCount = list()
reachPosition = list()


# ��ȡ�ļ���[reach]��λ��
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



# ��ȡ�ļ��и�reach�Ķ�������
for it in reachPosition:
    # linecache.getline(file, position)����ȡ�ļ����ض���
    arr = linecache.getline(path, it+1).strip()
    reachCount.append(int(arr))
#print(reachCount)


index = 0
for position in reachPosition:     # ��λ��ÿ��reach������
    count = reachCount[index]
    index += 1           # Ϊ��õ�ǰreach��������λ��

    reachid = "reach" + str(index)
    reachlist = list()

    tmpP = position+2
    it = 0
    while it < count:    # ��ȡÿ���ӵ�����
        line = linecache.getline(path, tmpP).strip().split()
        shapeid = "shape" + line[1]
        shapelist = list()
        it += 1          # Ϊwhile�жϼ���

        reachlist.append("cross" + line[1])

        it2 = 0
        while it2 < int(line[0]):  # ��ȡÿ��������״����
            tmpP += 1
            it2 += 1     # Ϊwhile�жϼ���
            data = linecache.getline(path,tmpP).strip().split()
            shapelist.extend([float(data[0]),float(data[1])])            
        shape[shapeid] = shapelist # ����shape�ֵ�
        tmpP += 1        # ����Ҫ������һ��
    reach[reachid] = reachlist     # ����reach�ֵ�    
#print(shape.keys())        


# ���ģ��[Reach]����        
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


# ���ģ��[Shape]����
with open(r"C:\Users\gr\Desktop\crossShape.inp", "w") as shapefile:
    shapefile.write("[Shape]\n")
    
    for key, value in shape.items():
        num,count = 0, len(value)
        shapefile.write("X1" + "\t" + key + "\t" + str(count/2) + "\n")
        while num < (count-1):
            shapefile.write("GR\t" + str(value[num]) +"\t" + str(value[num + 1]) + "\n")
            num += 2








'''
������ʽ--------------------- 
## ^      ƥ���ַ����Ŀ�ʼ��
## $      ƥ���ַ����Ľ�β��
## \b     ƥ��һ�����ʵı߽硣
## \d     ƥ���������֡�
## \D     ƥ������������ַ���
## x?     ƥ��һ����ѡ�� x �ַ� (����֮����ƥ��1�λ���0��x�ַ�)��
## x*     ƥ��0�λ��߶�� x �ַ���
## x+     ƥ��1�λ��߶�� x �ַ���
## x{n,m} ƥ�� x �ַ������� n �Σ����� m �Ρ�
## (a|b|c)Ҫôƥ�� a��Ҫôƥ�� b��Ҫôƥ�� c��
## (x)    һ������±�ʾһ�������� (remembered group)��
## ������ʽ�еĵ��ͨ����ζ�� ��ƥ�����ⵥ�ַ���

��ȡ�ַ���������---------------------
���ֵ���ʽһ���ǣ�������С����������С����
����һ����ʽ�ǣ� ����.����, ���ʽ��"\d+.?\d*",
\d+   ƥ��1�λ��߶�����֣�ע�����ﲻҪд��*����Ϊ������С����С����֮ǰҲ����һ�����֣�
\.?   �����ƥ��С����ģ������У�Ҳ����û�У�
\d*   �����ƥ��С����֮������ֵģ�������0�����߶����
--------------------- 
'''


import re


coordination = r"F:\\������Ŀ(����)\\��Ŀ��ģ����\\�»���������\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\Topo\\CrossSectionLocation.inp"
distance = r"F:\\������Ŀ(����)\\��Ŀ��ģ����\\�»���������\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\Topo\\dst.inp"

coorLines = list()
disLines = list()
# ��ȡ����Ͷ���������
with open(coordination, "r") as coor, open(distance, "r") as dis:  # ͬʱ�򿪶���ļ�
    coorLines = coor.readlines()
    disLines = dis.readlines()


keys= list()
values = list()
# ���ģ��[Cross]����
with open(r"C:\Users\gr\Desktop\result2.txt","w") as result:
    for key, value in shape.items():
        tmp = re.findall("\d+",key)           # ��ȡ�ַ���������
        keys.append(tmp[0])                   # �������id�е�����        
        values.append(value)


    for i,j in zip(coorLines, disLines):      # ͬʱѭ������б�
        if i == "" and j == "":    continue   # ����Ƿ����쳣
        elif i == "" or j == "":   raise Exception("line donot match in dis and coor")
        coor = i.strip().split()
        dist = j.strip().split()        
        if coor[0] != dist[0]:     raise Exception("line donot match in dis and coor")

        # д�����id��x���ꡢy����
        result.write("corss" + coor[0] + "\t")
        result.write(coor[1] + "\t" + coor[2] + "\t")

        # д�������͵�
        index = keys.index(coor[0])
        tmpList = [x for x in values[index] if x%2 != 0]  # �б�����������ȡ����߳�����
        result.write(str(min(tmpList)) + "\t")            # mn(list)����ȡ�б���Сֵ

        # д������ࡢ������״id
        result.write(dist[1])
        result.write("\tshape" + coor[0] + "\n")
        


junction = r"F:\\������Ŀ(����)\\��Ŀ��ģ����\\�»���������\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\Topo\\Junction.inp"

# ���ģ��[Junction]����
with open(junction,"r") as jun, open(r"C:\Users\gr\Desktop\result3.txt","w") as result:
    index,count = 0, 0
    for it in jun.readlines():
        index += 1
        line = it.strip().split(" ")
        
        if line[0] == "[JunctionsNum]":
            arr = linecache.getline(junction, index + 1).strip()
            count = int(arr[0]) # ����������
            continue
        elif line[0] == "[Junctions]":
            while count > 0:
                arr = linecache.getline(junction, index + count).strip().split()          
                count -= 1      # �����һ�����ݿ�ʼ���϶�ȡ������id
                
                result.write("junction" + arr[0] + "\t" + arr[1] + "\t")
                tmp = ["cross"+x for x in arr[2:]]        # �б���Ƭ������ҿ�          
                for p in tmp:
                    result.write(p + "\t")
                result.write("\n")
                    
                    
bc = r"F:\\������Ŀ(����)\\��Ŀ��ģ����\\�»���������\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\BC\\bcCrossSections.inp"

# ���ģ��[Boundary]����
with open(bc,"r") as bcfile, open(r"C:\Users\gr\Desktop\result4.txt","w") as result:
    bcLines = bcfile.readlines()
    count = int(linecache.getline(bc, 2).strip())  # linecache.getline(file, num)�У�������1��ʼ

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
      
            

bcTime = r"F:\\������Ŀ(����)\\��Ŀ��ģ����\\�»���������\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\BC\\bcTimeSeries.inp"

#���ģ��[TimeSeries]����
timeList = dict()
timeLen = int(linecache.getline(bcTime, 2).strip())
timeCount = len(linecache.getline(bcTime, 4).strip().split())
for i in range(1, timeCount):
    timeList["ts" + str(i)] = []          # �Զ����ɶ�Ӧ�������б������ͨ���ֵ��ֵ�ԣ�
num = 0
while num < timeLen:
    line = linecache.getline(bcTime, 4+num).strip().split() # split(), Ĭ���Կո�Ϊ�ָ�������ʱ���ÿ����м�ո�����
    for i in range(1, timeCount):         # �Զ����ض�Ӧ���б����ݣ�ȫ���̲����ֶ���������
        timeList["ts" + str(i)].extend([line[0], line[i]])  # �ֵ���ֵΪ�б�ʱ�Ĳ���
    num += 1

with open(r"C:\Users\gr\Desktop\timeSeries.inp", "w") as result:
    #result.write("[TimeSeries]\n")
    for keys, values in timeList.items(): # ͨ���ֵ��ֵ�ԣ��������
        num, count = 0, len(values)
        while num < (count-1):
            if num == 0:
                result.write(keys + "\t" + "1" + "\t")
            else:
                result.write(keys + "\t\t")
            result.write(values[num] + "\t" + values[num + 1] + "\n")
            num += 2


psource = r"F:\\������Ŀ(����)\\��Ŀ��ģ����\\�»���������\\1D\\2015.5.8 15-2015.5.15 12\\1D\\IN1D\\Setting\\PSource.inp"

# ���ģ��[Inflow]����
with open(psource, "r") as ps, \
     open(r"C:\Users\gr\Desktop\timeSeries.inp", "a") as ts, \
     open(r"C:\Users\gr\Desktop\result5.txt", "w") as result: # ͬʱ�򿪶���ļ�
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
        
    for i in range(1, len(psPosition)+1):  # ��ʱ�������ļ�β׷д���ݣ�"a"ģʽ
        timeid = "ts" + str(timeCount + i -1)
        key = linecache.getline(psource, psPosition[i-1]+4).strip().split()
        val = linecache.getline(psource, psPosition[i-1]+3).strip().split()
        for k,v in zip(key, val):
            ts.write(timeid + "\t\t")       # ��д"1"
            ts.write(k + "\t" + v + "\n")




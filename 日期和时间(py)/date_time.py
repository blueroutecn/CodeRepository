import time
import datetime





'''һ��time�ĳ��÷��� '''

import time,datetime

# ʱ��������չ�ַ�ʽ��ʱ�����ʱ��Ԫ�飬��ʽ����ʱ��
print(time.time())#��ǰʱ���
print(int(time.time()))
print(time.strftime('%Y-%m-%d %H:%M:%S'))#��ʽ����ʱ��
print(time.strftime('%Y-%m-%d'))
print(time.strftime('%H:%M:%S'))
print(time.gmtime())#��ȡ��׼ʱ����ʱ��Ԫ�飬���������ʱ��������ǰ�ʱ���ת����ʱ��Ԫ��
print(time.gmtime(1516194265))


#��ִ�н����

1516197631.0563018
1516197631
2018-01-17 22:00:31
2018-01-17
22:00:31
time.struct_time(tm_year=2018, tm_mon=1, tm_mday=17, tm_hour=14, tm_min=0, tm_sec=31, tm_wday=2, tm_yday=17, tm_isdst=0)
time.struct_time(tm_year=2018, tm_mon=1, tm_mday=17, tm_hour=13, tm_min=4, tm_sec=25, tm_wday=2, tm_yday=17, tm_isdst=0)



''' ���� datetime���÷��� '''

# ʹ��datetime��time��ȡ��ǰʱ��
now1 = datetime.datetime.now()
now2=time.strftime('%Y-%m-%d %H:%M:%S')
print(now1)
print(now2)
now = datetime.datetime.now()
d1 = now - datetime.timedelta(hours=1)#��ȡǰһСʱ
d2 = now - datetime.timedelta(days=1)#��ȡǰһ��
print(now)
print(d1)


ִ�н����

2018-01-17 22:03:04.686923
2018-01-17 22:03:04
2018-01-17 22:03:04.687486
2018-01-17 21:03:04.687486



''' ����ʹ��datetime��ȡ����ִ�е�ʱ�� '''

# ʹ��ʱ�����ȡ����ִ��ʱ��
s_time = time.time()
for i in range(0,10):
    time.sleep(1)
e_time=time.time()
print('��������ʱ���ǣ�',e_time - s_time)


# ִ�н����

��������ʱ���ǣ� 10.003105163574219



''' �ġ�ʱ������ַ����Ļ���ת�� '''

# �ַ�����ʽ��ʱ��ת��ʱ���
str_time = '2018-1-17'
print(time.mktime(time.strptime(str_time,'%Y-%m-%d')))

# ʱ���ת���ɸ�ʽ����ʱ���ַ���
gsh_time= time.time()
print(time.strftime('%Y-%m-%d',time.localtime(gsh_time)))

# datetime����ת����ʱ���
dt = datetime.datetime.now()
print(time.mktime(dt.timetuple()))

# ʱ���ת����datetime����
sjc_time = time.time()
print(datetime.datetime.fromtimestamp(sjc_time))


#��ִ�н����

1516118400.0
2018-01-17
1516198008.0
2018-01-17 22:06:48.944055
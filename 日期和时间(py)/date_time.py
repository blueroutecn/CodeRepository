import time
import datetime





'''一、time的常用方法 '''

import time,datetime

# 时间有三种展现方式：时间戳，时间元组，格式化的时间
print(time.time())#当前时间戳
print(int(time.time()))
print(time.strftime('%Y-%m-%d %H:%M:%S'))#格式化的时间
print(time.strftime('%Y-%m-%d'))
print(time.strftime('%H:%M:%S'))
print(time.gmtime())#获取标准时区的时间元组，如果传入了时间戳，就是把时间戳转换成时间元组
print(time.gmtime(1516194265))


#　执行结果：

1516197631.0563018
1516197631
2018-01-17 22:00:31
2018-01-17
22:00:31
time.struct_time(tm_year=2018, tm_mon=1, tm_mday=17, tm_hour=14, tm_min=0, tm_sec=31, tm_wday=2, tm_yday=17, tm_isdst=0)
time.struct_time(tm_year=2018, tm_mon=1, tm_mday=17, tm_hour=13, tm_min=4, tm_sec=25, tm_wday=2, tm_yday=17, tm_isdst=0)



''' 二、 datetime常用方法 '''

# 使用datetime和time获取当前时间
now1 = datetime.datetime.now()
now2=time.strftime('%Y-%m-%d %H:%M:%S')
print(now1)
print(now2)
now = datetime.datetime.now()
d1 = now - datetime.timedelta(hours=1)#获取前一小时
d2 = now - datetime.timedelta(days=1)#获取前一天
print(now)
print(d1)


执行结果：

2018-01-17 22:03:04.686923
2018-01-17 22:03:04
2018-01-17 22:03:04.687486
2018-01-17 21:03:04.687486



''' 三、使用datetime获取代码执行的时长 '''

# 使用时间戳获取代码执行时间
s_time = time.time()
for i in range(0,10):
    time.sleep(1)
e_time=time.time()
print('代码运行时间是：',e_time - s_time)


# 执行结果：

代码运行时间是： 10.003105163574219



''' 四、时间戳和字符串的互相转化 '''

# 字符串格式化时间转换时间戳
str_time = '2018-1-17'
print(time.mktime(time.strptime(str_time,'%Y-%m-%d')))

# 时间戳转换成格式化的时间字符串
gsh_time= time.time()
print(time.strftime('%Y-%m-%d',time.localtime(gsh_time)))

# datetime对象转换成时间戳
dt = datetime.datetime.now()
print(time.mktime(dt.timetuple()))

# 时间戳转换成datetime对象
sjc_time = time.time()
print(datetime.datetime.fromtimestamp(sjc_time))


#　执行结果：

1516118400.0
2018-01-17
1516198008.0
2018-01-17 22:06:48.944055
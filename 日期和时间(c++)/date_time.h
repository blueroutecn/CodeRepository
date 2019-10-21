
C/C++获取本地日期时间常见方法



1.跨平台方法

1.1方法一：手动暴力法（snprintf）

snprintf()函数用于格式化输出字符串到指定大小的缓冲，可以获取我们想要的日期时间格式。

#include <iostream>
using namespace std;
#include <time.h>
    
time_t t = time(NULL);
struct tm* stime=localtime(&t);
char tmp[32]{0};
snprintf(tmp,sizeof(tmp),"%04d-%02d-%02d %02d:%02d:%02d",1900+stime->tm_year,1+stime->tm_mon,stime->tm_mday, stime->tm_hour,stime->tm_min,stime->tm_sec);
cout<<tmp<<endl;
输出结果：2015-04-02 23:12:56



1.2方法二：便捷快速法（strftime）

strftime()函数用于将时间（struct tm）格式化为字符串，可用于替代snprintf()函数。

#include <iostream>
#include <time.h>
using namespace std;

time_t t = time(0); 
char tmp[32]={NULL};
strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&t)); 
cout<<tmp<<endl;
输出结果：2015-04-02 23:12:56



1.3方法三：简获日历时间法

1.3.1通过ctime()+time_t

ctime()函数可用于将时间戳（time_t值）转换为可读的日历时间（calendar time）字符串，转换结果格式为：

Www Mmm dd hh:mm:ss yyyy

其中Www是weekday， Mmm 表示Month，dd表示每月的多少号，hh:mm:ss表示time，yyyy表示year。具体示例如下：

#include <time.h>
#include <iostream>
#include <string>
using namespace std;

time_t ts=time(NULL);
char tmp[32]{0};
strncpy(tmp,ctime(&ts),sizeof(tmp));
cout<<tmp<<endl;
输出结果：Fri Aug 14 23:19:42 2015


1.3.2通过asctime()+struct tm

asctime()函数功能与ctime()函数类似，用于将struct tm表示的日期时间转换为可读的日历时间字符串，结果格式同ctime()。asctime()与ctime()不同之处是asctime()传入的参数类型是struct tm指针型，ctime()的是time_t指针型。具体示例如下：

time_t ts=time(NULL);
struct tm stm=*localtime(&ts);
char tmp[32]{0};
strncpy(tmp,asctime(&stm),sizeof(tmp));
cout<<tmp<<endl;
输出结果：Fri Aug 14 23:19:42 2015



2.Windows平台获取日期时间

#include <iostream>
using namespace std;
#include <time.h>
#include <windows.h>

SYSTEMTIME sys; 
GetLocalTime(&sys);
char tmp[64]={NULL};
sprintf(tmp,"%4d-%02d-%02d %02d:%02d:%02d ms:%03d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds); 
cout<<tmp<<endl;
输出结果：2015-08-14 23:41:56 ms:354




3.Unix平台获取日期时间

#include <sys/time.h> 
#include <unistd.h>

struct timeval now_time;
gettimeofday(&now_time, NULL);
time_t tt = now_time.tv_sec;
tm *temp = localtime(&tt);
char time_str[32]={NULL};
sprintf(time_str,"%04d-%02d-%02d%02d:%02d:%02d",temp->tm_year+ 1900,temp->tm_mon+1,temp->tm_mday,temp->tm_hour,temp->tm_min, temp->tm_sec);
cout<<tmp<<endl;
输出时间：2015-08-14 23:41:56





4.日期时间与时间戳的相互转换



4.1时间戳转日期时间

//@brief：时间戳转日期时间
static inline string getDateTimeFromTS(time_t ts)
{
    if(ts<0)
    {
        return "";
    }
    struct tm tm = *localtime(&ts);
    static char time_str[32]{0};
    snprintf(time_str,sizeof(time_str),"%04d-%02d-%02d %02d:%02d:%02d",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
    return string(time_str);
}



4.2日期时间转时间戳

（1）跨平台方法。
使用sscanf将日期时间字符串转为struct tm，再转为time_t。

//@brief：日期时间转时间戳
static inline time_t getTSFromDateTime(const string& dateTime,const char* fmt)
{
    int dY, dM, dD, dH, dMin, dS;
    sscanf(dateTime.c_str(),fmt, &dY, &dM, &dD, &dH, &dMin, &dS);
    struct tm stm;
    stm.tm_year = dY - 1900;
    stm.tm_mon = dM - 1;
    stm.tm_mday = dD;
    stm.tm_hour = dH;
    stm.tm_min = dMin;
    stm.tm_sec = dS;

    return mktime(&stm);    //返回时间戳
}

（2）Linux平台方法。
使用glibc库函数strptime()将日期时间字符串转为struct tm，再转为time_t。strptime()函数的功能与strftime()函数功能相反，用于将日期时间字符串转换为struct tm。函数原型如下：

char *strptime(const char *s, const char *format, struct tm *tm)

参数说明：
s：struct tm格式化后的C字符串（以0结尾）；
format：字符串格式，构建方式与strftime的format字符串完全一样；
struct tm *tm：转换后存储tm值的指针；

返回值：指向转换过程中第一个不能被处理的那个字符。

转换示例如下。

static inline time_t getTSFromDateTime(const string& dateTime, const char* fmt)
{
    struct tm t;
    memset(&t, 0, sizeof(t));
    if(strptime(dateTime.c_str(),fmt,&t)!=NULL)
    {
        return mktime(&t);
    }
    return 0;
}






5.需知知识点


（5）clock tick，时钟计时单元，一个时钟计时单元的时间长短是由CPU控制的。一个clock tick不是CPU的一个时钟周期，而是C/C++的一个基本计时单位。在VC++的time.h文件中，我们可以找到相关的定义：

#ifndef _CLOCK_T_DEFINED
typedef long clock_t;
#define _CLOCK_T_DEFINED
#endif

#define CLOCKS_PER_SEC ((clock_t)1000)

clock_t clock( void );

这个函数返回从“进程启动”到“程序中调用clock()函数”时之间的CPU时钟计时单元（clock tick）数，在MSDN中称之为挂钟时间（wal-clock）。

//获取逝去时间
clock_t start, finish;
start=clock();
…
finish=clock();
    
//逝去多少秒
long duration=(finish- start)/CLOCKS_PER_SEC;



（5）头文件<time.h>还提供了两种不同的函数将时间戳转换为年月日时分秒分开显示的日历时间格式struct tm。

struct tm* gmtime(const time_t* timer);
struct tm* localtime(const time_t* timer);

其中gmtime()函数是将时间戳转化为UTC+0的日历时间，即0时区的世界标准时间，并返回一个struct tm结构体来保存这个时间。而localtime()函数考虑时区，将时间戳转为本地日历时间。比如用gmtime()函数获得的时间是2018-07-30 00:00:00，那么用localtime()函数在中国地区获得的本地时间会比世界标准时间早8个小时，即2018-07-30 08:00:00。示例程序（VC++）如下：

#include <time.h>

#include <string>
#include <iostream>
using namespace std;

int main()
{
    time_t ts = 0;
    struct tm stm1;
    gmtime_s(&stm1,&ts);
    char tmp[32]{0};
    asctime_s(tmp, &stm1);
    cout<<tmp;

    struct tm stm2;
    localtime_s(&stm2, &ts);
    asctime_s(tmp, &stm2);
    cout <<tmp;
}

程序输出：

Thu Jan  1 00:00:00 1970
Thu Jan  1 08:00:00 1970




（6）分解时间就是以年月日时分秒等分量保存的时间结构，在C/C++中是struct tm结构。我们可以使用mktime()函数将用struct tm结构表示的日历时间转化为时间戳，其作用与gmtime()和localtime()函数具有相反。其函数原型如下：

time_t mktime(struct tm* timeptr);
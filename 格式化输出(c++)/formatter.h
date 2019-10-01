#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
int a;
cout<<"input a:";

cin>>a;
cout<<"dec:"<<dec<<a<<endl; //以十进制形式输出整数
cout<<"hex:"<<hex<<a<<endl; //以十六进制形式输出整数a
cout<<"oct:"<<setbase(8)<<a<<endl; //以八进制形式输出整数a

char *pt="China"; //pt指向字符串"China"
cout<<setw(10)<<pt<<endl; //指定域宽为,输出字符串
cout<<setfill('*')<<setw(10)<<pt<<endl; //指定域宽,输出字符串,空白处以'*'填充

double pi=22.0/7.0; //计算pi值
cout<<setiosflags(ios::scientific)<<setprecision(8); //按指数形式输出,8位小数
cout<<"pi="<<pi<<endl; //输出pi值
cout<<"pi="<<setprecision(4)<<pi<<endl; //改为位小数
cout<<"pi="<<setiosflags(ios::fixed)<<pi<<endl; //改为小数形式输出
return 0;
}


运行结果如下：
input a:34↙(输入a的值)
dec:34                   (十进制形式)
hex:22                   (十六进制形式)
oct:42                   (八进制形式)
         China               (域宽为)
*****China               (域宽为,空白处以'*'填充)
pi=3.14285714e+00        (指数形式输出,8位小数)
pi=3.1429e+00            (指数形式输出,4位小数)
pi=3.143                 (小数形式输出,精度仍为)
用流对象的成员函数控制输出格式
除了可以用控制符来控制输出格式外，还可以通过调用流对象cout中用于控制输出格式的成员函数来控制输出格式。


流成员函数		与之作用相同的控制符	作用
precision(n)	setprecision(n)			设置实数的精度为n位
width(n)		setw(n)					设置字段宽度为n位
fill(c)			setfill(c)				设置填充宇符c
setf()			setiosflags()			设置输出格式状态，括号中应给出格式状态，内容与控制符setiosflags括号中的内容相同
unsetf()		resetioflags()			终止已设置的输出格式状态，在括号中应指定内容


格式标志	作用
ios::left	输出数据在本域宽范围内向左对齐
ios::right	输出数据在本域宽范围内向右对齐
ios::internal	数值的符号位在域宽内左对齐，数值右对齐，中间由填充字符填充
ios::dec	设置整数的基数为10
ios::oct	设置整数的基数为8
ios::hex	设置整数的基数为16
ios::showbase	强制输出整数的基数(八进制数以0打头，十六进制数以0x打头)
ios::showpoint	强制输出浮点数的小点和尾数0
ios::uppercase	在以科学记数法格式E和以十六进制输出字母时以大写表示
ios::showpos	对正数显示“+”号
ios::scientific	浮点数以科学记数法格式输出
ios::fixed	浮点数以定点格式(小数形式)输出
ios::unitbuf	每次输出之后刷新所有的流
ios::stdio	每次输出之后清除stdout, stderr
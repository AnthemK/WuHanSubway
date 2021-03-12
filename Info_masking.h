#include <assert.h>　　　　//设定插入点
#include <ctype.h>　　　　 //字符处理
#include <errno.h>　　　　 //定义错误码
#include <float.h>　　　　 //浮点数处理
#include <iso646.h>        //对应各种运算符的宏
#include <limits.h>　　　　//定义各种数据类型最值的常量
#include <locale.h>　　　　//定义本地化C函数
#include <math.h>　　　　　//定义数学函数
#include <setjmp.h>        //异常处理支持
#include <signal.h>        //信号机制支持
#include <stdarg.h>        //不定参数列表支持
#include <stddef.h>        //常用常量
#include <stdio.h>　　　　 //定义输入／输出函数
#include <stdlib.h>　　　　//定义杂项函数及内存分配函数
#include <string.h>　　　　//字符串处理
#include <time.h>　　　　　//定义关于时间的函数
#include <wchar.h>　　　　 //宽字符处理及输入／输出
#include <wctype.h>　　　　//宽字符分类
#define char_max 127

int encf[1000],decf[1000];

/************************************ 
	函数名：Encryption_file
	描述：将一个文件加密到另一个文件里面 
	输入参数： 一个文件指针 
	输出参数：一个文件指针
	编程者：HUST IS-1901 李文重
	日期：2021.3.11 
	状态 ：已完成，已检查  
//**********************************/ 
int Encryption_file(FILE *fromfile,FILE *tofile,int cipher);

/************************************ 
	函数名：Encryption_file
	描述：将一个文件解密到另一个文件里面 
	输入参数： 一个文件指针 
	输出参数：一个文件指针
	编程者：HUST IS-1901 李文重
	日期：2021.3.11 
	状态 ：已完成，已检查  
//**********************************/ 
int Decryption_file(FILE *fromfile,FILE *tofile,int cipher);

/************************************ 
	函数名：Encryption_info
	描述： 这个函数指导完成对于所有加密文件的解密，加密文件均已enc_开头，解密文件以dec_开头 
	输入参数： 无
	输出参数：无
	编程者：HUST IS-1901 李文重
	日期：2021.3.11 
	状态 ：已完成，已检查  
//**********************************/ 
int Encryption_info();

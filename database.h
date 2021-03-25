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
#include <gtk/gtk.h>       // gtk
#include <windows.h>
#define lenth_of_station_name 50           
#define total_number_of_station 330
#define total_number_of_edge 3000
#define total_number_of_line 25 
#define max_status 200000 
#define inf (10000.0)
#define Inf (100000000.0)
#define Bidirectional_line_offset 10
#define avoid_index 15 
#define trans_time(x,y) (x*60+y)
#define eqs  (0.0000001) 
#define Image_Max_X  1285
#define Image_Max_Y   832
#define eexp(x) exp(exp(x))
#define can_you_reach_here    printf("Hello?\n")
#define outt(x) printf("%s = %d\n",getVariableName(x),x);
#define hh  puts("") 
#define my_swap(a,b) (a)^=(b)^=(a)^=(b)
#define getVariableName(x)  #x 
#define ANGLE(ang)	(ang * acos(-1.0) / 180.0)
//#define debug                    //通过定义此宏来输出调试信息 
#define use_gui                    //通过定义此宏来开启GUI界面 
//#define  Long_debugging_information      //通过定义此宏来将一些调试信息输出到文件./data/Long_debugging_information.txt里面
//#define gui_print_detailed_information              //通过定义此宏来输出详细的线路通过站点信息到gui中的文本输出框  ，内容很长 
#define maincost_ascending          //当定义这个宏的时候通过maincost的最小来完成输出路线排序，否则使用路程最短来排序 
  
struct Station_unit{          //每个结构体变量存储一个站点的信息 
	char name[lenth_of_station_name];    //名称，使用ANSI 
	int hed_of_edge,number_on_line[total_number_of_line];   
	//第一条出边：hed_of_edge 和 当前站点在各个线路上的位次number_on_line
	double longitude,latitude;	         //经纬度 
}Stations[total_number_of_station]; 

struct Edge_of_subway{       //描述一条边的信息 
	int next_edge,time,belonging_line,to_station;    //belonging_line分正负，表示一条线路的两种方向 
	double lenth,Crowding_factor;
}Edge[total_number_of_edge];

struct Subway_line{     //描述一条线路的总体信息 
	char first_endpoint[lenth_of_station_name],second_endpoint[lenth_of_station_name];   //两端点车站名 
	char stations[total_number_of_station][lenth_of_station_name];    //车站名数组 
	int Maximum_load,total_station;    
	double total_lenth;
}Subwaylines[total_number_of_line];

struct Cost_of_scheme{    //描述一个状态，是整个程序运作最重要的结构体 
	int nowtime,number_of_transfer,number_of_station,nowline,starttime,now_station;
	double dist,aver_crowd,maincost,limit_factor,tot_crowd;  
	//此处的aver_crowd表示的是平均拥挤系数，考虑到方便计算，在中间状态的时候，实际上存储的是前面经过所有边的拥挤系数之和
	// tot_crowd表示平均拥挤度，其他同上 
	struct Cost_of_scheme *last_status;
	//存储转移到本状态的上一个状态 
}; 

int line_in_this_station[total_number_of_line],avoid_status[total_number_of_station][total_number_of_line];
struct Cost_of_scheme station_dis[total_number_of_station][total_number_of_line],passing_situation[5][(total_number_of_station<<1)+100];
double Crowdedness[total_number_of_line][total_number_of_edge];
FILE *Long_debugging_information;


/************************************ 
	函数名：lines_that_pass_this_station
	描述：查找有哪些线路通过本站 
	函数原型： void lines_that_pass_this_station(int stationid);
	输入参数：站id 
	输出参数：无
		但是会修改 line_in_this_station 数组，初始或不通过这个节点为-1，否则为最近一班车到达的时间 ，考虑到线路有负数，故加10存储 
	编程者：HUST IS-1901 李文重
	日期：2021.3.4 
	状态 ：已完成，已检查 
//**********************************/ 
void lines_that_pass_this_station(int stationid,int now_time);

/************************************ 
	函数名：change_Crowdedness 
	描述：修改一个线路一段时间的拥挤度（并非系数，系数需要经过计算） 
	函数原型： int change_Crowdedness(int now_line,int frotime,int totime,double new_crowdedness);
	输入参数： now_line 要修改的线路 ,此处默认两个方向的拥挤度相同，因此没有偏移存储 
				fromtime totime 要修改的时间区间
				new_crowdedness 修改后的拥挤系数(0.0-1.0)
				添加了对传入参数越界和fromtime，totime大小反向的判断 
	输出参数： 如果程序正确修改了，就返回1
			   如果出现错误，就返回0 
	编程者：HUST IS-1901 李文重
	日期：2021.3.5 
	状态 ：已完成，已检查 
//**********************************/ 
int change_Crowdedness(int now_line,int fromtime,int totime,double new_crowdedness);

/************************************ 
	函数名：add_edge
	描述：在两个站间添加有向边
	函数原型： void add_edge(int frompoint,int topoint,double Crowding_factor,int belonging_line,double lenth)
	输入参数： 含义见英文，类型见函数定义 
	输出参数：无
	编程者：HUST IS-1901 李文重
	日期：2021.2.25 
	状态 ：已完成，已检查 
//**********************************/ 
void add_edge(int frompoint,int topoint,double Crowding_factor,int belonging_line,double lenth);

/************************************ 
	函数名：Name_find_station 
	描述：使用汉语车站名找到它的id 但此处的复杂度为 O(number_of_station) 
	函数原型： int Name_find_station(char *stationname);
	输入参数：一个车站的名字 stationname 
	输出参数：这个车站的序号 ，如果不存在就返回0 
	编程者：HUST IS-1901 李文重
	日期：2021.2.28 
	状态 ：已完成，已检查 
//**********************************/ 
int Name_find_station(char *stationname);

/************************************ 
	函数名：Calculate_the_fare 
	描述：根据给出的公里数，计算分段票价。使用向上取整的方法。 
	函数原型： int Calculate_the_fare(double total_km);
	输入参数：一个路程的总公里数 
	输出参数：收取的车票价格 
	编程者：HUST IS-1901 李文重
	日期：2021.2.25 
	状态 ：已完成，已检查 
//**********************************/ 
int Calculate_the_fare(double total_km);

/************************************ 
	函数名：get_modified_coefficient
	描述：计算当前边在当前时间，当前优先级类型下的权值修正系数 
	函数原型：double get_modified_coefficient(struct Edge_of_subway *now_edge,int typ,int time);
	输入参数：当前边now_edge，当前类型typ，当前时间 time
		其中typ == 1  表示不理会拥挤度，默认为1.0 找最少时间的方案 
		如果typ == 2 就使用宽松、一般拥挤、拥挤的约定 （默认方法） 
		如果typ == 3 就使用对拥挤系数取exp再取exp作为修正系数，这样可以保证会很大程度上规避拥挤，但是也不会出现因为等拥挤而等几个小时的问题 
		如果typ == 4 就设定一个可以容忍的上线limit_factor，不走拥挤系数大于他的边，但是由于没有传入这个上限，所以返回当前拥挤系数，在调用函数里判断是否不能走 	
	输出参数：修正系数 ,如果当前不可行就返回一个极大的数 
	编程者：HUST IS-1901 李文重
	日期：2021.2.25 
	状态 ：已完成，已检查 
//**********************************/ 
double get_modified_coefficient(struct Edge_of_subway *now_edge,int typ,struct Cost_of_scheme *now_status);

/************************************ 
	函数名：Calculate_the_cost 
	描述：计算当前边在当前时间，当前优先级类型下的消耗，再结合传进来的之前状态，获得走这条边之后的状态 
	函数原型： struct Cost_of_scheme Calculate_the_cost(struct Edge_of_subway *now_edge,struct Cost_of_scheme *now_situation,int typ);
	输入参数： 当前边now_edge，当前状态now_situation 
			  对于typ:
					case 1，2，3，4的含义见get_modified_coefficient 函数前的注释 
					case 5 最低票价（最小里程） 
					case 6 最少换乘下最低票价 
	输出参数：返回走这条边之后的状态 
	编程者：HUST IS-1901 李文重
	日期：2021.3.5
	状态 ：已完成，已检查  
//**********************************/ 
struct Cost_of_scheme Calculate_the_cost(struct Edge_of_subway *now_edge,struct Cost_of_scheme *now_status,int typ);

/************************************ 
	函数名：if_scheme_equal
	描述：判断两个方案是不是相同 
	函数原型： int if_scheme_equal(struct Cost_of_scheme a,struct Cost_of_scheme b); 
	输入参数：两个  Cost_of_scheme型变量 
	输出参数：0 不相等
			  1 相等 
	编程者：HUST IS-1901 李文重
	日期：2021.3.5 
	状态 ：已完成，已检查 
//**********************************/ 
int if_scheme_equal(struct Cost_of_scheme a,struct Cost_of_scheme b);




/************************************ 
	函数名：init_station 
	描述：导入车站信息，到结构体数组Stations里
	函数原型： void init_station(void)
	输入参数：无
		输入文件： ./data/stationinfo.txt   里面应该是所有站点（无重复）的信息（名称） 
	输出参数：无
	编程者：HUST IS-1901 李文重
	日期：2021.3.5 
	状态 ：已完成，已检查 
//**********************************/ 
void init_station();

/************************************ 
	函数名：init_line 
	描述：导入线路信息信息，到结构体里。 
	函数原型： void init_line(void)
	输入参数：无
		输入文件： ./data/subwayline.txt   里面应该是所有线路的信息，包括长度、线路经过站点个数、容量 
	输出参数：无
	编程者：HUST IS-1901 李文重
	日期：2021.2.25 
	状态 ：已完成，已检查 
//**********************************/ 
void init_line();

/************************************ 
	函数名：init_Crowdedness 
	描述：初始化各个线路在每个时间段上的拥挤系数。采用简化约定中的结果 
	函数原型： void init_Crowdedness()； 
	输入参数：无
	输出参数：无
	编程者：HUST IS-1901 李文重
	日期：2021.3.5 
	状态 ：已完成，已检查 
//**********************************/ 
void init_Crowdedness();

/************************************ 
	函数名：init_coordinates
	描述：输入各个车站的经纬度作标 
	函数原型： void init_coordinates()； 
	输入参数：在./data/Coordinates.txt中输入车站的经纬度坐标 
	输出参数：无
	编程者：HUST IS-1901 李文重
	日期：2021.3.10 
	状态 ：已完成，已检查 
//**********************************/ 
void init_coordinates();

/************************************ 
	函数名：init 
	描述：在main.c里面调用这个函数，在从这个函数来调用其他初始化函数，实现初始化过程。本函数是一个控制函数。 
	函数原型： void init()
	输入参数：无
	输出参数：无
	编程者：HUST IS-1901 李文重
	日期：2021.2.25 
	状态 ：已完成，已检查 
//**********************************/ 
void init();



/************************************ 
	函数名：Traverse_in_line
	描述：在当前状态下，沿当前线路移动一站 。返回的是移动后的状态 
	函数原型： struct Cost_of_scheme Traverse_in_line(struct Cost_of_scheme now_status,int aim_line);
	输入参数： now_situation 当前状态，主要是车站id和时间以及路程
			   aim_line 要移动的列车线路，其中正负表示不同的方向。 
	输出参数： 移动一条边之后的状态 
	编程者：HUST IS-1901 李文重
	日期：2021.3.2  
	状态 ：已完成，已检查 
//**********************************/ 
struct Cost_of_scheme Traverse_in_line(struct Cost_of_scheme now_status,int aim_line);

/************************************ 
	函数名：spfa
	描述：使用spfa算法计算两点间的最短路，最短指使得maincost最小。 设计最短路状态的时候，使用目前站+目前线路来确保找到最优。
			并且引进了avoid_status来减少走重复方案的可能 
	函数原型： int spfa(struct Cost_of_scheme now_status,int typ); 
	输入参数：now_status 最初的状态 
			  typ 寻找最短路的模式 
	输出参数：1 表示运行到了结尾 
	编程者：HUST IS-1901 李文重
	日期：2021.3.5 
	状态 ：已完成，已检查
//**********************************/ 
int spfa(struct Cost_of_scheme now_status,int typ);

/************************************ 
	函数名：Find_the_path
	描述：通过在spfa里的存储转移方案的指针，反向找到整个路径上的所有状态 
	函数原型： int Find_the_path(int aimstation,int pls); 
	输入参数：aimstation 目标车站的编号，最终要从这里倒序找出整个路径 
	输出参数：返回当前路径的长度 ，如果没有路径就返回0 
	编程者：HUST IS-1901 李文重
	日期：2021.3.5 
	状态 ：已完成，已检查
//**********************************/ 
int Find_the_path(int aimstation,int pls);

/************************************ 
	函数名：print_the_path
	描述：输出一条寻路方案
	函数原型： int print_the_path(int pls,int typ);
	输入参数：pls 输出的这个方案所存储的位置 
			  typ 这个方案的寻路类型 
	输出参数：正常结束输出0 
	编程者：HUST IS-1901 李文重
	日期：2021.3.5 
	状态 ：已完成，已检查
//**********************************/ 
int print_the_path(int pls,int typ);

/************************************ 
	函数名：find_shortest_path
	描述：寻路算法的总控制函数，在这个函数里面 
	函数原型： int find_shortest_path(int aimstation,struct Cost_of_scheme now_status,int typ); 
	输入参数：aimstation 目标车站的编号，最终要从这里倒序找出全部路径 
			  now_situation 初始的状态，重要的是当前车站id和时间 ，其他的都应该清零 
			  typ  寻路的类型 
	输出参数：在屏幕上完成多条路线的输出 
	编程者：HUST IS-1901 李文重
	日期：2021.3.5 
	状态 ：已完成，已检查  
//**********************************/ 
int find_shortest_path(int aimstation,struct Cost_of_scheme now_situation,int typ);

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
//#include "database.h"
#include "gtk-gui.h" 
//#include "answer.h"  

char name[lenth_of_station_name];
extern int line_in_this_station[total_number_of_line];
extern int free_travel_distance;
struct Cost_of_scheme nowwa_scheme,next_scheme; 
int main(int argc , char **argv)
{
//	freopen("example.txt","r",stdin); 
//	freopen("std.txt","w",stdout); 
	Encryption_info();
	//system("pause");
	init();            //初始化 

	//nowwa_scheme.nowtime=trans_time(hours,minutes);
	nowwa_scheme.aver_crowd=0;nowwa_scheme.dist=0;nowwa_scheme.last_status=NULL;nowwa_scheme.maincost=0;
	nowwa_scheme.nowline=0;	nowwa_scheme.number_of_transfer=0;
	nowwa_scheme.number_of_station=0;nowwa_scheme.limit_factor=1.0;nowwa_scheme.tot_crowd=0.0;
	free_travel_distance=0;
    /***************
    命令行式的操作方法（供测试后端用）
	1. 自由遍历 
    ***************/
    #ifndef use_gui
	puts("你将在哪里开始？");scanf("%s",name);
	int now_station=Name_find_station(name),hours=0,minutes=0;
	puts("现在的时间是（小时和分钟用空格隔开）");scanf("%d%d",&hours,&minutes);
	nowwa_scheme.nowtime=trans_time(hours,minutes);nowwa_scheme.now_station=now_station;nowwa_scheme.starttime=nowwa_scheme.nowtime; 
	passing_situation[0][++free_travel_distance]=nowwa_scheme;
	while(1){
		if(nowwa_scheme.maincost>=Inf) {printf("没有一条路线可以到达，可能是没有车了\n");break;}
		printf("你现在在 %s车站，时间是%d：%d，你已经行走了%lf km\n",Stations[nowwa_scheme.now_station].name,nowwa_scheme.nowtime/60,nowwa_scheme.nowtime%60,nowwa_scheme.dist);
		lines_that_pass_this_station(nowwa_scheme.now_station,nowwa_scheme.nowtime);
		printf("你可以乘坐:");
		int i,aimline=0;
		for(i=0;i<=20;++i){
			if(line_in_this_station[i]!=-1) printf("%d号线%s方向(请输入%d) (下一班将于%d:%d抵达这里)      ",abs(i-Bidirectional_line_offset),((i>10)?Subwaylines[abs(i-Bidirectional_line_offset)].second_endpoint:Subwaylines[abs(i-Bidirectional_line_offset)].first_endpoint),i-Bidirectional_line_offset,(line_in_this_station[i]+nowwa_scheme.nowtime)/60,(line_in_this_station[i]+nowwa_scheme.nowtime)%60);
		} 
		puts("          或者输入0退出自由遍历模式"); 
		scanf("%d",&aimline);
		while(line_in_this_station[aimline+Bidirectional_line_offset]==-1&&aimline!=0)
		{
		printf("%d号线路不经过这里\n",aimline);
		puts("请输入你要走的线路");
		scanf("%d",&aimline);			
		}
		if(aimline==0) break;
		next_scheme=Traverse_in_line(nowwa_scheme,aimline);
		passing_situation[0][++free_travel_distance]=next_scheme;
		nowwa_scheme=next_scheme;
	}
	
    /***************
    命令行式的操作方法（供测试后端用）
	2.路线推荐 
    ***************/	
	nowwa_scheme.aver_crowd=0;nowwa_scheme.dist=0;nowwa_scheme.last_status=NULL;nowwa_scheme.maincost=0;nowwa_scheme.nowline=0;
	nowwa_scheme.now_station=now_station;nowwa_scheme.number_of_station=0;nowwa_scheme.limit_factor=1.0;
	nowwa_scheme.number_of_transfer=0;nowwa_scheme.tot_crowd=0.0;
	nowwa_scheme.starttime=nowwa_scheme.nowtime;         //重新计时 
	puts("你要到哪里去？");scanf("%s",name);
	int aim_station=Name_find_station(name);
	puts("你的偏好:");
	int preferred_type;
	scanf("%d",&preferred_type);
	if(preferred_type==4){
		puts("请输入限制因子:");
		scanf("%lf",&nowwa_scheme.limit_factor);
	} 
//	qwq(aim_station,&nowwa_scheme,1);	
	find_shortest_path(aim_station,nowwa_scheme,preferred_type);
	#endif                     //以上部分为使用命令行操作的 
	
	
	
	
	#ifdef use_gui
    gtk_gui(argc,argv);
	#endif
	return 0; 
}

/*
华中科技大学 
13 35
0
武汉火车站 
2

循礼门 
8 30 
0
三阳路
4
0.81 



华中科技大学 
6 0
0
岳家嘴 
2



循礼门 
8 30 
0
三阳路
6
0.81  


应该1号线到底 


？？？？？？？？？？？？？ 
秀湖
0 01 
0
佛祖岭 
2

卡死 


*/


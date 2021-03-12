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
//#include "answer.h"
#include "database.h"

extern int line_in_this_station[total_number_of_line],avoid_status[total_number_of_station][total_number_of_line];
extern struct Cost_of_scheme station_dis[total_number_of_station][total_number_of_line],passing_situation[5][(total_number_of_station<<1)+100];
int vis[total_number_of_station][total_number_of_line],nowpls,my_queue1[max_status],my_queue2[max_status];
char temporary_string[1000000];

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
struct Cost_of_scheme Traverse_in_line(struct Cost_of_scheme now_status,int aim_line){
	int now_edge=Stations[now_status.now_station].hed_of_edge;
	for(;now_edge;now_edge=Edge[now_edge].next_edge){
		if(Edge[now_edge].belonging_line==aim_line){
			return Calculate_the_cost(&Edge[now_edge],&now_status,0); 
		}
	}
	return now_status;
}

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
int spfa(struct Cost_of_scheme now_status,int typ){
	struct Cost_of_scheme next_status;
	int hed=0,endd=1,i,j;
	for(i=0;i<total_number_of_station;++i) 
		for(j=0;j<total_number_of_line;++j)   
			station_dis[i][j].maincost=Inf,vis[i][j]=0;
	
	my_queue1[endd]=now_status.now_station;my_queue2[endd]=now_status.nowline+Bidirectional_line_offset;
	vis[my_queue1[endd]][my_queue2[endd]]=1;station_dis[my_queue1[endd]][my_queue2[endd]]=now_status;
	while(hed<endd){
		hed++;
		now_status=station_dis[my_queue1[hed]][my_queue2[hed]];
		#ifdef debug	
		printf("%s,line:%d ,maincost :%lf\n",Stations[now_status.now_station].name,now_status.nowline,now_status.maincost);
		#endif
		
		vis[my_queue1[hed]][my_queue2[hed]]=0;
		int now_edge=Stations[my_queue1[hed]].hed_of_edge;
		for(;now_edge;now_edge=Edge[now_edge].next_edge){
			next_status=Calculate_the_cost(&Edge[now_edge],&station_dis[my_queue1[hed]][my_queue2[hed]],typ);
			if(next_status.maincost+avoid_status[next_status.now_station][next_status.nowline+Bidirectional_line_offset]<station_dis[next_status.now_station][next_status.nowline+Bidirectional_line_offset].maincost){
				next_status.maincost+=avoid_status[next_status.now_station][next_status.nowline+Bidirectional_line_offset];
				station_dis[next_status.now_station][next_status.nowline+Bidirectional_line_offset]=next_status;
				
				if(vis[next_status.now_station][next_status.nowline+Bidirectional_line_offset]==0) {
					endd++;
					my_queue1[endd]=next_status.now_station;my_queue2[endd]=next_status.nowline+Bidirectional_line_offset;
					vis[my_queue1[endd]][my_queue2[endd]]=1;
				}
			} 
		}
	}
	return 1; 
}

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
int Find_the_path(int aimstation,int pls){
	struct Cost_of_scheme now_status;
	int i,j;
	now_status.maincost=Inf; 
	for(i=1;i<=20;++i)
		if(station_dis[aimstation][i].maincost<now_status.maincost){
			now_status=station_dis[aimstation][i];
		}
	if(now_status.maincost>=Inf){        
		printf("目前没有一条路线可以到达，可能是没有车了\n");
		return 0;
	}
	
	#ifdef debug 
	puts("Final_status:");
	printf("%s,line:%d ,maincost :%lf\n",Stations[now_status.now_station].name,now_status.nowline,now_status.maincost);
	#endif
	
	for(i=0;i<(total_number_of_station<<1);++i) passing_situation[pls][i].nowline=0,passing_situation[pls][i].last_status=NULL;
	passing_situation[pls][1]=now_status;
	int cnt=1;
	while(passing_situation[pls][cnt].last_status){
		passing_situation[pls][cnt+1]=*passing_situation[pls][cnt].last_status;
		cnt++; 
	}
	//outt(cnt);
	return cnt;
} 

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
int print_the_path(int pls,int typ){
	#ifdef debug   
	 	printf("This is plan %d\n",pls); 
	#endif
	
	char linshi[1000];
	
	int cnt=passing_situation[pls][1].number_of_station+1,i;  
	//outt(cnt);
	double averagecrowd=((double )passing_situation[pls][1].aver_crowd)/(double )passing_situation[pls][1].number_of_station;
	double averagecrowdindex=((double )passing_situation[pls][1].tot_crowd)/(double )passing_situation[pls][1].number_of_station;
	//if(typ==3) averagecrowd=log(log(averagecrowd));
	printf("当前为方案:%d\n全程用时%d分钟，共%lf公里，票价%d元，平均拥挤系数为%lf,平均拥挤度为%lf,共经过%d站,换乘%d次 ,体感时间：%lf\n",pls,passing_situation[pls][1].nowtime-passing_situation[pls][1].starttime,passing_situation[pls][1].dist,Calculate_the_fare(passing_situation[pls][1].dist),averagecrowd,averagecrowdindex,passing_situation[pls][1].number_of_station,passing_situation[pls][1].number_of_transfer,passing_situation[pls][1].maincost); 
	
	#ifdef use_gui
	strcat(temporary_string,"当前为方案：");
	itoa(pls,linshi,10);strcat(temporary_string,linshi);	
	strcat(temporary_string,"\n全程用时");
	itoa(passing_situation[pls][1].nowtime-passing_situation[pls][1].starttime,linshi,10);strcat(temporary_string,linshi);
	strcat(temporary_string,"分钟，共");
	gcvt(passing_situation[pls][1].dist,5,linshi);strcat(temporary_string,linshi);
	strcat(temporary_string,"公里，票价");
	itoa(Calculate_the_fare(passing_situation[pls][1].dist),linshi,10);strcat(temporary_string,linshi);
	strcat(temporary_string,"元\n平均拥挤系数为");
	gcvt(averagecrowd,10,linshi);strcat(temporary_string,linshi);
	strcat(temporary_string,"元,平均拥挤度为");
	gcvt(averagecrowdindex,10,linshi);strcat(temporary_string,linshi);	
	strcat(temporary_string,"    共经过");
	itoa(passing_situation[pls][1].number_of_station ,linshi,10);strcat(temporary_string,linshi);
	strcat(temporary_string,"站，换乘");
	itoa(passing_situation[pls][1].number_of_transfer ,linshi,10);strcat(temporary_string,linshi);
	strcat(temporary_string,"次 ,体感时间：");
	gcvt(passing_situation[pls][1].maincost,10,linshi);strcat(temporary_string,linshi);	
	strcat(temporary_string,"\n");
	#endif
	for(i=cnt;i>=1;i--){
		if(passing_situation[pls][i+1].nowline!=passing_situation[pls][i].nowline){

			printf("本站换乘%d号线%s方向\n",abs(passing_situation[pls][i].nowline),((passing_situation[pls][i].nowline>0)?Subwaylines[abs(passing_situation[pls][i].nowline)].second_endpoint:Subwaylines[abs(passing_situation[pls][i].nowline)].first_endpoint));

			#ifdef use_gui 
			strcat(temporary_string,"在");
			strcat(temporary_string,Stations[passing_situation[pls][i+1].now_station].name);
			strcat(temporary_string,"车站换乘");
			itoa(abs(passing_situation[pls][i].nowline),linshi,10);strcat(temporary_string,linshi);
			strcat(temporary_string,"号线");
			strcat(temporary_string,((passing_situation[pls][i].nowline>0)?Subwaylines[abs(passing_situation[pls][i].nowline)].second_endpoint:Subwaylines[abs(passing_situation[pls][i].nowline)].first_endpoint));
			strcat(temporary_string,"方向\n");
			#endif
		}  		

		printf("你将会在%d:%d,到达%s，此时你的总里程数是%lf\n",passing_situation[pls][i].nowtime/60,passing_situation[pls][i].nowtime%60,Stations[passing_situation[pls][i].now_station].name,passing_situation[pls][i].dist);

		#ifdef gui_print_detailed_information                //因为文本太长  暂时没开 
		strcat(temporary_string,"你将会在");
		itoa(passing_situation[pls][i].nowtime/60,linshi,10);strcat(temporary_string,linshi);
		strcat(temporary_string,":");
		itoa(passing_situation[pls][i].nowtime%60,linshi,10);strcat(temporary_string,linshi);
		strcat(temporary_string,"到达");
		strcat(temporary_string,Stations[passing_situation[pls][i].now_station].name);//strcat(temporary_string,"\n");	
		strcat(temporary_string," ，此时你的总里程数是");
		gcvt(passing_situation[pls][i].dist,5,linshi);strcat(temporary_string,linshi);
		strcat(temporary_string,"\n");		
		#endif
		
		#ifdef debug
		printf("此时的Maincost:%lf\n",passing_situation[pls][i].maincost);
		#endif
	}
	hh;hh;
	#ifdef use_gui 
		strcat(temporary_string,"\n\n\n");
	#endif
	return 0;
}


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
int find_shortest_path(int aimstation,struct Cost_of_scheme now_status,int typ){
//	can_you_reach_here;
	memset(avoid_status,0,sizeof(avoid_status));
	struct Cost_of_scheme initial_status=now_status;
	spfa(initial_status,typ);
	
	int cnt1=Find_the_path(aimstation,1),i;  
	#ifdef debug
	hh;puts("方案1:");print_the_path(1,typ);
	#endif
	for(i=1;i<cnt1;++i){              //走到这些状态，就要加代价，以此来找新的最短路 ，后面记得删除掉 
		avoid_status[passing_situation[1][i].now_station][passing_situation[1][i].nowline+Bidirectional_line_offset]=avoid_index;
	}
	 
	spfa(initial_status,typ);

	int cnt2=Find_the_path(aimstation,2); 
	#ifdef debug
	hh;puts("方案2:");print_the_path(2,typ);
	#endif
	for(i=1;i<cnt2;++i){              //走到这些状态，就要加代价，以此来找新的最短路 ，后面记得删除掉 
		avoid_status[passing_situation[2][i].now_station][passing_situation[2][i].nowline+Bidirectional_line_offset]+=avoid_index;
	}
	
	spfa(initial_status,typ);
	int cnt3=Find_the_path(aimstation,3); 
	#ifdef debug
	hh;puts("方案3:");print_the_path(3,typ);
	#endif
	
	
	
	int min_dis_pls=1;
	for(i=1;i<=3;++i){
		#ifdef maincost_ascending
		if(passing_situation[i][1].maincost<=passing_situation[min_dis_pls][1].maincost) min_dis_pls=i;		
		#else
		if(passing_situation[i][1].dist<=passing_situation[min_dis_pls][1].dist) min_dis_pls=i;
		#endif
	}
	#ifdef debug 
	outt(min_dis_pls);
	#endif
	
	puts("推荐路线依次如下："); 
	print_the_path(min_dis_pls,typ);

	int mid_dis_pls=1+((min_dis_pls==1)?1:0);
	for(i=1;i<=3;++i){
		#ifdef maincost_ascending
		if(passing_situation[i][1].maincost<=passing_situation[mid_dis_pls][1].maincost&&i!=min_dis_pls) mid_dis_pls=i;		
		#else
		if(passing_situation[i][1].dist<=passing_situation[mid_dis_pls][1].dist&&i!=min_dis_pls) mid_dis_pls=i;
		#endif
	}	 
	#ifdef debug
	outt(mid_dis_pls);
	#endif
	if(if_scheme_equal(passing_situation[mid_dis_pls][1],passing_situation[min_dis_pls][1])==0)
		 	 print_the_path(mid_dis_pls,typ);	
	int max_dis_pls=6-min_dis_pls-mid_dis_pls;
	#ifdef debug
	outt(max_dis_pls);
	#endif
	if(if_scheme_equal(passing_situation[max_dis_pls][1],passing_situation[min_dis_pls][1])==0&&if_scheme_equal(passing_situation[mid_dis_pls][1],passing_situation[max_dis_pls][1])==0)
		 	 print_the_path(max_dis_pls,typ);	
	 
	 
	 
	return 0;
}


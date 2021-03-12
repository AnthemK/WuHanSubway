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
#include "database.h"
#include "Info_masking.h"

int number_of_station,number_of_edge,free_travel_distance;
extern struct Station_unit Stations[total_number_of_station]; 
extern struct Edge_of_subway Edge[total_number_of_edge];
extern struct Subway_line Subwaylines[total_number_of_line];
extern int line_in_this_station[total_number_of_line];
extern double Crowdedness[total_number_of_line][total_number_of_edge];




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
void lines_that_pass_this_station(int stationid,int now_time){
	int i;
	for(i=0;i<total_number_of_line;++i) line_in_this_station[i]=-1;
	int now_edge=Stations[stationid].hed_of_edge;
	for(;now_edge;now_edge=Edge[now_edge].next_edge){
		int cost_to_station=((Edge[now_edge].belonging_line>0)?(Stations[Edge[now_edge].to_station].number_on_line[Edge[now_edge].belonging_line]-1)*3:\
		(Subwaylines[-1*Edge[now_edge].belonging_line].total_station-Stations[Edge[now_edge].to_station].number_on_line[-1*Edge[now_edge].belonging_line])*3);	
		cost_to_station-=3;
		if(now_time<=cost_to_station+trans_time(23,0)) {
			if(now_time<cost_to_station+trans_time(6,0)){
				line_in_this_station[Bidirectional_line_offset+Edge[now_edge].belonging_line]=cost_to_station+trans_time(6,0)-now_time;
			} else {
				line_in_this_station[Bidirectional_line_offset+Edge[now_edge].belonging_line]=((now_time-trans_time(6,0)-cost_to_station)%2==1)?1:0;
			}
		}
	}
	return;
}

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
int change_Crowdedness(int now_line,int fromtime,int totime,double new_crowdedness){
	if((now_line>8||now_line<1||now_line==5)||(fromtime<0||fromtime>trans_time(24,0))||(totime<0||totime>trans_time(24,0))||(new_crowdedness<0.0||new_crowdedness>1.0))
	return 0;
	if(fromtime>totime) my_swap(fromtime,totime);
	int i;
	for(i=fromtime;i<=totime;++i) Crowdedness[now_line][i]=new_crowdedness;
	return 1;
}

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
void add_edge(int frompoint,int topoint,double Crowding_factor,int belonging_line,double lenth){
	Edge[++number_of_edge].belonging_line=belonging_line;Edge[number_of_edge].Crowding_factor=Crowding_factor;
	Edge[number_of_edge].lenth=lenth;Edge[number_of_edge].time=2;
	Edge[number_of_edge].to_station=topoint;Edge[number_of_edge].next_edge=Stations[frompoint].hed_of_edge;
	Stations[frompoint].hed_of_edge=number_of_edge;
	#ifdef debug
	printf("车站间的边的信息:\n From %s",Stations[frompoint].name);
//	Edge[number_of_edge].print_info();
	printf(" to %s, belong to line %d ,length is %lf,time is %d,crowding factor is %lf.\n",Stations[topoint].name,Edge[number_of_edge].belonging_line,Edge[number_of_edge].lenth,Edge[number_of_edge].time,Edge[number_of_edge].Crowding_factor);
	#endif
	return;
}

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
int Name_find_station(char *stationname){
		int i;
		for(i=1;i<=number_of_station;++i)
			if(!strcmp(stationname,Stations[i].name)){
				return i;
			}
	return 0;
}

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
int Calculate_the_fare(double total_km){
	int Fare=0;
	if(total_km>50.0) Fare+=(int )ceil((total_km-50)/20.0),total_km=50.0;
	if(total_km>40.0) Fare+=(int )ceil((total_km-40)/10.0),total_km=40.0;	
	if(total_km>24.0) Fare+=(int )ceil((total_km-24)/8.0),total_km=24.0;
	if(total_km>12.0) Fare+=(int )ceil((total_km-12)/6.0),total_km=12.0;
	if(total_km>4.0) Fare+=(int )ceil((total_km-4)/4.0),total_km=4.0;	
	if(total_km>eqs) Fare+=2;
	return Fare;
}

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
double get_modified_coefficient(struct Edge_of_subway *now_edge,int typ,struct Cost_of_scheme *now_status){
	if(typ==6||typ==5||typ==0) typ=2; 
	double ans=1.0;
	int cost_to_station=((now_edge->belonging_line>0)?(Stations[now_edge->to_station].number_on_line[now_edge->belonging_line]-1)*3:\
	(Subwaylines[-1*now_edge->belonging_line].total_station-Stations[now_edge->to_station].number_on_line[-1*now_edge->belonging_line])*3);
	cost_to_station-=3;
	int line_id=abs(now_edge->belonging_line),time=now_status->nowtime;
	if(typ==1){
		ans=1.0;
	} else if(typ==2){
		ans=((Crowdedness[line_id][time]>0.75)?1.5:((Crowdedness[line_id][time]>0.2)?1.0:0.6));
	} else if(typ==3){
		ans=eexp(Crowdedness[line_id][time]); 
	} else if(typ==4){
		if(Crowdedness[line_id][time]>=now_status->limit_factor){
			ans=Inf;
		} else  ans=((Crowdedness[line_id][time]>0.75)?1.5:((Crowdedness[line_id][time]>0.2)?1.0:0.6));
	} 
	//if(time<trans_time(6,0)+cost_to_station)  ans+=(double)(360+cost_to_station)/now_edge->time;
//	outt(cost_to_station);
//	if(time>trans_time(23,0)+cost_to_station) return Inf;
	return ans;
}


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
struct Cost_of_scheme Calculate_the_cost(struct Edge_of_subway *now_edge,struct Cost_of_scheme *now_status,int typ){
	int cost_to_station=((now_edge->belonging_line>0)?(Stations[now_edge->to_station].number_on_line[now_edge->belonging_line]-1)*3:\
	(Subwaylines[-1*now_edge->belonging_line].total_station-Stations[now_edge->to_station].number_on_line[-1*now_edge->belonging_line])*3);
	struct Cost_of_scheme after_status=*now_status;
	after_status.last_status=now_status;cost_to_station-=3;
	//printf("\n%d\n%d\n",cost_to_station,now_situation->nowtime);
	if(now_status->nowtime<trans_time(6,0)+cost_to_station) {
		if(typ<=4&&typ>=1) after_status.maincost+=trans_time(6,0)+cost_to_station-now_status->nowtime;
		after_status.nowtime=trans_time(6,0)+cost_to_station;
		#ifdef debug 
		can_you_reach_here;
		#endif
	} 
		
	double Modified_Coefficient= get_modified_coefficient(now_edge,typ,now_status);
	if(Modified_Coefficient>=Inf)  after_status.maincost+=Inf;
	
	if(now_edge->belonging_line!=after_status.nowline&&after_status.nowline){
		int waiting_time=((after_status.nowtime+3-trans_time(6,0)-cost_to_station)%2==1)?2:1;		
		if(typ<=4&&typ>=1){
			after_status.maincost+=(3+waiting_time);
		} if(typ==6){
			after_status.maincost+=inf;
		}
		after_status.nowtime+=(3+waiting_time);after_status.number_of_transfer++;
	} else if(after_status.nowline==0){
		int waiting_time=((after_status.nowtime-trans_time(6,0)-cost_to_station)%2==1)?2:1;
		if(typ<=4&&typ>=1){
			after_status.maincost+=waiting_time;
		} 
		after_status.nowtime+=waiting_time;
	//	outt(waiting_time);
	} else {
		if(typ<=4&&typ>=1){
			after_status.maincost+=Modified_Coefficient;
		} 
		after_status.nowtime+=1;
	}
	if(typ==5||typ==6) after_status.maincost+=now_edge->lenth;
	else if(typ<=4&&typ>=1) after_status.maincost+=2*Modified_Coefficient;
	after_status.nowline=now_edge->belonging_line;after_status.now_station=now_edge->to_station;after_status.tot_crowd+=Crowdedness[after_status.nowline][after_status.nowtime];after_status.nowtime+=now_edge->time;
	after_status.dist+=now_edge->lenth;after_status.number_of_station++;after_status.aver_crowd+=Modified_Coefficient;
	//outt(ans_scheme.nowtime);outt(trans_time(23,0)+cost_to_station);
	if(after_status.nowtime-3>trans_time(23,0)+cost_to_station){
		after_status.maincost=Inf;
	}
	#ifdef debug
	printf("station:%s,line:%d,Modified Coefficient:%lf,first subway arrive time%d,and cost is %lf\n",Stations[after_status.now_station].name,after_status.nowline,Modified_Coefficient,cost_to_station,after_status.maincost); 
	#endif	
	return after_status;
}

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
int if_scheme_equal(struct Cost_of_scheme a,struct Cost_of_scheme b){
		if(abs(a.dist-b.dist)<=eqs&&abs(a.aver_crowd-b.aver_crowd)<=eqs\
		 	&&a.nowtime==b.nowtime&&a.number_of_transfer==b.number_of_transfer\
		 	&&a.number_of_station==b.number_of_station&&a.nowline==b.nowline) return 1;
		 return 0;
}

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
void init_station(){
	FILE *station_info = fopen("./data/dec_stationinfo.txt","r");
	if(!station_info) {
		printf("Wrong open stationinfo.txt\n");
		return;
	} 
	int cnt=0,i;
	while(~fscanf(station_info, "%s",Stations[++number_of_station].name)){
		Stations[number_of_station].hed_of_edge=0;
		for(i=0;i<=10;++i) Stations[number_of_station].number_on_line[i]=0;
		#ifdef debug
		printf("车站的信息和序号:\n");
		printf("%d  %s\n",++cnt,Stations[number_of_station].name);
		#endif
	};
	fclose(station_info);station_info=NULL; 
	return;
} 




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
void init_line(){
	FILE *subway_line = fopen("./data/dec_subwayline.txt","r");
	if(!subway_line) {
		printf("Wrong open subwayline.txt\n");  
		return;
	} 
	int now_line,i,j,first_station,second_station;
	char current_station_name[lenth_of_station_name];
	while(~fscanf(subway_line, "%d",&now_line)){ 
	#ifdef debug
		printf("On line %d:\n",now_line);
	#endif
		fscanf(subway_line, "%lf%d%d%s",&Subwaylines[now_line].total_lenth,&Subwaylines[now_line].total_station,&Subwaylines[now_line].Maximum_load,Subwaylines[now_line].first_endpoint);
		
		first_station=Name_find_station(Subwaylines[now_line].first_endpoint);
		Stations[first_station].number_on_line[now_line]=1;strcpy(Subwaylines[now_line].stations[1],Subwaylines[now_line].first_endpoint);
		for(i=2;i<=Subwaylines[now_line].total_station;++i){
			fscanf(subway_line,"%s",current_station_name);
			strcpy(Subwaylines[now_line].stations[i],current_station_name);
			second_station=Name_find_station(current_station_name);
			
			Stations[second_station].number_on_line[now_line]=i;
			add_edge(first_station,second_station,1.0,now_line,Subwaylines[now_line].total_lenth/((double )(Subwaylines[now_line].total_station-1)));
			add_edge(second_station,first_station,1.0,-1*now_line,Subwaylines[now_line].total_lenth/((double )(Subwaylines[now_line].total_station-1)));
			first_station=second_station;
		}
		strcpy(Subwaylines[now_line].second_endpoint,current_station_name); 
	}
	fclose(subway_line);subway_line=NULL;  
	return;
}

/************************************ 
	函数名：init_Crowdedness 
	描述：初始化各个线路在每个时间段上的拥挤系数。 
	函数原型： void init_Crowdedness()； 
	输入参数：无
	输出参数：无
	编程者：HUST IS-1901 李文重
	日期：2021.3.5 
	状态 ：已完成，已检查 
//**********************************/ 
void init_Crowdedness(){
	int i,j; 
	for(i=0;i<=trans_time(24,0);++i){
		for(j=1;j<=8;++j)
		{
			if(i<trans_time(6,0)||i>trans_time(23,0)) { Crowdedness[j][i]=1.0; continue;} 
			if(j==1||j==3){
				if(i>=trans_time(7,30)&&i<=trans_time(9,0)){
					Crowdedness[j][i]=0.8;
				} else if(time>=trans_time(16,30)&&time<=trans_time(18,30)){
					Crowdedness[j][i]=0.75;
				} else {
					Crowdedness[j][i]=0.4;
				}
			} else if(j==8){
				if(i>=trans_time(9,30)&&i<=trans_time(15,0)){
					Crowdedness[j][i]=0.65;
				} else {
					Crowdedness[j][i]=0.2;
				}			
			} else if(j==6||j==7){
				if(i>=trans_time(19,00)&&i<=trans_time(22,0)){
					Crowdedness[j][i]=0.65;
				} else {
					Crowdedness[j][i]=0.15;
				}			
			} else if(j==2||j==4){
					Crowdedness[j][i]=0.5;
			}	
		}
	}
	return;
}

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
void init_coordinates(){
	FILE *station_coordinates = fopen("./data/dec_Coordinates.txt","r");
	if(!station_coordinates) {
		printf("Wrong open Coordinates.txt\n");
		return;
	} 
	char stationname[lenth_of_station_name];
	int stationid,qwer;
	double maxx=-10000,maxy=-100000,minx=10000,miny=10000;
	while(~fscanf(station_coordinates,"%d",&qwer))
	{
		fscanf(station_coordinates,"%s",stationname);
		qwer=strlen(stationname);
		stationname[qwer-2]=0;
		stationid=Name_find_station(stationname);
		
		fscanf(station_coordinates,"%lf,%lf",&Stations[stationid].longitude,&Stations[stationid].latitude);
		#ifdef debug
		fprintf(Long_debugging_information,"Station:%s  ,id：%d，longitude:%lf   latitude :%lf\n",stationname,stationid,Stations[stationid].longitude,Stations[stationid].latitude);	
		#endif
		
		maxx=(maxx>Stations[stationid].longitude)?maxx:Stations[stationid].longitude;
		maxy=(maxy>Stations[stationid].latitude)?maxy:Stations[stationid].latitude;
		minx=(minx<Stations[stationid].longitude)?minx:Stations[stationid].longitude;
		miny=(miny<Stations[stationid].latitude)?miny:Stations[stationid].latitude;
		
		
		
		#ifdef use_gui
		//Stations[stationid].longitude = 2320.98 * Stations[stationid].longitude - 264764+180;
		Stations[stationid].longitude = 2855.55556 * (Stations[stationid].longitude - 114.000);
		//Stations[stationid].latitude =Image_Max_Y- 2079.77 * Stations[stationid].latitude + 63187.1;
		Stations[stationid].latitude =-1434.4827 * (Stations[stationid].latitude - 30.90);
		#ifdef debug
		fprintf(Long_debugging_information,"Xpixel:%lf   Ypixel :%lf\n",Stations[stationid].longitude,Stations[stationid].latitude);	
		#endif		
		#endif

	}
	#ifdef debug
		fprintf(Long_debugging_information,"%lf,%lf    ,%lf,%lf",maxx,minx,maxy,miny);
	#endif	
	return;
}



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
void init(){
	number_of_station=number_of_edge=0;
	Long_debugging_information = fopen("./data/Long_debugging_information.txt","w");
	init_station();init_coordinates();init_line();init_Crowdedness();
	return;
}

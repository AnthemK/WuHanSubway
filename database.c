#include <assert.h>��������//�趨�����
#include <ctype.h>�������� //�ַ�����
#include <errno.h>�������� //���������
#include <float.h>�������� //����������
#include <iso646.h>        //��Ӧ����������ĺ�
#include <limits.h>��������//�����������������ֵ�ĳ���
#include <locale.h>��������//���屾�ػ�C����
#include <math.h>����������//������ѧ����
#include <setjmp.h>        //�쳣����֧��
#include <signal.h>        //�źŻ���֧��
#include <stdarg.h>        //���������б�֧��
#include <stddef.h>        //���ó���
#include <stdio.h>�������� //�������룯�������
#include <stdlib.h>��������//������������ڴ���亯��
#include <string.h>��������//�ַ�������
#include <time.h>����������//�������ʱ��ĺ���
#include <wchar.h>�������� //���ַ��������룯���
#include <wctype.h>��������//���ַ�����
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
	��������lines_that_pass_this_station
	��������������Щ��·ͨ����վ 
	����ԭ�ͣ� void lines_that_pass_this_station(int stationid);
	���������վid 
	�����������
		���ǻ��޸� line_in_this_station ���飬��ʼ��ͨ������ڵ�Ϊ-1������Ϊ���һ�೵�����ʱ�� �����ǵ���·�и������ʼ�10�洢 
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.4 
	״̬ ������ɣ��Ѽ�� 
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
	��������change_Crowdedness 
	�������޸�һ����·һ��ʱ���ӵ���ȣ�����ϵ����ϵ����Ҫ�������㣩 
	����ԭ�ͣ� int change_Crowdedness(int now_line,int frotime,int totime,double new_crowdedness);
	��������� now_line Ҫ�޸ĵ���· ,�˴�Ĭ�����������ӵ������ͬ�����û��ƫ�ƴ洢 
				fromtime totime Ҫ�޸ĵ�ʱ������
				new_crowdedness �޸ĺ��ӵ��ϵ��(0.0-1.0)
				����˶Դ������Խ���fromtime��totime��С������ж� 
	��������� ���������ȷ�޸��ˣ��ͷ���1
			   ������ִ��󣬾ͷ���0 
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.5 
	״̬ ������ɣ��Ѽ�� 
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
	��������add_edge
	������������վ����������
	����ԭ�ͣ� void add_edge(int frompoint,int topoint,double Crowding_factor,int belonging_line,double lenth)
	��������� �����Ӣ�ģ����ͼ��������� 
	�����������
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.2.25 
	״̬ ������ɣ��Ѽ�� 
//**********************************/ 
void add_edge(int frompoint,int topoint,double Crowding_factor,int belonging_line,double lenth){
	Edge[++number_of_edge].belonging_line=belonging_line;Edge[number_of_edge].Crowding_factor=Crowding_factor;
	Edge[number_of_edge].lenth=lenth;Edge[number_of_edge].time=2;
	Edge[number_of_edge].to_station=topoint;Edge[number_of_edge].next_edge=Stations[frompoint].hed_of_edge;
	Stations[frompoint].hed_of_edge=number_of_edge;
	#ifdef debug
	printf("��վ��ıߵ���Ϣ:\n From %s",Stations[frompoint].name);
//	Edge[number_of_edge].print_info();
	printf(" to %s, belong to line %d ,length is %lf,time is %d,crowding factor is %lf.\n",Stations[topoint].name,Edge[number_of_edge].belonging_line,Edge[number_of_edge].lenth,Edge[number_of_edge].time,Edge[number_of_edge].Crowding_factor);
	#endif
	return;
}

/************************************ 
	��������Name_find_station 
	������ʹ�ú��ﳵվ���ҵ�����id ���˴��ĸ��Ӷ�Ϊ O(number_of_station) 
	����ԭ�ͣ� int Name_find_station(char *stationname);
	���������һ����վ������ stationname 
	��������������վ����� ����������ھͷ���0 
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.2.28 
	״̬ ������ɣ��Ѽ�� 
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
	��������Calculate_the_fare 
	���������ݸ����Ĺ�����������ֶ�Ʊ�ۡ�ʹ������ȡ���ķ����� 
	����ԭ�ͣ� int Calculate_the_fare(double total_km);
	���������һ��·�̵��ܹ����� 
	�����������ȡ�ĳ�Ʊ�۸� 
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.2.25 
	״̬ ������ɣ��Ѽ�� 
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
	��������get_modified_coefficient
	���������㵱ǰ���ڵ�ǰʱ�䣬��ǰ���ȼ������µ�Ȩֵ����ϵ�� 
	����ԭ�ͣ�double get_modified_coefficient(struct Edge_of_subway *now_edge,int typ,int time);
	�����������ǰ��now_edge����ǰ����typ����ǰʱ�� time
		����typ == 1  ��ʾ�����ӵ���ȣ�Ĭ��Ϊ1.0 ������ʱ��ķ��� 
		���typ == 2 ��ʹ�ÿ��ɡ�һ��ӵ����ӵ����Լ�� ��Ĭ�Ϸ����� 
		���typ == 3 ��ʹ�ö�ӵ��ϵ��ȡexp��ȡexp��Ϊ����ϵ�����������Ա�֤��ܴ�̶��Ϲ��ӵ��������Ҳ���������Ϊ��ӵ�����ȼ���Сʱ������ 
		���typ == 4 ���趨һ���������̵�����limit_factor������ӵ��ϵ���������ıߣ���������û�д���������ޣ����Է��ص�ǰӵ��ϵ�����ڵ��ú������ж��Ƿ����� 	
	�������������ϵ�� ,�����ǰ�����оͷ���һ��������� 
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.2.25 
	״̬ ������ɣ��Ѽ�� 
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
	��������Calculate_the_cost 
	���������㵱ǰ���ڵ�ǰʱ�䣬��ǰ���ȼ������µ����ģ��ٽ�ϴ�������֮ǰ״̬�������������֮���״̬ 
	����ԭ�ͣ� struct Cost_of_scheme Calculate_the_cost(struct Edge_of_subway *now_edge,struct Cost_of_scheme *now_situation,int typ);
	��������� ��ǰ��now_edge����ǰ״̬now_situation 
			  ����typ:
					case 1��2��3��4�ĺ����get_modified_coefficient ����ǰ��ע�� 
					case 5 ���Ʊ�ۣ���С��̣� 
					case 6 ���ٻ��������Ʊ�� 
	���������������������֮���״̬ 
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.5
	״̬ ������ɣ��Ѽ��  
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
	��������if_scheme_equal
	�������ж����������ǲ�����ͬ 
	����ԭ�ͣ� int if_scheme_equal(struct Cost_of_scheme a,struct Cost_of_scheme b); 
	�������������  Cost_of_scheme�ͱ��� 
	���������0 �����
			  1 ��� 
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.5 
	״̬ ������ɣ��Ѽ�� 
//**********************************/ 
int if_scheme_equal(struct Cost_of_scheme a,struct Cost_of_scheme b){
		if(abs(a.dist-b.dist)<=eqs&&abs(a.aver_crowd-b.aver_crowd)<=eqs\
		 	&&a.nowtime==b.nowtime&&a.number_of_transfer==b.number_of_transfer\
		 	&&a.number_of_station==b.number_of_station&&a.nowline==b.nowline) return 1;
		 return 0;
}

/************************************ 
	��������init_station 
	���������복վ��Ϣ�����ṹ������Stations��
	����ԭ�ͣ� void init_station(void)
	�����������
		�����ļ��� ./data/stationinfo.txt   ����Ӧ��������վ�㣨���ظ�������Ϣ�����ƣ� 
	�����������
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.5 
	״̬ ������ɣ��Ѽ�� 
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
		printf("��վ����Ϣ�����:\n");
		printf("%d  %s\n",++cnt,Stations[number_of_station].name);
		#endif
	};
	fclose(station_info);station_info=NULL; 
	return;
} 




/************************************ 
	��������init_line 
	������������·��Ϣ��Ϣ�����ṹ��� 
	����ԭ�ͣ� void init_line(void)
	�����������
		�����ļ��� ./data/subwayline.txt   ����Ӧ����������·����Ϣ���������ȡ���·����վ����������� 
	�����������
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.2.25 
	״̬ ������ɣ��Ѽ�� 
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
	��������init_Crowdedness 
	��������ʼ��������·��ÿ��ʱ����ϵ�ӵ��ϵ���� 
	����ԭ�ͣ� void init_Crowdedness()�� 
	�����������
	�����������
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.5 
	״̬ ������ɣ��Ѽ�� 
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
	��������init_coordinates
	���������������վ�ľ�γ������ 
	����ԭ�ͣ� void init_coordinates()�� 
	�����������./data/Coordinates.txt�����복վ�ľ�γ������ 
	�����������
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.10 
	״̬ ������ɣ��Ѽ�� 
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
		fprintf(Long_debugging_information,"Station:%s  ,id��%d��longitude:%lf   latitude :%lf\n",stationname,stationid,Stations[stationid].longitude,Stations[stationid].latitude);	
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
	��������init 
	��������main.c�����������������ڴ��������������������ʼ��������ʵ�ֳ�ʼ�����̡���������һ�����ƺ����� 
	����ԭ�ͣ� void init()
	�����������
	�����������
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.2.25 
	״̬ ������ɣ��Ѽ�� 
//**********************************/ 
void init(){
	number_of_station=number_of_edge=0;
	Long_debugging_information = fopen("./data/Long_debugging_information.txt","w");
	init_station();init_coordinates();init_line();init_Crowdedness();
	return;
}

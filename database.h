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
//#define debug
#define use_gui
//#define gui_print_detailed_information  
#define maincost_ascending
  
struct Station_unit{
	char name[lenth_of_station_name];
	int hed_of_edge,number_on_line[total_number_of_line];
	double longitude,latitude;	
}Stations[total_number_of_station]; 

struct Edge_of_subway{
	int next_edge,time,belonging_line,to_station;
	double lenth,Crowding_factor;
}Edge[total_number_of_edge];

struct Subway_line{
	char first_endpoint[lenth_of_station_name],second_endpoint[lenth_of_station_name];
	char stations[total_number_of_station][lenth_of_station_name];
	int Maximum_load,total_station;
	double total_lenth;
}Subwaylines[total_number_of_line];

struct Cost_of_scheme{
	int nowtime,number_of_transfer,number_of_station,nowline,starttime,now_station;
	double dist,aver_crowd,maincost,limit_factor,tot_crowd;
	struct Cost_of_scheme *last_status;
}; 

int line_in_this_station[total_number_of_line],avoid_status[total_number_of_station][total_number_of_line];
struct Cost_of_scheme station_dis[total_number_of_station][total_number_of_line],passing_situation[5][(total_number_of_station<<1)+100];
double Crowdedness[total_number_of_line][total_number_of_edge];
FILE *Long_debugging_information;



void lines_that_pass_this_station(int stationid,int now_time);
int change_Crowdedness(int now_line,int fromtime,int totime,double new_crowdedness);
double get_modified_coefficient(struct Edge_of_subway *now_edge,int typ,struct Cost_of_scheme *now_status);
void add_edge(int frompoint,int topoint,double Crowding_factor,int belonging_line,double lenth);
int Name_find_station(char *stationname);
int Calculate_the_fare(double total_km);
struct Cost_of_scheme Calculate_the_cost(struct Edge_of_subway *now_edge,struct Cost_of_scheme *now_status,int typ);
int if_scheme_equal(struct Cost_of_scheme a,struct Cost_of_scheme b);

void init();
void init_station();
void init_line();
void init_coordinates();
void init_Crowdedness();

struct Cost_of_scheme Traverse_in_line(struct Cost_of_scheme now_status,int aim_line);
int spfa(struct Cost_of_scheme now_status,int typ);
int Find_the_path(int aimstation,int pls);
int print_the_path(int pls,int typ);
int find_shortest_path(int aimstation,struct Cost_of_scheme now_situation,int typ);

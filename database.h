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
//#define debug                    //ͨ������˺������������Ϣ 
#define use_gui                    //ͨ������˺�������GUI���� 
//#define  Long_debugging_information      //ͨ������˺�����һЩ������Ϣ������ļ�./data/Long_debugging_information.txt����
//#define gui_print_detailed_information              //ͨ������˺��������ϸ����·ͨ��վ����Ϣ��gui�е��ı������  �����ݺܳ� 
#define maincost_ascending          //������������ʱ��ͨ��maincost����С��������·�����򣬷���ʹ��·����������� 
  
struct Station_unit{          //ÿ���ṹ������洢һ��վ�����Ϣ 
	char name[lenth_of_station_name];    //���ƣ�ʹ��ANSI 
	int hed_of_edge,number_on_line[total_number_of_line];   
	//��һ�����ߣ�hed_of_edge �� ��ǰվ���ڸ�����·�ϵ�λ��number_on_line
	double longitude,latitude;	         //��γ�� 
}Stations[total_number_of_station]; 

struct Edge_of_subway{       //����һ���ߵ���Ϣ 
	int next_edge,time,belonging_line,to_station;    //belonging_line����������ʾһ����·�����ַ��� 
	double lenth,Crowding_factor;
}Edge[total_number_of_edge];

struct Subway_line{     //����һ����·��������Ϣ 
	char first_endpoint[lenth_of_station_name],second_endpoint[lenth_of_station_name];   //���˵㳵վ�� 
	char stations[total_number_of_station][lenth_of_station_name];    //��վ������ 
	int Maximum_load,total_station;    
	double total_lenth;
}Subwaylines[total_number_of_line];

struct Cost_of_scheme{    //����һ��״̬��������������������Ҫ�Ľṹ�� 
	int nowtime,number_of_transfer,number_of_station,nowline,starttime,now_station;
	double dist,aver_crowd,maincost,limit_factor,tot_crowd;  
	//�˴���aver_crowd��ʾ����ƽ��ӵ��ϵ�������ǵ�������㣬���м�״̬��ʱ��ʵ���ϴ洢����ǰ�澭�����бߵ�ӵ��ϵ��֮��
	// tot_crowd��ʾƽ��ӵ���ȣ�����ͬ�� 
	struct Cost_of_scheme *last_status;
	//�洢ת�Ƶ���״̬����һ��״̬ 
}; 

int line_in_this_station[total_number_of_line],avoid_status[total_number_of_station][total_number_of_line];
struct Cost_of_scheme station_dis[total_number_of_station][total_number_of_line],passing_situation[5][(total_number_of_station<<1)+100];
double Crowdedness[total_number_of_line][total_number_of_edge];
FILE *Long_debugging_information;


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
void lines_that_pass_this_station(int stationid,int now_time);

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
int change_Crowdedness(int now_line,int fromtime,int totime,double new_crowdedness);

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
void add_edge(int frompoint,int topoint,double Crowding_factor,int belonging_line,double lenth);

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
int Name_find_station(char *stationname);

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
int Calculate_the_fare(double total_km);

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
double get_modified_coefficient(struct Edge_of_subway *now_edge,int typ,struct Cost_of_scheme *now_status);

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
struct Cost_of_scheme Calculate_the_cost(struct Edge_of_subway *now_edge,struct Cost_of_scheme *now_status,int typ);

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
int if_scheme_equal(struct Cost_of_scheme a,struct Cost_of_scheme b);




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
void init_station();

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
void init_line();

/************************************ 
	��������init_Crowdedness 
	��������ʼ��������·��ÿ��ʱ����ϵ�ӵ��ϵ�������ü�Լ���еĽ�� 
	����ԭ�ͣ� void init_Crowdedness()�� 
	�����������
	�����������
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.5 
	״̬ ������ɣ��Ѽ�� 
//**********************************/ 
void init_Crowdedness();

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
void init_coordinates();

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
void init();



/************************************ 
	��������Traverse_in_line
	�������ڵ�ǰ״̬�£��ص�ǰ��·�ƶ�һվ �����ص����ƶ����״̬ 
	����ԭ�ͣ� struct Cost_of_scheme Traverse_in_line(struct Cost_of_scheme now_status,int aim_line);
	��������� now_situation ��ǰ״̬����Ҫ�ǳ�վid��ʱ���Լ�·��
			   aim_line Ҫ�ƶ����г���·������������ʾ��ͬ�ķ��� 
	��������� �ƶ�һ����֮���״̬ 
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.2  
	״̬ ������ɣ��Ѽ�� 
//**********************************/ 
struct Cost_of_scheme Traverse_in_line(struct Cost_of_scheme now_status,int aim_line);

/************************************ 
	��������spfa
	������ʹ��spfa�㷨�������������·�����ָʹ��maincost��С�� ������·״̬��ʱ��ʹ��Ŀǰվ+Ŀǰ��·��ȷ���ҵ����š�
			����������avoid_status���������ظ������Ŀ��� 
	����ԭ�ͣ� int spfa(struct Cost_of_scheme now_status,int typ); 
	���������now_status �����״̬ 
			  typ Ѱ�����·��ģʽ 
	���������1 ��ʾ���е��˽�β 
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.5 
	״̬ ������ɣ��Ѽ��
//**********************************/ 
int spfa(struct Cost_of_scheme now_status,int typ);

/************************************ 
	��������Find_the_path
	������ͨ����spfa��Ĵ洢ת�Ʒ�����ָ�룬�����ҵ�����·���ϵ�����״̬ 
	����ԭ�ͣ� int Find_the_path(int aimstation,int pls); 
	���������aimstation Ŀ�공վ�ı�ţ�����Ҫ�����ﵹ���ҳ�����·�� 
	������������ص�ǰ·���ĳ��� �����û��·���ͷ���0 
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.5 
	״̬ ������ɣ��Ѽ��
//**********************************/ 
int Find_the_path(int aimstation,int pls);

/************************************ 
	��������print_the_path
	���������һ��Ѱ·����
	����ԭ�ͣ� int print_the_path(int pls,int typ);
	���������pls ���������������洢��λ�� 
			  typ ���������Ѱ·���� 
	��������������������0 
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.5 
	״̬ ������ɣ��Ѽ��
//**********************************/ 
int print_the_path(int pls,int typ);

/************************************ 
	��������find_shortest_path
	������Ѱ·�㷨���ܿ��ƺ������������������ 
	����ԭ�ͣ� int find_shortest_path(int aimstation,struct Cost_of_scheme now_status,int typ); 
	���������aimstation Ŀ�공վ�ı�ţ�����Ҫ�����ﵹ���ҳ�ȫ��·�� 
			  now_situation ��ʼ��״̬����Ҫ���ǵ�ǰ��վid��ʱ�� �������Ķ�Ӧ������ 
			  typ  Ѱ·������ 
	�������������Ļ����ɶ���·�ߵ���� 
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.5 
	״̬ ������ɣ��Ѽ��  
//**********************************/ 
int find_shortest_path(int aimstation,struct Cost_of_scheme now_situation,int typ);

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
	init();            //��ʼ�� 

	//nowwa_scheme.nowtime=trans_time(hours,minutes);
	nowwa_scheme.aver_crowd=0;nowwa_scheme.dist=0;nowwa_scheme.last_status=NULL;nowwa_scheme.maincost=0;
	nowwa_scheme.nowline=0;	nowwa_scheme.number_of_transfer=0;
	nowwa_scheme.number_of_station=0;nowwa_scheme.limit_factor=1.0;nowwa_scheme.tot_crowd=0.0;
	free_travel_distance=0;
    /***************
    ������ʽ�Ĳ��������������Ժ���ã�
	1. ���ɱ��� 
    ***************/
    #ifndef use_gui
	puts("�㽫�����￪ʼ��");scanf("%s",name);
	int now_station=Name_find_station(name),hours=0,minutes=0;
	puts("���ڵ�ʱ���ǣ�Сʱ�ͷ����ÿո������");scanf("%d%d",&hours,&minutes);
	nowwa_scheme.nowtime=trans_time(hours,minutes);nowwa_scheme.now_station=now_station;nowwa_scheme.starttime=nowwa_scheme.nowtime; 
	passing_situation[0][++free_travel_distance]=nowwa_scheme;
	while(1){
		if(nowwa_scheme.maincost>=Inf) {printf("û��һ��·�߿��Ե��������û�г���\n");break;}
		printf("�������� %s��վ��ʱ����%d��%d�����Ѿ�������%lf km\n",Stations[nowwa_scheme.now_station].name,nowwa_scheme.nowtime/60,nowwa_scheme.nowtime%60,nowwa_scheme.dist);
		lines_that_pass_this_station(nowwa_scheme.now_station,nowwa_scheme.nowtime);
		printf("����Գ���:");
		int i,aimline=0;
		for(i=0;i<=20;++i){
			if(line_in_this_station[i]!=-1) printf("%d����%s����(������%d) (��һ�ཫ��%d:%d�ִ�����)      ",abs(i-Bidirectional_line_offset),((i>10)?Subwaylines[abs(i-Bidirectional_line_offset)].second_endpoint:Subwaylines[abs(i-Bidirectional_line_offset)].first_endpoint),i-Bidirectional_line_offset,(line_in_this_station[i]+nowwa_scheme.nowtime)/60,(line_in_this_station[i]+nowwa_scheme.nowtime)%60);
		} 
		puts("          ��������0�˳����ɱ���ģʽ"); 
		scanf("%d",&aimline);
		while(line_in_this_station[aimline+Bidirectional_line_offset]==-1&&aimline!=0)
		{
		printf("%d����·����������\n",aimline);
		puts("��������Ҫ�ߵ���·");
		scanf("%d",&aimline);			
		}
		if(aimline==0) break;
		next_scheme=Traverse_in_line(nowwa_scheme,aimline);
		passing_situation[0][++free_travel_distance]=next_scheme;
		nowwa_scheme=next_scheme;
	}
	
    /***************
    ������ʽ�Ĳ��������������Ժ���ã�
	2.·���Ƽ� 
    ***************/	
	nowwa_scheme.aver_crowd=0;nowwa_scheme.dist=0;nowwa_scheme.last_status=NULL;nowwa_scheme.maincost=0;nowwa_scheme.nowline=0;
	nowwa_scheme.now_station=now_station;nowwa_scheme.number_of_station=0;nowwa_scheme.limit_factor=1.0;
	nowwa_scheme.number_of_transfer=0;nowwa_scheme.tot_crowd=0.0;
	nowwa_scheme.starttime=nowwa_scheme.nowtime;         //���¼�ʱ 
	puts("��Ҫ������ȥ��");scanf("%s",name);
	int aim_station=Name_find_station(name);
	puts("���ƫ��:");
	int preferred_type;
	scanf("%d",&preferred_type);
	if(preferred_type==4){
		puts("��������������:");
		scanf("%lf",&nowwa_scheme.limit_factor);
	} 
//	qwq(aim_station,&nowwa_scheme,1);	
	find_shortest_path(aim_station,nowwa_scheme,preferred_type);
	#endif                     //���ϲ���Ϊʹ�������в����� 
	
	
	
	
	#ifdef use_gui
    gtk_gui(argc,argv);
	#endif
	return 0; 
}

/*
���пƼ���ѧ 
13 35
0
�人��վ 
2

ѭ���� 
8 30 
0
����·
4
0.81 



���пƼ���ѧ 
6 0
0
������ 
2



ѭ���� 
8 30 
0
����·
6
0.81  


Ӧ��1���ߵ��� 


�������������������������� 
���
0 01 
0
������ 
2

���� 


*/


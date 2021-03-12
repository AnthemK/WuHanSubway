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
//#include "answer.h"
#include "database.h"

extern int line_in_this_station[total_number_of_line],avoid_status[total_number_of_station][total_number_of_line];
extern struct Cost_of_scheme station_dis[total_number_of_station][total_number_of_line],passing_situation[5][(total_number_of_station<<1)+100];
int vis[total_number_of_station][total_number_of_line],nowpls,my_queue1[max_status],my_queue2[max_status];
char temporary_string[1000000];

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
	��������Find_the_path
	������ͨ����spfa��Ĵ洢ת�Ʒ�����ָ�룬�����ҵ�����·���ϵ�����״̬ 
	����ԭ�ͣ� int Find_the_path(int aimstation,int pls); 
	���������aimstation Ŀ�공վ�ı�ţ�����Ҫ�����ﵹ���ҳ�����·�� 
	������������ص�ǰ·���ĳ��� �����û��·���ͷ���0 
	����ߣ�HUST IS-1901 ������
	���ڣ�2021.3.5 
	״̬ ������ɣ��Ѽ��
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
		printf("Ŀǰû��һ��·�߿��Ե��������û�г���\n");
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
	printf("��ǰΪ����:%d\nȫ����ʱ%d���ӣ���%lf���Ʊ��%dԪ��ƽ��ӵ��ϵ��Ϊ%lf,ƽ��ӵ����Ϊ%lf,������%dվ,����%d�� ,���ʱ�䣺%lf\n",pls,passing_situation[pls][1].nowtime-passing_situation[pls][1].starttime,passing_situation[pls][1].dist,Calculate_the_fare(passing_situation[pls][1].dist),averagecrowd,averagecrowdindex,passing_situation[pls][1].number_of_station,passing_situation[pls][1].number_of_transfer,passing_situation[pls][1].maincost); 
	
	#ifdef use_gui
	strcat(temporary_string,"��ǰΪ������");
	itoa(pls,linshi,10);strcat(temporary_string,linshi);	
	strcat(temporary_string,"\nȫ����ʱ");
	itoa(passing_situation[pls][1].nowtime-passing_situation[pls][1].starttime,linshi,10);strcat(temporary_string,linshi);
	strcat(temporary_string,"���ӣ���");
	gcvt(passing_situation[pls][1].dist,5,linshi);strcat(temporary_string,linshi);
	strcat(temporary_string,"���Ʊ��");
	itoa(Calculate_the_fare(passing_situation[pls][1].dist),linshi,10);strcat(temporary_string,linshi);
	strcat(temporary_string,"Ԫ\nƽ��ӵ��ϵ��Ϊ");
	gcvt(averagecrowd,10,linshi);strcat(temporary_string,linshi);
	strcat(temporary_string,"Ԫ,ƽ��ӵ����Ϊ");
	gcvt(averagecrowdindex,10,linshi);strcat(temporary_string,linshi);	
	strcat(temporary_string,"    ������");
	itoa(passing_situation[pls][1].number_of_station ,linshi,10);strcat(temporary_string,linshi);
	strcat(temporary_string,"վ������");
	itoa(passing_situation[pls][1].number_of_transfer ,linshi,10);strcat(temporary_string,linshi);
	strcat(temporary_string,"�� ,���ʱ�䣺");
	gcvt(passing_situation[pls][1].maincost,10,linshi);strcat(temporary_string,linshi);	
	strcat(temporary_string,"\n");
	#endif
	for(i=cnt;i>=1;i--){
		if(passing_situation[pls][i+1].nowline!=passing_situation[pls][i].nowline){

			printf("��վ����%d����%s����\n",abs(passing_situation[pls][i].nowline),((passing_situation[pls][i].nowline>0)?Subwaylines[abs(passing_situation[pls][i].nowline)].second_endpoint:Subwaylines[abs(passing_situation[pls][i].nowline)].first_endpoint));

			#ifdef use_gui 
			strcat(temporary_string,"��");
			strcat(temporary_string,Stations[passing_situation[pls][i+1].now_station].name);
			strcat(temporary_string,"��վ����");
			itoa(abs(passing_situation[pls][i].nowline),linshi,10);strcat(temporary_string,linshi);
			strcat(temporary_string,"����");
			strcat(temporary_string,((passing_situation[pls][i].nowline>0)?Subwaylines[abs(passing_situation[pls][i].nowline)].second_endpoint:Subwaylines[abs(passing_situation[pls][i].nowline)].first_endpoint));
			strcat(temporary_string,"����\n");
			#endif
		}  		

		printf("�㽫����%d:%d,����%s����ʱ������������%lf\n",passing_situation[pls][i].nowtime/60,passing_situation[pls][i].nowtime%60,Stations[passing_situation[pls][i].now_station].name,passing_situation[pls][i].dist);

		#ifdef gui_print_detailed_information                //��Ϊ�ı�̫��  ��ʱû�� 
		strcat(temporary_string,"�㽫����");
		itoa(passing_situation[pls][i].nowtime/60,linshi,10);strcat(temporary_string,linshi);
		strcat(temporary_string,":");
		itoa(passing_situation[pls][i].nowtime%60,linshi,10);strcat(temporary_string,linshi);
		strcat(temporary_string,"����");
		strcat(temporary_string,Stations[passing_situation[pls][i].now_station].name);//strcat(temporary_string,"\n");	
		strcat(temporary_string," ����ʱ������������");
		gcvt(passing_situation[pls][i].dist,5,linshi);strcat(temporary_string,linshi);
		strcat(temporary_string,"\n");		
		#endif
		
		#ifdef debug
		printf("��ʱ��Maincost:%lf\n",passing_situation[pls][i].maincost);
		#endif
	}
	hh;hh;
	#ifdef use_gui 
		strcat(temporary_string,"\n\n\n");
	#endif
	return 0;
}


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
int find_shortest_path(int aimstation,struct Cost_of_scheme now_status,int typ){
//	can_you_reach_here;
	memset(avoid_status,0,sizeof(avoid_status));
	struct Cost_of_scheme initial_status=now_status;
	spfa(initial_status,typ);
	
	int cnt1=Find_the_path(aimstation,1),i;  
	#ifdef debug
	hh;puts("����1:");print_the_path(1,typ);
	#endif
	for(i=1;i<cnt1;++i){              //�ߵ���Щ״̬����Ҫ�Ӵ��ۣ��Դ������µ����· ������ǵ�ɾ���� 
		avoid_status[passing_situation[1][i].now_station][passing_situation[1][i].nowline+Bidirectional_line_offset]=avoid_index;
	}
	 
	spfa(initial_status,typ);

	int cnt2=Find_the_path(aimstation,2); 
	#ifdef debug
	hh;puts("����2:");print_the_path(2,typ);
	#endif
	for(i=1;i<cnt2;++i){              //�ߵ���Щ״̬����Ҫ�Ӵ��ۣ��Դ������µ����· ������ǵ�ɾ���� 
		avoid_status[passing_situation[2][i].now_station][passing_situation[2][i].nowline+Bidirectional_line_offset]+=avoid_index;
	}
	
	spfa(initial_status,typ);
	int cnt3=Find_the_path(aimstation,3); 
	#ifdef debug
	hh;puts("����3:");print_the_path(3,typ);
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
	
	puts("�Ƽ�·���������£�"); 
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


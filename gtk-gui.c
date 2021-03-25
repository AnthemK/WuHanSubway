#include "gtk-gui.h" 
 
int startx = 0,i,j,k,lenth;
int w = 400;
int h = 300;
extern char temporary_string[1000000];
int just_number[50];
extern double Crowdedness[total_number_of_line][total_number_of_edge];
extern struct Subway_line Subwaylines[total_number_of_line];
extern struct Cost_of_scheme nowwa_scheme,next_scheme; 
GtkBuilder *main_builder;
char linshi[100000];

char *_utf8(char * str) {return g_locale_to_utf8(str, -1, NULL, NULL, NULL);}    

void printt_nowwa_scheme(int typ)        //�ַ����ڼ����ʱ���Ҫת��utf-8�� 
{
	GtkWidget *Entry_window = GTK_WIDGET(gtk_builder_get_object(main_builder,"Entry_window")); 
	GtkTextBuffer *Print_text_buffer = gtk_text_buffer_new(NULL);
	GtkWidget *Print_nowpls = GTK_WIDGET(gtk_builder_get_object(main_builder,"Print_nowpls"));           //GtkTextView
    gtk_window_set_title(GTK_WINDOW(Entry_window), _utf8("�ı��������"));
	gtk_window_set_position(GTK_WINDOW(Entry_window),GTK_WIN_POS_NONE);        //���̶����ڵ�λ�� 
	gtk_window_set_resizable(GTK_WINDOW(Entry_window),FALSE);
	gtk_widget_set_size_request(Entry_window,400,300);
	gtk_window_set_deletable(GTK_WINDOW(Entry_window),0);       //���ڲ��ܱ��ر� 
		
	temporary_string[0]=0;
	strcat(temporary_string,_utf8("��վ��"));
	strcat(temporary_string,_utf8(Stations[nowwa_scheme.now_station].name)); 
	strcat(temporary_string,"\n");
	//gtk_text_buffer_insert_at_cursor(Print_text_buffer,temporary_string,-1);
	
	strcat(temporary_string,_utf8("��ǰʱ����:"));
	itoa(nowwa_scheme.nowtime/60,linshi,10);strcat(temporary_string,linshi);
	strcat(temporary_string,":");
	itoa(nowwa_scheme.nowtime%60,linshi,10);strcat(temporary_string,linshi);
	strcat(temporary_string,"\n");
	//gtk_text_buffer_insert_at_cursor (Print_text_buffer,temporary_string,-1);
	
	strcat(temporary_string,_utf8("��ǰ�Ѿ������˵ľ�����"));
	gcvt(nowwa_scheme.dist,5,linshi);strcat(temporary_string,linshi);
	strcat(temporary_string,"km\n");
	//gtk_text_buffer_insert_at_cursor (Print_text_buffer,temporary_string,-1);
	
	if(typ==1){
		for(i=0;i<=20;++i){
			if(line_in_this_station[i]!=-1) 
			{
				strcat(temporary_string,_utf8("���Գ���"));
				itoa(abs(i-Bidirectional_line_offset),linshi,10);strcat(temporary_string,linshi);
				strcat(temporary_string,_utf8("���ߵ�"));
				strcat(temporary_string,_utf8(((i>10)?Subwaylines[abs(i-Bidirectional_line_offset)].second_endpoint:Subwaylines[abs(i-Bidirectional_line_offset)].first_endpoint))); 
				strcat(temporary_string,_utf8("����(��һ�ཫ��"));
				itoa((line_in_this_station[i]+nowwa_scheme.nowtime)/60,linshi,10);strcat(temporary_string,linshi);
				strcat(temporary_string," : ");
				itoa((line_in_this_station[i]+nowwa_scheme.nowtime)%60,linshi,10);strcat(temporary_string,linshi);
				strcat(temporary_string,_utf8("�ִﵱǰ��վ"));
				strcat(temporary_string,")\n");
			}
		} 
	}
	
	gtk_text_buffer_insert_at_cursor (Print_text_buffer,temporary_string,-1);
	gtk_text_view_set_buffer (GTK_TEXT_VIEW(Print_nowpls),Print_text_buffer);
//	gtk_widget_hide(GTK_WINDOW(Entry_window));
	gtk_widget_show_all(GTK_WINDOW(Entry_window));
	return; 
}

void printt_string(char *nowwa)
{
	GtkWidget *Entry_window = GTK_WIDGET(gtk_builder_get_object(main_builder,"Entry_window")); 
	GtkTextBuffer *Print_text_buffer = gtk_text_buffer_new(NULL);
	GtkWidget *Print_nowpls = GTK_WIDGET(gtk_builder_get_object(main_builder,"Print_nowpls")); 
    gtk_window_set_title(GTK_WINDOW(Entry_window), _utf8("�ı��������"));
	gtk_window_set_position(GTK_WINDOW(Entry_window),GTK_WIN_POS_NONE);
	gtk_window_set_resizable(GTK_WINDOW(Entry_window),FALSE);
	gtk_widget_set_size_request(Entry_window,600,300);	
	gtk_window_set_deletable(GTK_WINDOW(Entry_window),0);       //���ڲ��ܱ��ر� 
	
	gtk_text_buffer_insert_at_cursor(Print_text_buffer,nowwa,-1);
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(Print_nowpls),Print_text_buffer);
	//gtk_widget_hide(GTK_WINDOW(Entry_window));
	gtk_widget_show_all(GTK_WINDOW(Entry_window));
}

void Point_line_changed(GtkWidget *widget,gpointer data)
{
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(data));
	strcpy(temporary_string,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));
	int nowpls= temporary_string[6]-'0';
	#ifdef debug
	printf("Point_line_changed to: %d\n",nowpls);
	#endif
	for(i=1;i<=Subwaylines[nowpls].total_station;++i)
    	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(data),NULL,_utf8(Subwaylines[nowpls].stations[i]));  
	return;
}
 

void change_time_window_affirm_clicked(GtkWidget *widget,gpointer data)
{
	int nowhour=0,nowminute=0;
	GtkWidget *change_time_window_hour=GTK_WIDGET(gtk_builder_get_object(main_builder,"change_time_window_hour")),*change_time_window_minute=GTK_WIDGET(gtk_builder_get_object(main_builder,"change_time_window_minute")); 	
	strcpy(temporary_string,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(change_time_window_hour)));
	if(temporary_string[0]==0) temporary_string[0]=temporary_string[1]=0;nowhour=(temporary_string[0]-'0');
	if(temporary_string[1]!=0) nowhour=nowhour*10+(temporary_string[1]-'0');
	strcpy(temporary_string,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(change_time_window_minute)));
	if(temporary_string[0]==0) temporary_string[0]=temporary_string[1]=0;nowminute=(temporary_string[0]-'0');
	if(temporary_string[1]!=0) nowminute=nowminute*10+(temporary_string[1]-'0');
	nowwa_scheme.nowtime=trans_time(nowhour,nowminute);
	#ifdef debug
	outt(nowwa_scheme.nowtime);
	#endif
	gtk_widget_hide(GTK_WIDGET(data));       //�˴���������Ϊ��ʹ�����ص�Ҳ����ν 
	return;
}

void Change_time_button_clicked(GtkWidget *widget,gpointer data)
{
 	GtkWidget *change_time_window = GTK_WIDGET(gtk_builder_get_object(main_builder,"change_time_window"));   
    gtk_window_set_title(GTK_WINDOW(change_time_window), _utf8("ʱ�����"));
	gtk_window_set_position(GTK_WINDOW(change_time_window),GTK_WIN_POS_CENTER);    //���м���ʾ���� 
	gtk_widget_set_size_request(change_time_window,300,100);
	gtk_window_set_resizable(GTK_WINDOW(change_time_window),FALSE);
	gtk_window_set_deletable(GTK_WINDOW(change_time_window),0);
	
    GtkWidget *change_time_window_hour=GTK_WIDGET(gtk_builder_get_object(main_builder,"change_time_window_hour")),*change_time_window_minute=GTK_WIDGET(gtk_builder_get_object(main_builder,"change_time_window_minute")); 	
	int nowpls=0;
    for(i=0;i<=24;++i){
    	nowpls=0;
    	if(i>=10) {
    		temporary_string[nowpls]=(i/10)+'0';
    		nowpls++;
    	}
    	temporary_string[nowpls]=i%10+'0';temporary_string[nowpls+1]=0;
    	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(change_time_window_hour),NULL,temporary_string);   
    	//gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(topoint_line),NULL,temporary_string);   	
    } 	
    for(i=0;i<=59;++i){
    	nowpls=0;
    	if(i>=10) {
    		temporary_string[nowpls]=(i/10)+'0';
    		nowpls++;
    	}
    	temporary_string[nowpls]=i%10+'0';temporary_string[nowpls+1]=0;
    	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(change_time_window_minute),NULL,_utf8(temporary_string));   
    	//gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(topoint_line),NULL,temporary_string);   	
    } 	
    gtk_combo_box_set_active(GTK_COMBO_BOX_TEXT(change_time_window_hour),nowwa_scheme.nowtime/60);
    gtk_combo_box_set_active(GTK_COMBO_BOX_TEXT(change_time_window_minute),nowwa_scheme.nowtime%60);
    
 	GtkWidget *change_time_window_affirm_button = GTK_WIDGET(gtk_builder_get_object(main_builder,"change_time_window_affirm_button"));
	g_signal_connect(change_time_window_affirm_button,"clicked", G_CALLBACK(change_time_window_affirm_clicked), change_time_window);	
	//gtk_widget_hide(change_time_window);	       //û��Ҫˢ���� 
	gtk_widget_show_all(change_time_window);
	return;
}
 
 
void Change_crowding_factor_clicked(GtkWidget *widget,gpointer data)
{
	gtk_widget_hide(GTK_WIDGET(data));
	int now_line,fromtimehour,fromtimeminute,totimehour,totimeminute;
	double new_crowdedness;	
	Change_crowding_factor_failed:
	puts("����������Ҫ�޸ĵ���·,��ʼʱ��(����������ʾСʱ�ͷ���),����ʱ��(����������ʾСʱ�ͷ���),���޸ĺ��ӵ������ֵ(����0�򲻽����޸�):"); 
	scanf("%d",&now_line);
	if(now_line==0){
		puts("ȡ���޸ģ�"); 
		sleep(2);
		gtk_widget_show_all(GTK_WIDGET(data));
		return;
	} 	
	scanf("%d%d%d%d%lf",&fromtimehour,&fromtimeminute,&totimehour,&totimeminute,&new_crowdedness);

	if(now_line>=1&&now_line<=8&&now_line!=5&&fromtimehour>=0&&fromtimehour<=24&&fromtimeminute>=0&&fromtimeminute<=59&&totimehour>=0&&totimehour<=24&&totimeminute>=0&&totimeminute<=59&&new_crowdedness>=0.0&&new_crowdedness<=1.0)
	{
		change_Crowdedness(now_line,trans_time(fromtimehour,fromtimeminute),trans_time(totimehour,totimeminute),new_crowdedness);
		puts("�޸ĳɹ���");
	} else {
		puts("���ݲ��Ϸ���");
		goto Change_crowding_factor_failed;
	}
	sleep(2);
	//gtk_widget_hide(GTK_WIDGET(data));
	gtk_widget_show_all(GTK_WIDGET(data));
	return;
}
 
 
void Travel_at_line(GtkWidget *widget,gpointer data) 
{
	lines_that_pass_this_station(nowwa_scheme.now_station,nowwa_scheme.nowtime);
	if(line_in_this_station[*(int *)data]==-1)  return;
	next_scheme=Traverse_in_line(nowwa_scheme,*(int *)data-Bidirectional_line_offset);
	passing_situation[0][++free_travel_distance]=next_scheme;
	nowwa_scheme=next_scheme;
	passing_situation[0][1].number_of_station=free_travel_distance-1;
	printt_nowway_map(0); 
	lines_that_pass_this_station(nowwa_scheme.now_station,nowwa_scheme.nowtime);
	printt_nowwa_scheme(1);	
	return;
} 
 
void Free_travel(GtkWidget *widget,gpointer data) 
{
 	GtkWidget *frompoint_line=GTK_WIDGET(gtk_builder_get_object(main_builder,"frompoint_line")),*frompoint_station=GTK_WIDGET(gtk_builder_get_object(main_builder,"frompoint_station"));
 	temporary_string[0]=0; 
	strcpy(temporary_string,Subwaylines[gtk_combo_box_get_active(GTK_COMBO_BOX_TEXT(frompoint_line))+((gtk_combo_box_get_active(GTK_COMBO_BOX_TEXT(frompoint_line))>3)?2:1)].stations[gtk_combo_box_get_active(GTK_COMBO_BOX_TEXT(frompoint_station))+1]);
 	nowwa_scheme.now_station=Name_find_station(temporary_string);
 	
	//#ifndef debug             //������Ĭ�ϻ���� 
	printf("Start at:%s\n id is %d,at %d:%d\n",temporary_string,nowwa_scheme.now_station,nowwa_scheme.nowtime/60,nowwa_scheme.nowtime%60);
 	//#endif 
 	//  ��õ�ǰվ����Ϣ 

	nowwa_scheme.aver_crowd=0.0;nowwa_scheme.dist=0;nowwa_scheme.last_status=NULL;nowwa_scheme.maincost=0;nowwa_scheme.nowline=0;nowwa_scheme.tot_crowd=0.0;
	nowwa_scheme.number_of_transfer=0;nowwa_scheme.number_of_station=0;nowwa_scheme.limit_factor=1.0;nowwa_scheme.starttime=nowwa_scheme.nowtime;
 	for(i=1;i<=free_travel_distance+2;++i) passing_situation[0][i]=nowwa_scheme;   //��һ���Ǹ�ʲô���ŵģ����� 
 	free_travel_distance=0;          //���·�� 	
	lines_that_pass_this_station(nowwa_scheme.now_station,nowwa_scheme.nowtime);
	passing_situation[0][++free_travel_distance]=nowwa_scheme;passing_situation[0][1].number_of_station=free_travel_distance-1;
	printt_nowway_map(0); 
	printt_nowwa_scheme(1);	
	//��ʼ��������ɳ�����Ϣ 
	
	
	GtkWidget *Free_travel_option = GTK_WIDGET(gtk_builder_get_object(main_builder,"Free_travel_option")); 
    gtk_window_set_title(GTK_WINDOW(Free_travel_option), _utf8("���ɱ�������"));
	gtk_window_set_position(GTK_WINDOW(Free_travel_option),GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(Free_travel_option),FALSE);	
	gtk_window_set_deletable(GTK_WINDOW(Free_travel_option),0);
	
	//�ڶ����»��߱�ʾ����  �˴������ӻص����� 
	GtkWidget *line_id__1 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id__1")); 	
	g_signal_connect(line_id__1,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset-1);
	GtkWidget *line_id_1 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id_1")); 	
	g_signal_connect(line_id_1,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset+1);
	
	GtkWidget *line_id__2 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id__2")); 	
	g_signal_connect(line_id__2,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset-2);
	GtkWidget *line_id_2 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id_2")); 	
	g_signal_connect(line_id_2,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset+2);
	
	GtkWidget *line_id__3 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id__3")); 	
	g_signal_connect(line_id__3,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset-3);
	GtkWidget *line_id_3 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id_3")); 	
	g_signal_connect(line_id_3,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset+3);
	
	GtkWidget *line_id__4 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id__4")); 	
	g_signal_connect(line_id__4,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset-4);
	GtkWidget *line_id_4 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id_4")); 	
	g_signal_connect(line_id_4,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset+4);
	
	GtkWidget *line_id__6 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id__6")); 	
	g_signal_connect(line_id__6,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset-6);
	GtkWidget *line_id_6 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id_6")); 	
	g_signal_connect(line_id_6,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset+6);
	
	GtkWidget *line_id__7 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id__7")); 	
	g_signal_connect(line_id__7,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset-7);
	GtkWidget *line_id_7 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id_7")); 	
	g_signal_connect(line_id_7,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset+7);
	
	GtkWidget *line_id__8 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id__8")); 	
	g_signal_connect(line_id__8,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset-8);
	GtkWidget *line_id_8 = GTK_WIDGET(gtk_builder_get_object(main_builder,"line_id_8")); 	
	g_signal_connect(line_id_8,"clicked", G_CALLBACK(Travel_at_line), just_number+Bidirectional_line_offset+8);	
	//gtk_widget_hide(GTK_WIDGET(Free_travel_option));	
	gtk_widget_show_all(GTK_WIDGET(Free_travel_option));
  	return;
}
 
 
void Find_way_type_clicked(GtkWidget *widget,gpointer data)
{
	temporary_string[0]=0;
	#ifdef debug
	strcpy(temporary_string,gtk_button_get_label(GTK_BUTTON(widget)));
	printf("%s\n",temporary_string);
	#endif
	int aim_station,preferred_type=*((int *)data);
	if(preferred_type==4){
		GtkWidget *find_way_type4_entry = GTK_WIDGET(gtk_builder_get_object(main_builder,"find_way_type4_entry"));  
		strcpy(temporary_string,gtk_entry_get_text(GTK_ENTRY(find_way_type4_entry)));
		nowwa_scheme.limit_factor=atof(temporary_string);
		#ifdef  debug
		printf("type == 4 ,and limit_factor == %lf\n",nowwa_scheme.limit_factor);
		#endif
	} else {
		nowwa_scheme.limit_factor=1.0;
	}
	
 	GtkWidget *frompoint_line=GTK_WIDGET(gtk_builder_get_object(main_builder,"frompoint_line")),*frompoint_station=GTK_WIDGET(gtk_builder_get_object(main_builder,"frompoint_station"));
 	GtkWidget *topoint_line=GTK_WIDGET(gtk_builder_get_object(main_builder,"topoint_line")),*topoint_station=GTK_WIDGET(gtk_builder_get_object(main_builder,"topoint_station"));

 	strcpy(temporary_string,Subwaylines[gtk_combo_box_get_active(GTK_COMBO_BOX_TEXT(frompoint_line))+((gtk_combo_box_get_active(GTK_COMBO_BOX_TEXT(frompoint_line))>3)?2:1)].stations[gtk_combo_box_get_active(GTK_COMBO_BOX_TEXT(frompoint_station))+1]);
 	nowwa_scheme.now_station=Name_find_station(temporary_string);
 	
	//#ifndef debug             //������Ĭ�ϻ������������ 
	puts("��ʼѰ·���̣�");
	printf("Start at:%s\n id is %d,xxxxxxxxxx%s\n",temporary_string,nowwa_scheme.now_station,Stations[nowwa_scheme.now_station].name);
 	//#endif 
 	
 	strcpy(temporary_string,Subwaylines[gtk_combo_box_get_active(GTK_COMBO_BOX_TEXT(topoint_line))+((gtk_combo_box_get_active(GTK_COMBO_BOX_TEXT(topoint_line))>3)?2:1)].stations[gtk_combo_box_get_active(GTK_COMBO_BOX_TEXT(topoint_station))+1]);
 	aim_station=Name_find_station(temporary_string);	
 	//#ifndef debug            //������Ĭ�ϻ������������ 
	printf("End at:%s\n id is %d,xxxxxxxxxx%s\n",temporary_string,aim_station,Stations[aim_station].name);
 	//#endif 
	//#ifndef debug           //������Ĭ�ϻ������������ 
	printf("Time is :%d:%d,Typ is %d\n\n\n",nowwa_scheme.nowtime/60,nowwa_scheme.nowtime%60,preferred_type);
	//#endif
	
	nowwa_scheme.aver_crowd=0.0;nowwa_scheme.dist=0;nowwa_scheme.last_status=NULL;nowwa_scheme.maincost=0;nowwa_scheme.nowline=0;
	nowwa_scheme.number_of_transfer=0;nowwa_scheme.number_of_station=0;nowwa_scheme.starttime=nowwa_scheme.nowtime;nowwa_scheme.tot_crowd=0.0;

	GtkWidget *find_way_type_window = GTK_WIDGET(gtk_builder_get_object(main_builder,"find_way_type_window"));   
	gtk_widget_hide(GTK_WIDGET(find_way_type_window));    //�����ƺ�û�г������⣬��˲��޸� 
	
	if(nowwa_scheme.now_station==aim_station){
		printt_string(_utf8("�㲻��Ҫ��ͬվ�˵���"));
	}
	else {
		temporary_string[0]=0;
		find_shortest_path(aim_station,nowwa_scheme,preferred_type);
		strcat(temporary_string,"�����Ҫ�����ϸ�ľ���վ����Ϣ����ȥ��������д����������Ϣ��\n�����Ҫ�鿴·�ߵ�ģ��ͼ���������Ͻ�ѡ���Ӧ��·��\n");
		printt_string(_utf8(temporary_string)); 
	}
	return;
}

 
void Find_way_button_clicked(GtkWidget *widget,gpointer data)
{
	 GtkWidget *find_way_type_window = GTK_WIDGET(gtk_builder_get_object(main_builder,"find_way_type_window"));   
     gtk_window_set_title(GTK_WINDOW(find_way_type_window), _utf8("��ѡ��һ����������"));
	 gtk_window_set_position(GTK_WINDOW(find_way_type_window),GTK_WIN_POS_CENTER);
	 //gtk_widget_set_size_request(change_time_window,200,150);
	 gtk_window_set_resizable(GTK_WINDOW(find_way_type_window),FALSE);
	 gtk_window_set_deletable(GTK_WINDOW(find_way_type_window),0);	
	
  	 GtkWidget *find_way_type1=GTK_WIDGET(gtk_builder_get_object(main_builder,"find_way_type1"));
     g_signal_connect(find_way_type1,"clicked", G_CALLBACK(Find_way_type_clicked),just_number+1);   
  	 GtkWidget *find_way_type2=GTK_WIDGET(gtk_builder_get_object(main_builder,"find_way_type2"));
     g_signal_connect(find_way_type2,"clicked", G_CALLBACK(Find_way_type_clicked),just_number+2); 
	 GtkWidget *find_way_type3=GTK_WIDGET(gtk_builder_get_object(main_builder,"find_way_type3"));
     g_signal_connect(find_way_type3,"clicked", G_CALLBACK(Find_way_type_clicked),just_number+3); 
   	 GtkWidget *find_way_type4=GTK_WIDGET(gtk_builder_get_object(main_builder,"find_way_type4"));
     g_signal_connect(find_way_type4,"clicked", G_CALLBACK(Find_way_type_clicked),just_number+4); 
   	 GtkWidget *find_way_type5=GTK_WIDGET(gtk_builder_get_object(main_builder,"find_way_type5"));
     g_signal_connect(find_way_type5,"clicked", G_CALLBACK(Find_way_type_clicked),just_number+5); 
   	 GtkWidget *find_way_type6=GTK_WIDGET(gtk_builder_get_object(main_builder,"find_way_type6"));
     g_signal_connect(find_way_type6,"clicked", G_CALLBACK(Find_way_type_clicked),just_number+6); 	
	 //gtk_widget_hide(find_way_type_window);	
	 gtk_widget_show_all(find_way_type_window);
	 return;
}


void Drawing_orign_map(GtkWidget *widget,gpointer data)
{
     GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file("./data/orignal_map.png", NULL);  
 	 GtkImage *Display_map=GTK_IMAGE(gtk_builder_get_object(main_builder,"Display_map"));
	 gtk_image_set_from_pixbuf(Display_map,src_pixbuf); 
	 return;
}


void Drawing_crowded_map(GtkWidget *widget,gpointer data)
{
	 cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,Image_Max_X,Image_Max_Y);
	 cairo_t *cr_map = cairo_create(surface);    // ����cairo������ע�����
     cairo_set_line_width (cr_map, 2.0);
	 int i,j;
	 double now_crowded_factor,Slope_of_rgb=1.732;   //Slope_of_rgbӦ����1.0��2.0֮�� 
	 for(i=1;i<=number_of_station;++i){
	 	cairo_set_source_rgb(cr_map, 0.0, 0.0, 0.0);
	 	cairo_arc(cr_map,Stations[i].longitude,Stations[i].latitude,2.5,ANGLE(0.0),ANGLE(360.0));
	 	cairo_fill(cr_map);
	 	//  ��վ�� 
	 	for(j=Stations[i].hed_of_edge;j;j=Edge[j].next_edge){
	 		if(Edge[j].to_station<i||Edge[j].to_station==0) continue;
	 		now_crowded_factor=Crowdedness[abs(Edge[j].belonging_line)][nowwa_scheme.nowtime];
	 		
	 		//cairo_set_source_rgb(cr_map,(now_crowded_factor>0.5?((now_crowded_factor-0.5)*2):0.0),(now_crowded_factor<0.5?((0.5-now_crowded_factor)*2):0.0), 0.0);
	 		//����1�������Ŵ� now_crowded_factor-0.5����һ��Ϊ0 
	 		
			//cairo_set_source_rgb(cr_map,now_crowded_factor*now_crowded_factor,(1.0-now_crowded_factor)*(1.0-now_crowded_factor), 0.0);
		 	// ����2�� ͨ��now_crowded_factor��1.0-now_crowded_factor�Ķ��κ���ʵ�� 
		 	
			//cairo_set_source_rgb(cr_map,now_crowded_factor,(1.0-now_crowded_factor), 0.0);
		 	// ����3�� ͨ��now_crowded_factor��1.0-now_crowded_factor��һ�κ���ʵ�� 	 		

	 		cairo_set_source_rgb(cr_map,(now_crowded_factor>=0.5?Slope_of_rgb/2.0:now_crowded_factor*Slope_of_rgb),(now_crowded_factor<=0.5?Slope_of_rgb/2.0:(1.0-now_crowded_factor)*Slope_of_rgb), 0.0);
	 		//����4�� ͨ�� Slope_of_rgb��������ɫ�����ڵ�һ��ɫ������Ϊһ��ֱ�����Σ������ۺϵ���ɫ��Ӧ������ɫ()->��ɫ(0.5)->��ɫ(1.0) 
	 		
			cairo_move_to(cr_map,Stations[i].longitude,Stations[i].latitude);
	 		cairo_line_to(cr_map,Stations[Edge[j].to_station].longitude,Stations[Edge[j].to_station].latitude);
	 		cairo_stroke(cr_map);
	 	}
	 }
	 cairo_surface_write_to_png(surface, "./data/crowded_map.png");
	 cairo_destroy(cr_map);
	 cairo_surface_destroy (surface);
	 // ����ͼƬ�����ٲ��˳�Cairo��ͼ���� 
	 
     GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file("./data/crowded_map.png", NULL);  
 	 GtkImage *Display_map=GTK_IMAGE(gtk_builder_get_object(main_builder,"Display_map"));
	 gtk_image_set_from_pixbuf(Display_map,src_pixbuf); 
	 return;
}
 
 
 
void printt_nowway_map(int pls)
{
	int i,totstation;	
	totstation=passing_situation[pls][1].number_of_station+1;
	#ifdef debug
	printf("%d   %d\n",pls,totstation);
	#endif
	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,Image_Max_X,Image_Max_Y);
	cairo_t *cr_map = cairo_create(surface);    // ����cairo������ע�����
	cairo_set_line_width (cr_map, 2.0);
	cairo_set_source_rgb(cr_map, 1.0, 0.0, 0.0);
	cairo_arc(cr_map,Stations[passing_situation[pls][1].now_station].longitude,Stations[passing_situation[pls][1].now_station].latitude,3.5,ANGLE(0.0),ANGLE(360.0));	
	cairo_fill(cr_map);	
	cairo_set_source_rgb(cr_map,0.0, 0.0, 0.0);
	for(i=2;i<=totstation;++i)
	{
		if(i!=totstation){
			if(i<totstation&&passing_situation[pls][i+1].nowline!=passing_situation[pls][i].nowline)  
			{
				cairo_set_source_rgb(cr_map, 1.0, 0.0, 1.0);
				cairo_arc(cr_map,Stations[passing_situation[pls][i].now_station].longitude,Stations[passing_situation[pls][i].now_station].latitude,3.5,ANGLE(0.0),ANGLE(360.0));
	 		}else   cairo_arc(cr_map,Stations[passing_situation[pls][i].now_station].longitude,Stations[passing_situation[pls][i].now_station].latitude,2.5,ANGLE(0.0),ANGLE(360.0));
			cairo_fill(cr_map);	
			cairo_set_source_rgb(cr_map,0.0, 0.0, 0.0);		
		}
		
	 	cairo_move_to(cr_map,Stations[passing_situation[pls][i].now_station].longitude,Stations[passing_situation[pls][i].now_station].latitude);
	 	cairo_line_to(cr_map,Stations[passing_situation[pls][i-1].now_station].longitude,Stations[passing_situation[pls][i-1].now_station].latitude);
	 	cairo_stroke(cr_map);
	}
	
	cairo_set_source_rgb(cr_map, 0.0, 1.0, 0.0);
	cairo_arc(cr_map,Stations[passing_situation[pls][totstation].now_station].longitude,Stations[passing_situation[pls][totstation].now_station].latitude,(pls==0)?6.0:3.5,ANGLE(0.0),ANGLE(360.0));	
	cairo_fill(cr_map);
	cairo_surface_write_to_png(surface, "./data/nowway_map.png");  /* ���ٲ��˳�Cairo��ͼ���� */
	cairo_destroy(cr_map);
	cairo_surface_destroy (surface);
	 
    GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file("./data/nowway_map.png", NULL);  
 	GtkImage *Display_map=GTK_IMAGE(gtk_builder_get_object(main_builder,"Display_map"));
	gtk_image_set_from_pixbuf(Display_map,src_pixbuf); 
	return; 
}
 
 
void to_print_one_type(GtkWidget *widget,gpointer data)
{
	if(passing_situation[*(int *)data][1].number_of_station==0&&(*(int *)data!=0||free_travel_distance==0)){
		printt_string(_utf8("û���Ѿ��洢�˵���·")); 
	}else 
	{
		printt_nowway_map(*(int *)data);
	}
 	GtkWidget *choose_type_to_print = GTK_WIDGET(gtk_builder_get_object(main_builder,"choose_type_to_print"));   
	gtk_widget_hide(GTK_WIDGET(choose_type_to_print));	  //�ر�ѡ��Ľ��� 
}
 
 
void Drawing_nowway_map(GtkWidget *widget,gpointer data)
{
 	GtkWidget *choose_type_to_print = GTK_WIDGET(gtk_builder_get_object(main_builder,"choose_type_to_print"));   
    gtk_window_set_title(GTK_WINDOW(choose_type_to_print), "ѡ����Ҫ��ʾ����·");
	gtk_window_set_position(GTK_WINDOW(choose_type_to_print),GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(choose_type_to_print),FALSE);
	 gtk_window_set_deletable(GTK_WINDOW(choose_type_to_print),0);
	 	
 	GtkWidget *to_print_type0 = GTK_WIDGET(gtk_builder_get_object(main_builder,"to_print_type0")); 
    g_signal_connect(to_print_type0,"clicked", G_CALLBACK(to_print_one_type), just_number+0);	
 	GtkWidget *to_print_type1 = GTK_WIDGET(gtk_builder_get_object(main_builder,"to_print_type1")); 
    g_signal_connect(to_print_type1,"clicked", G_CALLBACK(to_print_one_type), just_number+1);
 	GtkWidget *to_print_type2 = GTK_WIDGET(gtk_builder_get_object(main_builder,"to_print_type2")); 
    g_signal_connect(to_print_type2,"clicked", G_CALLBACK(to_print_one_type), just_number+2);
 	GtkWidget *to_print_type3 = GTK_WIDGET(gtk_builder_get_object(main_builder,"to_print_type3")); 
    g_signal_connect(to_print_type3,"clicked", G_CALLBACK(to_print_one_type), just_number+3);
 	
	//gtk_widget_hide(GTK_WINDOW(choose_type_to_print));	   
	gtk_widget_show_all(GTK_WINDOW(choose_type_to_print));
}
 
int gtk_gui(int argc , char **argv)
{
	for(i=0;i<20;++i) just_number[i]=i;
	nowwa_scheme.nowtime=trans_time(8,0); free_travel_distance=0;
	passing_situation[0][1].number_of_station=passing_situation[1][1].number_of_station=passing_situation[2][1].number_of_station=passing_situation[3][1].number_of_station=0;
	gtk_init (&argc, &argv);   //���� GTKӦ�ó���Ҫ���øú��������ұ����ڿؼ�����֮ǰʹ�ã���Ϊ��������һЩȱʡֵ( �����Ӿ�����ɫ )ӳ������������������ʼ��������ȱʡ���źŴ������������ͨ�������д��ݸ�Ӧ�ó���Ĳ������Զ����һЩ��Ҫ�ĳ�ʼ�������� 
	
	/***************
    ����galde�ļ�����ø�ʽ 
    ***************/	
    main_builder = gtk_builder_new(); 
	if(!gtk_builder_add_from_file(main_builder,"./glade_build/Main_Framework.glade",NULL)){
		printf("Cannot load file!");
		return 0; 
	} 
	
 	/***************
    ��ȡ������ؼ��������������йز��� 
    ***************/
 	GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(main_builder,"main_window"));   
    gtk_window_set_title(GTK_WINDOW(window), "WuHanSubway");
 	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_widget_set_size_request(window,500,375);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	
	
 	/***************
    ��ȡ���Ͻ�comboboxtext�ؼ��������� 
	
	����  �ܲ�������һ����ʼֵ 
    ***************/    
 	GtkWidget *frompoint_line=GTK_WIDGET(gtk_builder_get_object(main_builder,"frompoint_line")),*frompoint_station=GTK_WIDGET(gtk_builder_get_object(main_builder,"frompoint_station"));
 	GtkWidget *topoint_line=GTK_WIDGET(gtk_builder_get_object(main_builder,"topoint_line")),*topoint_station=GTK_WIDGET(gtk_builder_get_object(main_builder,"topoint_station"));
    strcpy(temporary_string,_utf8("��·"));
	lenth=strlen(temporary_string);
    for(i=1;i<=8;++i){
    	if(i==5) ++i;
    	temporary_string[lenth]=i+'0';temporary_string[lenth+1]=0;
    	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(frompoint_line),NULL,temporary_string);   
    	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(topoint_line),NULL,temporary_string);   	
    } 
     g_signal_connect(frompoint_line, "changed", G_CALLBACK(Point_line_changed), frompoint_station);
     g_signal_connect(topoint_line, "changed", G_CALLBACK(Point_line_changed), topoint_station);

	 //gtk_combo_box_set_active_id (GTK_COMBO_BOX_TEXT(frompoint_line),"1");  /////////////// 
	 gtk_combo_box_set_active(GTK_COMBO_BOX_TEXT(frompoint_line),3);
	 gtk_combo_box_set_active(GTK_COMBO_BOX_TEXT(frompoint_station),0);	 
	 gtk_combo_box_set_active(GTK_COMBO_BOX_TEXT(topoint_line),1);
	 gtk_combo_box_set_active(GTK_COMBO_BOX_TEXT(topoint_station),29);
	 
	 
  	 GtkWidget *change_time_button=GTK_WIDGET(gtk_builder_get_object(main_builder,"change_time_button"));
     g_signal_connect(change_time_button,"clicked", G_CALLBACK(Change_time_button_clicked), NULL);	
 	
  	 GtkWidget *change_crowding_factor_button=GTK_WIDGET(gtk_builder_get_object(main_builder,"change_crowding_factor_button"));
     g_signal_connect(change_crowding_factor_button,"clicked", G_CALLBACK(Change_crowding_factor_clicked), window);	 
 	
 	
  	 GtkWidget *Free_travel_button=GTK_WIDGET(gtk_builder_get_object(main_builder,"Free_travel_button"));
     g_signal_connect(Free_travel_button,"clicked", G_CALLBACK(Free_travel), NULL);	 
  	
 	
 	 GtkWidget *Find_way_button=GTK_WIDGET(gtk_builder_get_object(main_builder,"Find_way_button"));
     g_signal_connect(Find_way_button,"clicked", G_CALLBACK(Find_way_button_clicked),NULL);
     
     
     GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file("./data/orignal_map.png", NULL);  
 	 GtkImage *Display_map=GTK_IMAGE(gtk_builder_get_object(main_builder,"Display_map"));
	 gtk_image_set_from_pixbuf(Display_map,src_pixbuf); 
	 
	 GtkWidget *Display_orign_map=GTK_WIDGET(gtk_builder_get_object(main_builder,"Display_orign_map"));
     g_signal_connect(Display_orign_map,"clicked", G_CALLBACK(Drawing_orign_map),NULL);     
     
	 GtkWidget *Display_crowded_map=GTK_WIDGET(gtk_builder_get_object(main_builder,"Display_crowded_map"));
     g_signal_connect(Display_crowded_map,"clicked", G_CALLBACK(Drawing_crowded_map),NULL);     


	 GtkWidget *Display_nowway_map=GTK_WIDGET(gtk_builder_get_object(main_builder,"Display_nowway_map"));
     g_signal_connect(Display_nowway_map,"clicked", G_CALLBACK(Drawing_nowway_map),NULL);   
 	/*
	 gtk_combo_box_get_active ()
	 gtk_combo_box_text_remove_all ()
	 gtk_combo_box_text_get_active_text ()
	 
	 */ 
 	
 	
 	
	/*
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL); // ���㴰��
	g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);	// ����λ����ʾ
	gtk_widget_set_size_request(window, 400, 300);		    // ������С��С
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);	// �̶����ڵĴ�С
 
	GtkWidget *table = gtk_table_new(5, 5, TRUE);	// ��񲼾�����
	gtk_container_add(GTK_CONTAINER(window), table); // �������봰��
 
	// button
	GtkWidget *button = gtk_button_new_with_label("click me");		// ��ť
	g_signal_connect(button, "clicked", G_CALLBACK(deal_button_clicked), window);
	gtk_table_attach_defaults(GTK_TABLE(table), button, 3, 4, 4, 5);// �Ѱ�ť���벼��
 
	// ��ͼ�¼��ź���ص�����������
	g_signal_connect(window, "event", G_CALLBACK(on_expose_event), NULL);
 
	gtk_widget_set_app_paintable(window, TRUE);	// �����ڿ��Ի�ͼ
	//*/
	
	//gtk_widget_hide(window);		 	
    gtk_widget_show_all(window);
    gtk_main();//����ÿ��GtkӦ�ó���Ҫ���õĺ�������������ͣ������ȴ��¼�(������¼�������¼�)�ķ������ȴ��û����������ڡ�
	return 0;
}

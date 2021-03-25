#include <gtk/gtk.h>       // gtk
#include <cairo.h>	// 绘图所需要的头文件 
#include "database.h"

 

extern int number_of_station,number_of_edge,free_travel_distance;
 

/************************************ 
	函数名：_utf8
	描述：将一个字符串转成utf8格式 
	输入参数： 一个字符串指针 
	输出参数：一个字符串指针（utf-8格式） 
	编程者：HUST IS-1901 李文重
	日期：2021.3.9
	状态 ：已完成，已检查  
//**********************************/ 
char *_utf8(char * str);

/************************************ 
	函数名：printt_nowwa_scheme
	描述：将当前状态输出到一个label窗口里，如果typ==1的话就输出当前站的换乘信息 
	注意：  字符串在加入到temporary_string的时候就要转成utf-8型 
	输入参数： typ 表示类型            默认调取nowwa_scheme 
				0 只输出当前位置等状态信息 
	输出参数：  输出到窗口里 
	编程者：HUST IS-1901 李文重
	日期：2021.3.11 
	状态 ：已完成，已检查  
//**********************************/ 
void printt_nowwa_scheme(int typ);

/************************************ 
	函数名：printt_string
	描述：将当前字符串输出到一个label窗口里
	输入参数： 一个字符串    此处我默认这个字符串已经被转为utf8格式 
	输出参数：  输出到窗口里 
	编程者：HUST IS-1901 李文重
	日期：2021.3.11 
	状态 ：已完成，已检查  
//**********************************/ 
void printt_string(char *nowwa);

/************************************ 
	函数名：Point_line_changed
	描述：当选择起、末车站的时候，首先需要选择线路，这个函数就是选择线路后的回调函数。 
	输入参数：  widget： 当前发生变化的控件 
				data ：将要修改的ComboBoxText 的指针 
	输出参数：使得data所指的 ComboBoxText变化为正确的结果（显示该条线路上的所有车站） 
	编程者：HUST IS-1901 李文重
	日期：2021.3.9
	状态 ：已完成，已检查  
//**********************************/ 
void Point_line_changed(GtkWidget *widget,gpointer data); 

/************************************ 
	函数名：change_time_window_affirm_clicked
	描述：当修改时间界面里面的确定按钮被点击之后，调用的回调函数。
			主要有两个功能，第一是调用 ComboBoxText中的数据，计算出时间，第二则是关闭（hide）修改时间页面 
	输入参数：  widget： 当前发生变化的控件 
				data ： 将要关闭的界面的指针 
	输出参数：修改nowwa_scheme.nowtime 并且隐藏界面 
	编程者：HUST IS-1901 李文重
	日期：2021.3.9
	状态 ：已完成，已检查  
//**********************************/ 
void change_time_window_affirm_clicked(GtkWidget *widget,gpointer data); 

/************************************ 
	函数名：Change_time_button_clicked
	描述：当修改时间被点击之后，调用的回调函数。
			主要有两个功能，第一是生成出修改时间的界面，包括界面生成和ComboBoxText中选项的添加，
							第二则是当确定被点击的时候确定调用回调函数 
	输入参数：  widget： 当前发生变化的控件 
				data ： 恒为NULL 
	输出参数：修改nowwa_scheme.nowtime 并且隐藏界面 
	编程者：HUST IS-1901 李文重
	日期：2021.3.9
	状态 ：已完成，已检查  
//**********************************/ 
void Change_time_button_clicked(GtkWidget *widget,gpointer data); 

/************************************ 
	函数名：Change_crowding_factor_clicked
	描述：当修改拥挤系数被点击之后，调用的回调函数。
			 
			 此处直接隐藏当前界面，然后让使用者按照数据格式，输入修改 
	
			主要有两个功能，第一先隐藏，后显示原界面，使得方便操作，第二则是通过命令行的方式接入修改命令，并且判断命令是否合法 
	输入参数：  widget： 当前发生变化的控件 
				data ： 主界面的指针window
				还有从命令行窗口读入的修改信息 
	输出参数：调用change_Crowdedness函数，完成对于的一定时间，一定线路的拥挤系数修改 
	编程者：HUST IS-1901 李文重
	日期：2021.3.9
	状态 ：已完成，已检查  
//**********************************/ 
void Change_crowding_factor_clicked(GtkWidget *widget,gpointer data);

/************************************ 
	函数名：Travel_at_line
	描述：当修某一个线路及方向被点击之后，调用的回调函数。
		  判断能否走，更新状态，输出当前站点的可走信息 
	输入参数：  widget： 当前发生变化的控件 
				data ：  作为int *指针存储着选择的线路的编号+Bidirectional_line_offset 
	输出参数：调用change_Crowdedness函数，完成对于的一定时间，一定线路的拥挤系数修改 
	编程者：HUST IS-1901 李文重
	日期：2021.3.9
	状态 ：已完成，已检查  
//**********************************/ 
void Travel_at_line(GtkWidget *widget,gpointer data); 

/************************************ 
	函数名：Change_crowding_factor_clicked
	描述：当修改自由遍历被点击之后，调用的回调函数。获得当前站点信息，弹出页面，链接按钮的回调函数 
	输入参数：  widget： 当前发生变化的控件 
				data ： 主界面的指针window 
	输出参数： 弹出选择按钮窗口和文本介绍窗口 
	编程者：HUST IS-1901 李文重
	日期：2021.3.9
	状态 ：已完成，已检查  
//**********************************/ 
void Free_travel(GtkWidget *widget,gpointer data); 

/************************************ 
	函数名：Find_way_type_clicked
	描述： 此界面显示的是所有支持的较优路线寻找方案 
			汇总全部的信息，并且初始化最初状态，然后调用find_shortest_path完成搜索
			
			而find_shortest_path中会调用print_the_path函数，这个函数会把要输出的字符串存到temporary_string（未转utf8）里面。
			因此继续调用printt_string(_utf8(temporary_string)); 
			 
	输入参数：  widget： 当前发生变化的控件 
				data ：  当前的type 
	输出参数： 找到并且输出最短路 
	编程者：HUST IS-1901 李文重
	日期：2021.3.11 
	状态 ：已完成，未检查 
//**********************************/ 
void Find_way_type_clicked(GtkWidget *widget,gpointer data); 

/************************************ 
	函数名：Find_way_button_clicked
	描述：当搜索路线被点击之后，调用的回调函数。根据后续的选择，进行路线推荐 
			主要有两个功能，第一先弹出一个新界面，在这里选择以何种标准运行最短路，第二是根据按的按钮不同，给回调函数传入适当的参数 
	输入参数：  widget： 当前发生变化的控件 
				data ： 恒为NULL 
	输出参数： 无 
	编程者：HUST IS-1901 李文重
	日期：2021.3.9
	状态 ：已完成，已检查  
//**********************************/ 
void Find_way_button_clicked(GtkWidget *widget,gpointer data); 


/************************************ 
	函数名：Drawing_orign_map
	描述： 将./data/orignal_map.png这个文件加载到 Display_map里面 
	输入参数：  widget： 当前发生变化的控件 
				data ： 恒为NULL 
	输出参数： 无 
	编程者：HUST IS-1901 李文重
	日期：2021.3.9
	状态 ：已完成，已检查  
//**********************************/ 
void Drawing_orign_map(GtkWidget *widget,gpointer data);

/************************************ 
	函数名：Drawing_crowded_map
	描述： 将拥挤度线图画到./data/crowded_map.png 
				再把./data/crowded_map.png这个文件加载到 Display_map里面 
	输入参数：  widget： 当前发生变化的控件 
				data ： 恒为NULL 
	输出参数： 无 
	编程者：HUST IS-1901 李文重
	日期：2021.3.11
	状态 ：已完成，已检查  
//**********************************/ 
void Drawing_crowded_map(GtkWidget *widget,gpointer data);

/************************************ 
	函数名：printt_nowway_map
	描述： 遍历存储在这个位置的路线，并且画图到./data/nowway_map.png ，最后加载图片 
	输入参数：  widget： 当前发生变化的控件 
				data ： （int *）表示选择的路线的id 
	输出参数： 无 
	编程者：HUST IS-1901 李文重
	日期：2021.3.11
	状态 ：已完成，已检查  
//**********************************/ 
void printt_nowway_map(int pls);

/************************************ 
	函数名：to_print_one_type
	描述： 当点击显示当前路径中的某一条路径时的回调函数，会弹出一个选择窗口 
			如果没有存储路线 就输出错误信息
			否则 调用 printt_nowway_map(int pls)完成画图 
	输入参数：  widget： 当前发生变化的控件 
				data ： （int *）表示选择的路线的id 
	输出参数： 无 
	编程者：HUST IS-1901 李文重
	日期：2021.3.11
	状态 ：已完成，已检查  
//**********************************/ 
void to_print_one_type(GtkWidget *widget,gpointer data); 

/************************************ 
	函数名：Drawing_crowded_map
	描述： 当点击显示当前路径时的回调函数，会弹出一个选择窗口 
				并链接四个按钮的回调函数 
	输入参数：  widget： 当前发生变化的控件 
				data ： 恒为NULL 
	输出参数： 无 
	编程者：HUST IS-1901 李文重
	日期：2021.3.11
	状态 ：已完成，已检查  
//**********************************/ 
void Drawing_nowway_map(GtkWidget *widget,gpointer data); 


/************************************ 
	函数名：gtk_gui
	描述：整个gtk内核的gui系统的中心函数，实际上起到了main的作用。 
	输入参数：  argc 是命令行在执行程序 加入的参数 （实际上是第二个参数的大小） 
				argv 是命令行在执行程序 加入的指针
	输出参数：无
	编程者：HUST IS-1901 李文重
	日期：2021.3.9
	状态 ：未完成，已检查  
//**********************************/ 
int gtk_gui(int argc , char **argv);

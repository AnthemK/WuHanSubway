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
/*

//*/ 

void callback(GtkButton *button,gpointer user_data){
		char *text = gtk_button_get_label(button);
 
        printf("%s----%s\n",(char *)user_data, text );
 
        return;

} 


int main(int argc , char **argv)
{
    gtk_init (&argc, &argv);   //所有 GTK应用程序都要调用该函数，而且必须在控件定义之前使用，它为我们设置一些缺省值( 例如视觉和颜色 )映射这个函数将函数库初始化，设置缺省的信号处理函数，并检查通过命令行传递给应用程序的参数，自动完成一些必要的初始化工作。 
    
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "WuHanSubway");
 	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
 	gtk_widget_set_size_request(window, 1600,1200);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
 	
 
 //	GtkWidget *table = gtk_table_new(2,2,TRUE);
 //	gtk_container_add(GTK_CONTAINER(window),table);
 	
 	GtkWidget *fixed = gtk_fixed_new();
 	gtk_container_add(GTK_CONTAINER(window),fixed);
 	
 	GtkWidget *entry = gtk_entry_new();
         //设置行编辑的内容
	gtk_entry_set_text(GTK_ENTRY(entry), "1+1=4");
         //设置行编辑不允许编辑，只能显示用
 	gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
 	
 	gtk_fixed_put(GTK_FIXED(fixed),entry,200,200); 
 	
        //3.创建一个button，文本信息为Hello gtk!
    GtkWidget *button1 = gtk_button_new_with_label("Hello gtk?");
        //4.将button放入窗口中（窗口也是一种容器）
    gtk_fixed_put(GTK_FIXED(fixed), button1, 0,0);
    g_signal_connect(button1, "pressed",G_CALLBACK(callback), "My name is");
    
    
    
    GtkWidget *button2 = gtk_button_new_with_label("Hello gtk!");
 
        //4.将button放入窗口中（窗口也是一种容器）
    gtk_fixed_put(GTK_FIXED(fixed), button2, 100,50);
 
        //5.显示button
    gtk_widget_show_all(window);
    gtk_main();//是在每个Gtk应用程序都要调用的函数。程序运行停在这里等待事件(如键盘事件或鼠标事件)的发生，等待用户来操作窗口。
	return 0;
}

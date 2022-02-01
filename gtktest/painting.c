#include <cairo.h>	// 绘图所需要的头文件
#include <gtk/gtk.h>
#include <stdio.h>
 
int startx = 0;
int w = 400;
int h = 300;
 
// 绘图事件
gboolean on_expose_event (GtkWidget * widget, GdkEventExpose *event, gpointer data)
{
	printf("1111");
	GdkWindow * gdk_window1 = gtk_widget_get_window (widget);
	cairo_t *cr = gdk_cairo_create(gdk_window1);	// 创建cairo环境，注意参数
 
	// 画背景图
	// 获取图片
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file("./image/back.jpg", NULL); 
	// 指定图片大小
	GdkPixbuf* dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR);
	
	// dst_pixbuf作为cr环境的画图原材料，(0, 0)：画图的起点坐标
	gdk_cairo_set_source_pixbuf(cr, dst_pixbuf, 0, 0);
	cairo_paint(cr);	// 绘图
	
	// 释放资源
	g_object_unref(dst_pixbuf);
	g_object_unref(src_pixbuf);
 
	// 画笑脸
	src_pixbuf = gdk_pixbuf_new_from_file("./image/head.jpg", NULL);
	dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, 80, 80, GDK_INTERP_BILINEAR);
	gdk_cairo_set_source_pixbuf(cr, dst_pixbuf, startx, (h/10)*3);
	cairo_paint(cr);
	g_object_unref(dst_pixbuf);
	g_object_unref(src_pixbuf);
 
	
	// 绘图与写字共存的测试
	// 如果绘完图片后想继续写字或画线，
	// 必须手动设置画笔颜色cairo_set_source_rgb()
	// 否则，字体或线条会被图片覆盖。
	cairo_set_source_rgb(cr, 0.627, 0, 0);  // 设置字体颜色
	cairo_set_font_size(cr, 40.0);			// 设置字体大小
	cairo_move_to(cr, 50.0, 130.0);			// 写字的起点坐标
	cairo_show_text(cr, "This is a test");	// 写字
 
	cairo_destroy(cr);	// 回收所有Cairo环境所占用的内存资源
 
	return FALSE;	// 必须返回FALSE
}
 
// 按钮按下回调函数
void deal_button_clicked(GtkWidget *widget, gpointer data)
{
	startx += 20;
	if(startx >= w){
		startx = 0;
	}
 
	gtk_widget_queue_draw( GTK_WIDGET(data) );	// 更新刷图区域，刷新整个窗口
}
 
int main (int argc, char *argv[])
{
	gtk_init (&argc, &argv);
 
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL); // 顶层窗口
	g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);	// 中央位置显示
	gtk_widget_set_size_request(window, 400, 300);		    // 窗口最小大小
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);	// 固定窗口的大小
 
	GtkWidget *table = gtk_table_new(5, 5, TRUE);	// 表格布局容器
	gtk_container_add(GTK_CONTAINER(window), table); // 容器加入窗口
 
	// button
	GtkWidget *button = gtk_button_new_with_label("click me");		// 按钮
	g_signal_connect(button, "clicked", G_CALLBACK(deal_button_clicked), window);
	gtk_table_attach_defaults(GTK_TABLE(table), button, 3, 4, 4, 5);// 把按钮加入布局
 
	// 绘图事件信号与回调函数的连接
	g_signal_connect(window, "event", G_CALLBACK(on_expose_event), NULL);
 
	gtk_widget_set_app_paintable(window, TRUE);	// 允许窗口可以绘图
 
	gtk_widget_show_all(window);	// 显示所有控件
 
	gtk_main();
	
	return 0;
}

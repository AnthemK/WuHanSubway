#include <cairo.h>	// ��ͼ����Ҫ��ͷ�ļ�
#include <gtk/gtk.h>
#include <stdio.h>
 
int startx = 0;
int w = 400;
int h = 300;
 
// ��ͼ�¼�
gboolean on_expose_event (GtkWidget * widget, GdkEventExpose *event, gpointer data)
{
	printf("1111");
	GdkWindow * gdk_window1 = gtk_widget_get_window (widget);
	cairo_t *cr = gdk_cairo_create(gdk_window1);	// ����cairo������ע�����
 
	// ������ͼ
	// ��ȡͼƬ
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file("./image/back.jpg", NULL); 
	// ָ��ͼƬ��С
	GdkPixbuf* dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR);
	
	// dst_pixbuf��Ϊcr�����Ļ�ͼԭ���ϣ�(0, 0)����ͼ���������
	gdk_cairo_set_source_pixbuf(cr, dst_pixbuf, 0, 0);
	cairo_paint(cr);	// ��ͼ
	
	// �ͷ���Դ
	g_object_unref(dst_pixbuf);
	g_object_unref(src_pixbuf);
 
	// ��Ц��
	src_pixbuf = gdk_pixbuf_new_from_file("./image/head.jpg", NULL);
	dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, 80, 80, GDK_INTERP_BILINEAR);
	gdk_cairo_set_source_pixbuf(cr, dst_pixbuf, startx, (h/10)*3);
	cairo_paint(cr);
	g_object_unref(dst_pixbuf);
	g_object_unref(src_pixbuf);
 
	
	// ��ͼ��д�ֹ���Ĳ���
	// �������ͼƬ�������д�ֻ��ߣ�
	// �����ֶ����û�����ɫcairo_set_source_rgb()
	// ��������������ᱻͼƬ���ǡ�
	cairo_set_source_rgb(cr, 0.627, 0, 0);  // ����������ɫ
	cairo_set_font_size(cr, 40.0);			// ���������С
	cairo_move_to(cr, 50.0, 130.0);			// д�ֵ��������
	cairo_show_text(cr, "This is a test");	// д��
 
	cairo_destroy(cr);	// ��������Cairo������ռ�õ��ڴ���Դ
 
	return FALSE;	// ���뷵��FALSE
}
 
// ��ť���»ص�����
void deal_button_clicked(GtkWidget *widget, gpointer data)
{
	startx += 20;
	if(startx >= w){
		startx = 0;
	}
 
	gtk_widget_queue_draw( GTK_WIDGET(data) );	// ����ˢͼ����ˢ����������
}
 
int main (int argc, char *argv[])
{
	gtk_init (&argc, &argv);
 
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
 
	gtk_widget_show_all(window);	// ��ʾ���пؼ�
 
	gtk_main();
	
	return 0;
}

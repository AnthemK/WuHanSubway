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
/*

//*/ 

void callback(GtkButton *button,gpointer user_data){
		char *text = gtk_button_get_label(button);
 
        printf("%s----%s\n",(char *)user_data, text );
 
        return;

} 


int main(int argc , char **argv)
{
    gtk_init (&argc, &argv);   //���� GTKӦ�ó���Ҫ���øú��������ұ����ڿؼ�����֮ǰʹ�ã���Ϊ��������һЩȱʡֵ( �����Ӿ�����ɫ )ӳ������������������ʼ��������ȱʡ���źŴ������������ͨ�������д��ݸ�Ӧ�ó���Ĳ������Զ����һЩ��Ҫ�ĳ�ʼ�������� 
    
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
         //�����б༭������
	gtk_entry_set_text(GTK_ENTRY(entry), "1+1=4");
         //�����б༭������༭��ֻ����ʾ��
 	gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
 	
 	gtk_fixed_put(GTK_FIXED(fixed),entry,200,200); 
 	
        //3.����һ��button���ı���ϢΪHello gtk!
    GtkWidget *button1 = gtk_button_new_with_label("Hello gtk?");
        //4.��button���봰���У�����Ҳ��һ��������
    gtk_fixed_put(GTK_FIXED(fixed), button1, 0,0);
    g_signal_connect(button1, "pressed",G_CALLBACK(callback), "My name is");
    
    
    
    GtkWidget *button2 = gtk_button_new_with_label("Hello gtk!");
 
        //4.��button���봰���У�����Ҳ��һ��������
    gtk_fixed_put(GTK_FIXED(fixed), button2, 100,50);
 
        //5.��ʾbutton
    gtk_widget_show_all(window);
    gtk_main();//����ÿ��GtkӦ�ó���Ҫ���õĺ�������������ͣ������ȴ��¼�(������¼�������¼�)�ķ������ȴ��û����������ڡ�
	return 0;
}

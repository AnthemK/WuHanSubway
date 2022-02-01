#include <gtk/gtk.h>
 #include <stdio.h>
 #include <string.h>
 
char buf[50] = "";
 
void deal_num(GtkButton *button, gpointer data)
{
	const char *text = gtk_button_get_label(button);
 
	//�˸����
	if (0 == strcmp(text, "Del")) {
			buf[strlen(buf) - 1] = 0;
	}
	else if(0 == strcmp(text, "c")){
			buf[0]=0;
	}
	else
	{
		int a = 0, b = 0;
		char c;
		strcat(buf,text);
 
		if (0 == strcmp("=", text)) {
			printf("text==##%s##\n",text);
			sscanf(buf, "%d%c%d", &a, &c, &b);
 
			printf("---------001-----%c--\n",c);
			if ('+' == c) {
					sprintf(buf,"%d", a+b);
			}
			else if ('-' == c) {
					sprintf(buf, "%d", a-b);
			}
			else if ('*' == c) {
					sprintf(buf, "%d", a*b);
			}
			else if ('/' == c) {
					sprintf(buf, "%g", (double )a/(double )b);
			}
 
		}
	}
 
	gtk_entry_set_text(GTK_ENTRY(data), buf);
 
	return;
}
 
 int main(int argc,char *argv[])
 {
 	// printf("%s","��������"); 
 	
	 //1.gtk������ʼ��
	 gtk_init(&argc, &argv);
 
	 //2.����һ������
	 GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	 g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	 
 	 gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
 	 gtk_widget_set_size_request(window, 400,300);
	 gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	 //3.����һ���������5��4��
	 GtkWidget *table = gtk_table_new(5,4,TRUE);
	 //��table���뵽window��
	 gtk_container_add(GTK_CONTAINER(window), table);
 
	 //4.����һ���б༭
	 GtkWidget *entry = gtk_entry_new();
	 //�����б༭������
	 gtk_entry_set_text(GTK_ENTRY(entry), "");
	 //�����б༭������༭��ֻ����ʾ��
	 gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
 
	 //5.���������ť
	 GtkWidget *button0 = gtk_button_new_with_label("0");//��ֵ��0
	 GtkWidget *button1 = gtk_button_new_with_label("1");//��ֵ��1
	 GtkWidget *button2 = gtk_button_new_with_label("2");//��ֵ��2
	 GtkWidget *button3 = gtk_button_new_with_label("3");//��ֵ��3
	 GtkWidget *button4 = gtk_button_new_with_label("4");//��ֵ��4
	 GtkWidget *button5 = gtk_button_new_with_label("5");//��ֵ��5
	 GtkWidget *button6 = gtk_button_new_with_label("6");//��ֵ��6
	 GtkWidget *button7 = gtk_button_new_with_label("7");//��ֵ��7
	 GtkWidget *button8 = gtk_button_new_with_label("8");//��ֵ��8
	 GtkWidget *button9 = gtk_button_new_with_label("9");//��ֵ��9
 
	 GtkWidget *button_add = gtk_button_new_with_label("+");//�Ӻ�
	 GtkWidget *button_minus = gtk_button_new_with_label("-");//����
	 GtkWidget *button_mul = gtk_button_new_with_label("*");//�˺�
	 GtkWidget *button_div = gtk_button_new_with_label("/");//����
	 GtkWidget *button_equal = gtk_button_new_with_label("=");//�Ⱥ�
	 GtkWidget *button_delete = gtk_button_new_with_label("Del");//�˸��
 	 GtkWidget *button_clear = gtk_button_new_with_label("c");
	 //6.���ֽ�����İ�ť������table������
	 gtk_table_attach_defaults(GTK_TABLE(table), entry, 0, 3, 0, 1);
 
	 gtk_table_attach_defaults(GTK_TABLE(table), button0, 0, 1, 4, 5);
	 gtk_table_attach_defaults(GTK_TABLE(table), button1, 0, 1, 3, 4);
	 gtk_table_attach_defaults(GTK_TABLE(table), button2, 1, 2, 3, 4);
	 gtk_table_attach_defaults(GTK_TABLE(table), button3, 2, 3, 3, 4);
	 gtk_table_attach_defaults(GTK_TABLE(table), button4, 0, 1, 2, 3);
	 gtk_table_attach_defaults(GTK_TABLE(table), button5, 1, 2, 2, 3);
	 gtk_table_attach_defaults(GTK_TABLE(table), button6, 2, 3, 2, 3);
	 gtk_table_attach_defaults(GTK_TABLE(table), button7, 0, 1, 1, 2);
	 gtk_table_attach_defaults(GTK_TABLE(table), button8, 1, 2, 1, 2);
	 gtk_table_attach_defaults(GTK_TABLE(table), button9, 2, 3, 1, 2);
 
	 gtk_table_attach_defaults(GTK_TABLE(table), button_add, 1, 2, 4, 5);
	 gtk_table_attach_defaults(GTK_TABLE(table), button_minus, 2, 3, 4, 5);
	 gtk_table_attach_defaults(GTK_TABLE(table), button_mul , 3, 4, 2, 3);
	 gtk_table_attach_defaults(GTK_TABLE(table), button_div, 3, 4, 3, 4);
	 gtk_table_attach_defaults(GTK_TABLE(table), button_equal, 3, 4, 4, 5);
	 gtk_table_attach_defaults(GTK_TABLE(table), button_delete, 3, 4, 1, 2);
 	 gtk_table_attach_defaults(GTK_TABLE(table), button_clear, 3, 4, 0, 1);
	 //7.ע���źź�������entry�����ص�����deal_num()
	 g_signal_connect(button0, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button1, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button2, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button3, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button4, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button5, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button6, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button7, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button8, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button9, "pressed", G_CALLBACK(deal_num), entry);
 
	 g_signal_connect(button_add, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button_mul, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button_div, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button_minus, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button_equal, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button_delete, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button_clear, "pressed", G_CALLBACK(deal_num), entry); 
	 //7.��ʾ���пؼ�
	 gtk_widget_show_all(window);
 
	 //8.���¼�ѭ��
	 gtk_main();
 
	 return 0;
 }


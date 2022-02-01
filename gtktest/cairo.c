#include <cairo.h>
 
#define SIZE	(640)		/* ̫��ͼ��С */
#define CX	(SIZE / 2)	/* ̫��Բ���ĺ����� */
#define CY	(SIZE / 2)	/* ̫��Բ���������� */
#define R	(SIZE / 2)	/* ̫��Բ�뾶 */
 
/* �ѽǶ�ת��Ϊ����Ӧ�Ļ��� */
#define ANGLE(ang)	(ang * 3.1415926 / 180.0)
 
int main(int argc, char *argv[])
{
	/* ����32λRGBA��ɫ��ʽ��Cairo��ͼ������ֱ����Memory����Ⱦ */
	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, SIZE + 1, SIZE + 1);
	cairo_t* cr = cairo_create(surface);
 
	/* ����̫���߿� */
	cairo_set_line_width(cr, 2.0);
	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	cairo_arc(cr, CX, CY, R, ANGLE(0), ANGLE(360));
	cairo_stroke(cr);
 
	/* ��������Բ */
	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	cairo_arc(cr, CX, CY, R, ANGLE(90), ANGLE(270));
	cairo_fill(cr);
	cairo_set_source_rgba(cr, 1, 1, 1, 1);
	cairo_arc(cr, CX, CY, R, ANGLE(-90), ANGLE(90));
	cairo_fill(cr);
 
	/* ���������� */
	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	cairo_arc(cr, CX, CY - R / 2, R / 2, ANGLE(-90), ANGLE(90));
	cairo_fill(cr);
	cairo_set_source_rgba(cr, 1, 1, 1, 1);
	cairo_arc(cr, CX, CY + R / 2, R / 2, ANGLE(90), ANGLE(270));
	cairo_fill(cr);
 
	/* ����̫���� */
	cairo_set_source_rgba(cr, 1, 1, 1, 1);
	cairo_arc(cr, CX, CY - R / 2, R / 10, ANGLE(0), ANGLE(360));
	cairo_fill(cr);
	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	cairo_arc(cr, CX, CY + R / 2, R / 10, ANGLE(0), ANGLE(360));
	cairo_fill(cr);
 
	/* ��Memory����ȾЧ���洢��ͼƬ�� */
	cairo_surface_write_to_png(surface, "taichi.png");
	/* ���ٲ��˳�Cairo��ͼ���� */
	cairo_destroy(cr);
	cairo_surface_destroy (surface);
 
	return 0;
}

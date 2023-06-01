#include "drawingboard_v1.h"
#include <QMouseEvent>
#include <QPainter>

drawingboard_v1::drawingboard_v1(QWidget* parent)
	: QWidget(parent)
{
	setFixedSize(400, 400);
}

void drawingboard_v1::mouseMoveEvent(QMouseEvent* event)
{
	// �����˻�ͼ������ֱ��return
	if (event->x() < 0 || event->x() > 400 || event->y() < 0 || event->y() > 400)
		return;
	// �ƶ����̣���¼����λ��
	m_vPoint.push_back(QPoint(event->x(), event->y()));

	update();
}

void drawingboard_v1::mousePressEvent(QMouseEvent* event)
{
	if (event->x() < 0 || event->x() > 400 || event->y() < 0 || event->y() > 400)
		return;
	
	m_vPoint.push_back(QPoint(event->x(), event->y()));
}

void drawingboard_v1::paintEvent(QPaintEvent* event)
{
	QPainter painter(this); // this���������

	QPen pen;
	pen.setWidth(10); // ���ñʵĿ��Ϊ10
	pen.setColor(Qt::red); // ���ñʵ���ɫΪ��ɫ

	painter.setPen(pen);

	painter.drawPoints(QPolygon(m_vPoint));

}


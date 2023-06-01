#pragma once

#include <Qwidget>
#include <QVector>

// �½�һ���࣬�̳��ڻ��� QWidget
class drawingboard_v1 : public QWidget
{
	Q_OBJECT

public:
	drawingboard_v1(QWidget* parent = nullptr);

protected:
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

private:
	QVector<QPoint> m_vPoint;
};
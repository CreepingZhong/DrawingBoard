# include <QApplication>

# include "drawingboard_v1.h"

// main�����ı�׼��ʽ
int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	drawingboard_v1 drawingboard;
	drawingboard.show();

	return app.exec();
}

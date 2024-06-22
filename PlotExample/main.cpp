#include <QtWidgets/QApplication>

#include "ChartWindow.h"

int main(int argc, char* argv[])
{
	QApplication main_loop(argc, argv);
	Chart::ChartWindow window;
	window.show();
	return main_loop.exec();
}
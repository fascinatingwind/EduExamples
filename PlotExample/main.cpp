#include <QtWidgets/QApplication>

#include "MainWindow.h"

int main(int argc, char* argv[])
{
	QApplication main_loop(argc, argv);
	Strata::MainWindow window;
	window.show();
	return main_loop.exec();
}
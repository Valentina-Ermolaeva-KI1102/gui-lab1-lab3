#include "kr4.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	kr4 w;
	w.show();
	return a.exec();
}

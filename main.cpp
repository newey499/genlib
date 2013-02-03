
/*******************
	Console
#include <QCoreApplication>


int main(int argc, char *argv[])
{
	QCoreApplication(argc, argv );

	return 0;
}
*****************************/

/****************************
GUI
*****************************/
#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
		QApplication a(argc, argv);
		MainWindow w;
        w.show();

        return a.exec();
}



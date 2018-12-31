#include "mainwindow.h"
#include <QApplication>
#include "paintwidget.h"
int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;
//   w.setFixedSize(width,height+50);
   w.show();

   return a.exec();
}

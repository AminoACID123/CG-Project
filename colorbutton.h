#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QWidget>
#include <QPushButton>
//#include "mainwindow.h"
#include <QWidget>
#include <Qcolor>
#include <QImage>
#include <QPen>
#include <QPainter>
#define HEIGHT 32
#define WIDTH 32

class ColorButton:public QPushButton{
    Q_OBJECT

public:
    ColorButton(QWidget *parent = 0,QColor c=Qt::black);

protected:
    QColor color=Qt::black;
    friend class MainWindow;
    void paintEvent(QPaintEvent *event);
};

#endif // COLORBUTTON_H

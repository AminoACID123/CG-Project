#include "colorbutton.h"


ColorButton::ColorButton(QWidget *parent,QColor c):QPushButton(parent){

    resize(HEIGHT,WIDTH);
    QPainter p(this);
    QImage *image = new QImage(WIDTH,HEIGHT,QImage::Format_RGB888);
    image->fill(c);
    p.drawImage(0,0,*image);
    color=c;
}

void ColorButton::paintEvent(QPaintEvent *event){
    QPainter p(this);
    QImage *image = new QImage(WIDTH,HEIGHT,QImage::Format_RGB888);
    image->fill(color);
    p.drawImage(0,0,*image);
}

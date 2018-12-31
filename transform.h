#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Action.h"
#include "Draw.h"

class Transform:public Action{
private:
    QPoint rotatePoint;
    QPoint origin;
    Draw* shapeToTrans;
    Transform* copy=0;
    QPoint transPoint[8];
    int currentTransformPoint;
    double mul[2][2]={{1,0},{0,1}};
    double add[2][1]={{0},{0}};
    double mul_temp[2][2];
    double add_temp[2][1];
    double angle=0;
public:
    QPoint moveStart;
    enum type{STRETCH,MOVE,ROTATE};
    type code;
    Transform(Draw *s);
    Transform(const Transform& t);
    void paint(QPainter &painter);
    bool isInArea(QPoint p);
    bool isAtTransformPoint(QPoint p);
    bool isAtRotatePoint(QPoint p);
    int getCurrentTransformPoint(){return currentTransformPoint;}
    Draw* getShape(){return shapeToTrans;}
    void stretch(QPoint p,bool transforming,bool);
    void move(QPoint p,bool transforming);
    void rotate(QPoint p,bool transforming);

};

#endif // TRANSFORM_H

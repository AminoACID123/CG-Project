#ifndef CLIP_H
#define CLIP_H

#include "Action.h"
#include "Draw.h"
class Clip:public Action{
public:
    enum Location{upleft,upright,downleft,downright};
    Clip(DrawLine* l){
        line=l;
    }
    QPoint moveStart;
    QPoint start;
    QPoint end;
    void paint(QPainter &painter);
    void setStart(QPoint p){start=p;}
    void setEnd(QPoint p){end=p;}
    bool isInArea(QPoint p);
    bool isAtPoint(QPoint p);
    void setPoint(QPoint p);
    void moveTo(QPoint p);
    bool clip();
    DrawLine* line;
    int currentPoint;
};

#endif // CLIP_H

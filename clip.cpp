#include "clip.h"


/*    enum Location{upleft,upright,downleft,downright};
    Clip(DrawLine* l){
        line=l;
    }
    QPoint start;
    QPoint end;
    void paint(QPainter &painter);
    void setStart(QPoint& p){start=p;}
    void setEnd(QPoint &p){end=p;}
    bool isInArea(QPoint &p);
    bool isAtPoint(QPoint &p);
    void setPoint(QPoint &p);
    void clip();
    DrawLine* line;
    int currentPoint;
    */
bool Clip:: isInArea(QPoint p){
    return p.x()>qMin(start.x(),end.x()) && p.x()<qMax(start.x(),end.x())
            &&p.y()>qMin(start.y(),end.y()) && p.y()<qMax(start.y(),end.y());
}

bool Clip::isAtPoint(QPoint p){
    if(abs(p.x()-start.x())+abs(p.y()-start.y())<=5){
        currentPoint=0;
        return true;
    }
    else if(abs(p.x()-end.x())+abs(p.y()-end.y())<=5){
        currentPoint=1;
        return true;
    }
    else return false;
}

void Clip::setPoint(QPoint p){
    if(currentPoint==0)
        start=p;
    else
        end=p;
}

void Clip::moveTo(QPoint p){
    start+=(p-moveStart);
    end+=(p-moveStart);
    moveStart=p;
}


bool Clip::clip(){
    QPoint p1=line->cpt[0];
    QPoint p2=line->cpt[1];

    if(isInArea(p1)&&isInArea(p2))return true;

    QPoint lp=line->cpt[0].x()<line->cpt[1].x()?line->cpt[0]:line->cpt[1];
    QPoint rp=line->cpt[0].x()>=line->cpt[1].x()?line->cpt[0]:line->cpt[1];

    int maxX=qMax(start.x(),end.x());
    int maxY=qMax(start.y(),end.y());
    int minX=qMin(start.x(),end.x());
    int minY=qMin(start.y(),end.y());


    if(qMax(lp.x(),minX)>qMin(rp.x(),maxX))
        return false;


    if(lp.x()!=rp.x()){
        double k=(double)(lp.y()-rp.y())/(double)(lp.x()-rp.x());
        if(minX>lp.x()&&minX<rp.x()){
            lp.setY(lp.y()+k*(minX-lp.x()));
            lp.setX(minX);

        }
        else if(minX>rp.x()){
            return false;
        }
        if(maxX>lp.x()&&maxX<rp.x()){
            rp.setY(rp.y()+k*(maxX-rp.x()));
            rp.setX(maxX);

        }
        else if(maxX<lp.x()){
            return false;
        }
    }
    else{
        if(minY>qMax(lp.y(),rp.y()) || maxY<qMin(lp.y(),rp.y()))
            return false;
    }
    QPoint dp=lp.y()<rp.y()?lp:rp;
    QPoint up=lp.y()>=rp.y()?lp:rp;

    if(up.y()!=dp.y()){
        double k=(double)(rp.x()-lp.x())/(double)(rp.y()-lp.y());
        if(minY>dp.y()&&minY<up.y()){
            dp.setX(dp.x()+k*(minY-dp.y()));
            dp.setY(minY);

        }
        else if(minY>up.y()){
            return false;
        }
        if(maxY>dp.y()&&maxY<up.y()){
            up.setX(up.x()+k*(maxY-up.y()));
            up.setY(maxY);

        }
        else if(maxY<dp.y()){
            return false;
        }
    }
    else{
        if(minX>qMax(lp.x(),rp.x()) || maxX<qMin(lp.x(),rp.x()))
            return false;
    }
    line->cpt[0]=up;
    line->cpt[1]=dp;
    return true;

}

void Clip:: paint(QPainter &painter){
    QPen tmp=painter.pen();
    QPen pen;
    pen.setBrush(QBrush(Qt::black));
    QVector<qreal> dashes;
    qreal space = 3;
    dashes << 5 << space << 5 <<space;
    pen.setDashPattern(dashes);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    int maxX=qMax(start.x(),end.x());
    int maxY=qMax(start.y(),end.y());
    int minX=qMin(start.x(),end.x());
    int minY=qMin(start.y(),end.y());

    painter.drawRect(minX,minY,maxX-minX,maxY-minY);
    QPen *p=new QPen(Qt::red, 1,Qt::SolidLine, Qt::RoundCap);
    painter.setPen(*p);
    painter.setBrush(Qt::white);
    painter.drawEllipse(start,5,5);
    painter.drawEllipse(end,5,5);
    painter.setPen(tmp);
}

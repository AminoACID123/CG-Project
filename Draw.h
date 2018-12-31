#ifndef SHAPE_H
#define SHAPE_H

#include "Action.h"
#define pi 3.141592653589




class Draw:public Action{
protected:
    bool special;
public:
    double mul[2][2]={{1,0},{0,1}};
    double add[2][1]={{0},{0}};
    QPoint center;
    double angle;
    QVector<QPoint> cpt;
    int currentCpt;
    void paintline(QPainter &painter,QPoint a,QPoint b);
    void DrawPoint(QPainter& painter,int x,int y);
    int upperbound_x=0;
    int upperbound_y=0;
    int lowerbound_x=20000;
    int lowerbound_y=20000;
    double coef[2][2]={{1,0},{0,1}};
    Draw(){
        QPoint *p=new QPoint(0,0);
        cpt.insert(cpt.begin(),2,*p);
        center.setX(0);
        center.setY(0);
        angle=0;
    }
    Draw(const Draw& d){
        this->cpt=d.cpt;
    }
    int type;
    virtual void paint(QPainter &painter,bool flag){}
    void clear(){
        cpt.clear();
        QPoint *p=new QPoint(0,0);
        cpt.insert(cpt.begin(),2,*p);
    }
    void setStart(QPoint s){
        cpt[0]=s;
    }
    void addEnd(QPoint e){
        cpt.push_back(e);
    }
    void setEnd(QPoint e,bool standardized=false){
        if(!standardized){
            cpt[cpt.size()-1]=e;
            return;
        }
        int len=abs(cpt[0].x()-e.x())<abs(cpt[0].y()-e.y())?abs(cpt[0].x()-e.x()):abs(cpt[0].y()-e.y());
        int xstep,ystep;
        if(e.x()>cpt[0].x())
            xstep=1;
        else if(e.x()<cpt[0].x())
            xstep=-1;
        else
            xstep=0;
        if(e.y()>cpt[0].y())
            ystep=1;
        else if(e.y()<cpt[0].y())

            ystep=-1;
        else
            ystep=0;
        cpt[cpt.size()-1].setX(cpt[0].x()+xstep*len);
        cpt[cpt.size()-1].setY(cpt[0].y()+ystep*len);


    }
    void deleteEnd(){
        cpt.pop_back();
    }
    bool isAtControlPoint(QPoint p){
        for(int i=0;i<cpt.size();i++){
            if(abs(cpt[i].x()-p.x())+abs(cpt[i].y()-p.y())<=5){
                currentCpt=i;
                return true;
            }
        }
        return false;
    }
    void setControlPoint(QPoint p){
        cpt[currentCpt]=p;
    }
    QVector<QPoint>& getCpt(){
        return cpt;
    }
    void adjust(QVector<QPoint>& original){
        double a=qMax(abs(mul[0][0]),abs(mul[1][0]));
        double b=qMax(abs(mul[0][1]),abs(mul[1][1]));
        double t=qMax(a,b);

        coef[0][0]=mul[0][0]/t;
        coef[0][1]=mul[0][1]/t;
        coef[1][0]=mul[1][0]/t;
        coef[1][1]=mul[1][1]/t;


        if(special)
            for(int i=0;i<cpt.size();i++){
                cpt[i].setX(original[i].x()*t);
                cpt[i].setY(original[i].y()*t);
            }
        else
            for(int i=0;i<cpt.size();i++){
                cpt[i].setX(original[i].x()*mul[0][0]+original[i].y()*mul[0][1]+add[0][0]);
                cpt[i].setY(original[i].x()*mul[1][0]+original[i].y()*mul[1][1]+add[1][0]);
            }

    }
};

class DrawLine:public Draw{
public:
    DrawLine(){special=false;}
    void paint(QPainter &painter,bool flag);
};

class DrawRect:public Draw{
public:
    DrawRect(){special=true;}
    void paint(QPainter &painter,bool flag);
};

class DrawEllipse:public Draw{
public:
    DrawEllipse(){special=true;}
    void paint(QPainter &painter,bool flag);
};




class AreaFill:public Draw{
public:
    AreaFill(){}
    void paint(QImage &image,QColor color);
};

class DrawPolyGon:public Draw{
private:
    bool finished;
public:
    DrawPolyGon(){finished=false;special=false;}
    void complete(){finished=true;}
    void paint(QPainter &painter,bool flag);


};

class DrawCurve:public Draw{
private:
    bool finished;
public:
    DrawCurve(){finished=false;special=true;}
    void complete(){finished=true;}
    void paint(QPainter &painter,bool flag);


};


#endif

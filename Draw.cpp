#include "Draw.h"

int findMaxX(QVector<QPoint> p){
    int max=-1;
    for(int i=0;i<p.size();i++){
        if(p[i].x()>max)
            max=p[i].x();
    }
    return max;
}


int findMinX(QVector<QPoint> p){
    int min=20000;
    for(int i=0;i<p.size();i++){
        if(p[i].x()<min)
            min=p[i].x();
    }
    return min;
}


int findMaxY(QVector<QPoint> p){
    int max=-1;
    for(int i=0;i<p.size();i++){
        if(p[i].y()>max)
            max=p[i].y();
    }
    return max;
}

int findMinY(QVector<QPoint> p){
    int min=20000;
    for(int i=0;i<p.size();i++){
        if(p[i].y()<min)
            min=p[i].y();
    }
    return min;
}

void Draw::DrawPoint(QPainter &painter,int x,int y){
    /*
    double SIN=sin(angle);
    double COS=cos(angle);
    int X=center.x()+COS*(x-center.x())-SIN*(y-center.y())+0.5;
    int Y=center.y()+SIN*(x-center.x())+COS*(y-center.y())+0.5;
    */
    //int X=mul[0][0]*x+mul[0][1]*y+add[0][0];
    //int Y=mul[1][0]*x+mul[1][1]*y+add[1][0];


    int X=coef[0][0]*x+coef[0][1]*y+add[0][0];
    int Y=coef[1][0]*x+coef[1][1]*y+add[1][0];
    if(X>upperbound_x)
        upperbound_x=X;
    if(X<lowerbound_x)
        lowerbound_x=X;
    if(Y>upperbound_y)
        upperbound_y=Y;
    if(Y<lowerbound_y)
        lowerbound_y=Y;
    painter.drawPoint(X,Y);
}


void Draw:: paintline(QPainter &painter,QPoint p1,QPoint p2){
    if(cpt[0]==cpt[1])return;
    QPoint lp=p1.x()<p2.x()?p1:p2;
    QPoint rp=p1.x()>p2.x()?p1:p2;
    QPoint up=p1.y()>p2.y()?p1:p2;
    QPoint dp=p1.y()<p2.y()?p1:p2;
    int maxY=p1.y()>p2.y()?p1.y():p2.y();
    int minY=p1.y()<p2.y()?p1.y():p2.y();
    if(cpt[0].x()==cpt[1].x()){
        for(int i=minY;i<=maxY;i++){
            //painter.drawPoint(cpt[0].x(),i);
            DrawPoint(painter,cpt[0].x(),i);
        }
        return;
    }
    double k=(double)(maxY-minY)/(double)(rp.x()-lp.x());
    if(k<=1){
        int x,y,dx,dy,p,step;
        x=lp.x();
        y=lp.y();
        dx=rp.x()-lp.x();
        if(lp.y()-rp.y()>=0){
            step=-1;
            dy=lp.y()-rp.y();
        }
        else{
            step=1;
            dy=rp.y()-lp.y();
        }
        p=2*dy-dx;
        for(;x<=rp.x();x++){
            //painter.drawPoint(x,y);
            DrawPoint(painter,x,y);
            if(p>0){y+=step;p+=2*(dy-dx);}
            else
                p+=2*dy;
        }
    }
    else{
        int x,y,dx,dy,p,step;
        x=dp.x();
        y=dp.y();
        dy=up.y()-dp.y();
        if(up.x()-dp.x()<=0){
            step=-1;
            dx=dp.x()-up.x();
        }
        else{
            step=1;
            dx=up.x()-dp.x();
        }
        p=2*dx-dy;
        for(;y<=up.y();y++){

            //painter.drawPoint(x,y);
            DrawPoint(painter,x,y);
            if(p>0){x+=step;p+=2*(dx-dy);}
            else
                p+=2*dx;
        }
    }
}

void DrawLine::paint(QPainter &painter, bool flag){


    upperbound_x=findMaxX(cpt);
    upperbound_y=findMaxY(cpt);
    lowerbound_x=findMinX(cpt);
    lowerbound_y=findMinY(cpt);
    painter.drawLine(cpt[0],cpt[1]);


    if(flag){
        QPen *p=new QPen(Qt::blue, 1,Qt::SolidLine, Qt::RoundCap);
        painter.setPen(*p);
        painter.setBrush(Qt::white);
        for(int i=0;i<2;i++)
            painter.drawEllipse(cpt[i],5,5);

    }
}


void DrawEllipse::paint(QPainter &painter, bool f){
    upperbound_x=0;
    upperbound_y=0;
    lowerbound_x=20000;
    lowerbound_y=20000;
        int midX=(cpt[0].x()+cpt[1].x())/2;
        int midY=(cpt[0].y()+cpt[1].y())/2;
        bool flag=false;

        long long int rx=abs(midX-cpt[0].x());
        long long int ry=abs(midY-cpt[0].y());

        if(rx==0 || ry== 0)return;
        if(rx<ry){
            flag=true;
            int tmp=rx;
            rx=ry;
            ry=tmp;
        }
        long long int x=0,y=ry;
        int x0,y0;
        long double p=ry*ry-rx*rx*ry+0.25*rx*rx;
        for(x=0;2*ry*ry*x<=2*rx*rx*y;x++){
            if(p<0){
                if(flag){
                    /*
                    painter.drawPoint(y+midX,x+1+midY);
                    painter.drawPoint(midX-y,x+1+midY);
                    painter.drawPoint(y+midX,midY-x-1);
                    painter.drawPoint(midX-y,midY-x-1);
                    */
                    DrawPoint(painter,y+midX,x+1+midY);
                    DrawPoint(painter,midX-y,x+1+midY);
                    DrawPoint(painter,y+midX,midY-x-1);
                    DrawPoint(painter,midX-y,midY-x-1);
                }
                else{
                 /*
                    painter.drawPoint(x+1+midX,y+midY);
                    painter.drawPoint(x+1+midX,midY-y);
                    painter.drawPoint(midX-x-1,y+midY);
                    painter.drawPoint(midX-x-1,midY-y);
                    */
                    DrawPoint(painter,x+1+midX,y+midY);
                    DrawPoint(painter,x+1+midX,midY-y);
                    DrawPoint(painter,midX-x-1,y+midY);
                    DrawPoint(painter,midX-x-1,midY-y);
                }
                p+=2*ry*ry*x+3*ry*ry;
                x0=x+1;
                y0=y;
            }
            else{
                if(flag){
                    /*
                    painter.drawPoint(y-1+midX,x+1+midY);
                    painter.drawPoint(1-y+midX,x+1+midY);
                    painter.drawPoint(y-1+midX,midY-x-1);
                    painter.drawPoint(1-y+midX,midY-x-1);
                    */
                    DrawPoint(painter,y-1+midX,x+1+midY);
                    DrawPoint(painter,1-y+midX,x+1+midY);
                    DrawPoint(painter,y-1+midX,midY-x-1);
                    DrawPoint(painter,1-y+midX,midY-x-1);
                }
                else{
                    /*
                    painter.drawPoint(x+1+midX,y-1+midY);
                    painter.drawPoint(x+1+midX,1-y+midY);
                    painter.drawPoint(midX-x-1,y-1+midY);
                    painter.drawPoint(midX-x-1,1-y+midY);
                    */
                    DrawPoint(painter,x+1+midX,y-1+midY);
                    DrawPoint(painter,x+1+midX,1-y+midY);
                    DrawPoint(painter,midX-x-1,y-1+midY);
                    DrawPoint(painter,midX-x-1,1-y+midY);
                }
                x0=x+1;
                y0=y-1;
                p+=2*ry*ry*x+2*ry*ry-(2*rx*rx*y-2*rx*rx)+ry*ry;
                y--;
            }
        }
        p=ry*ry*(x0+0.5)*(x0+0.5)+rx*rx*(y0-1)*(y0-1)-rx*rx*ry*ry;
        for(;y>=0;y--){
                if(p>0){
                   if(flag){
                       /*
                       painter.drawPoint(y-1+midX,x+midY);
                       painter.drawPoint(1-y+midX,x+midY);
                       painter.drawPoint(y-1+midX,-x+midY);
                       painter.drawPoint(-y+1+midX,-x+midY);
                       */
                       DrawPoint(painter,y-1+midX,x+midY);
                       DrawPoint(painter,1-y+midX,x+midY);
                       DrawPoint(painter,y-1+midX,-x+midY);
                       DrawPoint(painter,-y+1+midX,-x+midY);
                    }
                   else{
                       /*
                        painter.drawPoint(x+midX,y-1+midY);
                        painter.drawPoint(x+midX,-y+1+midY);
                        painter.drawPoint(-x+midX,y-1+midY);
                        painter.drawPoint(-x+midX,-y+1+midY);
                        */
                       DrawPoint(painter,x+midX,y-1+midY);
                       DrawPoint(painter,x+midX,-y+1+midY);
                       DrawPoint(painter,-x+midX,y-1+midY);
                       DrawPoint(painter,-x+midX,-y+1+midY);
                   }
                   p+=(rx*rx-2*rx*rx*(y-1));


                }
                else{
                    if(flag){
                        /*
                        painter.drawPoint(y-1+midX,x+1+midY);
                        painter.drawPoint(-y+1+midX,x+1+midY);
                        painter.drawPoint(y-1+midX,-x-1+midY);
                        painter.drawPoint(-y+1+midX,-x-1+midY);
                        */
                        DrawPoint(painter,y-1+midX,x+1+midY);
                        DrawPoint(painter,-y+1+midX,x+1+midY);
                        DrawPoint(painter,y-1+midX,-x-1+midY);
                        DrawPoint(painter,-y+1+midX,-x-1+midY);
                    }
                    else{
                        /*
                        painter.drawPoint(x+1+midX,y-1+midY);
                        painter.drawPoint(x+1+midX,-y+1+midY);
                        painter.drawPoint(-x-1+midX,y-1+midY);
                        painter.drawPoint(-x-1+midX,-y+1+midY);
                        */
                        DrawPoint(painter,x+1+midX,y-1+midY);
                        DrawPoint(painter,x+1+midX,-y+1+midY);
                        DrawPoint(painter,-x-1+midX,y-1+midY);
                        DrawPoint(painter,-x-1+midX,-y+1+midY);
                    }
                    p+=(2*ry*ry*(x+1)-2*rx*rx*(y-1)+rx*rx);
                    x++;
                }

            }

        if(f){
            QPen *p=new QPen(Qt::blue, 1,Qt::SolidLine, Qt::RoundCap);
            painter.setPen(*p);
            painter.setBrush(Qt::white);
            for(int i=0;i<2;i++)
                painter.drawEllipse(cpt[i],5,5);

        }
}

void DrawRect::paint(QPainter &painter, bool flag){
    upperbound_x=0;
    upperbound_y=0;
    lowerbound_x=20000;
    lowerbound_y=20000;
    QPoint p1;
    QPoint p2;
    QPoint p3;
    QPoint p4;


    int minX=cpt[0].x()<cpt[1].x()?cpt[0].x():cpt[1].x();
    int maxX=cpt[0].x()>cpt[1].x()?cpt[0].x():cpt[1].x();

    int minY=cpt[0].y()<cpt[1].y()?cpt[0].y():cpt[1].y();
    int maxY=cpt[0].y()>cpt[1].y()?cpt[0].y():cpt[1].y();
    p1.setX(minX);p1.setY(minY);
    p2.setX(minX);p2.setY(maxY);
    p3.setX(maxX);p3.setY(minY);
    p4.setX(maxX);p4.setY(maxY);

    paintline(painter,p1,p2);
    paintline(painter,p1,p3);
    paintline(painter,p3,p4);
    paintline(painter,p4,p2);



    if(flag){
        QPen *p=new QPen(Qt::blue, 1,Qt::SolidLine, Qt::RoundCap);
        painter.setPen(*p);
        painter.setBrush(Qt::white);
        for(int i=0;i<2;i++)
            painter.drawEllipse(cpt[i],5,5);
        delete p;

    }
}

void AreaFill::paint(QImage &image,QColor color){
    int width=image.width();
        int x=cpt[0].x();
        int y=cpt[0].y();
        QColor seed=image.pixelColor(x,y);
        if(seed==color)return;
        int left=cpt[0].x();
        int right=cpt[0].x()+1;
        QStack<QPoint> src;
        src.push(cpt[0]);
        while(!src.empty()){
            QPoint p=src.pop();
            x=p.x();
            y=p.y();
            left=x;
            right=x+1;
            for(;left>=0&&image.pixelColor(left,y)==seed;left--){
                image.setPixelColor(left,y,color);
            }//向左填充扫描线
            left++;
            for(;right<width&&image.pixelColor(right,y)==seed;right++){
                image.setPixelColor(right,y,color);
            }//向右填充扫描线
            right--;
            int yn=y+1;
            for(int k=0;k<2;k++){
                if(yn<image.height() && yn>=0){
                    bool isLeft=true;
                    for(int i=left;i<=right;i++){
                        if(image.pixelColor(i,yn)==seed&&isLeft){
                            src.push(*(new QPoint(i,yn)));//找到待填充区间左端点
                            isLeft=false;
                        }
                        else if(image.pixelColor(i,yn)!=seed)
                            isLeft=true;
                    }
                }
                yn=y-1;
            }

        }
}

void DrawPolyGon::paint(QPainter &painter, bool flag){

    upperbound_x=findMaxX(cpt);
    upperbound_y=findMaxY(cpt);
    lowerbound_x=findMinX(cpt);
    lowerbound_y=findMinY(cpt);
    for(int i=0;i<cpt.size()-1;i++){
        //paintline(painter,cpt[i],cpt[i+1]);
        painter.drawLine(cpt[i],cpt[i+1]);
    }
    if(finished){
        painter.drawLine(cpt[0],cpt[cpt.size()-1]);
        //paintline(painter,cpt[0],cpt[cpt.size()-1]);
    }


    if(flag){
        QPen tp=painter.pen();
        QPen *p=new QPen(Qt::blue, 1,Qt::SolidLine, Qt::RoundCap);
        painter.setPen(*p);
        painter.setBrush(Qt::white);
        for(int i=0;i<cpt.size();i++)
            painter.drawEllipse(cpt[i],5,5);
        painter.setPen(tp);

    }
}

void DrawCurve::paint(QPainter &painter, bool flag){

    upperbound_x=0;
    upperbound_y=0;
    lowerbound_x=20000;
    lowerbound_y=20000;
    painter.save();
    QPen temp=painter.pen();
    QPen pen;
    pen.setBrush(QBrush(Qt::black));
    QVector<qreal> dashes;
    qreal space = 3;
    dashes << 5 << space << 5 <<space;
    pen.setDashPattern(dashes);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    if((!finished && !flag)||(finished && flag))
        for(int i=0;i<cpt.size()-1;i++){
            //paintline(painter,cpt[i],cpt[i+1]);
            painter.drawLine(cpt[i],cpt[i+1]);
        }
    painter.restore();
    if(finished){
        QPoint last=cpt[0];
       for(double u=0;u<1;u+=0.005){
            QVector<QPoint> points(cpt);
            while(points.size()!=1){
                for(int i=0;i<points.size()-1;i++)
                    points[i]=(1-u)*points[i]+u*points[i+1];
                points.pop_back();
            }
            paintline(painter,last,points[0]);
            last=points[0];
       }

    }


    if(flag){
        painter.save();
        QPen *p=new QPen(Qt::blue, 1,Qt::SolidLine, Qt::RoundCap);
        painter.setPen(*p);
        painter.setBrush(Qt::white);
        for(int i=0;i<cpt.size();i++)
            painter.drawEllipse(cpt[i],5,5);
        painter.restore();
    }

}



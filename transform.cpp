#include "Transform.h"



void Matrix_mul(double** target,double** matrix_a,int a_rows,int a_cols,double** matrix_b,int b_rows,int b_cols){

    for(int m=0;m<a_rows;m++){
         for(int s=0;s<b_cols;s++){
             target[m][s]=0;
             for(int n=0;n<a_cols;n++){
                 target[m][s]+=matrix_a[m][n]*matrix_b[n][s];
             }
         }
     }
}

void Matrix_add(double** target,double** matrix_a,double** matrix_b,int rows,int cols){
    for(int i=0;i<cols;i++)
        for(int j=0;j<rows;j++){
            target[i][j]=matrix_a[i][j]+matrix_b[i][j];
        }
}


Transform::Transform(Draw *s){
    this->shapeToTrans=s;
    QVector<QPoint> points=s->getCpt();
    /*
    int minX=findMinX(points);
    int maxX=findMaxX(points);
    int minY=findMinY(points);
    int maxY=findMaxY(points);
    */
    int minX=s->lowerbound_x;
    int minY=s->lowerbound_y;
    int maxX=s->upperbound_x;
    int maxY=s->upperbound_y;
    transPoint[upleft].setX(minX);transPoint[upleft].setY(minY);
    transPoint[upright].setX(maxX);transPoint[upright].setY(minY);
    transPoint[downleft].setX(minX);transPoint[downleft].setY(maxY);
    transPoint[downright].setX(maxX);transPoint[downright].setY(maxY);

    transPoint[upmid].setX((minX+maxX)/2);transPoint[upmid].setY(minY);
    transPoint[downmid].setX((minX+maxX)/2);transPoint[downmid].setY(maxY);

    transPoint[leftmid].setY((minY+maxY)/2);transPoint[leftmid].setX(minX);
    transPoint[rightmid].setY((minY+maxY)/2);transPoint[rightmid].setX(maxX);
}

Transform::Transform(const Transform& t){
    this->shapeToTrans=new Draw(*t.shapeToTrans);
    for(int i=0;i<8;i++)
        this->transPoint[i]=t.transPoint[i];
}

void Transform:: paint(QPainter &painter){
//           shapeToTrans->paint(painter,false);
           QPen pen;
           pen.setBrush(QBrush(Qt::black));
           QVector<qreal> dashes;
           qreal space = 3;
           dashes << 5 << space << 5 <<space;
           pen.setDashPattern(dashes);
           pen.setWidth(1);
           painter.setPen(pen);
           painter.setBrush(Qt::NoBrush);


           int minX=shapeToTrans->lowerbound_x;
           int maxX=shapeToTrans->upperbound_x;
           int minY=shapeToTrans->lowerbound_y;
           int maxY=shapeToTrans->upperbound_y;
           transPoint[upleft].setX(minX);transPoint[upleft].setY(minY);
           transPoint[upright].setX(maxX);transPoint[upright].setY(minY);
           transPoint[downleft].setX(minX);transPoint[downleft].setY(maxY);
           transPoint[downright].setX(maxX);transPoint[downright].setY(maxY);

           transPoint[upmid].setX((minX+maxX)/2);transPoint[upmid].setY(minY);
           transPoint[downmid].setX((minX+maxX)/2);transPoint[downmid].setY(maxY);

           transPoint[leftmid].setY((minY+maxY)/2);transPoint[leftmid].setX(minX);
           transPoint[rightmid].setY((minY+maxY)/2);transPoint[rightmid].setX(maxX);

           painter.drawRect(transPoint[upleft].x(),transPoint[upleft].y(),transPoint[upright].x()-transPoint[upleft].x(),transPoint[downleft].y()-transPoint[upleft].y());

           rotatePoint.setX(transPoint[upmid].x());
           rotatePoint.setY(transPoint[upmid].y()-30);

           QPen *p=new QPen(Qt::red, 1,Qt::SolidLine, Qt::RoundCap);
           painter.setPen(*p);
           painter.setBrush(Qt::white);
           for(int i=0;i<8;i++)
               painter.drawEllipse(transPoint[i],5,5);
           painter.setBrush(Qt::red);
           painter.drawEllipse(rotatePoint,5,5);
}


bool Transform:: isAtTransformPoint(QPoint p){
    for(int i=0;i<8;i++){
        if(abs(transPoint[i].x()-p.x())+abs(transPoint[i].y()-p.y())<=5){
            currentTransformPoint=i;
            return true;
        }
    }
    return false;
}

bool Transform:: isAtRotatePoint(QPoint p){

        if(abs(rotatePoint.x()-p.x())+abs(rotatePoint.y()-p.y())<=5){
            return true;
        }

    return false;
}


bool Transform::isInArea(QPoint p){
    return p.x()>transPoint[upleft].x() && p.x()<transPoint[upright].x() && p.y()>transPoint[upleft].y()&&p.y()<transPoint[downleft].y();
}

void Transform::stretch(QPoint p,bool transforming,bool preserveRatio){

    if(copy==NULL)
        copy=new Transform(*this);
    origin=transPoint[7-currentTransformPoint];
    QPoint oldvec=transPoint[currentTransformPoint]-origin;
    QPoint newvec=p-origin;
    if(oldvec.x()*newvec.x()<=0)
        newvec.setX(oldvec.x()>0?5:-5);
    if(oldvec.y()*newvec.y()<=0)
        newvec.setY(oldvec.y()>0?5:-5);
    double ratioX;
    double ratioY;
    if(oldvec.x()==0)ratioX=1;
    else
        ratioX=(double)newvec.x()/(double)oldvec.x();
    if(oldvec.y()==0)ratioY=1;
    else
        ratioY=(double)newvec.y()/(double)oldvec.y();
    if(preserveRatio){
        ratioX=qMin(ratioX,ratioY);
        ratioY=ratioX;
    }

    mul_temp[0][0]=ratioX;mul_temp[0][1]=0;
    mul_temp[1][0]=0;mul_temp[1][1]=ratioY;
    add_temp[0][0]=origin.x()-ratioX*origin.x();
    add_temp[1][0]=origin.y()-ratioY*origin.y();

    for(int m=0;m<2;m++){
         for(int s=0;s<2;s++){
             shapeToTrans->mul[m][s]=0;
             for(int n=0;n<2;n++){
                 shapeToTrans->mul[m][s]+=mul_temp[m][n]*this->mul[n][s];
             }
         }
     }
    for(int m=0;m<2;m++){
         for(int s=0;s<1;s++){
             shapeToTrans->add[m][s]=0;
             for(int n=0;n<2;n++){
                 shapeToTrans->add[m][s]+=mul_temp[m][n]*this->add[n][s];
             }
              shapeToTrans->add[m][s]+=add_temp[m][s];
         }
     }
    shapeToTrans->adjust(copy->shapeToTrans->cpt);
    if(!transforming){
            memcpy((char*)this->add[0],(char*)shapeToTrans->add[0],16);
            memcpy((char*)this->mul[0],(char*)shapeToTrans->mul[0],32);
    }

}


void Transform::move(QPoint p,bool transforming){
    if(copy==NULL)
        copy=new Transform(*this);
    QPoint vec=p-moveStart;
    add_temp[0][0]=vec.x()+this->add[0][0];
    add_temp[1][0]=vec.y()+this->add[1][0];
    memcpy((char*)shapeToTrans->add,(char*)this->add_temp,16);
    shapeToTrans->adjust(copy->shapeToTrans->cpt);
    if(!transforming)
          memcpy((char*)this->add[0],(char*)shapeToTrans->add[0],16);
}


void Transform::rotate(QPoint p,bool transforming){


    if(copy==NULL)
        copy=new Transform(*this);
    int midX=transPoint[upmid].x();
    int midY=transPoint[leftmid].y();
    QPoint *c=new QPoint(midX,midY);
    double tmp_angle;
    if(p.y()==midY && p.x()>=midX)tmp_angle=pi;
    else if(p.y()==midY && p.x()<midX)
        tmp_angle=0;
    else if(p.y()<midY)
        tmp_angle=atan(-(double)(p.x()-midX)/(double)(p.y()-midY));
    else if(p.y()>midY)
        tmp_angle=pi-atan((double)(p.x()-midX)/(double)(p.y()-midY));
    double SIN=sin(tmp_angle);
    double COS=cos(tmp_angle);
    mul_temp[0][0]=COS;mul_temp[0][1]=-SIN;
    mul_temp[1][0]=SIN;mul_temp[1][1]=COS;
    add_temp[0][0]=SIN*midY-COS*midX+midX;
    add_temp[1][0]=midY-midX*SIN-midY*COS;



    for(int m=0;m<2;m++){
         for(int s=0;s<2;s++){
             shapeToTrans->mul[m][s]=0;
             for(int n=0;n<2;n++){
                 shapeToTrans->mul[m][s]+=mul_temp[m][n]*this->mul[n][s];
             }
         }
     }
    for(int m=0;m<2;m++){
         for(int s=0;s<1;s++){
             shapeToTrans->add[m][s]=0;
             for(int n=0;n<2;n++){
                 shapeToTrans->add[m][s]+=mul_temp[m][n]*this->add[n][s];
             }
              shapeToTrans->add[m][s]+=add_temp[m][s];
         }
     }
    shapeToTrans->adjust(copy->shapeToTrans->cpt);
    if(!transforming){
            memcpy((char*)this->add[0],(char*)shapeToTrans->add[0],16);
            memcpy((char*)this->mul[0],(char*)shapeToTrans->mul[0],32);
    }
}







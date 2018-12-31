#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H




//#include <QtGui>
#include <QDebug>

#include "Draw.h"
#include "Transform.h"
#include <QWidget>
#include <Qcolor>
#include <QImage>
#include <QPen>

#include "clip.h"
#define width 1920
#define height 1080

enum ActionType{drawLine,drawEllipse,freeDraw,fill,drawPolygon,drawRect,erase,transform,drawCurve,clip};

class PaintWidget:public QWidget
{
        Q_OBJECT

        friend class MainWindow;
public:
        PaintWidget(QWidget *parent = 0);

public slots:
        void setCurrentAction(ActionType s)
        {
            transformable=false;
            transforming=false;
            editing=false;
            editable=false;
            drawing=false;
            drawFinished=true;
            repaint();
                if(s != currentAction) {
                        currentAction = s;
                }
        }
        void save(QString url){
            transformable=false;
            transforming=false;
            drawFinished=true;
            editable=false;
            editing=false;
            drawing=false;
            actionChanged=false;
            drawingPolygonLine=false;
            repaint();
            image->save(url);
            drawFinished=false;

        }
protected:

        void paintEvent(QPaintEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);
private:
        Qt::CursorShape pattern[8]={Qt::SizeFDiagCursor,Qt::SizeVerCursor,Qt::SizeBDiagCursor,Qt::SizeHorCursor,Qt::SizeHorCursor,Qt::SizeBDiagCursor,Qt::SizeVerCursor,Qt::SizeFDiagCursor};
        QPen *currentPen;
        QColor c=Qt::gray;
        QImage *image;
        ActionType currentAction;
        QCursor currentCusor;
        Action *action;
        Transform* trans;
        bool drawFinished;
        bool editable;
        bool editing;
        bool drawing;
        bool actionChanged;
        bool drawingPolygonLine;
        bool transforming;
        bool transformable;
        bool preserveRatio;
        bool drawingClipWindow=false;
        bool clipWindowEdittable=false;
        bool movingClipWindow=false;
        bool edittingClipWindow=false;
        bool clipWindowSet=false;
        bool clipFinished=false;

};

#endif // PAINTWIDGET_H

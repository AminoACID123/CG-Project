#include "paintwidget.h"


PaintWidget::PaintWidget(QWidget *parent)
        : QWidget(parent),currentCusor(Qt::CrossCursor), currentAction(freeDraw), action(NULL),transformable(false),transforming(false), drawFinished(false),editable(false),editing(false),drawing(false),actionChanged(false),drawingPolygonLine(false),preserveRatio(false)
{
       setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
       resize(800,600);
        this->grabKeyboard();
        image = new QImage(width,height,QImage::Format_RGB888);
        image->fill(Qt::white);
        currentPen= new QPen(Qt::black, 3,Qt::SolidLine, Qt::RoundCap);
        setMouseTracking(1);


}

void PaintWidget::paintEvent(QPaintEvent *event)
{
        QPainter  screenpainter(this);
        QPainter vmempainter(image);
        screenpainter.setPen(*currentPen);
        vmempainter.setPen(*currentPen);
        screenpainter.drawImage(0,0,*image);

            if(action){
                if(drawingClipWindow ||clipWindowEdittable ){
                    ((Clip*)action)->paint(screenpainter);
                    ((Clip*)action)->line->paint(screenpainter,editable);
                }
                else if(transformable){
                    ((Draw*)action)->paint(screenpainter,editable);
                    if(!transforming)
                    trans->paint(screenpainter);
                    screenpainter.setPen(*currentPen);
                }
                else if(currentAction==drawPolygon||currentAction==drawEllipse||currentAction==drawLine||currentAction==drawRect||currentAction==freeDraw||currentAction==erase||currentAction==drawCurve)
                    if(!drawFinished)
                        ((Draw*)action)->paint(screenpainter,editable);
                    else{
                        ((Draw*)action)->paint(vmempainter,editable);
                        screenpainter.drawImage(0,0,*image);
                    }
            }



}

void PaintWidget::mouseDoubleClickEvent(QMouseEvent *event){
    if(currentAction==drawPolygon && !transformable){
        ((DrawPolyGon*)action)->complete();

        ((DrawPolyGon*)action)->deleteEnd();
 //       ((DrawPolygon*)action)->addEnd(((DrawPolygon*)action)->getStartControlPoint());
        drawing=false;
        editable=true;
        editing=false;
        repaint();
        return;
    }

    else if(currentAction==drawCurve && !transformable){
        ((DrawCurve*)action)->complete();

        //((DrawPolyGon*)action)->deleteEnd();
 //       ((DrawPolygon*)action)->addEnd(((DrawPolygon*)action)->getStartControlPoint());
        drawing=false;
        editable=true;
        editing=false;
        repaint();
        return;
    }

}


void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton&&currentAction==drawLine&&editable){
        drawingClipWindow=true;
        action = new Clip((DrawLine*) action);
        currentAction=clip;
        editable=false;
        ((Clip*)action)->setStart(event->pos());
        return;
    }

    else if(event->button()==Qt::LeftButton){
        if(editable){
            if(((Draw *)action)->isAtControlPoint(event->pos())){
                setCursor(pattern[3]);
                editing=true;
            }
            else{
                editable=false;
                drawFinished=true;
                //update();
                repaint();
                //goto L;

            }
        }
        else if(clipWindowEdittable){
            if(((Clip*)action)->isAtPoint(event->pos())){
                edittingClipWindow=true;
            }
            else if(((Clip*)action)->isInArea(event->pos())){
                movingClipWindow=true;
                ((Clip*)action)->moveStart=event->pos();
            }
            else{
                clipWindowEdittable=false;
                movingClipWindow=false;
                edittingClipWindow=false;
                currentAction=drawLine;
                if(!((Clip*)action)->clip()){
                    editable=false;
                    action=0;
                    return;
                }

                action= ((Clip*)action)->line;
                editable=true;
                repaint();
                return;
            }
        }
        else if(transformable){
            if(trans->isAtTransformPoint(event->pos())){
                setCursor(pattern[trans->getCurrentTransformPoint()]);
                trans->code=Transform::STRETCH;
                transforming=true;
            }
            else if(trans->isInArea(event->pos())){
                trans->code=Transform::MOVE;
                trans->moveStart=event->pos();
                transforming=true;
            }
            else if(trans->isAtRotatePoint(event->pos())){
                setCursor(Qt::ClosedHandCursor);
                trans->code=Transform::ROTATE;
                transforming=true;
            }
            else{
                transformable=false;
                drawFinished=true;
                //update();
                repaint();
                //goto L;

            }
        }
        else if(!drawing && !editable && !transformable){
            switch(currentAction)
        {
        case drawLine:
            action = new DrawLine();currentCusor=Qt::CrossCursor;break;
        case freeDraw:
            action = new DrawLine();break;
        case drawEllipse:
            action = new DrawEllipse();currentCusor=Qt::CrossCursor;break;
        case drawRect:
            action = new DrawRect();currentCusor=Qt::CrossCursor;break;
        case drawPolygon:
            action = new DrawPolyGon();currentCusor=Qt::CrossCursor;break;
        case fill:
            action = new AreaFill();break;
        case erase:
            action = new DrawLine();break;
        case drawCurve:
            action = new DrawCurve();break;



        }
            if(action != NULL) {
                setCursor(currentCusor);
                drawing=true;
                drawFinished = false;
                ((Draw*)action)->setStart(event->pos());
                ((Draw*)action)->setEnd(event->pos());
                if(currentAction==drawPolygon){
                    drawingPolygonLine=true;
                }
            }
            drawing = true;
            return;
        }
        else if(currentAction==drawPolygon){
            ((DrawPolyGon*)action)->addEnd(event->pos());
            drawingPolygonLine=true;
        }
        else if(currentAction==drawCurve){
            ((DrawCurve*)action)->addEnd(event->pos());
            drawingPolygonLine=true;
        }
    }
}

void PaintWidget::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Space && editable && !editing){
        trans=new Transform((Draw*)action);
        drawing=false;
        editable=false;
        editing=false;
        transformable=true;
        setCursor(Qt::ArrowCursor);
        repaint();
    }
    else if(event->key()==Qt::Key_Shift)
        preserveRatio=true;
    /*
    else if(event->key() == Qt::Key_Space && transformable && !transforming){
        drawing=false;
        editable=true;
        editing=false;
        transformable=false;
        setCursor(Qt::ArrowCursor);
        repaint();
    }
    */
}

void PaintWidget::keyReleaseEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Shift)
        preserveRatio=false;
}


void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{

    if(drawing){
        drawFinished=false;
        QPainter p(image);
        p.setPen(*currentPen);
        if(action && currentAction==freeDraw){
            ((DrawLine *)action)->setEnd(event->pos());
            ((DrawLine *)action)->paint(p,false);
            ((DrawLine *)action)->setStart(event->pos());
        }
        else if(action && currentAction==erase){
            QPen *eraser=new QPen(Qt::white, 30,Qt::SolidLine, Qt::RoundCap);
            p.setPen(*eraser);
            ((DrawLine *)action)->setEnd(event->pos());
            ((DrawLine *)action)->paint(p,false);
            ((DrawLine *)action)->setStart(event->pos());
        }
        else if(action && currentAction==fill);
        else if(action && currentAction==drawPolygon&&drawingPolygonLine==true)
            ((DrawPolyGon*)action)->setEnd(event->pos());
        else if(action && currentAction==drawCurve&&drawingPolygonLine==true)
            ((DrawCurve*)action)->setEnd(event->pos());
        else if(action&& (currentAction==drawEllipse ||currentAction==drawRect))
            ((Draw *)action)->setEnd(event->pos(),preserveRatio);
        else if(action)
            ((Draw *)action)->setEnd(event->pos());
       update();
    }
    else if(drawingClipWindow){
        ((Clip*)action)->setEnd(event->pos());
        repaint();
        return;
    }
    else if(edittingClipWindow){
        ((Clip*)action)->setPoint(event->pos());
        repaint();
        return;
    }
    else if(movingClipWindow){
        ((Clip*)action)->moveTo(event->pos());
        repaint();
        return;
    }
    else if(clipWindowEdittable){
        if(((Clip*)action)->isAtPoint(event->pos()))
            setCursor(Qt::SizeHorCursor);
        else if(((Clip*)action)->isInArea(event->pos()))
            setCursor(Qt::SizeAllCursor);
        else
            setCursor(currentCusor);
    }
    else if(editable && !editing)
        if(((Draw *)action)->isAtControlPoint(event->pos()))
             setCursor(pattern[3]);
        else
            setCursor(currentCusor);
    else if(transformable && !transforming)
        if(trans->isAtTransformPoint(event->pos()))
             setCursor(pattern[trans->getCurrentTransformPoint()]);
        else if(trans->isInArea(event->pos()))
            setCursor(Qt::SizeAllCursor);
        else if(trans->isAtRotatePoint(event->pos()))
            setCursor(Qt::OpenHandCursor);
        else
            setCursor(currentCusor);
    else if(editing){
        ((Draw*)action)->setControlPoint(event->pos());
        update();
    }
    else if(transforming){
        if(trans->code==Transform::STRETCH)
            trans->stretch(event->pos(),transforming,preserveRatio);
        else if(trans->code==Transform::MOVE)
            trans->move(event->pos(),transforming);
        else if(trans->code==Transform::ROTATE)
            trans->rotate(event->pos(),transforming);
        repaint();
    }


}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(drawing){
        QPainter p(image);
        p.setPen(*currentPen);
        if(action && currentAction==fill){
            ((AreaFill *)action)->paint(*image,c);
            drawing=false;
            editable=false;
            editing=false;
        }
        else if(action &&(currentAction==freeDraw || currentAction==erase)){
            drawing=false;
            editable=false;
            editing=false;
        }
        else if(action && (currentAction==drawPolygon || currentAction==drawCurve)){
            drawingPolygonLine=false;
        }
        else if(action && (currentAction==drawCurve || currentAction==drawCurve)){
            drawingPolygonLine=false;
        }
        else if(action){
            if(drawing){
                editable=true;
                drawing=false;
            }

        }
        // update();
    }

    else if(editing){
        editing=false;
        editable=true;
    }
    else if(transforming){
        transforming=false;
        transformable=true;
        if(trans->code==Transform::STRETCH)
            trans->stretch(event->pos(),transforming,preserveRatio);
        else if(trans->code==Transform::MOVE)
            trans->move(event->pos(),transforming);
        else if(trans->code==Transform::ROTATE)
            trans->rotate(event->pos(),transforming);
    }
    else if(drawingClipWindow){
        drawingClipWindow=false;
        clipWindowEdittable=true;
        return;
    }
    else if(edittingClipWindow){
        edittingClipWindow=false;
        clipWindowEdittable=true;
        return;
    }
    else if(movingClipWindow){
        movingClipWindow=false;
        clipWindowEdittable=true;
        return;
    }
    update();
}

#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent){


        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        resize(800,600);
        QToolBar *bar = this->addToolBar("Draw");


        QActionGroup *group = new QActionGroup(bar);


        setMouseTracking(true);

        //drawline
        QAction *drawLineAction = new QAction("Line", bar);
        drawLineAction->setIcon(QIcon(":/action/line.png"));
        drawLineAction->setToolTip(tr("Draw a line."));
        drawLineAction->setStatusTip(tr("Draw a line."));
        drawLineAction->setCheckable(true);

        group->addAction(drawLineAction);
        bar->addAction(drawLineAction);


        //freedraw
        QAction *freeDrawAction = new QAction("freeDraw", bar);
        freeDrawAction->setIcon(QIcon(":/action/pen.png"));
		freeDrawAction->setToolTip(tr("Draw anything."));
		freeDrawAction->setStatusTip(tr("Draw anything."));
		freeDrawAction->setCheckable(true);
		freeDrawAction->setChecked(true);
        group->addAction(freeDrawAction);
        bar->addAction(freeDrawAction);


        //drawellipse
        QAction *drawEllipseAction = new QAction("drawEllipse", bar);
        drawEllipseAction->setIcon(QIcon(":/action/ellipse.png"));
        drawEllipseAction->setToolTip(tr("Draw Ellipse."));
        drawEllipseAction->setStatusTip(tr("Draw Ellipse."));
        drawEllipseAction->setCheckable(true);
        drawEllipseAction->setChecked(false);
        group->addAction( drawEllipseAction);
        bar->addAction( drawEllipseAction);

        //drawrect
        QAction *drawRectAction = new QAction("drawRect", bar);
        drawRectAction->setIcon(QIcon(":/action/rectangle.png"));
        drawRectAction->setToolTip(tr("Draw Rect."));
        drawRectAction->setStatusTip(tr("Draw Rect."));
        drawRectAction->setCheckable(true);
        drawRectAction->setChecked(false);
        group->addAction( drawRectAction);
        bar->addAction( drawRectAction);

        //drawpolygon
        QAction *drawPolygonAction = new QAction("drawPolygon", bar);
        drawPolygonAction->setIcon(QIcon(":/action/polygon.png"));
        drawPolygonAction->setToolTip(tr("Draw Polygon."));
        drawPolygonAction->setStatusTip(tr("Draw Polygon."));
        drawPolygonAction->setCheckable(true);
        drawPolygonAction->setChecked(false);
        group->addAction( drawPolygonAction);
        bar->addAction( drawPolygonAction);


        //erase
        QAction *eraseAction = new QAction("erase", bar);
        eraseAction->setIcon(QIcon(":/action/eraser.png"));
        eraseAction->setToolTip(tr("Erase."));
        eraseAction->setStatusTip(tr("Erase."));
        eraseAction->setCheckable(true);
        eraseAction->setChecked(false);
        group->addAction( eraseAction);
        bar->addAction( eraseAction);



        //floodfill
        QAction *floodFillAction = new QAction("FloodFill", bar);
        floodFillAction->setIcon(QIcon(":/action/floodfill.png"));
        floodFillAction->setToolTip(tr("FloodFill."));
        floodFillAction->setStatusTip(tr("FloodFill."));
        floodFillAction->setCheckable(true);
        floodFillAction->setChecked(false);
        group->addAction(floodFillAction);
        bar->addAction(floodFillAction);


        //drawCurve
        QAction *drawCurveAction = new QAction("drawCurve", bar);
        drawCurveAction->setIcon(QIcon(":/action/curve.png"));
        drawCurveAction->setToolTip(tr("Draw a curve."));
        drawCurveAction->setStatusTip(tr("Draw a curve"));
        drawCurveAction->setCheckable(true);
        drawCurveAction->setChecked(false);
        group->addAction(drawCurveAction);
        bar->addAction(drawCurveAction);

        QLabel *statusMsg = new QLabel;
        statusBar()->addWidget(statusMsg);

        paintWidget = new PaintWidget(this);
        setCentralWidget(paintWidget);

        QAction *openAction = new QAction(tr("&Open 3D Model"), this);
        openAction->setShortcut(QKeySequence::Open);
        openAction->setStatusTip(tr("Open a file."));
        QMenu *file = menuBar()->addMenu(tr("&File"));
        file->addAction(openAction);


        QAction *saveAction = new QAction(tr("&Save Image"), this);
        saveAction->setShortcut(QKeySequence::Save);
        saveAction->setStatusTip(tr("Save Image"));
        file->addAction(saveAction);


        pen_Button=new ColorButton(bar,Qt::black);
        pen_Button->setStatusTip("Set Pen Color");
        bar->addWidget(pen_Button);

        fill_Button=new ColorButton(bar,Qt::gray);
        fill_Button->setStatusTip("Set Fill Color");
        bar->addWidget(fill_Button);



        connect(openAction, SIGNAL(triggered()),
                        this, SLOT(openActionTriggered()));

        connect(saveAction, SIGNAL(triggered()),
                        this, SLOT(saveActionTriggered()));


        connect(drawLineAction, SIGNAL(triggered()),
                        this, SLOT(drawLineActionTriggered()));

        connect(freeDrawAction, SIGNAL(triggered()),
                        this, SLOT(freeDrawActionTriggered()));


        connect(drawEllipseAction, SIGNAL(triggered()),
                        this, SLOT(drawEllipseActionTriggered()));

        connect(drawRectAction, SIGNAL(triggered()),
                        this, SLOT(drawRectActionTriggered()));

        connect(eraseAction, SIGNAL(triggered()),
                        this, SLOT(eraseActionTriggered()));

        connect(floodFillAction, SIGNAL(triggered()),
                        this, SLOT(floodFillActionTriggered()));

        connect(drawPolygonAction, SIGNAL(triggered()),
                        this, SLOT(drawPolygonActionTriggered()));

        connect(drawCurveAction, SIGNAL(triggered()),
                        this, SLOT(drawCurveActionTriggered()));

        connect(this, SIGNAL(changeCurrentAction(ActionType)),
                        paintWidget, SLOT(setCurrentAction(ActionType)));
        connect(this, SIGNAL(saveImage(QString)),
                        paintWidget, SLOT(save(QString)));

        connect(pen_Button, SIGNAL(clicked(bool)),
                        this, SLOT(setPenColorActionTriggered()));

        connect(fill_Button, SIGNAL(clicked(bool)),
                        this, SLOT(setFillColorActionTriggered()));

}



void MainWindow::openActionTriggered()
{
        QFileDialog *dialog=new QFileDialog(this);
        QString filter= "OFF files (*.off)";

        dialog->setNameFilter(filter);
        QString name=dialog->getOpenFileName(this,QString(),QString(), filter,0,0 );
        if(name!=""){
            OpenGLWidget *opengl=new OpenGLWidget(this);
            opengl->filename=name;
            opengl->show();
        }


}

void MainWindow::saveActionTriggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,QString::fromLocal8Bit("Save Image"),"",tr("(*.jpg)"));
    qDebug()<<fileName;
    if(fileName!=NULL)
        emit saveImage(fileName);
}


void MainWindow::setPenColorActionTriggered(){
    QColor c= QColorDialog::getColor();
    pen_Button->color=c;
    pen_Button->update();
    if(c.isValid())
        paintWidget->currentPen->setColor(c);

}

void MainWindow::setFillColorActionTriggered(){
    QColor c= QColorDialog::getColor();
    fill_Button->color=c;
    fill_Button->update();
    if(c.isValid())
        paintWidget->c=c;

}


void MainWindow::drawLineActionTriggered()
{
        emit changeCurrentAction(drawLine);


}

void MainWindow::drawCurveActionTriggered()
{
        emit changeCurrentAction(drawCurve);


}

void MainWindow::freeDrawActionTriggered()
{
    emit changeCurrentAction(freeDraw);
}


void MainWindow:: drawEllipseActionTriggered(){
    emit changeCurrentAction(drawEllipse);
}

void MainWindow:: drawRectActionTriggered(){
    emit changeCurrentAction(drawRect);
}

void MainWindow:: eraseActionTriggered(){

    emit changeCurrentAction(erase);
}

void MainWindow:: floodFillActionTriggered(){

    emit changeCurrentAction(fill);
}

void MainWindow:: drawPolygonActionTriggered(){

    emit changeCurrentAction(drawPolygon);
}



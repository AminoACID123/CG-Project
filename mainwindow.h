#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <ui_mainwindow.h>
#include <QAbstractSlider>
#include <QDial>
#include <QMainWindow>
#include "paintwidget.h"
#include <QActionGroup>
#include <qtoolbar.h>
#include <qlabel.h>
#include  <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include "openglwidget.h"
#include <QPushButton>
#include <QColorDialog>
#include <QPixmap>
#include "colorbutton.h"
class MainWindow : public QMainWindow
{
        Q_OBJECT

public:
        MainWindow(QWidget *parent = 0);
        QMenu* menu;
    //    ColorButton *penButton;
    //    ColorButton *fillButton;

signals:
        void changeCurrentAction(ActionType newAction);
        void changeCurrentCursor();
        void openFile();
        void saveImage(QString url);

private slots:
        void setFillColorActionTriggered();
        void openActionTriggered();
        void saveActionTriggered();
        void drawLineActionTriggered();
        void freeDrawActionTriggered();
        void drawEllipseActionTriggered();
        void drawRectActionTriggered();
        void eraseActionTriggered();
        void floodFillActionTriggered();
        void drawPolygonActionTriggered();
        void drawCurveActionTriggered();
        void setPenColorActionTriggered();

private:

        ColorButton* pen_Button;
        ColorButton* fill_Button;
        PaintWidget* paintWidget;

};

#endif // MAINWINDOW_H

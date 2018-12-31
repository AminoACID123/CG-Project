#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QtGui>
#include <QSlider>
#include <QWheelEvent>


struct vertice{
    double x;
    double y;
    double z;
};

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    QString filename;
    OpenGLWidget(QWidget *parent = 0);
private slots:
    void setXRotate(){
        xRotate=xSlider->value();
        update();
    }

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent*event);
private:
    double xRotate=0;
    double yRotate=0;
    double zRotate=0;
    double ratio=1;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;

    int nVertices;
    int nFaces;
    QVector<QVector<int>> Faces;
    QVector<vertice> Vertices;
};

#endif // OPENGLWIDGET_H

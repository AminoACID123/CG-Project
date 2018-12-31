#include "openglwidget.h"
OpenGLWidget:: OpenGLWidget(QWidget *parent){

    xSlider=new QSlider(Qt::Horizontal,this);
    xSlider->setRange(-180,180);

    connect(xSlider, SIGNAL(valueChanged(int)), this, SLOT(setXRotate()));
    resize(800,800);

    //setGeometry(400,200,640,480);
}


void OpenGLWidget:: keyPressEvent(QKeyEvent *event){
        if(event->key()==Qt::Key_Shift)
                qDebug()<<"aaa"<<endl;
}


void OpenGLWidget:: wheelEvent(QWheelEvent*event){
    if(event->delta()>0)
        ratio+=0.2;
    else if(event->delta()<0 && ratio>0.2)
            ratio-=0.2;
    update();
}

void OpenGLWidget::initializeGL()
{

    initializeOpenGLFunctions();
    //设置背景色——用于填充背景
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //设置多边形填充模式为smooth 方式
    glShadeModel(GL_SMOOTH);
    //打开深度测试开关——用于检测物体间的z 深度差异
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    //线的抗锯齿开关
    glEnable(GL_LINE_SMOOTH);
    //启用抗锯齿效果
    glHint(GL_LINE_SMOOTH,GL_NICEST);
    //指定混合函数
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //启用色彩混合状态
    glEnable(GL_BLEND);



    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    in.readLine();

    QStringList list=in.readLine().split(' ');
    nVertices=list[0].toInt();
    nFaces=list[1].toInt();
    for(int i=0;i<nVertices;i++){
        list=in.readLine().split(' ');
        vertice tmp;
        tmp.x=list[0].toDouble();
        tmp.y=list[1].toDouble();
        tmp.z=list[2].toDouble();
        Vertices.push_back(tmp);
    }
    for(int i=0;i<nFaces;i++){
        list=in.readLine().split(' ');
        int num=list[0].toInt();

        QVector<int>tmp;
        for(int j=1;j<=num;j++)
           tmp.push_back(list[j].toInt());

        Faces.push_back(tmp);
    }

}
void OpenGLWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //将当前点移置屏幕中心，相当于复位的操作
        glLoadIdentity();
        //平移函数,参数指的是分别从X轴，Y轴，Z轴平移
        glTranslatef(0.0,0.0,-6.0);


        glScaled(ratio,ratio,ratio);
        glRotatef(xRotate,1,0,0);


        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for(int i=0;i<nFaces;i++){
            glBegin(GL_POLYGON);
            glColor3f(1.0, 1.0,1.0);
            for(int j=0;j<Faces[i].size();j++){

                glVertex3f( Vertices[Faces[i][j]].x , Vertices[Faces[i][j]].y, Vertices[Faces[i][j]].z);
            }
            glEnd();
        }

        glLineWidth(2);
        for(int i=0;i<nFaces;i++){
            glBegin(GL_LINE_LOOP);
            glColor3f(0, 0, 0);
            for(int j=0;j<Faces[i].size();j++){

                glVertex3f( Vertices[Faces[i][j]].x , Vertices[Faces[i][j]].y, Vertices[Faces[i][j]].z);
            }
            glEnd();
        }



}
void OpenGLWidget::resizeGL(int width, int height)
{
    if(0==height)
           height=1;
       //告诉绘制到窗体的哪个位置
       glViewport(0,0,width,height);
       // 设置矩阵模式，参数是设置为投影矩阵
       glMatrixMode(GL_PROJECTION);
       //复位操作
       glLoadIdentity();

       GLdouble aspectRatio=(GLfloat)width/(GLfloat)height;
       GLdouble rFov=45.0*3.14159265/180.0;
       GLdouble zNear=0.1;
       GLdouble zFar=100.0;
       //调用glFrustum，生成矩阵与当前矩阵相乘，生成透视效果
       glFrustum(-zNear*tan(rFov/2.0)*aspectRatio,
                 zNear*tan(rFov/2.0)*aspectRatio,
                 -zNear*tan(rFov/2.0),
                 zNear*tan(rFov/2.0),
                 zNear,zFar);
       //切回模型视图矩阵
       glMatrixMode(GL_MODELVIEW);
       //复位
       glLoadIdentity();


}

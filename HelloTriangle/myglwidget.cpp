#include "myglwidget.h"
#include<QTimer>
#include<QMatrix4x4>
#include<QOpenGLShaderProgram>
#include<QObject>
#include<QDebug>
#include"shape.h"
#include"triangle.h"
#include"rectangle.h"
#include"cube.h"

MyGLWidget::MyGLWidget(QWidget *parent):
    QOpenGLWidget (parent),
    animateflag(false)
{
    m_timer=new QTimer(this);
    m_timer->setInterval(60);
}


MyGLWidget::~MyGLWidget()
{

    qDeleteAll(shapevec);

}

void MyGLWidget::initializeGL()
{
    // 为当前环境初始化OpenGL函数
   initializeOpenGLFunctions();
   Triangle* tri=new Triangle(width(),height());
   shapevec.append(tri);
   connect(m_timer,&QTimer::timeout,this,&MyGLWidget::timeoutFunc);
   shapevec.append(new MyRectangle(width(),height()));
   Cube* cub=new Cube(width(),height());
   shapevec.append(cub);
  // cub->ChangeVisible(true);

  glEnable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
    /*清空颜色缓存，深度缓存，模板缓存*/
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // shapetest->Render();
     QVector<Shape*>::iterator i;
     for(i=shapevec.begin();i!=shapevec.end();++i)
     {
         (*i)->Render();
     }
    // shapevec[1]->Render();
}

void MyGLWidget::resizeGL(int width, int height)
{
const qreal retinascale=devicePixelRatio();
glViewport(0,0,width*retinascale,height*retinascale);

QVector<Shape*>::iterator i;
for(i=shapevec.begin();i!=shapevec.end();++i)
{
    (*i)->Resize(width,height);
}
}


void MyGLWidget::EnableTriangle()
{
    QVector<Shape*>::iterator i;
    for(i=shapevec.begin();i!=shapevec.end();++i)
    {
        if((*i)->inherits("Triangle"))
        {
            (*i)->ChangeVisible(true);
        }
        else {
            (*i)->ChangeVisible(false);
        }
    }


}
void MyGLWidget::EnableRectangle()
{
    QVector<Shape*>::iterator i;
    for(i=shapevec.begin();i!=shapevec.end();++i)
    {
        if((*i)->inherits("MyRectangle"))
        {
            (*i)->ChangeVisible(true);
        }
        else {
            (*i)->ChangeVisible(false);
        }
    }
}
void MyGLWidget::EnableCube()
{
    QVector<Shape*>::iterator i;
    for(i=shapevec.begin();i!=shapevec.end();++i)
    {
        if((*i)->inherits("Cube"))
        {
            (*i)->ChangeVisible(true);
        }
        else {
            (*i)->ChangeVisible(false);
        }
    }
}

void MyGLWidget::StartAnimate(bool flag)
 {
   animateflag=!animateflag;

     if(animateflag)
    {
          qDebug()<<"start";
         m_timer->start();

     }
     else {
         m_timer->stop();
     }

 }

 void MyGLWidget::timeoutFunc()
 {
     QVector<Shape*>::iterator i;
     for(i=shapevec.begin();i!=shapevec.end();++i)
     {
         if( (*i)->inherits("Triangle"))
         {
            Shape* temp=*i;
            Triangle* temptri= qobject_cast<Triangle *>(temp);
            temptri->Animate();
         }
         else if((*i)->inherits("Cube"))
         {
             Shape* temp=*i;
             Cube* tempcube= qobject_cast<Cube *>(temp);
             tempcube->Animate();
         }
     }

     update();
 }



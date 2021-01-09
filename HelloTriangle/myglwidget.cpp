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
#include"colorcube.h"
#include"lightcube.h"
#include"camera.h"
#include<QKeyEvent>
#include<QMouseEvent>
MyGLWidget::MyGLWidget(QWidget *parent):
    QOpenGLWidget (parent),
    animateflag(false),
    firstMouse(true),
    leftbuttonpressed(false),
    deltaTime(0.0f),
    lastFrame(0.0f),
    m_frame(0)
{
    m_timer=new QTimer(this);
    m_timer->setInterval(60);
    m_camera=new Camera(QVector3D(0.0f,0.0f,3.0f));
    lastX = width() / 2.0f;
    lastY = height() / 2.0f;
    //test
    deltaTime=1.0f;
    setFocusPolicy(Qt::StrongFocus);
    connect(m_timer,&QTimer::timeout,this,&MyGLWidget::timeoutFunc);

}


MyGLWidget::~MyGLWidget()
{

    qDeleteAll(shapevec);
    delete m_camera;

}

void MyGLWidget::initializeGL()
{
    // 为当前环境初始化OpenGL函数
   initializeOpenGLFunctions();
   Triangle* tri=new Triangle(width(),height());
   tri->ShapeCamera=m_camera;
   shapevec.append(tri);
   MyRectangle * rect=new MyRectangle(width(),height());
   shapevec.append(rect);
   rect->ShapeCamera=m_camera;
   Cube* cub=new Cube(width(),height());
   shapevec.append(cub);
   cub->ShapeCamera=m_camera;
   ColorCube* colcube=new ColorCube(width(),height());
   shapevec.append(colcube);
  // colcube->SetTranslate(QVector3D(1.2f, 1.0f, 2.0f));
   colcube->ShapeCamera=m_camera;
   LightCube *lightcube=new LightCube(width(),height());
   shapevec.append(lightcube);
  // lightcube->SetTranslate(QVector3D(1.2f, 1.0f, 2.0f));
    lightcube->SetTranslate(QVector3D(1.2f, 1.0f, 2.0f));
   lightcube->SetScale(QVector3D(0.1f,0.1f,0.1f));
   lightcube->ShapeCamera=m_camera;


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
         else if((*i)->inherits("ColorCube"))
         {
           m_frame++;
           QVector3D lightcolor;
            lightcolor.setX(sin(m_frame*2.0f));
            lightcolor.setY(sin(m_frame*0.7f));
            lightcolor.setZ(sin(m_frame*1.3f));
            Shape* temp=*i;
            ColorCube* tempcube= qobject_cast<ColorCube *>(temp);
            tempcube->SetLightColor(lightcolor);

         }
     }

     update();
 }


 void MyGLWidget::keyPressEvent(QKeyEvent *event)
 {
     switch (event->key()) {
     case Qt::Key_W:
     {
         m_camera->ProcessKeyboard(FORWARD,deltaTime);
         break;
     }
     case Qt::Key_S:
     {
         m_camera->ProcessKeyboard(BACKWARD,deltaTime);
         break;
     }
     case Qt::Key_D:
     {
         m_camera->ProcessKeyboard(LEFT,deltaTime);
         break;
     }
     case Qt::Key_A:
     {
         m_camera->ProcessKeyboard(RIGHT,deltaTime);
         break;
     }
     case Qt::Key_Up:
     {
         m_camera->ProcessKeyboard(UP,deltaTime);
         break;
     }
     case Qt::Key_Down:
     {
         m_camera->ProcessKeyboard(DOWN,deltaTime);
         break;
     }
     default:
     {
         break;
     }
     }
     update();
 }

 void MyGLWidget::keyReleaseEvent(QKeyEvent *event)
 {
     Q_UNUSED(event);
     qDebug()<<"keyrelease";
 }

 void MyGLWidget::mousePressEvent(QMouseEvent *event)
 {
  if(Qt::LeftButton== event->button())
  {
      lastX =event->pos().x();
      lastY = event->pos().y();
      leftbuttonpressed=true;
      update();
  }
 }

 void MyGLWidget::mouseReleaseEvent(QMouseEvent *event)
 {
      Q_UNUSED(event);
    leftbuttonpressed=false;
 }

 void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
 {
   if(leftbuttonpressed)
   {
       int xpos = event->pos().x();
       int ypos = event->pos().y();
       int xoffset = xpos - lastX;
       int yoffset =lastY - ypos;// reversed since y-coordinates go from bottom to top
       lastX= event->pos().x();
       lastY= event->pos().y();
       m_camera->ProcessMouseMovement(-1*xoffset, -yoffset);
       update();
   }

 }

 void MyGLWidget::wheelEvent(QWheelEvent *event)
 {
     QPoint offset = event->angleDelta();
     m_camera->ProcessMouseScroll(offset.y()/20.0f);
     update();
 }

 void MyGLWidget::EnableColorCube()
 {
     QVector<Shape*>::iterator i;
     for(i=shapevec.begin();i!=shapevec.end();++i)
     {
         if((*i)->inherits("ColorCube"))
         {
             (*i)->ChangeVisible(true);
         }
         else if ((*i)->inherits("LightCube")) {
               (*i)->ChangeVisible(true);
         }
         else {
             (*i)->ChangeVisible(false);
         }
     }
 }
 void MyGLWidget::SetAimbientValue(float value)
 {
     QVector<Shape*>::iterator i;
     for(i=shapevec.begin();i!=shapevec.end();++i)
     {
         if((*i)->inherits("ColorCube"))
         {
             Shape* temp=*i;
             ColorCube* tempcube= qobject_cast<ColorCube *>(temp);
             tempcube->SetAmbientStrength(value);

         }
     }
 }

    void MyGLWidget::SetSpecularValue(float value)
    {
        QVector<Shape*>::iterator i;
        for(i=shapevec.begin();i!=shapevec.end();++i)
        {
            if((*i)->inherits("ColorCube"))
            {
                Shape* temp=*i;
                ColorCube* tempcube= qobject_cast<ColorCube *>(temp);
                tempcube->SetSpecularStrength(value);

            }
        }
    }



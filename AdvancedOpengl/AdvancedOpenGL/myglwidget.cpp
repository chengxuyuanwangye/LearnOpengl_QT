#include "myglwidget.h"
#include<QTimer>
#include<QMatrix4x4>
#include<QOpenGLShaderProgram>
#include<QObject>
#include<QDebug>
#include"shape.h"
#include"cube.h"
#include"camera.h"
#include<QKeyEvent>
#include<QMouseEvent>
#include<QPainter>
#include<math.h>
#include"plane.h"
MyGLWidget::MyGLWidget(QWidget *parent):
    QOpenGLWidget (parent),
    animateflag(false),
    firstMouse(true),
    leftbuttonpressed(false),
    deltaTime(0.0f),
    lastFrame(0.0f),
    m_frame(0),
    curdepthfunc(DEPTHFUNC::LESS)
{
    m_timer=new QTimer(this);
    m_timer->setInterval(60);
    m_camera=new Camera(QVector3D(0.0f,0.0f,3.0f));//3.0
    lastX = width() / 2.0f;
    lastY = height() / 2.0f;
    //test
    deltaTime=1.0f;
    setFocusPolicy(Qt::StrongFocus);
    connect(m_timer,&QTimer::timeout,this,&MyGLWidget::timeoutFunc);

}


MyGLWidget::~MyGLWidget()
{
    makeCurrent();
    qDeleteAll(shapevec);
    delete m_camera;
    doneCurrent();
}

void MyGLWidget::initializeGL()
{
    // 为当前环境初始化OpenGL函数
   initializeOpenGLFunctions();

   Cube* cub=new Cube(width(),height());
   shapevec.append(cub);
   cub->ShapeCamera=m_camera;
   cub->SetTranslateVec(QVector3D(-1.0f, 0.0f, -1.0f));

   Cube* cub2=new Cube(width(),height());
   shapevec.append(cub2);
   cub2->ShapeCamera=m_camera;
   cub2->SetTranslateVec(QVector3D(2.0f, 0.0f, 0.0f));

   Plane *plane=new Plane(width(),height());
   shapevec.append(plane);
   plane->ShapeCamera=m_camera;
  // plane->SetTranslateVec(QVector3D(0.0f, 0.0f, 0.0f));
   //glEnable(GL_DEPTH_TEST);
 glEnable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
     qDebug()<<"paintgl";

    /*清空颜色缓存，深度缓存，模板缓存*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    switch (curdepthfunc) {
    case DEPTHFUNC::LESS:
    {
        glDepthFunc(GL_LESS);
        break;
    }
    case DEPTHFUNC::ALWAYS:
    {
        glDepthFunc(GL_ALWAYS);
        qDebug()<<"always";
        break;
    }
    case DEPTHFUNC::NEVER:
    {
        glDepthFunc(GL_NEVER);
         qDebug()<<"never";
        break;
    }
    case DEPTHFUNC::EQUAL:
    {
        glDepthFunc(GL_EQUAL);
        break;
    }
    case DEPTHFUNC::LEQUAL:
    {
        glDepthFunc(GL_LEQUAL);
        break;
    }
    case DEPTHFUNC::GREATER:
    {
        glDepthFunc(GL_GREATER);
        break;
    }
    case DEPTHFUNC::NOTEQUAL:
    {
        glDepthFunc(GL_NOTEQUAL);
        break;
    }
    case DEPTHFUNC::GEQUAL:
    {
        glDepthFunc(GEQUAL);
        break;
    }

    }
    QVector<Shape*>::iterator i;
    for(i=shapevec.begin();i!=shapevec.end();++i)
    {
        (*i)->Render();
    }
    //glDisable(GL_DEPTH_TEST);

}

void MyGLWidget::EnableCube()
{
    QVector<Shape*>::iterator i;
    for(i=shapevec.begin();i!=shapevec.end();++i)
    {
        (*i)->ChangeVisible(true);
    }
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
        if((*i)->inherits("Cube"))
         {
             Shape* temp=*i;
             Cube* tempcube= qobject_cast<Cube *>(temp);
             tempcube->Animate();
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


 void MyGLWidget::SetDepthFunc(DEPTHFUNC depthfunc)
 {
     curdepthfunc=depthfunc;


 }


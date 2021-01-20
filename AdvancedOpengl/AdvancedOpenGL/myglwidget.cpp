#include "myglwidget.h"
#include<QTimer>
#include<QMatrix4x4>
#include<QOpenGLShaderProgram>
#include<QObject>
#include<QDebug>
#include <iostream>
#include"shape.h"
#include"cube.h"
#include"camera.h"
#include<QKeyEvent>
#include<QMouseEvent>
#include<QPainter>
#include<math.h>
#include"plane.h"
#include"cubeframe.h"
#include"vegetation.h"
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
    plane=nullptr;
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
    qDeleteAll(framevec);
    qDeleteAll(cubevec);
    qDeleteAll(grassvec);
    if(plane!=nullptr)
    delete plane;
    delete m_camera;

    doneCurrent();
}

void MyGLWidget::initializeGL()
{
    // 为当前环境初始化OpenGL函数
    initializeOpenGLFunctions();

    Cube* cub=new Cube(width(),height());
    cubevec.append(cub);
    cub->ShapeCamera=m_camera;
    cub->SetTranslateVec(QVector3D(-1.0f, 0.0f, -1.0f));

   /* CubeFrame* frame1=new CubeFrame(width(),height());
    framevec.append(frame1);
    frame1->ShapeCamera=m_camera;
    frame1->SetTranslateVec(QVector3D(-1.0f, 0.0f, -1.0f));
    frame1->SetScale(1.1f);*/

    Cube* cub2=new Cube(width(),height());
    cubevec.append(cub2);
    cub2->ShapeCamera=m_camera;
    cub2->SetTranslateVec(QVector3D(2.0f, 0.0f, 0.0f));

  /*  CubeFrame* frame2=new CubeFrame(width(),height());
    framevec.append(frame2);
    frame2->ShapeCamera=m_camera;
    frame2->SetTranslateVec(QVector3D(2.0f, 0.0f, 0.0f));
    frame2->SetScale(1.1f);*/


    windows.push_front(QVector3D(-1.5f, 0.0f, -0.48f));
    windows.push_front( QVector3D( 1.5f, 0.0f, 0.51f));
    windows.push_front(QVector3D( 0.0f, 0.0f, 0.7f));
    windows.push_front(QVector3D(-0.3f, 0.0f, -2.3f));
    windows.push_front(QVector3D (0.5f, 0.0f, -0.6f));

     QVector<QVector3D>::iterator i;

     for(i=windows.begin();i!=windows.end();++i)
     {
         Vegetation * temp=new Vegetation(width(),height()) ;
         grassvec.append(temp);
         temp->ShapeCamera=m_camera;
         temp->SetTranslateVec(*i);
     }


    plane=new Plane(width(),height());
    plane->ShapeCamera=m_camera;
    plane->ChangeVisible(true);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //  glEnable(GL_STENCIL_TEST);
  //  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  //  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //*模板测试和深度测试都通过时将模板纸设置为glStencilFunc函数设置的ref值
}

void MyGLWidget::paintGL()
{

    /*清空颜色缓存，深度缓存，模板缓存*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //此时所有片段的模板缓冲内都是0
   // glEnable(GL_STENCIL_BUFFER_BIT);

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
  //  glStencilMask(0x00);//1.1禁止写入模板缓冲
    plane->Render();//1.2绘制地板平面，此时不会影响模板缓冲，测试总是通过

   // glStencilFunc(GL_ALWAYS, 1, 0xFF);//2.1设置模板测试函数，当前总是通过测试，并将对应片段的模板缓冲设置为1
  //  glStencilMask(0xFF);//2.2 开启模板缓冲

    QVector<Shape*>::iterator i;
    for(i=cubevec.begin();i!=cubevec.end();++i)
    {
        //2.2绘制箱子，此时其对应的片段的模板缓冲是1 ，其余部分仍旧为0
        (*i)->Render();
    }

    //排序
    std::map<float,QVector3D> sorted;
    for(int j=0;j<windows.size();j++)
    {
        QVector3D disvec=m_camera->Position-windows[j];
        float distance=disvec.lengthSquared();
        sorted[distance] = windows[j];

    }
    int count=0;
    for(std::map<float,QVector3D>::reverse_iterator  it=sorted.rbegin();it!=sorted.rend();++it)
    {
        grassvec[count]->SetTranslateVec(it->second);
        grassvec[count]->Render();
        count++;
    }
   /*   for(i=grassvec.begin();i!=grassvec.end();++i)
    {
         (*i)->Render();

    }*/


   /* glStencilFunc(GL_NOTEQUAL, 1, 0xFF);  //3.1 模板测试的通过条件是不等于1
    glStencilMask(0x00);//3.2禁止此后的操作写入模板缓冲
    glDisable(GL_DEPTH_TEST);
    for(i=framevec.begin();i!=framevec.end();++i)
    {
        //3.3绘制单色边框，此时片段中模板缓冲为1（也就是2.2中绘制箱子部分对应的片段）无法通过模板测试，因此将不会绘制
        (*i)->Render();
    }*/
   // glStencilMask(0xFF);
   // glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
}

void MyGLWidget::EnableCube()
{
    QVector<Shape*>::iterator i;
    for(i=cubevec.begin();i!=cubevec.end();++i)
    {
        (*i)->ChangeVisible(true);
    }
    for(i=framevec.begin();i!=framevec.end();++i)
    {
         (*i)->ChangeVisible(true);
    }
    for(i=grassvec.begin();i!=grassvec.end();++i)
    {
         (*i)->ChangeVisible(true);
    }
}


void MyGLWidget::resizeGL(int width, int height)
{
const qreal retinascale=devicePixelRatio();
glViewport(0,0,width*retinascale,height*retinascale);

QVector<Shape*>::iterator i;
for(i=cubevec.begin();i!=cubevec.end();++i)
{
    (*i)->Resize(width,height);
}
for(i=framevec.begin();i!=framevec.end();++i)
{
     (*i)->Resize(width,height);
}
for(i=grassvec.begin();i!=grassvec.end();++i)
{
   (*i)->Resize(width,height);
}

plane->Resize(width,height);
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
     for(i=cubevec.begin();i!=cubevec.end();++i)
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


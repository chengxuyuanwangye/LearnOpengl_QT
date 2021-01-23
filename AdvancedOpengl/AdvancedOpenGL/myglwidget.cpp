#include "myglwidget.h"
#include<QTimer>
#include<QMatrix4x4>
#include<QOpenGLShaderProgram>
#include<QOpenGLFunctions_3_3_Core>
#include <QOpenGLExtraFunctions>
#include<QOpenGLTexture>
#include <QOpenGLFramebufferObject>
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

    m_vbo=nullptr;//顶点缓冲对象
    m_vao=nullptr;//顶点数组对象
    m_program=nullptr;
    fbo=nullptr;
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
  //  delete screenTexture;
    if(m_vbo!=nullptr)delete m_vbo;
    if(m_vao!=nullptr)delete m_vao;
    delete m_program;
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

    m_program = new QOpenGLShaderProgram(this);
    bool success = m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/framebuffer.vert");
     if (!success) {
         qDebug() << "cube addShaderFromSourceFile failed!" << m_program->log();
         return;
     }

     //加载片段着色器程序
   success = m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/framebuffer.frag");
   if (!success) {
       qDebug() << "cube addShaderFromSourceFile failed!" << m_program->log();
       return;
   }
     //链接着色器程序
     success = m_program->link();

     if(!success) {
            qDebug() << "shaderProgram link failed!" << m_program->log();
        }

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
          // positions   // texCoords
          -1.0f,  1.0f,  0.0f, 1.0f,
          -1.0f, -1.0f,  0.0f, 0.0f,
           1.0f, -1.0f,  1.0f, 0.0f,

          -1.0f,  1.0f,  0.0f, 1.0f,
           1.0f, -1.0f,  1.0f, 0.0f,
           1.0f,  1.0f,  1.0f, 1.0f
      };
    // screen quad VAO
    m_vao=new QOpenGLVertexArrayObject;
    m_vbo=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    QOpenGLVertexArrayObject::Binder vaoBind(m_vao);
    m_vbo->create();
    m_vbo->bind();
    m_vbo->allocate(quadVertices, sizeof(quadVertices));
    int attr = -1;
    attr = m_program->attributeLocation("aPos");
    m_program->setAttributeBuffer(attr, GL_FLOAT, 0, 2, sizeof(GLfloat) * 4);
    m_program->enableAttributeArray(attr);

    int texattr=-1;
    texattr=m_program->attributeLocation("aTexCoords");
    m_program->setAttributeBuffer(texattr,GL_FLOAT,sizeof(GLfloat) * 2,2,sizeof(GLfloat) * 4);
    m_program->enableAttributeArray(texattr);
    m_vbo->release();
    m_vao->release();
    fbo=new QOpenGLFramebufferObject(this->width(),this->height(),QOpenGLFramebufferObject::Depth);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void MyGLWidget::paintGL()
{

    bool result = fbo->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable( GL_CULL_FACE);
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
    plane->Render();
    glEnable( GL_CULL_FACE);
    QVector<Shape*>::iterator i;
    for(i=cubevec.begin();i!=cubevec.end();++i)
    {
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
     glDisable(GL_CULL_FACE);
    int count=0;
    for(std::map<float,QVector3D>::reverse_iterator  it=sorted.rbegin();it!=sorted.rend();++it)
    {
        grassvec[count]->SetTranslateVec(it->second);
        grassvec[count]->Render();
        count++;
    }

    if(result)
    {
        GLuint textureid=fbo->texture();
        fbo->release();
      //   QImage img= fbo->toImage();
      //   img.save("rendertotexture.jpg");
        m_program->bind();
        {
         // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureid);
        glUniform1i(m_program->uniformLocation("screenTexture"), 0);
        QMatrix4x4 view=m_camera->GetViewMatrix();
        m_program->setUniformValue("view", view);

        QMatrix4x4 projection;
        projection.perspective(m_camera->Zoom, 1.0f * this->width() /this->height(), 0.1f, 100.0f);
        m_program->setUniformValue("projection", projection);

        QOpenGLVertexArrayObject::Binder vaoBind(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        }
       m_program->release();

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


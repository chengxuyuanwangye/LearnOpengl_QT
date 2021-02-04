#include "myglwidget.h"
#include<QTimer>
#include<QMatrix4x4>
#include<QOpenGLShaderProgram>
#include<QOpenGLFunctions_3_3_Core>
#include<QOpenGLExtraFunctions>
#include<QOpenGLTexture>
#include<QOpenGLFramebufferObject>
#include<QObject>
#include<QDebug>
#include<iostream>
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
#include"skycube.h"
#include"room.h"
#include"multirect.h"
#include "catmullromspline.h"
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
    fbo=nullptr;
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
    delete sky;
    if(m_vbo!=nullptr)delete m_vbo;
    if(m_vao!=nullptr)delete m_vao;
    if(fbo!=nullptr)delete  fbo;
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
    CatmullRomSpline* cromspline=new CatmullRomSpline(width(),height());
    cubevec.append(cromspline);

    //  MultiRect* rect=new MultiRect(width(),height(),QOpenGLContext::currentContext());
    //  cubevec.append(rect);

    windows.push_front(QVector3D(-1.5f, 0.0f, -0.48f));
    windows.push_front(QVector3D( 1.5f, 0.0f, 0.51f));
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

    sky=new SkyCube(width(),height());
    sky->ShapeCamera=m_camera;
    sky->ChangeVisible(true);
    //创建FBO相关
    bool programflag=CreateFBOShaderProgram();
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
    if(programflag)
    {
        m_vao=new QOpenGLVertexArrayObject;
        m_vao->bind();
        m_vbo=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_vbo->create();
        m_vbo->bind();
        m_vbo->allocate(quadVertices,sizeof(quadVertices));
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

        //MultiSampling set to 4 now
        QOpenGLFramebufferObjectFormat muliSampleFormat;
        muliSampleFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        muliSampleFormat.setMipmap(true);
        muliSampleFormat.setSamples(4);
        muliSampleFormat.setTextureTarget(GL_TEXTURE_2D);
        muliSampleFormat.setInternalTextureFormat(GL_RGBA32F_ARB);

        //fbo=new QOpenGLFramebufferObject(this->width(),this->height(),QOpenGLFramebufferObject::Depth);
        fbo=new QOpenGLFramebufferObject(this->width(),this->height(),muliSampleFormat);

        QOpenGLFramebufferObjectFormat downSampledFormat;
        downSampledFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        downSampledFormat.setMipmap(true);
        downSampledFormat.setTextureTarget(GL_TEXTURE_2D);
        downSampledFormat.setInternalTextureFormat(GL_RGBA32F_ARB);

        renderfbo=new QOpenGLFramebufferObject(this->width(),this->height(),downSampledFormat);
         bool  bValid = fbo->isValid();
         qDebug()<<"bValid="<<bValid;
    }
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);

}

void MyGLWidget::paintGL()
{
    if(fbo!=nullptr)
    {
        bool result = fbo->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        QVector<Shape*>::iterator i;
        for(i=cubevec.begin();i!=cubevec.end();++i)
        {
            (*i)->Render();
        }
        glDepthFunc(GL_LEQUAL);
        sky->Render();
        glDepthFunc(GL_LESS); // set depth function back to default
        glDisable(GL_CULL_FACE);
        if(result)
        {
          //  GLuint textureid=fbo->texture();

            fbo->release();
            renderfbo->bind();
            QOpenGLFramebufferObject::blitFramebuffer(renderfbo,fbo,GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT,GL_NEAREST);
            renderfbo->release();
            GLuint textureid=renderfbo->texture();

            m_program->bind();
            {
                // bind textures on corresponding texture units
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textureid);
                //GLenum glerr= glGetError();
                glUniform1i(m_program->uniformLocation("screenTexture"), 0);
                m_vao->bind();
                glDrawArrays(GL_TRIANGLES, 0, 6);
                m_vao->release();

            }
            m_program->release();
        }

    }
    else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //plane->Render();
        QVector<Shape*>::iterator i;
        for(i=cubevec.begin();i!=cubevec.end();++i)
        {
            (*i)->Render();
        }
        glDepthFunc(GL_LEQUAL);
        sky->Render();
        glDepthFunc(GL_LESS); // set depth function back to default

    }
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
    sky->Resize(width,height);
}


void MyGLWidget::StartAnimate(bool flag)
 {
    Q_UNUSED(flag);
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

 void  MyGLWidget::DepthFunc()
  {
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

  }

  bool MyGLWidget::CreateFBOShaderProgram()
  {
      m_program = new QOpenGLShaderProgram(this);
      bool success = m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/framebuffer.vert");
      if (!success) {
          qDebug() << "FBO addShaderFromSourceFile failed!" << m_program->log();
          return false;
      }

      //加载片段着色器程序
      success = m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/framebuffer.frag");
      if (!success) {
          qDebug() << "FBO addShaderFromSourceFile failed!" << m_program->log();
          return false;
      }
      //链接着色器程序
      success = m_program->link();

      if(!success) {
          qDebug() << "shaderProgram link failed!" << m_program->log();
          return false;
      }
      return success;

  }

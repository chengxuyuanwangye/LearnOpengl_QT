#include "room.h"
#include<QOpenGLFunctions>
#include<QOpenGLShaderProgram>
#include<QDebug>
#include"math.h"

Room::Room(int width,int height):
    Shape(width,height)
{
    m_vbo=nullptr;//顶点缓冲对象
    m_ebo=nullptr;//索引缓冲对象
    m_vao=nullptr;//顶点数组对象
    m_program=nullptr;
   initializeOpenGLFunctions();
   m_program = new QOpenGLShaderProgram(this);
   bool success = m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/room.vert");
    if (!success) {
        qDebug() << "vert: addShaderFromSourceFile failed!" << m_program->log();

    }

    //加载片段着色器程序
  success = m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/room.frag");
  if (!success) {
      qDebug() << " frag:addShaderFromSourceFile failed!" << m_program->log();

  }

  success = m_program->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shader/room.geom");
  if (!success) {
      qDebug() << "geo: addShaderFromSourceFile failed!" << m_program->log();

  }
    //链接着色器程序
    success = m_program->link();

    if(!success) {
           qDebug() << "shaderProgram link failed!" << m_program->log();
       }
 if(!success)return;

 float points[] = {
     -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
      0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
     -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
 };
 m_vao=new QOpenGLVertexArrayObject;
 m_vbo=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

        QOpenGLVertexArrayObject::Binder vaobind(m_vao);
 m_vbo->create();
 m_vbo->bind();
m_vbo->allocate(points, sizeof(points));

int attr = -1;
attr = m_program->attributeLocation("aPos");
m_program->setAttributeBuffer(attr, GL_FLOAT, 0, 2, sizeof(GLfloat) * 5);
m_program->enableAttributeArray(attr);
attr = m_program->attributeLocation("aColor");
m_program->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 5);
m_program->enableAttributeArray(attr);

m_vao->release();
m_vbo->release();


}
  Room::~Room()
  {

  }


  void Room::Render()
  {
     // if(m_visible)
    //  {
          glEnable(GL_PROGRAM_POINT_SIZE);
          m_program->bind();
          {

              QOpenGLVertexArrayObject::Binder vaoBind(m_vao);
              glDrawArrays(GL_POINTS, 0, 4);

          }
          m_program->release();
       //   glDisable(GL_PROGRAM_POINT_SIZE);

   //   }

  }
   void Room::Resize(int width, int height)
   {
      Q_UNUSED(width);
      Q_UNUSED(height);
   }
   void Room::SetTranslateVec(QVector3D vec)
   {
      Q_UNUSED(vec);
   }

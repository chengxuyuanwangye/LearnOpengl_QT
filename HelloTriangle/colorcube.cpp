#include "colorcube.h"
#include<QOpenGLFunctions>
#include<QOpenGLShaderProgram>
#include<QDebug>
#include<QOpenGLTexture>
#include<QTimer>
#include"camera.h"
ColorCube::ColorCube(int width,int height):
    Shape(width,height),
    m_frame(0),
    m_modeltransmat(0,0,0)
{
  initializeOpenGLFunctions();
  m_program = new QOpenGLShaderProgram(this);
  bool success = m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/colorcube.vert");
   if (!success) {
       qDebug() << "cube addShaderFromSourceFile failed!" << m_program->log();
       return;
   }

   //加载片段着色器程序
 success = m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/colorcube.frag");
 if (!success) {
     qDebug() << "cube addShaderFromSourceFile failed!" << m_program->log();
     return;
 }
   //链接着色器程序
   success = m_program->link();

   if(!success) {
          qDebug() << "shaderProgram link failed!" << m_program->log();
      }

//VAO，VBO数据部分
   float vertices[] = {
           -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
           -0.5f,  0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f,

           -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,
           -0.5f, -0.5f,  0.5f,

           -0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f,
           -0.5f, -0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,

           -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
           -0.5f, -0.5f,  0.5f,
           -0.5f, -0.5f, -0.5f,

           -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f, -0.5f,
       };

   m_vao=new QOpenGLVertexArrayObject;
   m_vbo=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

   QOpenGLVertexArrayObject::Binder vaoBind(m_vao);
   m_vbo->create();
   m_vbo->bind();
   m_vbo->allocate(vertices, sizeof(vertices));
   int attr = -1;
   attr = m_program->attributeLocation("aPos");
   m_program->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
   m_program->enableAttributeArray(attr);
   m_vbo->release();
   m_vao->release();
   m_program->bind();
   QMatrix4x4 model;
   model.setToIdentity();
   m_program->setUniformValue("model", model);
   QMatrix4x4 view;
   view.setToIdentity();
   float radius = 5.0f;
   float camX   = sin(100.0f * m_frame /120) * radius;
   float camZ   = cos(100.0f * m_frame /120) * radius;
   view.lookAt(QVector3D(camX,0.0f,camZ),QVector3D(0.0f,0.0f,0.0f),QVector3D(0.0f,1.0f,0.0f));
   m_program->setUniformValue("view", view);
   QMatrix4x4 projection;
   projection.perspective(45.0f, 1.0f * width / height, 0.1f, 100.0f);
   m_program->setUniformValue("projection", projection);

   m_program->setUniformValue("objectcolor", QVector3D(1.0f, 0.5f, 0.31f));
   m_program->setUniformValue("lightcolor", QVector3D(1.0f, 1.0f, 1.0f));
   m_program->release();
}

ColorCube::~ColorCube()
{

}
void ColorCube::Render()
{
    if(m_visible)
    {
    m_program->bind();
    {

    QMatrix4x4 view=ShapeCamera->GetViewMatrix();
    m_program->setUniformValue("view", view);

    QMatrix4x4 projection;
    projection.perspective(ShapeCamera->Zoom, 1.0f * m_width / m_height, 0.1f, 100.0f);
    m_program->setUniformValue("projection", projection);

    QOpenGLVertexArrayObject::Binder vaoBind(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    }
    m_program->release();
    }

}

void  ColorCube::Resize(int width, int height)
{
    m_width=width;
    m_height=height;
    m_program->bind();
   {
      QMatrix4x4 projection;
      projection.perspective(ShapeCamera->Zoom, 1.0f * width / height, 0.1f, 100.0f);
      m_program->setUniformValue("projection", projection);
   }
    m_program->release();

}

void ColorCube::Animate()
{
    m_frame++;
}

 void ColorCube::SetTranslate(QVector3D trans)
 {
     m_modeltransmat.setX(trans.x());
     m_modeltransmat.setY(trans.y());
     m_modeltransmat.setZ(trans.z());

     m_program->bind();
     {
         QMatrix4x4 model;
         model.translate(m_modeltransmat);
         m_program->setUniformValue("model", model);
     }
     m_program->release();


 }



#include "lightcube.h"
#include<QOpenGLFunctions>
#include<QOpenGLShaderProgram>
#include<QDebug>
#include<QOpenGLTexture>
#include<QTimer>
#include"camera.h"
LightCube::LightCube(int width,int height):
    Shape(width,height),
    m_modeltransmat(0,0,0),
    m_modelsaclemat(1.0f,1.0f,1.0f)
{
  initializeOpenGLFunctions();
  m_program = new QOpenGLShaderProgram(this);
  bool success = m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/lightcube.vert");
   if (!success) {
       qDebug() << "cube addShaderFromSourceFile failed!" << m_program->log();
       return;
   }

   //加载片段着色器程序
 success = m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/lightcube.frag");
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
   m_program->setUniformValue("view", view);
   QMatrix4x4 projection;
   projection.perspective(45.0f, 1.0f * width / height, 0.1f, 100.0f);
   m_program->setUniformValue("projection", projection);
   m_program->release();
}

LightCube::~LightCube()
{

}
void LightCube::Render()
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

void  LightCube::Resize(int width, int height)
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



 void LightCube::SetTranslate(QVector3D trans)
 {
     m_modeltransmat.setX(trans.x());
     m_modeltransmat.setY(trans.y());
     m_modeltransmat.setZ(trans.z());

     UpDateModel();


 }
 void LightCube::SetScale(QVector3D scale)
 {
     m_modelsaclemat.setX(scale.x());
     m_modelsaclemat.setY(scale.y());
     m_modelsaclemat.setZ(scale.z());

     UpDateModel();
 }

 void LightCube::UpDateModel()
 {
     m_program->bind();
     {
         QMatrix4x4 model;
         model.translate(m_modeltransmat);
         model.scale(m_modelsaclemat);
         m_program->setUniformValue("model", model);
     }
     m_program->release();
 }

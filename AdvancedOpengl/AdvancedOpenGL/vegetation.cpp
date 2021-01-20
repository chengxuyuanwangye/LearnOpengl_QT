#include "vegetation.h"
#include<QOpenGLFunctions>
#include<QOpenGLShaderProgram>
#include<QDebug>
#include<QOpenGLTexture>
#include<QTimer>
#include"camera.h"
#include"math.h"

Vegetation::Vegetation(int width, int height):
     Shape(width,height)
{
    ourtexture=nullptr;
     m_vbo=nullptr;//顶点缓冲对象
     m_ebo=nullptr;//索引缓冲对象
     m_vao=nullptr;//顶点数组对象
     m_program=nullptr;
    initializeOpenGLFunctions();
    m_program = new QOpenGLShaderProgram(this);
    bool success = m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vegetation.vert");
     if (!success) {
         qDebug() << "cube addShaderFromSourceFile failed!" << m_program->log();
         return;
     }

     //加载片段着色器程序
   success = m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/vegetation.frag");
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
     GLfloat vertices[] = {
         // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
         0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
         0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
         1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

         0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
         1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
         1.0f,  0.5f,  0.0f,  1.0f,  0.0f
     };

     m_vao=new QOpenGLVertexArrayObject;
     m_vbo=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

     QOpenGLVertexArrayObject::Binder vaoBind(m_vao);
     m_vbo->create();
     m_vbo->bind();
     m_vbo->allocate(vertices, sizeof(vertices));
     int attr = -1;
     attr = m_program->attributeLocation("aPos");
     m_program->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 5);
     m_program->enableAttributeArray(attr);

     int texattr=-1;
     texattr=m_program->attributeLocation("aTexCoord");
     m_program->setAttributeBuffer(texattr,GL_FLOAT,sizeof(GLfloat) * 3,2,sizeof(GLfloat) * 5);
     m_program->enableAttributeArray(texattr);
     m_vbo->release();
     m_vao->release();
     ourtexture=new QOpenGLTexture(QImage(":/img/grass.png"), QOpenGLTexture::GenerateMipMaps);
     if(!ourtexture->isCreated())
     {
         qDebug()<<"failed to load texture";
     }
     else {
         ourtexture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
         ourtexture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
         ourtexture->setMinificationFilter(QOpenGLTexture::Linear);
         ourtexture->setMagnificationFilter(QOpenGLTexture::Linear);
     }

     m_program->bind();
     QMatrix4x4 model;
     model.setToIdentity();
     m_program->setUniformValue("model", model);
     QMatrix4x4 view;
     view.setToIdentity();
     //float radius = 5.0f;
     float camX   = 0;
     float camZ   = 5;
     view.lookAt(QVector3D(camX,0.0f,camZ),QVector3D(0.0f,0.0f,0.0f),QVector3D(0.0f,1.0f,0.0f));
     m_program->setUniformValue("view", view);
     QMatrix4x4 projection;
     projection.perspective(45.0f, 1.0f * width / height, 0.1f, 100.0f);
     m_program->setUniformValue("projection", projection);
     m_program->setUniformValue("ourTexture", 0);
     m_program->release();
}
Vegetation::~Vegetation()
{
     delete  ourtexture;
}
void Vegetation::Render()
{
    if(m_visible)
    {
    m_program->bind();
    {
    glActiveTexture(GL_TEXTURE0);
    ourtexture->bind();
    QMatrix4x4 view=ShapeCamera->GetViewMatrix();
    m_program->setUniformValue("view", view);

    QMatrix4x4 projection;
    projection.perspective(ShapeCamera->Zoom, 1.0f * m_width / m_height, 0.1f, 100.0f);
    m_program->setUniformValue("projection", projection);

    QOpenGLVertexArrayObject::Binder vaoBind(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    ourtexture->release();
    }
    m_program->release();
    }

}

void  Vegetation::Resize(int width, int height)
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
void Vegetation::SetTranslateVec(QVector3D vec)
{
    m_program->bind();
    {
        QMatrix4x4 model;
        model.translate(vec);
        m_program->setUniformValue("model", model);
    }
    m_program->release();
}





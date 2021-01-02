#include "rectangle.h"
#include<QOpenGLFunctions>
#include<QOpenGLShaderProgram>
#include<QDebug>
#include<QOpenGLTexture>
MyRectangle::MyRectangle(int width,int height)
       :Shape(width,height)
{
     initializeOpenGLFunctions();
     m_program = new QOpenGLShaderProgram(this);
     bool success = m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/myrectangle.vert");
      if (!success) {
          qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_program->log();
          return;
      }
      //加载片段着色器程序
    success = m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/myrectangle.frag");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_program->log();
        return;
    }
    success = m_program->link();

    if(!success) {
           qDebug() << "shaderProgram link failed!" << m_program->log();
       }

    GLfloat vertices[] = {
        //positions            //texture coords
        0.5f,  0.5f, 0.0f,    1.0f,1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f,0.0f,// bottom right
        -0.5f, -0.5f, 0.0f,   0.0f,0.0f,// bottom left
        -0.5f,  0.5f, 0.0f,   0.0f,1.0f// top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    m_vao=new QOpenGLVertexArrayObject;
    m_vbo=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_ebo=new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    QOpenGLVertexArrayObject::Binder vaoBind(m_vao);

    m_vbo->create();
    m_vbo->bind();
    m_vbo->allocate(vertices, sizeof(vertices));

    m_ebo->create();
    m_ebo->bind();
    m_ebo->allocate(indices, sizeof(indices));

    int attr = -1;
    attr = m_program->attributeLocation("aPos");
    m_program->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 5);
    m_program->enableAttributeArray(attr);

    int texattr=-1;
    texattr=m_program->attributeLocation("aTexCoord");
    m_program->setAttributeBuffer(texattr,GL_FLOAT,sizeof(GLfloat) * 3,2,sizeof(GLfloat) * 5);
    m_program->enableAttributeArray(texattr);

    m_vbo->release();

    ourtexture=new QOpenGLTexture(QImage(":/img/wall.jpg"), QOpenGLTexture::GenerateMipMaps);
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
    view.translate(QVector3D(0.0f,0.0f,-1.0f));
    m_program->setUniformValue("view", view);
    QMatrix4x4 projection;
    projection.setToIdentity();
    float temp=m_width>m_height?m_width:m_height;
    projection.ortho(-1*m_width/temp, m_width/temp, -1*m_height/temp, m_height/temp, 0.1f, 100.0f);
    m_program->setUniformValue("projection", projection);
    m_program->setUniformValue("ourTexture", 0);
    m_program->release();


}

MyRectangle::~MyRectangle()
{
  delete  ourtexture;
}
void MyRectangle::Render()
{
    if(m_visible)
    {
    m_program->bind();
    {

    glActiveTexture(GL_TEXTURE0);
    ourtexture->bind();
    QOpenGLVertexArrayObject::Binder vaoBind(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
     m_vao->release();
     ourtexture->release();
    }
    m_program->release();
    }

}

void  MyRectangle::Resize(int width, int height)
{
    m_width=width;
    m_height=height;
    m_program->bind();
   {
      QMatrix4x4 projection;
      projection.setToIdentity();
     // projection.perspective(45.0f, 1.0f * width / height, 0.1f, 100.0f);
      float temp=m_width>m_height?m_width:m_height;
        projection.ortho(-1*m_width/temp, m_width/temp, -1*m_height/temp, m_height/temp, 0.1f, 100.0f);
      m_program->setUniformValue("projection", projection);
   }
    m_program->release();

}

#include "colorcube.h"
#include<QOpenGLFunctions>
#include<QOpenGLShaderProgram>
#include<QDebug>
#include<QOpenGLTexture>
#include<QTimer>
#include"camera.h"
ColorCube::ColorCube(int width,int height):
    Shape(width,height),
    m_modeltransmat(0,0,0),
    m_lightPosition(0,0,0),
    ambientStrength(0.1f),
    Specular(2),
    m_curColor(1.0f, 0.5f, 0.31f),
    m_lightColor(1.0f,1.0f,1.0f)
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
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    m_vao=new QOpenGLVertexArrayObject;
    m_vbo=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    QOpenGLVertexArrayObject::Binder vaoBind(m_vao);
    m_vbo->create();
    m_vbo->bind();
    m_vbo->allocate(vertices, sizeof(vertices));
    int attr = -1;
    attr = m_program->attributeLocation("aPos");
    m_program->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 8);
    m_program->enableAttributeArray(attr);
    int nattr=-1;
    nattr=m_program->attributeLocation("aNormal");
    m_program->setAttributeBuffer(nattr, GL_FLOAT, 3*sizeof(GLfloat), 3, sizeof(GLfloat) * 8);
    m_program->enableAttributeArray(nattr);
    int textureattr=-1;
    textureattr=m_program->attributeLocation("aTexCoords");
    m_program->setAttributeBuffer(textureattr, GL_FLOAT, 6*sizeof(GLfloat), 2, sizeof(GLfloat) * 8);
    m_program->enableAttributeArray(textureattr);

    m_vbo->release();
    m_vao->release();
    //Texture

    ourtexture=new QOpenGLTexture(QImage(":/img/container2.png"), QOpenGLTexture::GenerateMipMaps);
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
    speculartexture=new QOpenGLTexture(QImage(":/img/container2_specular.png"), QOpenGLTexture::GenerateMipMaps);

    if(!speculartexture->isCreated())
    {
        qDebug()<<"failed to load texture";
    }
    else {
        speculartexture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
        speculartexture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
        speculartexture->setMinificationFilter(QOpenGLTexture::Linear);
        speculartexture->setMagnificationFilter(QOpenGLTexture::Linear);
    }

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
    m_program->setUniformValue("ourTexture", 0);
    m_program->setUniformValue("speculartexture", 1);
    m_program->release();
}

ColorCube::~ColorCube()
{
 delete  ourtexture;
 delete speculartexture;
}
void ColorCube::Render()
{
    if(m_visible)
    {
        m_program->bind();
        {
            m_program->setUniformValue("light.position",ShapeCamera->Position);
            m_program->setUniformValue("light.direction",ShapeCamera->Front);
            m_program->setUniformValue("light.cutOff",static_cast<float>(cos(12.5f*3.1415/180)));
            m_program->setUniformValue("light.outerCutOff",static_cast<float>(cos(17.5f*3.1415/180)));


            m_program->setUniformValue("light.ambient", m_lightColor);
            m_program->setUniformValue("light.diffuse", m_lightColor*8);
            m_program->setUniformValue("light.specular", QVector3D(1.0f,1.0f,1.0f));

            m_program->setUniformValue("light.constant", 1.0f);
            m_program->setUniformValue("light.linear", 0.09f);
            m_program->setUniformValue("light.quadratic", 0.032f);

            m_program->setUniformValue("material.diffuse", 0);
            m_program->setUniformValue("material.specular", QVector3D(0.5f,0.5f,0.5f));
            m_program->setUniformValue("material.shininess",Specular);

            glActiveTexture(GL_TEXTURE0);
            ourtexture->bind();
            glActiveTexture(GL_TEXTURE1);
            speculartexture->bind();

            QMatrix4x4 view=ShapeCamera->GetViewMatrix();
            m_program->setUniformValue("view", view);
            //viewPos
            m_program->setUniformValue("viewPos", ShapeCamera->Position);

            QMatrix4x4 projection;
            projection.perspective(ShapeCamera->Zoom, 1.0f * m_width / m_height, 0.1f, 100.0f);
            m_program->setUniformValue("projection", projection);

            QOpenGLVertexArrayObject::Binder vaoBind(m_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            ourtexture->release();
            speculartexture->release();
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

  void ColorCube::SetLightPosition(QVector3D lightpos)
  {


  }
  void  ColorCube:: SetAmbientStrength(float strength)
  {
      ambientStrength=strength;
      m_program->bind();
      {
          m_program->setUniformValue("ambientStrength", ambientStrength);
      }
      m_program->release();
  }

   void ColorCube::SetSpecularStrength(float value)
   {
       Specular=value;
       m_program->bind();
       {
           m_program->setUniformValue("SpecularPara", Specular);
       }
       m_program->release();
   }

   void ColorCube::SetLightColor(QVector3D lightColor)
   {
       m_lightColor=lightColor;
   }

   ///
   /// \brief ColorCube::SetModelMatrix
   /// \param mat
   void ColorCube::SetModelMatrix(QMatrix4x4 mat)
   {
       m_program->bind();
       {
           m_program->setUniformValue("model", mat);
       }
       m_program->release();
   }


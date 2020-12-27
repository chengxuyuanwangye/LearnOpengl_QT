#include "triangle.h"
#include<QOpenGLFunctions>
#include<QOpenGLShaderProgram>
#include<QDebug>
   Triangle::Triangle()
   {
       initializeOpenGLFunctions();

       m_program = new QOpenGLShaderProgram(this);
       //加载顶点着色器程序
       bool success = m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/triangle.vert");
        if (!success) {
            qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_program->log();
            return;
        }
           //加载片段着色器程序
         success = m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/triangle.frag");
         if (!success) {
             qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_program->log();
             return;
         }
           //链接着色器程序
           success = m_program->link();

           if(!success) {
                  qDebug() << "shaderProgram link failed!" << m_program->log();
              }

    //VAO，VBO数据部分
       GLfloat vertices[] = {
           -0.5f,  -0.5f, 0.0f,  // bottom left
            0.5f, -0.5f, 0.0f,  // bottom right
           0.0f, 0.5f, 0.0f,  // top

       };
       unsigned int indices[] = {  // note that we start from 0!
           0, 1, 2,
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
       m_program->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
       m_program->enableAttributeArray(attr);
       m_vbo->release();
   }

   void Triangle::Render()
   {
       if(m_visible)
       {
           m_program->bind();
       {
       QOpenGLVertexArrayObject::Binder vaoBind(m_vao);
       glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        m_vao->release();
       }
       m_program->release();
       }
   }

#include "multirect.h"
#include<QOpenGLFunctions>
#include<QOpenGLShaderProgram>
#include<QDebug>
#include"math.h"
#include <QOpenGLExtraFunctions>
#include<QSurfaceFormat >
#include <QOpenGLVertexArrayObject>

MultiRect::MultiRect(int width,int height, QOpenGLContext *context):
    Shape(width,height)
{
    m_vbo=nullptr;//顶点缓冲对象
    m_ebo=nullptr;//索引缓冲对象
    m_vao=nullptr;//顶点数组对象
    m_program=nullptr;
    initializeOpenGLFunctions();
    _context =context;
    bool success=CreateShaderProgram(":/shader/multirect.vert",":/shader/multirect.frag");
    if(!success)return;

    float quadVertices[] = {
        // 位置          // 颜色
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        0.05f,  0.05f,  0.0f, 1.0f, 1.0f
    };
    m_vao=new QOpenGLVertexArrayObject;
    m_vbo=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    QOpenGLVertexArrayObject::Binder vaobind(m_vao);
    m_vbo->create();
    m_vbo->bind();
    m_vbo->allocate(quadVertices, sizeof(quadVertices));

    int attr = -1;
    attr = m_program->attributeLocation("aPos");
    m_program->setAttributeBuffer(attr, GL_FLOAT, 0, 2, sizeof(GLfloat) * 5);
    m_program->enableAttributeArray(attr);
    attr = m_program->attributeLocation("aColor");
    m_program->setAttributeBuffer(attr, GL_FLOAT, sizeof(GLfloat) * 2, 3, sizeof(GLfloat) * 5);
    m_program->enableAttributeArray(attr);

    m_vao->release();
    m_vbo->release();
    m_program->bind();
    QVector2D translations[100];
    int index = 0;
    float offset = 0.1f;
    for(int y = -10; y < 10; y += 2)
    {
        for(int x = -10; x < 10; x += 2)
        {
            QVector2D translation;
            translation.setX( (float)x / 10.0f + offset);
            translation.setY( (float)y / 10.0f + offset);
            translations[index++] = translation;
        }
    }

    for(unsigned int i = 0; i < 100; i++)
    {
        QString indexstr=QString("offsets[%1]").arg(i);
        QByteArray ba = indexstr.toLatin1();
        m_program->setUniformValue(ba.data(), translations[i]);
    }
    m_program->release();
}




MultiRect::~MultiRect()
{

}


void MultiRect::Render()
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

   // glEnable(GL_PROGRAM_POINT_SIZE);
    m_program->bind();
    {
        m_vao->bind();
        GLenum result= glGetError();
        f->glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
        result= glGetError();
        m_vao->release();
    }
    m_program->release();
}
 void MultiRect::Resize(int width, int height)
 {
    Q_UNUSED(width);
    Q_UNUSED(height);
 }
 void MultiRect::SetTranslateVec(QVector3D vec)
 {
    Q_UNUSED(vec);
 }

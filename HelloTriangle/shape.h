#ifndef SHAPE_H
#define SHAPE_H

#include<QOpenGLBuffer>
#include<QOpenGLVertexArrayObject>
#include<QOpenGLFunctions>
class QOpenGLShaderProgram;

class Shape : public QObject,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    Shape();
    ~Shape();
    virtual void Render()=0;
    void ChangeVisible(bool flag);

protected:
    QOpenGLBuffer* m_vbo;//顶点缓冲对象
    QOpenGLBuffer* m_ebo;//索引缓冲对象
    QOpenGLVertexArrayObject* m_vao;//顶点数组对象
    QOpenGLShaderProgram *m_program;
    bool m_visible;

};

#endif // SHAPE_H

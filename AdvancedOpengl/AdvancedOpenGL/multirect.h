#ifndef MULTIRECT_H
#define MULTIRECT_H
#include<shape.h>
#include <QOpenGLContext>
class MultiRect:public Shape
{
    Q_OBJECT
public:
    MultiRect(int width,int height, QOpenGLContext *context);
    ~MultiRect();
    virtual  void Render();
    virtual void Resize(int width, int height);
    virtual void SetTranslateVec(QVector3D vec);
    private:
     QOpenGLContext *_context;
     QOpenGLBuffer * m_instanceVBO;
};

#endif // MULTIRECT_H

#ifndef RECTANGLE_H
#define RECTANGLE_H
#include"shape.h"
class QOpenGLTexture;
class MyRectangle: public Shape
{
     Q_OBJECT
public:
    MyRectangle(int width,int height);
    ~MyRectangle();
    virtual  void Render();
    virtual void Resize(int width, int height);
private:
    QOpenGLTexture * ourtexture;
};

#endif // RECTANGLE_H

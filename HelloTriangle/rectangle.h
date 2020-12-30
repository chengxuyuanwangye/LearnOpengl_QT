#ifndef RECTANGLE_H
#define RECTANGLE_H
#include"shape.h"
class QOpenGLTexture;
class MyRectangle: public Shape
{
     Q_OBJECT
public:
    MyRectangle();
    ~MyRectangle();
    virtual  void Render();

private:
    QOpenGLTexture * ourtexture;
};

#endif // RECTANGLE_H

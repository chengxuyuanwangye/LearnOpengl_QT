#ifndef CUBE_H
#define CUBE_H

#include"shape.h"
class QOpenGLTexture;
class Cube:public Shape
{
     Q_OBJECT
public:
    Cube(int width,int height);
    ~Cube();
    virtual  void Render();
    virtual void Resize(int width, int height);
public slots:
    void Animate();
private:
    int m_frame;
    QOpenGLTexture * ourtexture;

};

#endif // CUBE_H

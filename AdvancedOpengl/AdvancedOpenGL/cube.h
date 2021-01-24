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
    virtual void SetTranslateVec(QVector3D vec);
public slots:
    void Animate();
private:
    int m_frame;
    QOpenGLTexture * ourtexture;
     void CreateCubeTexture();

};

#endif // CUBE_H

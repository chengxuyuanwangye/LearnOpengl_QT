#ifndef VEGETATION_H
#define VEGETATION_H

#include"shape.h"
class QOpenGLTexture;
class Vegetation:public Shape
{
      Q_OBJECT
public:
    Vegetation(int width,int height);
    ~Vegetation();
    virtual  void Render();
    virtual void Resize(int width, int height);
    virtual void SetTranslateVec(QVector3D vec);

private:
   QOpenGLTexture * ourtexture;
};

#endif // VEGETATION_H

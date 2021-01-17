#ifndef PLANE_H
#define PLANE_H

#include"shape.h"
class QOpenGLTexture;
class Plane:public Shape
{
    Q_OBJECT
public:
   Plane(int width,int height);
   ~Plane();
   virtual  void Render();
   virtual void Resize(int width, int height);
   virtual void SetTranslateVec(QVector3D vec);
public slots:
   void Animate();
private:
   int m_frame;
   QOpenGLTexture * ourtexture;

};

#endif // PLANE_H

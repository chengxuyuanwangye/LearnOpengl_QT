#ifndef CUBEFRAME_H
#define CUBEFRAME_H
#include"shape.h"
#include<QVector3D>
class CubeFrame:public Shape
{
    Q_OBJECT
public:
   CubeFrame(int width,int height);
   ~CubeFrame();
   virtual  void Render();
   virtual void Resize(int width, int height);
   virtual void SetTranslateVec(QVector3D vec);
   void SetScale(float scale);
public slots:
   void Animate();
private:
   int m_frame;
   QVector3D m_trans;
   float m_scale;

};

#endif // CUBEFRAME_H

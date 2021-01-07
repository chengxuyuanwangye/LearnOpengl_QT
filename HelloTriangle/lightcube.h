#ifndef LIGHTCUBE_H
#define LIGHTCUBE_H

#include"shape.h"
#include<QVector3D>
class LightCube:public Shape
{
    Q_OBJECT
public:
   LightCube(int width,int height);
   ~LightCube();
   virtual  void Render();
   virtual void Resize(int width, int height);
   void SetTranslate(QVector3D trans);
   void SetScale(QVector3D scale);
private:
   QVector3D m_modeltransmat;
   QVector3D m_modelsaclemat;
   void UpDateModel();
};

#endif // LIGHTCUBE_H

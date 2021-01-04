#ifndef COLORCUBE_H
#define COLORCUBE_H

#include"shape.h"
#include<QVector3D>
class ColorCube:public Shape
{
     Q_OBJECT
public:
    ColorCube(int width,int height);
    ~ColorCube();
    virtual  void Render();
    virtual void Resize(int width, int height);
    void SetTranslate(QVector3D trans);
public slots:
    void Animate();
private:
    int m_frame;
    QVector3D m_modeltransmat;


};

#endif // CUBE_H

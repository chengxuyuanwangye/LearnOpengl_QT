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
    void SetLightPosition(QVector3D lightpos);
    void SetAmbientStrength(float strength);
    void SetSpecularStrength(float value);
    void SetLightColor(QVector3D lightColor);
private:

    QVector3D m_modeltransmat;
    QVector3D m_lightPosition;
    float ambientStrength;
    float Specular;
    QVector3D m_curColor;
    QVector3D m_lightColor;





};

#endif // CUBE_H

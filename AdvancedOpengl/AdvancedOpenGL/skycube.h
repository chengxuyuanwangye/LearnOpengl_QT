#ifndef SKYCUBE_H
#define SKYCUBE_H

#include"shape.h"
class QOpenGLTexture;
class SkyCube:public Shape
{
    Q_OBJECT
public:
    SkyCube(int width,int height);
    ~SkyCube();
    virtual  void Render();
    virtual void Resize(int width, int height);
    virtual void SetTranslateVec(QVector3D vec);
private:
    QOpenGLTexture * ourtexture;
    void CreateCubeTexture();

};

#endif // SKYCUBE_H

#ifndef CATMULLROMSPLINE_H
#define CATMULLROMSPLINE_H


#include"shape.h"
class CatmullRomSpline:public Shape
{
    Q_OBJECT
public:
    CatmullRomSpline(int width,int height);
    ~CatmullRomSpline();
    virtual  void Render();
    virtual void Resize(int width, int height);
    virtual void SetTranslateVec(QVector3D vec);

};

#endif // CATMULLROMSPLINE_H

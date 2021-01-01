#ifndef TRIANGLE_H
#define TRIANGLE_H
#include"shape.h"
#include<QMatrix4x4>

class Triangle: public Shape
{
 Q_OBJECT
public:
    Triangle(int width,int height);
    virtual  void Render();
    virtual void Resize(int width, int height);
public slots:
    void Animate();
private:
    QMatrix4x4 m_rotmatrix;
    int m_frame;

};

#endif // TRIANGLE_H

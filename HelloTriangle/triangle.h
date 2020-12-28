#ifndef TRIANGLE_H
#define TRIANGLE_H
#include"shape.h"
#include<QMatrix4x4>

class Triangle: public Shape
{
 Q_OBJECT
public:
    Triangle();
    virtual  void Render();
public slots:
    void Animate();
private:
    QMatrix4x4 m_rotmatrix;
    int m_frame;

};

#endif // TRIANGLE_H

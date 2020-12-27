#ifndef TRIANGLE_H
#define TRIANGLE_H
#include"shape.h"

class Triangle: public Shape
{
 Q_OBJECT
public:
    Triangle();
    virtual  void Render();
};

#endif // TRIANGLE_H

#ifndef RECTANGLE_H
#define RECTANGLE_H
#include"shape.h"

class MyRectangle: public Shape
{
     Q_OBJECT
public:
    MyRectangle();
    virtual  void Render();
};

#endif // RECTANGLE_H

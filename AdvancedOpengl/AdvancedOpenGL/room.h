#ifndef ROOM_H
#define ROOM_H

#include"shape.h"
class Room:public Shape
{
     Q_OBJECT
public:
    Room(int width,int height);
    ~Room();
    virtual  void Render();
    virtual void Resize(int width, int height);
    virtual void SetTranslateVec(QVector3D vec);

};

#endif // ROOM_H

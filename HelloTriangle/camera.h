#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include<QMatrix4x4>

#define PI 3.14159265
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};
// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


class Camera
{

public:

    // constructor with vectors
    Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f),
           QVector3D up = QVector3D(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH);
    // constructor with scalar values
   Camera(float posX, float posY, float posZ, float upX, float upY,
          float upZ, float yaw, float pitch);
   QMatrix4x4 GetViewMatrix();

   // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
     void ProcessKeyboard(Camera_Movement direction, float deltaTime);
     // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
     void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

     // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
     void ProcessMouseScroll(float yoffset)
     {
         Zoom -= (float)yoffset;
         if (Zoom < 1.0f)
             Zoom = 1.0f;
         if (Zoom > 45.0f)
             Zoom = 45.0f;
     }

     QVector3D Position;
     QVector3D Front;
     QVector3D Up;
     QVector3D Right;
     QVector3D WorldUp;
     // euler Angles
     float Yaw;
     float Pitch;
     // camera options
     float MovementSpeed;
     float MouseSensitivity;
     float Zoom;

 private:
     // calculates the front vector from the Camera's (updated) Euler Angles
     void updateCameraVectors();
     float Radians(float angle)
     {
         return angle*PI/180;
     }



};

#endif // CAMERA_H

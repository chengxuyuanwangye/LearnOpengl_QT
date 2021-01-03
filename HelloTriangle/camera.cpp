#include "camera.h"

// constructor with vectors
Camera::Camera(QVector3D position,QVector3D up ,
       float yaw , float pitch ):
    Position(position),
    Up(up),
    Yaw(yaw),
    Pitch(pitch),
    Front(-position),
    WorldUp(up),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY),
    Zoom(ZOOM)
{
     updateCameraVectors();
}
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
    Front(QVector3D(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY),
    Zoom(ZOOM)
   {
       Position =QVector3D(posX, posY, posZ);
       WorldUp = QVector3D(upX, upY, upZ);
       Yaw = yaw;
       Pitch = pitch;
       updateCameraVectors();
   }

 // returns the view matrix calculated using Euler Angles and the LookAt Matrix
QMatrix4x4 Camera::GetViewMatrix()
   {
    QMatrix4x4 result;
    result.lookAt(Position, Position + Front, Up);
    return result;
   }


void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::updateCameraVectors()
  {
      // calculate the new Front vector
      QVector3D front;
      front.setX(cos(Radians(this->Yaw)) * cos(Radians(this->Pitch)));
      front.setY(sin(Radians(this->Pitch)));
      front.setZ(sin(Radians(this->Yaw)) * cos(Radians(this->Pitch)));
      Front = front.normalized();
      // also re-calculate the Right and Up vector
      Right =  QVector3D::crossProduct(Front, WorldUp).normalized();
      Up    = QVector3D::crossProduct(Right, Front).normalized();
  }








#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include<QOpenGLWidget>
#include<QOpenGLFunctions>
class Shape;
class QTimer;
class Camera;
class Plane;

enum DEPTHFUNC {ALWAYS, NEVER, LESS, EQUAL,LEQUAL,GREATER,NOTEQUAL,GEQUAL};

class MyGLWidget:public QOpenGLWidget,protected QOpenGLFunctions
{
     Q_OBJECT
public:
   explicit MyGLWidget(QWidget *parent=nullptr);
    ~MyGLWidget() Q_DECL_OVERRIDE;
    void StartAnimate(bool flag);
    void EnableCube();
    void SetDepthFunc(DEPTHFUNC depthfunc);
protected:
    void initializeGL()Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
private:
    QVector<Shape*> cubevec;
    QVector<Shape*> framevec;
    QVector<Shape*> grassvec;
    QVector<QVector3D> windows;
    Plane* plane;

    QTimer* m_timer;
    bool animateflag;
    void timeoutFunc();
    Camera* m_camera;

    float lastX ;
    float lastY ;
    bool firstMouse ;
    bool leftbuttonpressed;

    // timing
    float deltaTime ;	// time between current frame and last frame
    float lastFrame ;

    int m_frame;
    DEPTHFUNC curdepthfunc;


};
#endif // MYGLWIDGET_H

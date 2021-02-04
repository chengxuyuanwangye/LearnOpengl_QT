#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include<QOpenGLWidget>
#include<QOpenGLFunctions>
#include<QOpenGLBuffer>
#include<QOpenGLVertexArrayObject>
#include<QOpenGLContext>
class QOpenGLShaderProgram;
class Shape;
class QTimer;
class Camera;
class Plane;
class QOpenGLTexture;
class QOpenGLFramebufferObject;
class SkyCube;

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
    SkyCube *sky;

    QTimer* m_timer;
    bool animateflag;
    void timeoutFunc();
    void DepthFunc();
    bool CreateFBOShaderProgram();
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
    QOpenGLBuffer* m_vbo;//顶点缓冲对象
    QOpenGLVertexArrayObject* m_vao;//顶点数组对象
    QOpenGLShaderProgram *m_program;
   // QOpenGLTexture * screenTexture;
    QOpenGLFramebufferObject *fbo;//fbo used for multisample
    QOpenGLFramebufferObject *renderfbo;//fbo used for render
    QOpenGLContext *_context;


};
#endif // MYGLWIDGET_H

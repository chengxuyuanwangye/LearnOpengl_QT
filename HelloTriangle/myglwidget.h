#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include<QOpenGLWidget>
#include<QOpenGLFunctions>
class Shape;
class QTimer;

class MyGLWidget:public QOpenGLWidget,protected QOpenGLFunctions
{
     Q_OBJECT
public:
   explicit MyGLWidget(QWidget *parent=nullptr);
    ~MyGLWidget() Q_DECL_OVERRIDE;
    void EnableTriangle();
    void EnableRectangle();
    void StartAnimate(bool flag);
    void EnableCube();

protected:
    void initializeGL()Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
private:
    QVector<Shape*> shapevec;
    QTimer* m_timer;
    bool animateflag;
    void timeoutFunc();

};
#endif // MYGLWIDGET_H

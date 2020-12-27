#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include<QOpenGLWidget>
#include<QOpenGLFunctions>
class Shape;
class MyGLWidget:public QOpenGLWidget,protected QOpenGLFunctions
{
     Q_OBJECT
public:
   explicit MyGLWidget(QWidget *parent=nullptr);
    ~MyGLWidget() Q_DECL_OVERRIDE;
    void EnableTriangle();
    void EnableRectangle();
protected:
    void initializeGL()Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
private:
    QVector<Shape*> shapevec;

};
#endif // MYGLWIDGET_H

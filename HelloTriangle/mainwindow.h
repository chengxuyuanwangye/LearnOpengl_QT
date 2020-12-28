#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MyGLWidget;
class QFile;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
     void resizeEvent(QResizeEvent* event);
private:
    QDockWidget *toolpanel;
    MyGLWidget *centreimgwidget;
    QWidget *centralWidget;
    QFile *qss;
    QWidget *dockWidgetContents;
    QWidget *contwidget;
    void createDockTool();

    void btntriangle_clicked(bool);
    void btnrectangle_clicked(bool);
    void btnanimate_clicked(bool);
};

#endif // MAINWINDOW_H

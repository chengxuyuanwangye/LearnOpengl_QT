#include "mainwindow.h"
#include<QFile>
#include<QDockWidget>
#include"myglwidget.h"
#include<QPalette>
#include<QPushButton>
#include <QGridLayout>
#include<QDebug>
#include<QSurfaceFormat>
#include <QLabel>
#include<QSlider>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
 setWindowTitle(tr("Hello Triangle"));
 this->setMinimumSize(800,400);
 centralWidget = new QWidget(this);
 centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
 centreimgwidget=new MyGLWidget(centralWidget);
 centreimgwidget->setGeometry(QRect(0, 0, 351, 251));
 QSurfaceFormat format;
 //设置像素采样的样本个数，用于抗锯齿
 format.setSamples(16);
 centreimgwidget->setFormat(format);
 this->setCentralWidget(centralWidget);
 createDockTool();
 this->resize(1000,500);
}

void MainWindow:: createDockTool()
{
     toolpanel=new QDockWidget(this);
     addDockWidget(Qt::RightDockWidgetArea,toolpanel);
     toolpanel->setWindowTitle("工具");
     toolpanel->setMinimumSize(QSize(200,400));
     toolpanel->setAllowedAreas(Qt::RightDockWidgetArea|Qt::LeftDockWidgetArea);
     toolpanel->setFeatures(QDockWidget::DockWidgetMovable);
     toolpanel->setAutoFillBackground(true);
     QPalette pal;
     pal.setColor(QPalette::Background,Qt::gray);
     toolpanel->setPalette(pal);
     dockWidgetContents=new QWidget;
     contwidget=new QWidget(dockWidgetContents);
     contwidget->setGeometry(QRect(25,28,100,400));

     QGridLayout *btnlayout=new QGridLayout(contwidget);

     QPushButton *btntriangle=new QPushButton(contwidget);
     btntriangle->setText("Triangle");
     QPushButton *btnrectangle=new QPushButton(contwidget);
     btnrectangle->setText("Rectangle");
     QPushButton *btnanimate=new QPushButton(contwidget);
     btnanimate->setText("startRotate");

     QPushButton *btncube=new QPushButton(contwidget);
     btncube->setText("Cube");

     QPushButton *btncolortest=new QPushButton(contwidget);
     btncolortest->setText("colortest");
     QSlider * aimbientSlider=new QSlider(contwidget);
     aimbientSlider->setOrientation(Qt::Horizontal);
     aimbientSlider->setMinimum(0);
     aimbientSlider->setMaximum(10);
     aimbientSlider->setSingleStep(1);
     aimbientSlider->setValue(1);




     btnlayout->addWidget(btntriangle,0,0);
     btnlayout->addWidget(btnanimate,1,0);
     btnlayout->addWidget(btnrectangle,2,0);
     btnlayout->addWidget(btncube,3,0);
     btnlayout->addWidget(btncolortest,4,0);
     btnlayout->addWidget(aimbientSlider,5,0);





     contwidget->setLayout(btnlayout);
     toolpanel->setWidget(dockWidgetContents);
     connect(btntriangle,&QPushButton::clicked,this,&MainWindow::btntriangle_clicked);
     connect(btnrectangle,&QPushButton::clicked,this,&MainWindow::btnrectangle_clicked);
     connect(btnanimate,&QPushButton::clicked,this,&MainWindow::btnanimate_clicked);
     connect(btncube,&QPushButton::clicked,this,&MainWindow::btncube_clicked);
     connect(btncolortest,&QPushButton::clicked,this,&MainWindow::btncolortest_clicked);
     connect(aimbientSlider,&QSlider::valueChanged,this,&MainWindow::aimbientValueChanged);
}

MainWindow::~MainWindow()
{
delete qss;


}
 void MainWindow::resizeEvent(QResizeEvent* event)
 {
     centreimgwidget->setGeometry(QRect(0,0,centralWidget->width(),centralWidget->height()));
      QMainWindow::resizeEvent(event);

 }
 void MainWindow::btntriangle_clicked(bool clicked)
 {
     Q_UNUSED(clicked);
     centreimgwidget->EnableTriangle();
     centreimgwidget->update();

 }
 void MainWindow::btnrectangle_clicked(bool clicked)
 {
      Q_UNUSED(clicked);
      centreimgwidget->EnableRectangle();
      centreimgwidget->update();
 }
 void MainWindow::btnanimate_clicked(bool clicked)
 {

      centreimgwidget->StartAnimate(clicked);
 }
 void MainWindow::btncube_clicked(bool clicked)
 {
     Q_UNUSED(clicked);
     centreimgwidget->EnableCube();
     centreimgwidget->update();
 }

 void MainWindow::btncolortest_clicked(bool)
 {
     centreimgwidget->EnableColorCube();
     centreimgwidget->update();
 }

 void MainWindow::aimbientValueChanged(int value)
 {
     centreimgwidget->SetAimbientValue(value/10.0f);
     centreimgwidget->update();
 }

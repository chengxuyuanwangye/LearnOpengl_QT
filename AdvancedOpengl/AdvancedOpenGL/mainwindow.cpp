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
#include"math.h"
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

    QPushButton *btncube=new QPushButton(contwidget);
    btncube->setText("Cube");
    QPushButton *btnalways=new QPushButton(contwidget);
    btnalways->setText("Always");
    btnalways->hide();

    QPushButton *btnnever=new QPushButton(contwidget);
    btnnever->setText("Never");
    btnnever->hide();

    QPushButton *btnless=new QPushButton(contwidget);
    btnless->setText("Less");
    btnless->hide();

    QPushButton *btnequal=new QPushButton(contwidget);
    btnequal->setText("Equal");
    btnequal->hide();

    QPushButton *btnlequal=new QPushButton(contwidget);
    btnlequal->setText("LEqual");
    btnlequal->hide();
    QPushButton *btngreater=new QPushButton(contwidget);
    btngreater->setText("Greater");
    btngreater->hide();


    btnlayout->addWidget(btncube,0,0);
    btnlayout->addWidget(btnalways,1,0);
    btnlayout->addWidget(btnnever,2,0);
    btnlayout->addWidget(btnless,3,0);
    btnlayout->addWidget(btnequal,4,0);
    btnlayout->addWidget(btnlequal,5,0);
    btnlayout->addWidget(btngreater,6,0);

    contwidget->setLayout(btnlayout);
    toolpanel->setWidget(dockWidgetContents);
    connect(btncube,&QPushButton::clicked,this,&MainWindow::btncube_clicked);
    connect(btnalways,&QPushButton::clicked,this,&MainWindow::btnalways_clicked);
    connect(btnnever,&QPushButton::clicked,this,&MainWindow::btnnever_clicked);
    connect(btnless,&QPushButton::clicked,this,&MainWindow::btnless_clicked);
    connect(btnlequal,&QPushButton::clicked,this,&MainWindow::btnlequal_clicked);
    connect(btngreater,&QPushButton::clicked,this,&MainWindow::btngreater_clicked);

}

MainWindow::~MainWindow()
{

}
 void MainWindow::resizeEvent(QResizeEvent* event)
 {
     centreimgwidget->setGeometry(QRect(0,0,centralWidget->width(),centralWidget->height()));
      QMainWindow::resizeEvent(event);

 }
 void MainWindow::btncube_clicked(bool clicked)
 {
     Q_UNUSED(clicked);
     centreimgwidget->EnableCube();
     centreimgwidget->update();
 }

  void MainWindow::btnalways_clicked(bool clicked)
  {
      Q_UNUSED(clicked);
      centreimgwidget->SetDepthFunc(DEPTHFUNC::ALWAYS);
      centreimgwidget->update();
  }

  void MainWindow::btnnever_clicked(bool clicked)
  {
      Q_UNUSED(clicked);
      centreimgwidget->SetDepthFunc(DEPTHFUNC::NEVER);
      centreimgwidget->update();
  }
  void MainWindow::btnless_clicked(bool clicked)
  {
      Q_UNUSED(clicked);
      centreimgwidget->SetDepthFunc(DEPTHFUNC::LESS);
      centreimgwidget->update();
  }

  void MainWindow::btnequal_clicked(bool clicked)
  {
      Q_UNUSED(clicked);
      centreimgwidget->SetDepthFunc(DEPTHFUNC::EQUAL);
      centreimgwidget->update();
  }
  void MainWindow::btnlequal_clicked(bool clicked)
  {
      Q_UNUSED(clicked);
      centreimgwidget->SetDepthFunc(DEPTHFUNC::LEQUAL);
      centreimgwidget->update();
  }

  void MainWindow::btngreater_clicked(bool clicked)
  {
      Q_UNUSED(clicked);
      centreimgwidget->SetDepthFunc(DEPTHFUNC::GREATER);
      centreimgwidget->update();
  }





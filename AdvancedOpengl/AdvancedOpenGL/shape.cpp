#include "shape.h"
#include<QDebug>
#include<QOpenGLShaderProgram>
Shape::Shape(int width,int height)
: QObject(nullptr),
  m_visible(false),
  m_width(width),
  m_height(height)
{


}

Shape::~Shape()
{

    if(m_vbo!=nullptr)
    {
        m_vbo->destroy();
        delete m_vbo;
    }
    if(m_ebo!=nullptr){
        m_ebo->destroy();
        delete m_ebo;
    }
    if(m_vao!=nullptr)
    {
        m_vao->destroy();
        delete m_vao;
    }
    if(m_program!=nullptr)
        delete m_program;


}

void Shape::Render()
{

}

 void Shape::ChangeVisible(bool flag)
 {
     m_visible=flag;
 }

 void Shape::Resize(int width, int height)
 {

 }


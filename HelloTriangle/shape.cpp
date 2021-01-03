#include "shape.h"

Shape::Shape(int width,int height)
: QObject(nullptr),
  m_visible(false),
  m_width(width),
  m_height(height)
{


}

Shape::~Shape()
{
    m_vbo->destroy();
    if(m_ebo!=nullptr){
        m_ebo->destroy();
        delete m_ebo;
    }
    m_vao->destroy();
    delete m_vbo;

    delete m_vao;

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


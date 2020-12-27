#include "shape.h"

Shape::Shape()
: QObject(nullptr),
  m_visible(false)
{

}

Shape::~Shape()
{
    m_vbo->destroy();
    m_ebo->destroy();
    m_vao->destroy();
    delete m_vbo;
    delete m_ebo;
    delete m_vao;
}

void Shape::Render()
{

}

 void Shape::ChangeVisible(bool flag)
 {
     m_visible=flag;
 }


#include "myqlabel.h"

#include <QMouseEvent>

MyQLabel::MyQLabel(QWidget *parent,Qt::WindowFlags f):QLabel(parent)
{

}

MyQLabel::~MyQLabel()
{

}

void MyQLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev != nullptr && ev->button() == Qt::LeftButton)
    {
       emit clicked();
    }
}

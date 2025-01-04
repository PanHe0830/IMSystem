#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QLabel>

class MyQLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyQLabel(QWidget *parent = 0,Qt::WindowFlags f=Qt::WindowFlags());
    ~MyQLabel();

signals:
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *ev);

};

#endif // MYQLABEL_H

#include "PayWindow.h"

PayWindow::PayWindow(QWidget *parent) :QWidget(parent)
{

    this->setWindowTitle("Thank You!!!");
    this->setFixedSize(640,840);
    this->hide();
    backgr = new QLabel(this);
    backgr->setPixmap(QPixmap(":/new/prefix1/pay.png"));
    backgr->setScaledContents(true);
    backgr->resize(this->size());
    backgr->show();
}

PayWindow::~PayWindow()
{
}

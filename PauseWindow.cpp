#include "pausewindow.h"

PauseWindow::PauseWindow(QWidget *parent) : QWidget(parent),
mConBtn(Q_NULLPTR),mReBtn(Q_NULLPTR)
{
    this->setWindowTitle("Pause");
    this->setFixedSize(320,320);

    //Continue键
    mConBtn = new QPushButton("Continue",this);
    mConBtn->setFixedSize(80,30);
    mConBtn->move(120,90);
    connect(mConBtn, &QPushButton::clicked,[=](){
        //暂停窗口可能需要复用,故没有delete
        this->close();
    });

    mReBtn = new QPushButton("Return",this);
    mReBtn->setFixedSize(80,30);
    mReBtn->move(120,140);

    connect(mReBtn, &QPushButton::clicked, [=]() {
        emit this->backToMainWindow();
        });
}

PauseWindow::~PauseWindow()
{

}

#include "CharacterWindow.h"
#include "EmojiView.h"
#include "EmojiScene.h"

CharacterWindow::CharacterWindow(QWidget *parent) : 
    QWidget(parent),mConBtn(Q_NULLPTR),mReBtn(Q_NULLPTR),
    mGameView(Q_NULLPTR),mScene(Q_NULLPTR)
{
    this->setWindowTitle("Tutorial");
    this->setFixedSize(1080,640);


    //continue按钮
    mConBtn = new QPushButton("continue",this);
    mConBtn->setFixedSize(100,30);
    mConBtn->move(980,610);
    connect(mConBtn, &QPushButton::clicked,
        this,&CharacterWindow::createGameView);


    //返回上一级菜单(此时为主菜单)的按钮
    mReBtn = new QPushButton("Return", this);
    mReBtn->setFixedSize(100, 30);
    mReBtn->move(0,610);
    connect(mReBtn, &QPushButton::clicked, [=]() {
        emit this->backToLastWindow();
    });

    //background
    QPalette pal =this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/new/prefix1/ch_rev.png")));
    setPalette(pal);

}

void CharacterWindow::createGameView()
{
    mGameView = new EmojiView;
    mScene = new EmojiScene();
    mGameView->mySetScene(mScene);
    mGameView->setRenderHint(QPainter::Antialiasing);
    mGameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGameView->setScene(mScene);
    mGameView->setFixedSize(mScene->sceneRect().size().toSize());
    mGameView->show();

    //处理子窗口返回的backToMainWindow信号
    connect(mGameView, &EmojiView::backToMainWindow, [=]() {
        mGameView->setAttribute(Qt::WA_DeleteOnClose);
        mGameView->close();
        emit this->backToMainWindow();
        });

    //处理子窗口发出的返回到父窗口的信号
    connect(mGameView, &EmojiView::backToLastWindow, [=]() {
        mGameView->setAttribute(Qt::WA_DeleteOnClose);
        mGameView->close();
        });
}

CharacterWindow::~CharacterWindow()
{
    delete mScene;
}

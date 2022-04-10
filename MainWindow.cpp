#include "mainwindow.h"
#include "characterwindow.h"

#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mCharaWindow(Q_NULLPTR),
    mStartBtn(Q_NULLPTR),mPayBtn(Q_NULLPTR),mQuitBtn(Q_NULLPTR),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //主窗口
    this->setWindowTitle("EmojiGame");
    this->setFixedSize(640,640);
    

    //开始键位
    mStartBtn = new QPushButton("Start", this);
    mStartBtn->setFixedSize(80, 30);
    mStartBtn->move(280,400);
    
    //按下开始键位则建立选择角色页面
    connect(mStartBtn, &QPushButton::clicked,[=](){
        mCharaWindow = new CharacterWindow;
        this->hide();

        mCharaWindow = new CharacterWindow;
        mCharaWindow->show();

        //处理返回主界面信号
        connect(mCharaWindow, &CharacterWindow::backToMainWindow, [=]() {
            mCharaWindow->setAttribute(Qt::WA_DeleteOnClose);
            mCharaWindow->close();
            this->show();    //show mainwindow
            });

        //处理返回上一级界面信号
        connect(mCharaWindow, &CharacterWindow::backToLastWindow, [=]() {
            mCharaWindow->setAttribute(Qt::WA_DeleteOnClose);
            mCharaWindow->close();
            this->show();    //show mainwindow
            });
    });


    //付款界面按钮
    mPayBtn = new QPushButton("Pay",this);
    mPayBtn->setFixedSize(80,30);
    mPayBtn->move(280, 450);

    connect(mPayBtn, &QPushButton::clicked,[=](){
        mPayWindow = new PayWindow;
        mPayWindow->show();
    });


    //退出按钮
    mQuitBtn = new QPushButton("Quit", this);
    mQuitBtn->setFixedSize(80,30);
    mQuitBtn->move(280, 500);
   
    connect(mQuitBtn, &QPushButton::clicked,[=](){
        //直接关闭,不用delete,否则会有问题
        this->close();
    }); 
}

MainWindow::~MainWindow()
{
    delete ui;
}


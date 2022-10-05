#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QPointer>
#include "paywindow.h"

class CharacterWindow;

namespace Ui{
    class MainWindow;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    

private:
    Ui::MainWindow* ui;
    QPushButton* mStartBtn;
    QPushButton* mPayBtn;
    QPushButton* mQuitBtn;
    
    QPointer<CharacterWindow> mCharaWindow;
    QPointer<PayWindow> mPayWindow;
};
#endif // MAINWINDOW_H

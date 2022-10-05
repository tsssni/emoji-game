#ifndef PAUSEWINDOW_H
#define PAUSEWINDOW_H
#include <QMainWindow>
#include <QPushButton>
class GameWindow;
class PauseWindow : public QWidget
{
    Q_OBJECT
public:
    PauseWindow(QWidget* parent = nullptr);
    ~PauseWindow();

signals:
    void backToMainWindow();
    void backToLastWindow();
private:
    QPushButton* mConBtn;
    QPushButton* mReBtn;
};

#endif // PAUSEWINDOW_H

#ifndef PAYWINDOW_H
#define PAYWINDOW_H
#include <QMainWindow>
#include <QLabel>


class PayWindow : public QWidget
{
    Q_OBJECT
public:
    PayWindow(QWidget* parent = nullptr);
    ~PayWindow();
private:
    QLabel* backgr;
};

#endif // PAYWINDOW_H

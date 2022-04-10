#ifndef CHARACTERWINDOW_H
#define CHARACTERWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QPointer>
#include <QGraphicsView>

class GameWindow;
class EmojiScene;
class EmojiView;

class CharacterWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CharacterWindow(QWidget* parent = nullptr);
    ~CharacterWindow();

signals:
    void backToMainWindow();
    void backToLastWindow();

private slots:
    void createGameView();

private:
    QPushButton* mConBtn;
    QPushButton* mReBtn;
    QPointer<EmojiView> mGameView;
    QPointer<EmojiScene> mScene;
};


#endif // SECONDWINDOW_H

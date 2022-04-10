#pragma once
#include <qgraphicsview.h>
#include <QPointer>
class PauseWindow;
class EmojiScene;

class EmojiView :
    public QGraphicsView
{
    Q_OBJECT
public:
    void mySetScene(EmojiScene* scene);//该函数用于设置mScene成员,从而传递keyEvent
signals:
    void backToMainWindow();
    void backToLastWindow();

private:
    void keyPressEvent(QKeyEvent* event);

    EmojiScene* mScene;
    QPointer<PauseWindow> mPauseWindow;
};


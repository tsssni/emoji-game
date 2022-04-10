#include "EmojiView.h"
#include "EmojiScene.h"
#include "PauseWindow.h"
#include <QKeyEvent>

void EmojiView::keyPressEvent(QKeyEvent* event)
{
    //´¥·¢ÔÝÍ£²Ëµ¥
    mScene->keyPressEvent(event);
    if (event->key() == Qt::Key_Escape)
    {
        if(!mPauseWindow) mPauseWindow = new PauseWindow;
        mPauseWindow->show();
        connect(mPauseWindow, &PauseWindow::backToMainWindow, [=]() {
            mPauseWindow->setAttribute(Qt::WA_DeleteOnClose);
            mPauseWindow->close();
            emit this->backToMainWindow();
        });
    }
}

void EmojiView::mySetScene(EmojiScene* scene)
{
    mScene = scene;
}

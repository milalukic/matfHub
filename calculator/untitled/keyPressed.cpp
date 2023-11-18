#include "keyPressed.h"
#include <QEvent>
#include <QKeyEvent>
#include <iostream>

void keyEnterReceiver::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type()==QEvent::KeyPress) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if ( (key->key()==Qt::Key_Enter) || (key->key()==Qt::Key_Return) ) {
            std::cout << "Hello word!" << std::endl;
        }
//        else {
//            return QObject::eventFilter(obj, event);
//        }
//        return true;
//    } else {
//        return QObject::eventFilter(obj, event);
//    }
//    return false;
}

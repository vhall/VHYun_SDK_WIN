#include "CustomEventType.h"

CustomEventType::CustomEventType(QEvent::Type type, QString data) :
    QEvent(type) {
    msg = data;
};

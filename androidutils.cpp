#include "androidutils.h"

#include <QAndroidJniObject>

AndroidUtils::AndroidUtils()
{
}

void AndroidUtils::makeToast(const QString& message) {
    QAndroidJniObject msg = QAndroidJniObject::fromString(message);
    QAndroidJniObject::callStaticMethod<void>("bz/jako/chromiumInstaller/MyToast", "toast", "(Ljava/lang/String;)V", msg.object<jstring>());
}

#ifndef ANDROIDUTILS_H
#define ANDROIDUTILS_H

#include <QString>

class AndroidUtils
{
public:
    static void makeToast(const QString& message);

protected:
    AndroidUtils();
};

#endif // ANDROIDUTILS_H

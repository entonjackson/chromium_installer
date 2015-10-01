#include "apphandler.h"

#include <QtGui>
#include <QtQuick>

#include <QGuiApplication>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QQuickView view;

    qmlRegisterType<AppHandler>("org.example", 1, 0, "AppHandler");

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();

    return app.exec();
}

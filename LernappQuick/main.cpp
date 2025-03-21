#include <QGuiApplication>
#include <QQmlApplicationEngine>
// #include <QQmlContext>
// #include <networtbackend.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // NetwortBackend backend;
    // engine.rootContext()->setContextProperty("backend", &backend);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("LernappQuick", "Main");

    return app.exec();
}

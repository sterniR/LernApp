/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<networtbackend.h>)
#  include <networtbackend.h>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_LernappQuick()
{
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    qmlRegisterTypesAndRevisions<NetwortBackend>("LernappQuick", 1);
    QT_WARNING_POP
    qmlRegisterModule("LernappQuick", 1, 0);
}

static const QQmlModuleRegistration lernappQuickRegistration("LernappQuick", qml_register_types_LernappQuick);

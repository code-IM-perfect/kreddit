#include "api/redditmodel.h"

#include <KAboutData>
#include <KIconTheme>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QUrl>
#include <QtQml>
#include <klocalizedcontext.h>
#include <kuitsetup.h>
#include <qapplication.h>
#include <qcoreapplication.h>
#include <qqml.h>
#include <qqmlapplicationengine.h>
#include <qquickstyle.h>
#include <qstringliteral.h>
#include <qtenvironmentvariables.h>

using namespace Qt::StringLiterals;
// TODO: Convert other QStringLiterals to _L1 for consistancy

int main(int argc, char *argv[])
{
    KIconTheme::initTheme();

    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("kreddit");
    QApplication::setOrganizationName("KDE"_L1);
    QApplication::setOrganizationDomain("kde.org"_L1);
    QApplication::setApplicationName("Kreddit"_L1);
    QApplication::setDesktopFileName("org.kde.kreddit"_L1);

    QApplication::setStyle(QStringLiteral("breeze"));
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }

    KAboutData aboutData(QStringLiteral("Kreddit"),
                         i18nc("@title", "Kreddit - "),
                         QStringLiteral("1.0"),
                         i18n("A Kirigami based Reddit Client"),
                         KAboutLicense::GPL,
                         i18n("(c) 2025"));

    aboutData.addAuthor(i18nc("@info:credit", "Harshit Tomar"),
                        i18nc("@info:credit", "Author"),
                        QStringLiteral("harshitt345@gmail.com"),
                        QStringLiteral("code-IM-perfect.github.io"));

    // Set aboutData as information about the app
    KAboutData::setApplicationData(aboutData);

    qmlRegisterSingletonType("org.kde.kreddit", // Import statement
                             1,
                             0, // Major and minor versions of the import
                             "About", // The name of the QML object
                             [](QQmlEngine *engine, QJSEngine *) -> QJSValue {
                                 // Here we retrieve our aboutData and give it to the QML engine
                                 // to turn it into a QML type
                                 return engine->toScriptValue(KAboutData::applicationData());
                             });

    // static constexpr auto hotUrl = "https://oauth.reddit.com/hot"_L1;
    qmlRegisterType<RedditModel>("RedditModel", 1, 0, "RedditModel");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.loadFromModule("org.kde.kreddit", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}

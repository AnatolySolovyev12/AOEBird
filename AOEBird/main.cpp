//#define QT_NO_DEBUG_OUTPUT // Аналогично работают QT_NO_INFO_OUTPUT и QT_NO_WARNING_OUTPUT
#include <QtCore/QCoreApplication>
#include <GeneralClass.h>
#include <Windows.h>

int main(int argc, char *argv[])
{
    LC_ALL(setlocale, "russian");
    QCoreApplication app(argc, argv);
    SetConsoleOutputCP(1251);
    GeneralClass * mainGeneralClass = new GeneralClass(nullptr);

    return app.exec();
}

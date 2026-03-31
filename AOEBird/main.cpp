//#define QT_NO_DEBUG_OUTPUT // Аналогично работают QT_NO_INFO_OUTPUT и QT_NO_WARNING_OUTPUT
#include <QtCore/QCoreApplication>
#include <GeneralClass.h>
#include <Windows.h>
#include <clocale>

int main(int argc, char *argv[])
{
    SetConsoleCP(65001);        // UTF‑8 вход
    SetConsoleOutputCP(65001);  // UTF‑8 вывод
    setlocale(LC_ALL, "ru_RU.UTF-8");

    QCoreApplication app(argc, argv);
    GeneralClass * mainGeneralClass = new GeneralClass(nullptr);

    return app.exec();
}

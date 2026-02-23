#include <QtCore/QCoreApplication>
#include <GeneralClass.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    GeneralClass * mainGeneralClass = new GeneralClass(nullptr);

    return app.exec();
}

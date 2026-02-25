#include "GeneralClass.h"

GeneralClass::GeneralClass(QObject *parent)
	: QObject(parent)
{
	dataBaseClass = new DataBaseClass(nullptr);
	paramsClass =  new ParamsClass(nullptr);
	connect(paramsClass, &ParamsClass::signalFromParamsClassForConnectToMainDb, dataBaseClass, &DataBaseClass::connectionToMainDb);

	QTimer::singleShot(2000, [this]() {
		dataBaseClass->insertInUsers("bakalavr12@mail.ru", "art54011212");

		QStringList tempList;
		tempList << "1" << "1" << "1" << "89898989898" << "asd@mail.ru" << "TRUE" << "FALSE" << "TRUE";
		qDebug() << tempList;
		dataBaseClass->insertInQueueAndHistory(tempList);

		});
}



GeneralClass::~GeneralClass()
{
	delete dataBaseClass;
	dataBaseClass = nullptr;

	delete paramsClass;
	paramsClass = nullptr;
}


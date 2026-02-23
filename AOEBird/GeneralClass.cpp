#include "GeneralClass.h"

GeneralClass::GeneralClass(QObject *parent)
	: QObject(parent)
{
	paramsClass =  new ParamsClass(nullptr);
	connect(paramsClass, &ParamsClass::signalFromParamsClassForConnectToMainDb, this, &GeneralClass::test);
	dataBaseClass = new DataBaseClass(nullptr);
}

GeneralClass::~GeneralClass()
{}

void GeneralClass::test(QStringList tempString)
{
	qDebug() << tempString;
}


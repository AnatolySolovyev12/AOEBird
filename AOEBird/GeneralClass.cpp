#include "GeneralClass.h"

GeneralClass::GeneralClass(QObject *parent)
	: QObject(parent)
{
	paramsClass =  new ParamsClass(nullptr);

	dataBaseClass = new DataBaseClass(nullptr);
	qDebug() << "TEST2";
}

GeneralClass::~GeneralClass()
{}


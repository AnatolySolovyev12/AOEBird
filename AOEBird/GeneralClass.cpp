#include "GeneralClass.h"

GeneralClass::GeneralClass(QObject *parent)
	: QObject(parent)
{
	paramsClass =  new ParamsClass(nullptr);

	dataBaseClass = new DataBaseClass(nullptr);
}

GeneralClass::~GeneralClass()
{}


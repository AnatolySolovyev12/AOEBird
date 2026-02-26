#pragma once

#include <QObject>

#include <ParamsClass.h>
#include <DataBaseClass.h>
#include "SMTP.h"
#include "MaxClass.h"

class GeneralClass : public QObject
{
	Q_OBJECT

public:
	GeneralClass(QObject* parent);
	~GeneralClass();

private:
	ParamsClass* paramsClass = nullptr;
	DataBaseClass* dataBaseClass = nullptr;
	SMTP* smtpClass = nullptr;
	MaxClass* maxClass = nullptr;
};


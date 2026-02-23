#pragma once

#include <QObject>

#include <ParamsClass.h>
#include <DataBaseClass.h>



class GeneralClass : public QObject
{
	Q_OBJECT

public:
	GeneralClass(QObject* parent);
	~GeneralClass();

	void test();

private:
	ParamsClass* paramsClass = nullptr;
	DataBaseClass* dataBaseClass = nullptr;
};


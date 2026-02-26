#include "GeneralClass.h"

GeneralClass::GeneralClass(QObject *parent)
	: QObject(parent)
{
	dataBaseClass = new DataBaseClass(nullptr);
	paramsClass =  new ParamsClass(nullptr);

	connect(paramsClass, &ParamsClass::signalFromParamsClassForConnectToMainDb, dataBaseClass, &DataBaseClass::connectionToMainDb);
	
	connect(paramsClass, &ParamsClass::signalFromParamsClassForSmtpWithParams, [this](QStringList tempList) {
		smtpClass = new SMTP(tempList[0], tempList[1], tempList[2]);
		});

	connect(paramsClass, &ParamsClass::signalFromParamsClassForMaxWithParams, [this](QStringList tempList) {
		maxClass = new MaxClass(nullptr,  tempList[0], tempList[1]);
		});
	/*
	QTimer::singleShot(2000, [this]() {
		dataBaseClass->insertInUsers("bakalavr12@mail.ru", "art54011212"); 
		 
		QStringList tempList;
		tempList << "1" << "1" << "1" << "89825313114" << "bakalavr12@mail.ru" << "TRUE" << "FALSE" << "TRUE" << "FALSE";
		dataBaseClass->insertInQueueAndHistory(tempList);
		});
	*/


	/*
	QTimer::singleShot(4000, [this]() {
		smtpClass->sendMail("bakalavr12@mail.ru", "TEST", "");
		});
		*/

		
	QTimer::singleShot(4000, [this]() {
		maxClass->sendMessage("TEST");
		});
		


}



GeneralClass::~GeneralClass()
{
	delete dataBaseClass;
	dataBaseClass = nullptr;

	delete paramsClass;
	paramsClass = nullptr;
}


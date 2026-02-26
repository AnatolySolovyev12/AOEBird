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
		tempList << "1" << "1" << "1" << "89825313114" << "bakalavr12@mail.ru" << "TRUE" << "FALSE" << "TRUE" << "FALSE";
		dataBaseClass->insertInQueueAndHistory(tempList);

		});
	
	QTimer::singleShot(4000, [this]() {

		smtpClass = new SMTP("rassylka.pokazaniyutek@mail.ru", "giH6VBc7ghukaiJh6h4M", "smtp.mail.ru");

		smtpClass->sendMail("rassylka.pokazaniyutek@mail.ru", "bakalavr12@mail.ru", "TEST", "");


		});
}



GeneralClass::~GeneralClass()
{
	delete dataBaseClass;
	dataBaseClass = nullptr;

	delete paramsClass;
	paramsClass = nullptr;
}


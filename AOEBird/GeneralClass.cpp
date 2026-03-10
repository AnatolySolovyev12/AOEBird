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

	connect(paramsClass, &ParamsClass::signalFromParamsClassForTelegramWithParams, [this](QStringList tempList) {
		tgClass = new TelegramJacket(nullptr, tempList[0], tempList[1]);
		});

	connect(paramsClass, &ParamsClass::signalFromParamsClassForSmsClassWithParams, [this](QStringList tempList) {
		smsClass = new SMSClass(nullptr, tempList);
		});

	connect(paramsClass, &ParamsClass::signalFromParamsClassForStartCheckClass, [this](bool readyMax, bool readyTelegram, bool readyMail, bool readySms) {
		checkClass = new CheckClass(nullptr, readyMax, readyTelegram, readyMail, readySms);

		connect(checkClass, &CheckClass::checkDbForEvent, dataBaseClass, &DataBaseClass::getQueueValue);
		connect(dataBaseClass, &DataBaseClass::sendStringListFromQueue, checkClass, &CheckClass::checkValuesFromDb);
		connect(checkClass, &CheckClass::sendMax, maxClass, &MaxClass::checkNumber);
		connect(checkClass, &CheckClass::sendTelegram, tgClass, &TelegramJacket::sendMessage);
		connect(checkClass, &CheckClass::sendMail, smtpClass, &SMTP::sendMail);
		connect(checkClass, &CheckClass::sendSMSsignal, smsClass, &SMSClass::sendSMS);
		connect(checkClass, &CheckClass::deleteInDbSignal, dataBaseClass, &DataBaseClass::deleteFromDb);

		});

	/*
	QTimer::singleShot(2000, [this]() {
		//dataBaseClass->insertInUsers("bakalavr12@mail.ru", "art54011212"); 
		 
		QStringList tempList;
		tempList << "1" << "1" << "1" << "89825313114" << "bakalavr12@mail.ru" << "TRUE" << "FALSE" << "TRUE" << "FALSE" << "2026-03-06" << "14:21:30";
		dataBaseClass->insertInQueueAndHistory(tempList);
		});
	*/


	/*
	QTimer::singleShot(4000, [this]() {
		smtpClass->sendMail("bakalavr12@mail.ru", "TEST", "");
		});
		*/

	// Надо будет вернуть в методы отправки сообщений параметр с chatId/Телефон
	/*
		QTimer::singleShot(4000, [this]() {
		maxClass->sendMessage("admin", "TEST");
		});
		*/


	/*
		QTimer::singleShot(4000, [this]() {
			maxClass->checkNumber("admin", "TEST WITH ADMIN");
			});
		*/



	/*
	QTimer::singleShot(4000, [this]() {
		tgClass->sendMessage("TEST");
		});
		*/
		
}



GeneralClass::~GeneralClass()
{
	delete dataBaseClass;
	dataBaseClass = nullptr;

	delete paramsClass;
	paramsClass = nullptr;
}


#include "GeneralClass.h"

GeneralClass::GeneralClass(QObject* parent)
	: QObject(parent)
{
	dataBaseClass = new DataBaseClass(nullptr);
	paramsClass = new ParamsClass(nullptr);

	connect(paramsClass, &ParamsClass::signalFromParamsClassForConnectToMainDb, dataBaseClass, &DataBaseClass::connectionToMainDb);

	connect(paramsClass, &ParamsClass::signalFromParamsClassForSmtpWithParams, [this](QStringList tempList) {
		smtpClass = new SMTP(tempList[0], tempList[1], tempList[2]);
		});

	connect(paramsClass, &ParamsClass::signalFromParamsClassForMaxWithParams, [this](QStringList tempList) {
		maxClass = new MaxClass(nullptr, tempList[0], tempList[1]);
		});

	connect(paramsClass, &ParamsClass::signalFromParamsClassForTelegramWithParams, [this](QStringList tempList) {
		tgClass = new TelegramJacket(nullptr, tempList[0], tempList[1]);
		connect(tgClass, &TelegramJacket::sendToDataBaseChatIdAndPhoneNumber, dataBaseClass, &DataBaseClass::insertInTelegramPhoneTable);
		});

	connect(paramsClass, &ParamsClass::signalFromParamsClassForSmsClassWithParams, [this](QStringList tempList) {
		smsClass = new SMSClass(nullptr, tempList);
		});

	connect(paramsClass, &ParamsClass::signalFromParamsClassForStartCheckClass, [this](bool readyMax, bool readyTelegram, bool readyMail, bool readySms) {
		checkClass = new CheckClass(nullptr, readyMax, readyTelegram, readyMail, readySms);
		serverClass = new TcpServerClass(nullptr);

		connect(checkClass, &CheckClass::checkDbForEvent, dataBaseClass, &DataBaseClass::getQueueValue);
		connect(dataBaseClass, &DataBaseClass::sendStringListFromQueue, checkClass, &CheckClass::checkValuesFromDb);
		connect(checkClass, &CheckClass::deleteInDbSignal, dataBaseClass, &DataBaseClass::deleteFromDb);
		connect(serverClass, &TcpServerClass::sendNewRecordToDb, dataBaseClass, &DataBaseClass::insertInQueueAndHistory);
		connect(serverClass, &TcpServerClass::sendVerifyData, dataBaseClass, &DataBaseClass::verifyFuncDb);
		connect(dataBaseClass, &DataBaseClass::sendVerifyResult, serverClass, &TcpServerClass::sendVerithyResult);




		if (readyMax)
			connect(checkClass, &CheckClass::sendMax, maxClass, &MaxClass::checkNumber);
		if (readyTelegram)
			connect(checkClass, &CheckClass::sendTelegram, tgClass, &TelegramJacket::sendMessage);
		if (readyMail)
			connect(checkClass, &CheckClass::sendMail, smtpClass, &SMTP::sendMail);
		if (readySms)
			connect(checkClass, &CheckClass::sendSMSsignal, smsClass, &SMSClass::sendSMS);
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


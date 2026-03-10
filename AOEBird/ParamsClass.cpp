#include "ParamsClass.h"

ParamsClass::ParamsClass(QObject* parent)
	: QObject(parent)
{
	if (createOrConnectParamsBd())
		if (createTable())
		{
			writeFirstParamsDb();
			writeParamsSmtp();
			writeParamMax();
			writeParamTg();
			writeParamSms();
		}

	QTimer::singleShot(1000, [this]() 
		{
		readyPostreSQL = sendStringListForMainDb(); //////////////////надо добавить обратную связь не апредмет подклюбчения к БД
		readyMail = sendStringListForSmtpClass();
		readyMax = sendStringListForMaxClass();
		readyTelegram = sendStringListForTelegramClass();
		readySms = sendStringListForSmsClass();

		if (readyPostreSQL)
			signalFromParamsClassForStartCheckClass(readyMax, readyTelegram, readyMail, readySms);
		else
			qDebug() << "Check params for PostreSQL connect and try to start again. Delete startingParamsDb in main directory and try again";
		});
}



ParamsClass::~ParamsClass()
{
	if (mainConnection.isOpen())
	{
		mainConnection.close();
	}

	if (QSqlDatabase::contains("sqlite_connection"))
	{
		QSqlDatabase::removeDatabase("sqlite_connection");
	}
}



bool ParamsClass::createOrConnectParamsBd()
{
	QFile tempForCheckDb;
	tempForCheckDb.setFileName("startingParamsDb");
	bool tempForCreateTable = true;

	if (tempForCheckDb.exists()) tempForCreateTable = false;

	mainConnection = QSqlDatabase::addDatabase("QSQLITE", "sqlite_connection");
	mainConnection.setDatabaseName("startingParamsDb");

	if (!mainConnection.open())
	{
		if (mainConnection.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::createOrConnectParamsBd() when try to create/open startingParamsDb. Error:\n" << mainConnection.lastError().text();
			emit errorLog("Error in ParamsClass::createOrConnectParamsBd() when try to create/open startingParamsDb. Error:\n" + mainConnection.lastError().text());
		}
		else
			qDebug() << "NOT OPEN startingParamsDb";

		return false;
	}

	if (!tempForCreateTable)
		qDebug() << "OPEN SQLite file with starting params (startingParamsDb)";
	else
		qDebug() << "CREATE SQLite file with starting params (startingParamsDb)";

	readySQLite = true;

	return tempForCreateTable;
}



bool ParamsClass::createTable()
{
	QSqlQuery query(mainConnection);

	QString queryString = ("CREATE TABLE DbGeneralParams (ip VARCHAR(15), port VARCHAR(8), name VARCHAR(30), login VARCHAR(30), password VARCHAR(30));");

	if (!query.exec(queryString))
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::createTable() when try to create DbGeneralParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::createTable() when try to create DbGeneralParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT CREATE DbGeneralParams";

		return false;
	}

	qDebug() << "CREATE DbGeneralParams";

	queryString = ("CREATE TABLE smtpParams (mailSender VARCHAR(40), passForApp VARCHAR(30), host VARCHAR(30));");

	if (!query.exec(queryString))
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::createTable() when try to create smtpParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::createTable() when try to create smtpParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT CREATE smtpParams";

		return false;
	}

	qDebug() << "CREATE smtpParams";

	queryString = ("CREATE TABLE maxParams (url VARCHAR(300), chatIdAdmin VARCHAR(30));");

	if (!query.exec(queryString))
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::createTable() when try to create maxParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::createTable() when try to create maxParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT CREATE maxParams";

		return false;
	}

	qDebug() << "CREATE maxParams";

	queryString = ("CREATE TABLE tgParams (token VARCHAR(100), chatIdAdmin VARCHAR(30));");

	if (!query.exec(queryString))
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::createTable() when try to create tgParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::createTable() when try to create tgParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT CREATE tgParams";

		return false;
	}

	qDebug() << "CREATE tgParams";

	queryString = ("CREATE TABLE smsParams (portName VARCHAR(20), speed VARCHAR(8), dataBits VARCHAR(1), partity VARCHAR(1), stopBits VARCHAR(1));");

	if (!query.exec(queryString))
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::createTable() when try to create smsParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::createTable() when try to create smsParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT CREATE smsParams";

		return false;
	}

	qDebug() << "CREATE smsParams";

	return true;
}



void ParamsClass::writeFirstParamsDb()
{
	std::string host;
	qDebug() << "Host DataBase:";
	std::cin >> host;
	host = validateHost(host);

	std::string port;
	qDebug() << "Port DataBase:";
	std::cin >> port;
	port = validatePort(port);

	std::string name;
	qDebug() << "Name DataBase:";
	std::cin >> name;
	name = validateText(name);

	std::string login;
	qDebug() << "Login for connection to DataBase:";
	std::cin >> login;
	login = validateText(login);

	std::string password;
	qDebug() << "Password for connection to DataBase:";
	std::cin >> password;
	password = validateText(password);

	QSqlQuery query(mainConnection);

	query.prepare("INSERT INTO DbGeneralParams (ip,  port, name, login, password) VALUES (?, ?, ?, ?, ?)");
	query.addBindValue(QString::fromStdString(host));
	query.addBindValue(QString::fromStdString(port));
	query.addBindValue(QString::fromStdString(name));
	query.addBindValue(QString::fromStdString(login));
	query.addBindValue(QString::fromStdString(password));

	if (!query.exec())
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::writeFirstParamsDb() when try to insert value in DbGeneralParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::writeFirstParamsDb() when try to insert value in DbGeneralParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT INSERT in DbGeneralParams";
	}
	else
		qDebug() << "Params for connection to DataBase was write";
}



std::string ParamsClass::validateHost(std::string tempHost)
{
	std::string tempHostLocal = tempHost;

	do {
		QRegularExpression strPattern(QString(R"([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3})"));

		QRegularExpressionMatch matchReg = strPattern.match(QString::fromStdString(tempHostLocal));

		if (matchReg.hasMatch())
		{
			qDebug() << matchReg.captured();
			break;
		}

		qDebug() << "Not find in your messege HOST with format \"0.0.0.0\". Try again";

		std::cin >> tempHostLocal;

	} while (true);

	return tempHostLocal;
}



std::string ParamsClass::validatePort(std::string tempPort)
{
	std::string tempPortLocal = tempPort;

	do {
		QRegularExpression strPattern(QString(R"([0-9]{4,5})"));

		QRegularExpressionMatch matchReg = strPattern.match(QString::fromStdString(tempPortLocal));

		if (matchReg.hasMatch())
		{
			qDebug() << matchReg.captured();
			break;
		}

		qDebug() << "Not find in your messege HOST with format \"4-5 (digit)\". Try again";

		std::cin >> tempPortLocal;

	} while (true);

	return tempPortLocal;
}



std::string ParamsClass::validateText(std::string tempString)
{
	std::string tempStringForCheck = tempString;

	do {

		if (tempStringForCheck.length() >= 35 || tempStringForCheck.length() <= 2)
		{
		}
		else
			break;

		qDebug() << "Incorrect length for your messege. Try again";

		std::cin >> tempStringForCheck;

	} while (true);

	return tempStringForCheck;
}



bool ParamsClass::sendStringListForMainDb()
{
	QSqlQuery query(mainConnection);
	QString queryString = ("SELECT ip, port, name, login, password FROM DbGeneralParams");
	QStringList tempList;

	if (!query.exec(queryString) || !query.next())
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::sendStringListForMainDb() when try to read from DbGeneralParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::sendStringListForMainDb() when try to read from DbGeneralParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT READ from DbGeneralParams table";

		return false;
	}
	else
	{
		for (int count = 0; count < 5; count++)
		{
			tempList << query.value(count).toString();
		}

		emit signalFromParamsClassForConnectToMainDb(tempList);

		return true;
	}
}



void ParamsClass::writeParamsSmtp()
{
	QString queryString = ("CREATE TABLE smtpParams (mailSender VARCHAR(40), passForApp VARCHAR(30), host VARCHAR(30));");

	std::string mailSender;
	qDebug() << "Write Mail for send or (P/p) for pass this block:";
	std::cin >> mailSender;
	if (mailSender == "P" || mailSender == "p")
	{
		qDebug() << "Mail sending was pass";
		return;
	}
	else
		mailSender = validateText(mailSender);

	std::string passForApp;
	qDebug() << "Password for external app:";
	std::cin >> passForApp;
	passForApp = validateText(passForApp);

	std::string host;
	qDebug() << "Smtp host:";
	std::cin >> host;
	host = validateText(host);

	QSqlQuery query(mainConnection);

	query.prepare("INSERT INTO smtpParams (mailSender,  passForApp, host) VALUES (?, ?, ?)");
	query.addBindValue(QString::fromStdString(mailSender));
	query.addBindValue(QString::fromStdString(passForApp));
	query.addBindValue(QString::fromStdString(host));

	if (!query.exec())
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::writeParamsSmtp() when try to insert value in smtpParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::writeParamsSmtp() when try to insert value in smtpParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT INSERT in smtpParams";
	}
	else
		qDebug() << "Params for smtp was write";
}



bool ParamsClass::sendStringListForSmtpClass()
{
	QSqlQuery query(mainConnection);
	QString queryString = ("SELECT mailSender,  passForApp, host FROM smtpParams");
	QStringList tempList;

	if (!query.exec(queryString) || !query.next())
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::sendStringListForSmtpClass() when try to read from smtpParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::sendStringListForSmtpClass() when try to read from smtpParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT READ from smtpParams table";

		return false;
	}
	else
	{
		for (int count = 0; count < 3; count++)
		{
			tempList << query.value(count).toString();
		}

		emit signalFromParamsClassForSmtpWithParams(tempList);

		return true;
	}
}



void ParamsClass::writeParamMax()
{
	std::string url;
	qDebug() << "Write URL for MAX or (P/p) for pass this block:";
	std::cin >> url;

	if (url == "P" || url == "p")
	{
		qDebug() << "MAX sending was pass";
		return;
	}
	else
		do {
			if (url.length() >= 300 || url.length() <= 10)
			{
			}
			else
				break;

			qDebug() << "Incorrect length for your messege. Try again";

			std::cin >> url;

		} while (true);

	std::string chatIdAdmin;
	qDebug() << "Phone number for admin:";
	std::cin >> chatIdAdmin;

	do {
		if (chatIdAdmin.length() >= 25 || chatIdAdmin.length() <= 5)
		{
		}
		else
			break;

		qDebug() << "Incorrect length for your messege. Try again";

		std::cin >> chatIdAdmin;

	} while (true);

	QSqlQuery query(mainConnection);

	query.prepare("INSERT INTO maxParams (url,  chatIdAdmin) VALUES (?, ?)");
	query.addBindValue(QString::fromStdString(url));
	query.addBindValue(QString::fromStdString(chatIdAdmin));

	if (!query.exec())
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::writeParamMax() when try to insert value in maxParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::writeParamMax() when try to insert value in maxParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT INSERT in maxParams";
	}
	else
		qDebug() << "Params for MAX was write";
}



bool ParamsClass::sendStringListForMaxClass()
{
	QSqlQuery query(mainConnection);
	QString queryString = ("SELECT url, chatIdAdmin FROM maxParams");
	QStringList tempList;

	if (!query.exec(queryString) || !query.next())
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::sendStringListForMaxClass() when try to read from maxParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::sendStringListForMaxClass() when try to read from maxParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT READ from maxParams table";

		return false;
	}
	else
	{
		for (int count = 0; count < 2; count++)
		{
			tempList << query.value(count).toString();
		}

		emit signalFromParamsClassForMaxWithParams(tempList);

		return true;
	}
}



void ParamsClass::writeParamTg()
{
	std::string token;
	qDebug() << "Write Token for Telegram or (P/p) for pass this block:";
	std::cin >> token;

	if (token == "P" || token == "p")
	{
		qDebug() << "Telegram sending was pass";
		return;
	}
	else
		do {
			if (token.length() >= 100 || token.length() <= 10)
			{
			}
			else
				break;

			qDebug() << "Incorrect length for your messege. Try again";

			std::cin >> token;

		} while (true);

	std::string chatIdAdmin;
	qDebug() << "ChatID for admin:";
	std::cin >> chatIdAdmin;

	do {
		if (chatIdAdmin.length() >= 25 || chatIdAdmin.length() <= 5)
		{
		}
		else
			break;

		qDebug() << "Incorrect length for your messege. Try again";

		std::cin >> chatIdAdmin;

	} while (true);

	QSqlQuery query(mainConnection);

	query.prepare("INSERT INTO tgParams (token,  chatIdAdmin) VALUES (?, ?)");
	query.addBindValue(QString::fromStdString(token));
	query.addBindValue(QString::fromStdString(chatIdAdmin));

	if (!query.exec())
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::writeParamTg() when try to insert value in tgParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::writeParamTg() when try to insert value in tgParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT INSERT in tgParams";
	}
	else
		qDebug() << "Params for Telegram was write";
}



bool ParamsClass::sendStringListForTelegramClass()
{
	QSqlQuery query(mainConnection);
	QString queryString = ("SELECT token, chatIdAdmin FROM tgParams");
	QStringList tempList;

	if (!query.exec(queryString) || !query.next())
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::sendStringListForTelegramClass() when try to read from tgParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::sendStringListForTelegramClass() when try to read from tgParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT READ from tgParams table";

		return false;
	}
	else
	{
		for (int count = 0; count < 2; count++)
		{
			tempList << query.value(count).toString();
		}

		emit signalFromParamsClassForTelegramWithParams(tempList);

		return true;
	}
}



void ParamsClass::writeParamSms()
{
	std::string comPort;
	qDebug() << "COM port for sms (Example: COM1) or (P/p) for pass this block:";
	std::cin >> comPort;

	if (comPort == "P" || comPort == "p")
	{
		qDebug() << "SMS sending was pass";
		return;
	}
	else
		do {
			if (comPort.length() >= 6 || comPort.length() <= 3)
			{
			}
			else
				break;

			qDebug() << "Incorrect length for your messege. Try again";

			std::cin >> comPort;

		} while (true);

	std::string baudRate;
	qDebug() << "Baud rate:";
	std::cin >> baudRate;

	do {
		if (baudRate.length() >= 7 || baudRate.length() <= 2)
		{
		}
		else
			break;

		qDebug() << "Incorrect length for your messege. Try again";

		std::cin >> baudRate;

	} while (true);

	std::string bitsInFrame;
	qDebug() << "Bits is frame:";
	std::cin >> bitsInFrame;

	do {
		if (bitsInFrame.length() >= 3 || bitsInFrame.length() <= 0)
		{
		}
		else
			break;

		qDebug() << "Incorrect length for your messege. Try again";

		std::cin >> bitsInFrame;

	} while (true);

	std::string partity;
	qDebug() << "Choose your partity: (1-NoParity (recommendary), 2-EvenParity, 3-OddParity, 4-SpaceParity, 5-MarkParity)";
	std::cin >> partity;

	do {
		if (partity == "1" || partity == "2" || partity == "3" || partity == "4" || partity == "5")
		{
			break;
		}
		else
		{
		}

		qDebug() << "Incorrect number of partity in your messege. Try again";

		std::cin >> partity;

	} while (true);

	std::string stopBits;
	qDebug() << "Stop bits:";
	std::cin >> stopBits;

	do {
		if (stopBits.length() >= 2 || stopBits.length() <= 0)
		{
		}
		else
			break;

		qDebug() << "Incorrect length for your messege. Try again";

		std::cin >> stopBits;

	} while (true);

	QSqlQuery query(mainConnection);

	query.prepare("INSERT INTO smsParams (portName, speed, dataBits, partity, stopBits) VALUES (?, ?, ?, ?, ?)");
	query.addBindValue(QString::fromStdString(comPort));
	query.addBindValue(QString::fromStdString(baudRate));
	query.addBindValue(QString::fromStdString(bitsInFrame));
	query.addBindValue(QString::fromStdString(partity));
	query.addBindValue(QString::fromStdString(stopBits));

	if (!query.exec())
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::writeParamTg() when try to insert value in tgParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::writeParamTg() when try to insert value in tgParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT INSERT in tgParams";
	}
	else
		qDebug() << "Params for SMS was write";
}



bool ParamsClass::sendStringListForSmsClass()
{
	QSqlQuery query(mainConnection);
	QString queryString = ("SELECT portName, speed, dataBits, partity, stopBits FROM smsParams");
	QStringList tempList;

	if (!query.exec(queryString) || !query.next())
	{
		if (query.lastError().isValid())
		{
			qDebug() << "Error in ParamsClass::signalFromParamsClassForSmsClass() when try to read from smsParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
			emit errorLog("Error in ParamsClass::signalFromParamsClassForSmsClass() when try to read from smsParams table. Query:\n" + query.lastQuery() + "\nError text:\n" + query.lastError().text());
		}
		else
			qDebug() << "NOT READ from smsParams table";

		return false;
	}
	else
	{
		for (int count = 0; count < 5; count++)
		{
			tempList << query.value(count).toString();
		}

		emit signalFromParamsClassForSmsClassWithParams(tempList);

		return true;
	}
}
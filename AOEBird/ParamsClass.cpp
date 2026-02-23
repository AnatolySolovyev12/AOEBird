#include "ParamsClass.h"

ParamsClass::ParamsClass(QObject* parent)
	: QObject(parent)
{
	if (createOrConnectParamsBd())
		if (createTable())
			writeFirstParamsDb();

	QTimer::singleShot(1000, [this]() {
		emit signalFromParamsClassForConnectToMainDb();
		});
}



ParamsClass::~ParamsClass()
{
}



bool ParamsClass::createOrConnectParamsBd()
{
	QFile tempForCheckDb;
	tempForCheckDb.setFileName("startingParamsDb");
	bool tempForCreateTable = true;

	if (tempForCheckDb.exists()) tempForCreateTable = false;

	mainConnection = QSqlDatabase::addDatabase("QSQLITE");
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

	qDebug() << "OPEN startingParamsDb";

	return tempForCreateTable;
}



bool ParamsClass::createTable()
{
	QSqlQuery query(mainConnection);
	QString queryString = ("CREATE TABLE DbGeneralParams (ip TEXT, port TEXT, name TEXT, login TEXT, password TEXT);");

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
	validateNameLoginPassword(name);

	std::string login;
	qDebug() << "Login for connection to DataBase:";
	std::cin >> login;
	validateNameLoginPassword(login);

	std::string password;
	qDebug() << "Password for connection to DataBase:";
	std::cin >> password;
	validateNameLoginPassword(password);

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
		if (tempHostLocal == "localhost") break;

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



std::string ParamsClass::validateNameLoginPassword(std::string tempString)
{
	std::string tempStringForCheck = tempString;

	do {
		
		if (tempStringForCheck.length() >= 25 || tempStringForCheck.length() <= 2)
		{
		}
		else
			break;

		qDebug() << "Incorrect length for your messege. Try again";

		std::cin >> tempStringForCheck;

	} while (true);

	return tempStringForCheck;
}
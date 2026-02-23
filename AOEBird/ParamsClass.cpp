#include "ParamsClass.h"

ParamsClass::ParamsClass(QObject* parent)
	: QObject(parent)
{
	if (createOrConnectParamsBd())
		createTable();
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
		{
			qDebug() << "NOT OPEN startingParamsDb";
		}
		return false;
	}

	qDebug() << "OPEN startingParamsDb";

	return tempForCreateTable;
}



bool ParamsClass::createTable()
{
	QSqlQuery query(mainConnection);
	QString queryString = ("CREATE TABLE DbGeneralParams ( ip TEXT, port TEXT, login TEXT,password TEXT);");

	if (!query.exec(queryString))
	{
		qDebug() << "Error in ParamsClass::createTable() when try to create DbGeneralParams table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError();
		return false;
	}

	qDebug() << "CREATE DbGeneralParams";

	return true;
}
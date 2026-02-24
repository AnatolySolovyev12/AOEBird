#include "DataBaseClass.h"

DataBaseClass::DataBaseClass(QObject *parent)
	: QObject(parent)
{}

DataBaseClass::~DataBaseClass()
{
	if (mainDbConnection.isOpen()) 
	{
		mainDbConnection.close();
	}

	if (QSqlDatabase::contains("postgres_connection")) 
	{
		QSqlDatabase::removeDatabase("postgres_connection");
	}
}

void DataBaseClass::connectionToMainDb(QStringList signalList)
{
	mainDbConnection = QSqlDatabase::addDatabase("QPSQL", "postgres_connection");
	mainDbConnection.setHostName(signalList[0]);
	mainDbConnection.setPort(signalList[1].toInt());  // По умолчанию 5432
	mainDbConnection.setDatabaseName(signalList[2]);
	mainDbConnection.setUserName(signalList[3]);
	mainDbConnection.setPassword(signalList[4]);

	///////////////////////qDebug() << mainDbConnection.hostName() << " " << mainDbConnection.port() << " " << mainDbConnection.databaseName() << " " << mainDbConnection.userName() << " " << mainDbConnection.password();

	if (!mainDbConnection.open())
	{
		qDebug() << "Error in DataBaseClass::connectionToMainDb() when try to OPEN database." << "\nError text:\n" << mainDbConnection.lastError().text();
	}
	else
	{
		qDebug() << "OPEN " << signalList[2];
		test();
	}
}


void DataBaseClass::test()
{
	QSqlQuery query(mainDbConnection);


	if (!query.exec("CREATE TABLE IF NOT EXISTS testTable (id INT, name TEXT)"))
	{
		qDebug() << "Error in DataBaseClass::connectionToMainDb() when try to create test table. Query:\n" << query.lastQuery() << "\nError text:\n" << query.lastError().text();
	}
	else
		qDebug() << "TABLE testTable WAS ADD OR SKIP";
}
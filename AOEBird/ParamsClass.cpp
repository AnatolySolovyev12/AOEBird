#include "ParamsClass.h"

ParamsClass::ParamsClass(QObject* parent)
	: QObject(parent)
{
	if (!createOrConnectParamsBd())
		createTable();
	qDebug() << "TEST2";
}



ParamsClass::~ParamsClass()
{
}



bool ParamsClass::createOrConnectParamsBd()
{

	mainConnection = QSqlDatabase::addDatabase("QSQLITE");

	mainConnection.setDatabaseName("startingParamsDb");
	qDebug() << "TEST!";

	if (!mainConnection.open())
	{

		emit errorLog("Cannot open database: " /*+ mainConnection.lastError().text() + '\n'*/);

		return false;
	}

	return true;
}



bool ParamsClass::createTable()
{


	/*

QSqlQuery query;

db_input = (R"(
CREATE TABLE channelTable (
  number TEXT,
  date TEXT,
  channelFirst TEXT,
  channelSecond TEXT,
  channelThird TEXT,
  channelFour TEXT,
  repeatCounter INTEGER DEFAULT 0 NOT NULL,
  UNIQUE(number, date)
);
	   )");

if (!query.exec(db_input)) // Âûïîëíÿåì çàïðîñ. exec - âåðí¸ò true åñëè óñïåøíî. Ñèíòàêñèñ äîëæåí îòâå÷àòü çàïðàøèâàåìîé ÁÄ.
{
	if (query.lastError().text() != "table channelTable already exists Unable to execute statemen")
	{
	}
	else
		emit messegeLog("Unable to create a channelTable. " + query.lastError().text() + '\n', QColor(240, 14, 14));
}
else
{
	emit messegeLog("channelTable was create!\n", QColor(255, 128, 0));
}

db_input = (R"(
CREATE TABLE counterTable (
  number TEXT,
  date TEXT,
  channelFirst TEXT,
  channelSecond TEXT,
  channelThird TEXT,
  channelFour TEXT,
  repeatCounter INTEGER DEFAULT 0 NOT NULL,
  UNIQUE(number, date)
);
	   )");
*/

}
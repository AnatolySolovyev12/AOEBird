#include "CheckClass.h"

CheckClass::CheckClass(QObject *parent)
	: QObject(parent), checkTimer(new QTimer())
{
	connect(checkTimer, &QTimer::timeout, this, &CheckClass::checkDbForEvent);
	checkTimer->start(1000);
}

CheckClass::~CheckClass()
{}

void CheckClass::checkValuesFromDb(QStringList temp)
{
	qDebug() << temp;

	if (QDate::fromString(temp[9]) < QDate::currentDate() && QTime::fromString(temp[10]) < QTime::currentTime())
		qDebug() << "LESS THEN NOW";

	qDebug() << temp[9]<< " " << temp[10] << "  VS  " << QDate::currentDate().toString("yyyy.MM.dd") << " " << QTime::currentTime().toString();

}
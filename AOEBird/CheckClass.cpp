#include "CheckClass.h"

CheckClass::CheckClass(QObject* parent)
	: QObject(parent), checkTimer(new QTimer())
{
	connect(checkTimer, &QTimer::timeout, this, &CheckClass::checkDbForEvent);
	checkTimer->start(5000);
}

CheckClass::~CheckClass()
{
}

void CheckClass::checkValuesFromDb(QStringList temp)
{
	qDebug() << temp;
	qDebug() << temp[9] << " " << temp[10] << "  VS  " << QDate::currentDate().toString("yyyy.MM.dd") << " " << QTime::currentTime().toString();

	if (QDate::fromString(temp[9]) <= QDate::currentDate() && QTime::fromString(temp[10]) <= QTime::currentTime())
	{
		if (temp[5] == "true") // MAX
		{
			if (temp[3][0] == '8')
			{
				temp[3][0] = '7';
			}

			if (temp[3][0] == '+')
			{
				temp[3] = temp[3].sliced(1);
				temp[3][0] = '7';
			}
			
			emit sendMax(temp[3], "TEST AUTO FOR MAX");
			qDebug() << "MAX TEST";
		}

		if (temp[6] == "true") // Telegram
		{
			emit sendTelegram("TEST AUTO FOR TELEGRAM"); // надо рихтовать для отправки с chatId
			qDebug() << "TELEGRAM TEST";

		}

		if (temp[7] == "true") // Mail
		{
			emit sendMail(temp[4], "TEST AUTO FOR MAIL", "");

			//вопрос по частоте отправки сообщений
		}

		if (temp[8] == "true") // SMS
		{
			emit sendSMS("89825313114", "TEST SMS");
		}
	}



}
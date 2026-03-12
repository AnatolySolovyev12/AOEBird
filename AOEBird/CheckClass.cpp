#include "CheckClass.h"

CheckClass::CheckClass(QObject* parent, bool readyMax, bool readyTelegram, bool readyMail, bool readySms)
	: QObject(parent), checkTimer(new QTimer()), m_readyMax(readyMax), m_readyTelegram(readyTelegram), m_readyMail(readyMail), m_readySms(readySms)
{
	connect(checkTimer, &QTimer::timeout, this, &CheckClass::checkDbForEvent);
	checkTimer->start(5000);
}

CheckClass::~CheckClass()
{
}

void CheckClass::checkValuesFromDb(QStringList temp, QString chatIdFromSignal)
{
	qDebug() << QDate::currentDate().toString("yyyy-MM-dd") + " " + QTime::currentTime().toString() + " Check...";

	if (QDate::currentDate() >= QDate::fromString(temp[9], "yyyy-MM-dd"))
	{
		if (QTime::fromString(temp[10]) <= QTime::currentTime() || QDate::currentDate() > QDate::fromString(temp[9], "yyyy-MM-dd"))
		{
			qDebug() << temp;
			qDebug() << "Event is Done: " << QDate::currentDate() << " Current: " << QDate::fromString(temp[9], "yyyy-MM-dd") << "   " << QTime::fromString(temp[10]) << " Current: " << QTime::currentTime();

			if (temp[5] == "true" && m_readyMax) // MAX
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
				qDebug() << "MAX TEST MESSEGE";
			}

			if (temp[6] == "true" && m_readyTelegram && chatIdFromSignal != "") // Telegram
			{
				emit sendTelegram(chatIdFromSignal, "TEST AUTO FOR TELEGRAM"); // надо рихтовать для отправки с chatId
				qDebug() << "TELEGRAM TEST MESSEGE";

			}
			
			if (temp[7] == "true" && m_readyMail) // Mail
			{
				emit sendMail(temp[4], "TEST AUTO FOR MAIL", "");
				qDebug() << "MAIL TEST MESSEGE";
			}

			if (temp[8] == "true" && m_readySms) // SMS
			{
				QString tempNumber = temp[3];

				if (tempNumber[0] == '8')
				{
					tempNumber[0] = '7';
					tempNumber.push_front('+');
				}

				if (tempNumber[0] == '7')
				{
					tempNumber.push_front('+');
				}

				emit sendSMSsignal(tempNumber, "TEST SMS");
				qDebug() << "SMS TEST MESSEGE";
			}

			emit deleteInDbSignal(temp[0], temp[1], temp[2]);
		}
	}
}
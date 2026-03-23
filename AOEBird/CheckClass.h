#pragma once

#include <QObject>
#include <QTimer>
#include <QDebug>;
#include <QDate>

class CheckClass  : public QObject
{
	Q_OBJECT

public:
	CheckClass(QObject *parent, bool readyMax, bool readyTelegram, bool readyMail, bool readySms);
	~CheckClass();

	void checkValuesFromDb(QStringList temp, QString chatIdFromSignal);

signals:
	void checkDbForEvent();
	void sendMax(QString phoneNumber, QString messege);
	void sendTelegram(QString chatId, QString messege);
	void sendMail(QString eMail, QString subject, QString messege, QString files);
	void sendSMSsignal(QString phoneNumber, QString messege);
	void deleteInDbSignal(QString id, QString request, QString position);

private:
	QTimer* checkTimer = nullptr;

	bool m_readyMax = false;
	bool m_readyTelegram = false;
	bool m_readyMail = false;
	bool m_readySms = false;
};


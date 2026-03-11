#pragma once

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QUrlQuery>
#include <QFile.h>

class TelegramJacket : public QObject
{
	Q_OBJECT

public:
	TelegramJacket(QObject* parent = nullptr, QString m_token = "", QString m_chatId = "");

	void sendMessage(const QString chatId, const QString message);
	void getUpdates();

signals:
	void sendToDataBaseChatIdAndPhoneNumber(QString chatId, QString phoneNumber);

private:
	QNetworkAccessManager* manager = nullptr;
	QString token = "";
	QString chatIdAdmin = "";

	bool isBusy = false;
	qint64 iD;
};
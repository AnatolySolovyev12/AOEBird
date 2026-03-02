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

	void sendMessage(const QString message);

private:
	QNetworkAccessManager* manager = nullptr;
	QString token = "";
	QString chatId = "";
};
#pragma once

#include <qapplication.h>
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


class MaxClass : public QObject
{
	Q_OBJECT

public:
	MaxClass(QObject* parent = nullptr, QString urlStringFromSignal = "", QString chatIdFromSignal = "");

public slots:
	void sendMessage(const QString phoneNumber, const QString message);
	void checkNumber(const QString phoneNumber, const QString message);

signals:
	void sendAfterGetChatId(const QString chatIdFromSignal, const QString messege);

private:
	QNetworkAccessManager* manager = nullptr;
	QString chatIdAdmin = "";
	QString urlString = "";
	QString urlForCheck = "";
	QString chatIdFromCheck = "";
};
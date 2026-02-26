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
	MaxClass(QObject* parent = nullptr, QString chatIdFromSignal = "", QString urlStringFromSignal = "");

public slots:
	void sendMessage(const QString message);

private:
	QNetworkAccessManager* manager = nullptr;
	QString chatId = "";
	QString urlString = "";
};
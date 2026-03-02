#include "TelegramJacket.h"

TelegramJacket::TelegramJacket(QObject* parent, QString m_token, QString m_chatId)
	: QObject(parent), manager(new QNetworkAccessManager), token(m_token), chatId(m_chatId)
{
}



void TelegramJacket::sendMessage(const QString message)
{
	// Формирование URL запроса
	QString urlString = QString("https://api.telegram.org/bot%1/sendMessage").arg(token);

	QUrl url(urlString);

	// Настройка параметров сообщения
	QUrlQuery query;
	query.addQueryItem("chat_id", chatId);
	query.addQueryItem("text", message);

	// Создание запроса
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	// Отправка запроса
	manager->post(request, query.toString(QUrl::FullyEncoded).toUtf8());

	
	// Обработчик ответа (если необходимо). Пригодится.
	QObject::connect(manager, &QNetworkAccessManager::finished, [](QNetworkReply* reply) {

		if (reply->error() == QNetworkReply::NoError)
		{
			QString response = reply->readAll();
			qDebug() << response;
		}
		else
		{
			qDebug() << "Error:: " << reply->error();
		}
		reply->deleteLater();
		});
}

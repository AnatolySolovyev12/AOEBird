#include "MaxClass.h"

MaxClass::MaxClass(QObject* parent, QString urlStringFromSignal, QString chatIdFromSignal)
	: QObject(parent), manager(new QNetworkAccessManager), urlString(urlStringFromSignal), chatIdAdmin(chatIdFromSignal)
{
	urlForCheck = urlString;
	int pos = urlForCheck.indexOf("sendMessage");
	urlForCheck.remove("sendMessage");
	urlForCheck.insert(pos, "checkAccount");

	// Отправку будем производить после получения chatId по номеру телефона
	connect(this, &MaxClass::sendAfterGetChatId, this, &MaxClass::sendMessage);
}



void MaxClass::sendMessage(const QString phoneNumber, const QString message)
{
	if (message.isEmpty()) {
		qWarning() << "Attempt to send empty message";
		return;
	}

	QUrl url(urlString);

	QJsonObject json;
	json["chatId"] = phoneNumber;
	json["message"] = message; // Используем переданное сообщение

	// Преобразование JSON-объекта в строку
	QJsonDocument jsonDoc(json);
	QByteArray jsonData = jsonDoc.toJson();

	// Создание запроса
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	// Отправка запроса
	QNetworkReply* reply = manager->post(request, jsonData);

	// Обработчик ответа (если необходимо). Пригодится.
	QObject::connect(reply, &QNetworkReply::finished, [reply]() {

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



void MaxClass::checkNumber(const QString phoneNumber, const QString message)
{
	// Производим проверку номера телефона на предмет наличия аккаунта в МАХ

	if (phoneNumber.isEmpty()) {
		qWarning() << "Attempt to send empty message";
		return;
	}

	QUrl url(urlForCheck);

	QJsonObject json;
	json["phoneNumber"] = (phoneNumber == "admin" ? chatIdAdmin : phoneNumber);

	// Преобразование JSON-объекта в строку
	QJsonDocument jsonDoc(json);
	QByteArray jsonData = jsonDoc.toJson();

	// Создание запроса
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	// Отправка запроса
	QNetworkReply* reply = manager->post(request, jsonData);

	// Обработчик ответа (если необходимо). Пригодится.
	QObject::connect(reply, &QNetworkReply::finished, [reply, message, phoneNumber, this]() {

		if (reply->error() == QNetworkReply::NoError)
		{
			QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());

			if (jsonDoc["exist"].toBool())
			{
				chatIdFromCheck = jsonDoc["chatId"].toString();

				// Если аккаунт существует то отправляем сигнал для отправки сообщения с полученным chatId

				emit sendAfterGetChatId(chatIdFromCheck, message);
			}
			else
				qDebug() << "ChatID for " << phoneNumber << " not found";
		}
		else
		{
			qDebug() << "Error:: " << reply->error();
		}

		reply->deleteLater();
		});
}


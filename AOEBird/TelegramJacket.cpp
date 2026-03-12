#include "TelegramJacket.h"

TelegramJacket::TelegramJacket(QObject* parent, QString m_token, QString m_chatId)
	: QObject(parent), manager(new QNetworkAccessManager), managerForCheckBot(new QNetworkAccessManager), token(m_token), chatIdAdmin(m_chatId)
{
	getMe();
	QTimer::singleShot(3000, [this]() {getUpdates(); });
}



void TelegramJacket::sendMessage(const QString chatId, const QString message)
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

	/*
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
		*/
}



void TelegramJacket::getUpdates()
{
	if (isBusy) return; // Уже выполняется запрос

	isBusy = true;

	/*
	QString urlString = QString("https://api.telegram.org/bot%1/deleteWebhook") // метод удаления перехватичков. Защиа от захвата бота и не только.
		.arg(token);
	QNetworkRequest request(urlString);
	QNetworkReply* reply = manager->get(request);
	*/

	// добавлен таймаут для LongPoll (при 0 ShortPoll) в секундах. Также добавлен offset для подтверждения получения сообщдения в Telegram (чтобы повторно не поулчать старые сообщения)
	QString urlString = QString("https://api.telegram.org/bot%1/getUpdates?offset=%2&timeout=12")
		.arg(token)
		.arg(iD);

	QNetworkRequest request(urlString);
	QNetworkReply* reply = manager->get(request);

	QObject::connect(reply, &QNetworkReply::finished, [reply, this]()
		{

			if (reply->error() == QNetworkReply::NoError)
			{
				QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());

				if (jsonDoc["ok"].toBool())
				{
					QJsonArray updates = jsonDoc["result"].toArray();

					for (const QJsonValue& value : updates)
					{

						QJsonObject messageObj = value["message"].toObject();
						QJsonObject fromObj = messageObj["from"].toObject();

						if (messageObj.contains("text") && fromObj.contains("id")) {

							QString text = messageObj["text"].toString();

							qDebug() << "Received message (" << value["update_id"].toInteger() << "): " << messageObj["text"].toString() << "chatId: " << fromObj["id"].toInteger();

							iD = value["update_id"].toInteger() + 1;
							QString chatId = QString::number(fromObj["id"].toInteger());

							if (text == "/start")
							{
								sendMessage(chatId, "(" + chatId + ") " + "Напишите свой номер телефона (Пример: 79995550000)");
							}
							else
							{
								if (text.length() >= 13 || text.length() <= 10)
								{
									sendMessage(chatId, "Неверный формат сообщения. Некорректная длина. Попробуйте еще раз!");
								}
								else
								{
									if (text.contains(QRegularExpression("[A-Za-zА-Яа-я]")))
										sendMessage(chatId, "Неверный формат сообщения. Имеются буквы. Попробуйте еще раз!");
									else
									{
										emit sendToDataBaseChatIdAndPhoneNumber(chatId, text);
										sendMessage(chatId, "Спасибо!");
									}
								}
							}
						}
						else
						{
							qDebug() << "Error in TelegramJacket::getUpdates(1) when try to parse JSON: " << reply->error() << reply->errorString();
						}
					}
				}

				reply->deleteLater();
				isBusy = false;
				QTimer::singleShot(200, this, &TelegramJacket::getUpdates);
			}
			else
			{
				// Обработка ошибки сети
				qDebug() << "Error in TelegramJacket::getUpdates(2) when try to parse JSON: " << reply->error() << reply->errorString();

				reply->deleteLater();
				isBusy = false;

				// Пауза 10 секунд перед повторной попыткой при ошибке
				QTimer::singleShot(10000, this, &TelegramJacket::getUpdates);
			}
		});
}



void TelegramJacket::getMe()
{
	// Формирование URL запроса
	QString urlString = QString("https://api.telegram.org/bot%1/getMe").arg(token);

	QUrl url(urlString);

	// Создание запроса
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	// Отправка запроса
	managerForCheckBot->post(request, "");

	// Обработчик ответа (если необходимо). Пригодится.
	QObject::connect(managerForCheckBot, &QNetworkAccessManager::finished, [](QNetworkReply* reply)
		{
			if (reply->error() == QNetworkReply::NoError)
			{
				QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
				QJsonObject messageObj;

				if (jsonDoc["ok"].toBool())
				{
					messageObj = jsonDoc["result"].toObject();
					qDebug() << "Telegram Bot with name " + messageObj["first_name"].toString() + " is work";
				}
				else
					qDebug() << "Error in TelegramJacket::getMe() because return FALSE in JSON";
			}
			else
			{
				qDebug() << "Error in TelegramJacket::getMe() when try to parse JSON: " << reply->error() << reply->errorString();
			}
			reply->deleteLater();
		});
}

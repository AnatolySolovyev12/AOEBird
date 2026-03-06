#include "SMSClass.h"

SMSClass::SMSClass(QObject* parent)
	: QObject(parent), serial(new QSerialPort)
{
	connect(serial, &QSerialPort::errorOccurred, [=](QSerialPort::SerialPortError error) {
		if (error != QSerialPort::NoError) {
			qDebug() << "Error occurred: " << error;
		}
		});

	connect(serial, &QSerialPort::readyRead, this, &SMSClass::readData);

	serial->setPortName("COM8"); // задаём имя последовательного порта
	serial->setBaudRate(9600, QSerialPort::AllDirections); // скорость обмена и тип направления
	serial->setDataBits(QSerialPort::DataBits(8)); // количество бит данных в кадре
	serial->setFlowControl(QSerialPort::NoFlowControl); // контроль управления потоком
	serial->setParity(QSerialPort::NoParity); // контроль четности
	serial->setStopBits(QSerialPort::StopBits(1)); // устанавливаем стоп биты



	if (!serial->open(QIODeviceBase::ReadWrite)) // открываем в режиме чтения и записи
		qDebug() << "Error in SMSClass wneh try to open COM port. Error:\n" << serial->errorString();
	else
	{
		serial->setDataTerminalReady(true); //DTS
		serial->setRequestToSend(true); //RTS

		qDebug() << "COM8 is OPEN";

		//QTimer::singleShot(500, [this]() {writeData("AT"); }); // лучше начинать с неё
		//QTimer::singleShot(1000, [this]() {writeData("AT+CSCA?"); }); // выводит номер симкарты и формат начала номера (международный(+7) /национальный (8)). То через что будет производится отправка.
		//"AT+CSCA=?" формат с добавление , перед = просто выводит статус ОК, без подробностей. Тестовая команда и так для всех

		//sendSMS("+79825313114", "Dobryj den'. V vashem rajone planiruetsya otklyuchenie elektrichestva v svyazi s provedeniem rabot.");
	}


}

SMSClass::~SMSClass()
{
	closeSerialPort();
}

void SMSClass::closeSerialPort()
{
	if (serial->isOpen())
		serial->close();
}

void SMSClass::readData()
{
	buffer += serial->readAll(); // обязательно буферризируем т.к. модем порой отвечает кусками. Там будем получаеть полный ответ для полноценной его обработки

	if (buffer.contains("OK") || buffer.contains("ERROR"))
	{
		qDebug() << "RX:" << buffer << '\n';
		buffer.clear();
	}
}

void SMSClass::writeData(QByteArray data)
{
	QByteArray command = data + "\r\n"; // без добавления модем не поймёт команды
	serial->write(command);
	qDebug() << "TX:" << command;
}


void SMSClass::sendSMS(QString phone, QString text)
{
	writeData("AT+CMGF=1"); // включаем текстовый режим. Отправка будет производится не из хранилища

	QTimer::singleShot(1000, [this, phone, text]() {

		QString temp = "AT+CMGS=\"" + phone + "\""; // передаём номер телефона в соответствующем формате
		writeData(temp.toUtf8());
		});

	QTimer::singleShot(1500, [this, text]() {
		QByteArray smsData = text.toUtf8() + char(26);  //передаём текст для отправки и собственно завершаем формирование смс и отправляем его со спец символом. Без него ждём ввода.

		//Из мануала:
		//To send the message issue Ctrl - Z char(0x1A hex).
		//To exit without sending the message issue ESC char(0x1B hex).

		serial->write(smsData);
		//	qDebug() << "SMS sent:" << smsData;
		});
}


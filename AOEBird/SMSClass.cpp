#include "SMSClass.h"

SMSClass::SMSClass(QObject *parent)
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
		qDebug() << "COM8 is OPEN";
		writeData("AT#ENHRST = <mod>[, <1440>]");
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
	const QByteArray data = serial->readAll();
	qDebug() << "TEST READ";
	qDebug() << data;
}

void SMSClass::writeData(const QByteArray& data)
{
	const qint64 written = serial->write(data);
	qDebug() << "TEST WRITE";
	qDebug() << data;
	qDebug() << written;
}


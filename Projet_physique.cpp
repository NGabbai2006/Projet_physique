#include "Projet_physique.h"


Projet_physique::Projet_physique(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    for (int i = 0; i < availablePorts.size(); ++i) {
        QSerialPortInfo info = availablePorts[i];
        ui.comboBox_port->addItem(info.portName(), QVariant(info.portName()));

    }
	socket = new QWebSocket();
	QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    QObject::connect(socket, SIGNAL(textMessageReceived(QString)), this, SLOT(onSocketTextMessageReceived(QString)));


}

Projet_physique::~Projet_physique()
{

}

// arduino
void Projet_physique::onSerialPortReadyRead()
{
    QByteArray data = port->readAll();
    QString str(data);
    ui.textEdit_donnees->insertPlainText(str);
    if (socket->isValid()) {
		socket->sendTextMessage(str);
    }
    else {
        ui.InfoConnection->setText("connecte toi au server");
    }
}


void Projet_physique::onSendMessageButtonClicked()
{
    if (port->isOpen())
    {
        port->write(ui.lineEdit_envoi->text().toStdString().c_str());
        //QByteArray donnees = port->readAll();
        //ui.textEdit_donnees->insertPlainText("Message envoye : " +donnees+ "\n");
        //ui.lineEdit_envoi->clear(); 
    }
}

void Projet_physique::onOpenPortButtonClicked()
{
    if (ui.comboBox_port->currentIndex() >= 0)
    {
        port = new QSerialPort(ui.comboBox_port->currentText());
        QObject::connect(port, SIGNAL(readyRead()), this, SLOT(onSerialPortReadyRead()));
        port->setBaudRate(9600);
        port->setDataBits(QSerialPort::DataBits::Data8);
        port->setParity(QSerialPort::Parity::NoParity);
        port->setStopBits(QSerialPort::StopBits::OneStop);
        if (port->open(QIODevice::OpenModeFlag::ExistingOnly | QIODevice::OpenModeFlag::ReadWrite))
        {
            ui.label_status->setText("Status port : Ouvert");
        }
        else 
        {
            ui.label_status->setText("Status port : Erreur d'ouverture");
		}
    }
}



void Projet_physique::onDisplayButtonConnectionClicked()
{
    QString ip = ui.infoIp->text();
    QString port = ui.infoPort->text();

    bool ok;
	int portAsInt = port.toInt(&ok);
    if (ok) {
		socket->open(QUrl("ws://" + ip + ":" + port));
	}
	
}

// websocket

void Projet_physique::onSocketConnected()
{
	ui.InfoConnection->setText("Status Connexion: Connected");
}

void Projet_physique::onSocketDisconnected()
{
	ui.InfoConnection->setText("Status Connexion: Disconnected");
}


void Projet_physique::onSocketTextMessageReceived(const QString& msg) {

    ui.InfoConnection->setText("Status connexion");
    ui.textEdit_AfficheWB->append("Message recu du client : " + msg);
}


void Projet_physique::onDisplayButtonMessageClicked()
{
    QString str = ui.lineEdit_TestEnvoie->text();
    if (socket->isValid()) {
        socket->sendTextMessage(str);

    }
    else {
        ui.InfoConnection->setText("connecte toi au server");
    }
}


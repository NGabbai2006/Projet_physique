#include "Projet_physique.h"
#include <QNetworkInterface>
#include <QDesktopServices>
#include <QDir>
#include <QCoreApplication>

Projet_physique::Projet_physique(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    for (int i = 0; i < availablePorts.size(); ++i) {
        QSerialPortInfo info = availablePorts[i];
        ui.comboBox_port->addItem(info.portName(), QVariant(info.portName()));

    }
    // créer le server
	server = new QWebSocketServer(QStringLiteral("Projet Physique Server"), QWebSocketServer::NonSecureMode, this);

}

Projet_physique::~Projet_physique()
{

}

// arduino
void Projet_physique::onSerialPortReadyRead()
{
	// evite les bugs de valeurs en lisant ligne par ligne et pas tout d'un coup 
    while (port->canReadLine()) {
        QByteArray data = port->readLine();
        QString ligne(data);

		// retire les espaces et les retours à la ligne
        ligne = ligne.trimmed();
        
        ui.textEdit_donnees->insertPlainText(ligne + "\n");

        // --- CORRECTION : Envoi à TOUTES les pages web connectées à votre serveur ---
        if (!ligne.isEmpty()) {
            for (QWebSocket* client : qAsConst(m_clients)) {
                if(client->isValid()) {
                    client->sendTextMessage(ligne);
                }
            }
            qDebug() << "ENVOYE :" << ligne;
        }
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
            quint16 wsPortNumber = 2000; 

            // lance le serveur
            if (server->listen(QHostAddress::Any, wsPortNumber)) {
                QObject::connect(server, &QWebSocketServer::newConnection, this, &Projet_physique::onServerNewConnection);
                QString ipAddress;
                QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
                // cherche la première adresse IPv4 qui n'est pas "localhost" (127.0.0.1)
                for (const QHostAddress &entry : ipAddressesList) {
                    if (entry != QHostAddress::LocalHost && entry.toIPv4Address()) {
                        ipAddress = entry.toString();
                        break;
                    }
                }

                // affiche l'IP et le port dans votre labelServer
                ui.labelServer->setText("Serveur: " + ipAddress + ":" + QString::number(wsPortNumber));
                ui.InfoConnection2->setText("Serveur demarre");
            }
            else {
                ui.InfoConnection2->setText("Erreur démarrage serveur (port " + QString::number(wsPortNumber) + " bloque)");
                ui.labelServer->setText("Serveur hors ligne");
            }
        }
        else
        {
            ui.label_status->setText("Status port : Erreur d'ouverture");
        }
    }
}



// websocket

void Projet_physique::onServerNewConnection() {
    ui.InfoConnection2->setText("s'est connectee !");

    QWebSocket* client = server->nextPendingConnection();

    QObject::connect(client, SIGNAL(textMessageReceived(QString)), this, SLOT(onSocketTextMessageReceived(QString)));
    QObject::connect(client, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));

    m_clients.append(client);
    qDebug() << "client co ! nb Clients :" << m_clients.size();
}

void Projet_physique::onSocketTextMessageReceived(const QString& msg) {

    ui.InfoConnection2->setText("Status connexion");
	qDebug() << "Message reçu du client : " << msg;
}


void Projet_physique::onDisplayButtonMessageClicked()
{
    QString str = ui.lineEdit_TestEnvoie->text();
    
    // Envoie ce message test à toutes les pages web connectées
    for (QWebSocket* client : qAsConst(m_clients)) {
        client->sendTextMessage(str);
    }
}

void Projet_physique::onClientDisconnected() {
    QWebSocket* client = qobject_cast<QWebSocket*>(sender());
    if (client) {
        m_clients.removeAll(client);
        client->deleteLater();
        qDebug() << "client deco ! nb Clients :" << m_clients.size();
    }
}

// lance la page web 
void Projet_physique::onDisplayButtonPageWebClicked()
{
    QString htmlPath = "C:/Users/GABBAI/Desktop/Projet_physique/index.html";
    QDesktopServices::openUrl(QUrl::fromLocalFile(htmlPath));
}


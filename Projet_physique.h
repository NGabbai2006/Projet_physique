#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Projet_physique.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

class Projet_physique : public QMainWindow
{
    Q_OBJECT
   QString bufferSerie;

public:
    Projet_physique(QWidget* parent = nullptr);
    ~Projet_physique();

public slots:
    //arduino
    void onSerialPortReadyRead();
    void onSendMessageButtonClicked();
    void onOpenPortButtonClicked();
    // websocket
    void onDisplayButtonMessageClicked();
    void onSocketTextMessageReceived(const QString& msg);
    void onServerNewConnection();
    void onDisplayButtonPageWebClicked();
    void onClientDisconnected();

private:
    Ui::Projet_physiqueClass ui;
    QSerialPort* port;
    QWebSocketServer* server;
    QList<QWebSocket*> m_clients; 
};




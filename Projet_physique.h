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
    void onDisplayButtonConnectionClicked();
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketTextMessageReceived(const QString& msg);



private:
    Ui::Projet_physiqueClass ui;
    QSerialPort* port;
    QWebSocket* socket;
    QWebSocketServer* server;



};




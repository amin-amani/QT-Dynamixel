#include <QCoreApplication>
#include <QtDebug>
#include <QDebug>
#include "dynamixel.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//printf("salam\n");
//TCPServer *server=new TCPServer(6000);
qDebug("sa");
//qDebug()<<"hi";

//Dynamixel ax12;
//ax12.Init("/dev/ttyUSB0",1000000);
//ax12.SetPosition(40,300,0);
//ax12.SetLed(40,false);
//qDebug();
//ax12.Ping(40);

//qDebug()<<ax12.ReadPosition(40);
    return a.exec();
}

// $Id$
/**
 * @file media.c
 * description : Media class use to read and write over selected interface like serial port or network
 *
 * @brief Media wrapper class
 *
 * @author A.Amani
   [Note: didi_1364@yahoo.com]
 * @version 1.0
 * @depricated nothing
 * @bug i dont know yet
 * @warning no
 */
#include "media.h"
//========================================================================================================================================
Media::Media(QObject *parent) : QObject(parent)
{
    _timeout=0;
}
//========================================================================================================================================
Media::MEDIA_ERROS Media::Init(QString portName,int baudrate,int byteTimeout,QSerialPort::FlowControl flowControl)
{
    _timeout=0;
    _comport.setPortName(portName);
    _comport.setBaudRate(baudrate);
    _comport.setReadBufferSize(10000);
    _comport.setFlowControl(flowControl);
    _eachBytetimeout=byteTimeout;

    if(_comport.isOpen())return MEDIA_ERROS_DEVICE_NOT_OPEN;

    _comport.open(QSerialPort::ReadWrite);

    if(!_comport.isOpen()){return MEDIA_ERROS_DEVICE_NOT_OPEN;}
    connect(&_comport,SIGNAL(readyRead()),this,SLOT(ComportReadyRead()));
    connect(&_comport,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(ComportError(QSerialPort::SerialPortError)));
    connect(&_timer,SIGNAL(timeout()),this,SLOT(ReceiveDataTimeout()));
    return MEDIA_ERROS_OK;
}
//========================================================================================================================================
Media::MEDIA_ERROS Media::WriteCommand(QByteArray command,QByteArray&response,int timeout,int respnseDelay){
    bool result=false;
    QSignalSpy waitForResPonse(this,SIGNAL(ReceiveDataCompleted()));
    QSignalSpy waitForNoting(this,SIGNAL(Dummy()));
    if(!_comport.isOpen()){
        _comport.open(QSerialPort::ReadWrite);
        return MEDIA_ERROS_DEVICE_NOT_OPEN;
    }
    if(_comport.bytesAvailable()>0){ _comport.readAll();}
    _buffer.clear();

    if(!_comport.write(command)){
        return MEDIA_ERROS_WRITE_ERROR;
    }
    _comport.flush();
#ifdef MediaTrace
  //  qDebug()<<"===================================================================";
 //   qDebug()<<">>"<<command.toHex();
 //   qDebug()<<"===================================================================";
#endif
    waitForNoting.wait(respnseDelay);
    _timer.start(_eachBytetimeout);
    result=waitForResPonse.wait(timeout);

    if(!result){
        emit WriteCommandCompleted();
      // qDebug()<<"MEDIA_ERROS_TIMEOUT_RESPONSE";
        return MEDIA_ERROS_TIMEOUT_RESPONSE;

    }

    if(_buffer.isEmpty()){
        emit WriteCommandCompleted();
//   qDebug()<<"MEDIA_ERROS_NO_RESPONSE";
        return MEDIA_ERROS_NO_RESPONSE;


    }
//    qDebug()<<"MEDIA_OK";
    response=_buffer;
#ifdef MediaTrace
    qDebug()<<"===================================================================";
    qDebug()<<"<<"<<response.toHex();
    qDebug()<<"===================================================================";
#endif
    emit WriteCommandCompleted();
    return MEDIA_ERROS_OK;
}
//========================================================================================================================================
void Media::ReceiveDataTimeout(){
    _timeout++;
    if(_timeout<_eachBytetimeout)return;
    _timer.stop();
    _timeout=0;
    emit ReceiveDataCompleted();
}
//========================================================================================================================================
void Media::ClearBuffer()
{
    _buffer.clear();

}
void Media::SetRTS(bool value)
{
    _comport.setRequestToSend(value);
}
void Media::SetDTR(bool value)
{
    _comport.setDataTerminalReady(value);
}
//========================================================================================================================================
void Media::ComportReadyRead(){
    QByteArray data= _comport.readAll();
//            _comport.waitForReadyRead(300);
    _buffer.append(data);
    emit ReadyRead(_buffer);
    _timeout=0;
}
//========================================================================================================================================
void Media::Disconnect(){
    if (_comport.isOpen()){
        //qInfo() << "media close";
        _comport.waitForBytesWritten(400);
        _comport.readAll();
        _comport.waitForReadyRead(1000);
        _comport.close();
    }
}
//========================================================================================================================================
void Media::ComportError(QSerialPort::SerialPortError e){
    if(_comport.isOpen()){
        const QMetaObject &mo = QSerialPort::staticMetaObject;
        int index = mo.indexOfEnumerator("SerialPortError");
        QMetaEnum metaEnum = mo.enumerator(index);
       // qInfo()<<"MEDIA Error:" <<metaEnum.valueToKey(e);
        _comport.close();
    }
}
//========================================================================================================================================

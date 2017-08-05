// $Id$
/**
 * @file media.h
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
#ifndef MEDIA_H
#define MEDIA_H

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

#include <qdebug.h>
#include <QtTest/QSignalSpy>

//#define MediaTrace
#define Q_ENUM(ENUM) \
    friend Q_DECL_CONSTEXPR const QMetaObject *qt_getEnumMetaObject(ENUM) Q_DECL_NOEXCEPT { return &staticMetaObject; } \
    friend Q_DECL_CONSTEXPR const char *qt_getEnumName(ENUM) Q_DECL_NOEXCEPT { return #ENUM; }
#define Q_FLAG(ENUM) Q_ENUM(ENUM)

class Media : public QObject
{
    Q_OBJECT
private:

    /**
     * @brief Serial port instance
     */
    QSerialPort _comport;
    QByteArray _buffer;
    /**
     * @brief generates event if byte receive complete
     */
    QTimer _timer;
    int _eachBytetimeout;
    int _timeout;


public:
    typedef enum{
        MEDIA_ERROS_DEVICE_NOT_OPEN=-1,
        MEDIA_ERROS_OK=0,
        MEDIA_ERROS_NO_RESPONSE,
        MEDIA_ERROS_WRITE_ERROR,
        MEDIA_ERROS_TIMEOUT_RESPONSE
    }MEDIA_ERROS;
    Q_ENUM(MEDIA_ERROS)
    explicit Media(QObject *parent = 0);
        /**
        *
        *@brief Initialize class
        *@param             portName        Serial port name
        *@param             baudrate        Serial port baudrate
        *@param             byteTimeout     timeout (ms) serial port cant be free more than this
        *@return            MEDIA_ERROS
        *
        */
    MEDIA_ERROS Init(QString portName,int baudrate,int byteTimeout,QSerialPort::FlowControl flowControl=QSerialPort::NoFlowControl);
        /**
        *
        *@brief             write to media and waite until get replay with specific timeout
        *@param             command     QString command to send
        *@param             replay      command answer
        *@param             timeou      replay timeout
        *@return            MEDIA_ERROS
        *
        */
    MEDIA_ERROS WriteCommand(QByteArray command,QByteArray&replay,int timeout,int respnseDelay=0);

        /**
         *
         * @brief Disconnect and close media even delee media pointer
         *
         */
    void Disconnect(void);

    void ClearBuffer();
    void SetDTR(bool value);
    void SetRTS(bool value);
signals:
        /**
         *
         * @brief Automatically becoms emit when WriteCommand() rutin finish
         *
         */
    void WriteCommandCompleted();
        /**
         *
         * @brief ReceiveDataCompleted internalaly used and become emit when serial port readyread free for specific timeout
         *
         */
    void ReceiveDataCompleted(void);
    void Dummy();
    void ReadyRead(QByteArray);
public slots:
        /**
         *
         * @brief Serial port ready to read slot
         *
         */
    void ComportReadyRead(void);
        /**
         *
         * @brief ReceiveDataTimeout slot
         *
         */
    void ReceiveDataTimeout(void);
        /**
         *
         * @brief ComportError
         * @param e     Error event argument
         *
         */
    void ComportError(QSerialPort::SerialPortError e);
};

#endif // MEDIA_H

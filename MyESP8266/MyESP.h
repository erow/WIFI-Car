
#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "Arduino.h"

//#define ESP8266_USE_SOFTWARE_SERIAL

#ifdef ESP8266_USE_SOFTWARE_SERIAL
#include <SoftwareSerial.h>
#endif

//#define SERIAL_DB

#ifdef SERIAL_DB
#define DB(M)   Serial.println(String("DB:")+M);
#else
#define DB(M);
#endif // SERIAL_DB

#define _
enum MSG_TYPE{
	CONNECTED,
	DISCONNECTED,
	RECIEVE,
	NOTHING,
	UNKNOWN
};
class ESP8266 {
 public:
     #ifdef ESP8266_USE_SOFTWARE_SERIAL
    ESP8266(SoftwareSerial &uart, uint32_t baud = 115200);
    #else
    ESP8266(HardwareSerial &uart, uint32_t baud);
    #endif // ESP8266_USE_SOFTWARE_SERIAL
    void rx_empty(void);


    bool testAT(String x);
    bool queryAT(String x);
    bool setAT(String x,String value);
    bool doAT(String x);
    bool connectWIFI(String ssid,String pwd);//AT+CWJAP="AAAAAA","85810967"
    bool startTCPSERVER(String port);//AT+CIPSERVER=1,500
    bool closeTCPSERVER();
    bool sendMSG(char id,String msg);
    bool do_recv(String s="OK");
    bool ready();//返回当前是否有消息
    bool check(String t="FAIL");
    /*检查收到的数据 是否出错*/

    String listWIFI();//AT+CWLAP
    String showIP();//AT+CIFSR
    String TCP_recv(uint32_t);
    String recvString( uint32_t timeout=200);
    /*  不断接收数据。只要有数据发送过来就不会断开，只有一段时间收不到才断开。*/

	MSG_TYPE TCP_LOOP();
	char getTCP_CILENT();
	/* 分析TCP消息，返回当前状态*/
    String tmp_recv;
    #ifdef ESP8266_USE_SOFTWARE_SERIAL
    SoftwareSerial * _esp; /* The UART to communicate with ESP8266 */
    #else // ESP8266_USE_SOFTWARE_SERIAL
    HardwareSerial * _esp;
    #endif // ESP8266_USE_SOFTWARE_SERIAL
private:
	char TCP_CILENT;//标记连接的客户端，每一位表示一个建立的连接。
};

#endif

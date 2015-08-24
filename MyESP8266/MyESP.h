
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
    bool ready();//���ص�ǰ�Ƿ�����Ϣ
    bool check(String t="FAIL");
    /*����յ������� �Ƿ����*/

    String listWIFI();//AT+CWLAP
    String showIP();//AT+CIFSR
    String TCP_recv(uint32_t);
    String recvString( uint32_t timeout=200);
    /*  ���Ͻ������ݡ�ֻҪ�����ݷ��͹����Ͳ���Ͽ���ֻ��һ��ʱ���ղ����ŶϿ���*/

	MSG_TYPE TCP_LOOP();
	char getTCP_CILENT();
	/* ����TCP��Ϣ�����ص�ǰ״̬*/
    String tmp_recv;
    #ifdef ESP8266_USE_SOFTWARE_SERIAL
    SoftwareSerial * _esp; /* The UART to communicate with ESP8266 */
    #else // ESP8266_USE_SOFTWARE_SERIAL
    HardwareSerial * _esp;
    #endif // ESP8266_USE_SOFTWARE_SERIAL
private:
	char TCP_CILENT;//������ӵĿͻ��ˣ�ÿһλ��ʾһ�����������ӡ�
};

#endif

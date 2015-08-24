

#include "MyESP.h"
#include <avr/pgmspace.h>



#ifdef ESP8266_USE_SOFTWARE_SERIAL
    ESP8266::ESP8266(SoftwareSerial &uart, uint32_t baud): _esp(&uart)
    {
    _esp->begin(baud);
    rx_empty();
}
#else // ESP8266_NO_USE_SOFTWARE_SERIAL
   ESP8266::ESP8266(HardwareSerial &uart, uint32_t baud): _esp(&uart)
   {
    _esp->begin(baud,SERIAL_8O1);
    //AT+UART_CUR=115200,8,1,1,3
    rx_empty();
    }
#endif // ESP8266_USE_SOFTWARE_SERIAL

bool ESP8266::doAT(String x){
    _esp->print("AT+"+x+"\r\n");

    #ifdef SERIAL_DB
    Serial.print("DB:AT+"+x+"\r\n");
    #endif // SERIAL_DB
    recvString(500);
    return !check();
}
bool ESP8266::testAT(String x){
    _esp->print("AT+"+x+"=?\r\n");

    #ifdef SERIAL_DB
    Serial.print("DB:AT+"+x+"=?\r\n");
    #endif // SERIAL_DB
     recvString(500);
    return !check();
}
bool ESP8266::queryAT(String x){
    _esp->print("AT+"+x+"?\r\n");

    #ifdef SERIAL_DB
    Serial.print("DB:AT+"+x+"?\r\n");
    #endif // SERIAL_DB
     recvString(500);
    return !check();
}
bool ESP8266::setAT(String x,String value){
    _esp->print("AT+"+x+ "="+value+"\r\n");

    #ifdef SERIAL_DB
    Serial.print("DB:AT+"+x+ "="+value+"\r\n");
    #endif // SERIAL_DB
     recvString(500);
    return !check();
}

bool ESP8266::check(String t){
   // recvString();
    if (tmp_recv.indexOf("ERROR") != -1)
    {
        #ifdef SERIAL_DB
         DB("wrong AT");
         DB(tmp_recv);
        #endif // SERIAL_DB
         return false;
    }
    if (tmp_recv.indexOf(t) != -1) {
        return true;
    }
    return false;
}
void ESP8266::rx_empty(void)
{
    while(_esp->available() > 0) {
        _esp->read();
    }
}
bool ESP8266::connectWIFI(String ssid,String pwd)
{
    return setAT("CWJAP",String("\"")+ssid+"\""+",\""+pwd+"\"");
}
bool ESP8266::do_recv(String s)
{
    while(!ready())
        ;
    recvString();
    return check(s);
}
bool ESP8266::startTCPSERVER(String port)
{
	/*
	AT+CIPMUX=1
	AT+CIPSERVER=1,port
	*/
    if(setAT("CIPMUX","1"))
        return setAT("CIPSERVER","1,"+port);
    else
        return false;
}
bool ESP8266::closeTCPSERVER()
{
    return setAT("CIPSERVER","0");
}
String ESP8266::listWIFI()
{
    if(!doAT("CWLAP"))
       return "wrong";
    DB(tmp_recv);
    return tmp_recv;
	/*
	AT+CWLAP
	+CWLAP:(4,"sonia",-90,"9c:21:6a:63:d7:e6",1)
	+CWLAP:(4,"FAST_502A",5,(-:),6e7A,a648:+A9f)"2::L01ePK24)
*/
}
bool ESP8266::ready()
{
    return (_esp->available()>0);
}
String ESP8266::TCP_recv(uint32_t id)
{
    String data;
    char a;

    return recvString(100);
}
char ESP8266::getTCP_CILENT()
{
	return TCP_CILENT;
}
MSG_TYPE ESP8266::TCP_LOOP()
{
	if (!ready())
		return NOTHING;
    recvString(100);
    char a=tmp_recv[0];
    if(a>='0'&&a<'5')
    {//极有可能是客户端的连接与断开。
        char i=a-'0';//客户端id

        if(check("CLOSE"))
        {
            TCP_CILENT&=~(1<<i);
            DB("close"+a);
            return DISCONNECTED;
        }
        if(check("CONNECT"))
        {
            TCP_CILENT|=(1<<i);
            DB("CONNECT"+a);
            return CONNECTED;
        }
        DB("something wrong :a");
    }
    else
    if(a==13)
    {
      if(check("+IPD"))
      {
          return RECIEVE;
      }
    }
    else
    {
        DB("assic:"+String((int)a));
    }

    return UNKNOWN;
}
bool ESP8266::sendMSG(char id,String msg)
{

    _esp->print("AT+CIPSENDEX="+String(id)+","+(String)msg.length()+"\r\n");
    while(!ready())        ;
    recvString(100);
    DB("TRY SENDMSG :")
    if(check(">")||check("OK"))
    {
        //DB("AT+CIPSENDEX="+id+","+msg.length())
        _esp->print(msg);
        while(!ready())       ;
        recvString(100);
        return (check("SEND OK"));
    }

    return false;
}
String ESP8266::recvString( uint32_t timeout)
{
    String data;
    char a;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        if(_esp->available() > 0) {
            a = _esp->read();
			if(a == '\0') continue;
            data += a;

            start = millis();
        }
    }
    DB("RECV:"+data);
    tmp_recv=data;
    return data;
}

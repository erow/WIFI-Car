//#define SERIAL_DB

#include <MyESP.h>
#include <AFMotor.h>
#include <SoftwareSerial.h>
SoftwareSerial myserial(2,3);
/*
 * 3-- rx
 * 2-- tx
 */
 #define SET_MODE(A,B) if(!esp.setAT(A,B)) Serial.println("FAIL:"+A+B);
ESP8266 esp(Serial,115200);
AF_DCMotor motorl(1);
AF_DCMotor motorr(3);
 unsigned char  speed_l=0,speed_r=0;
void setup()
{
  Serial.begin(115200);
 /*
  SET_MODE(String("CIPMUX"),"1")
  else
  SET_MODE(String("CIPSERVER"),"1,500");
  esp.connectWIFI("AAAAAA","85810967");
*/
 
  motorl.setSpeed(speed_l);
  motorr.setSpeed(speed_r);
  motorl.run(FORWARD);
  motorr.run(FORWARD);
  delay(1000);//等待ESP启动
  pinMode(13,OUTPUT);//led灯灭表示系统启动正常
  digitalWrite(13,HIGH);
  while(1){
  esp.startTCPSERVER("500");
  if(esp.check("OK")){
  DB("SERVER SUC!")
  break;
  }
  else
  DB("SERVER FAIL!")
  delay(1000);
  }
  digitalWrite(13,LOW);
  /*
  while(!esp.check("CONNECT"))
  Serial.println(esp.tmp_recv);
  */
 // Serial.println("tcp connected");
}
unsigned char stat=3;
//标识车轮的状态：FORWARD/BACKWARD
void loop()
{
  if(stat&1)
      motorl.run(FORWARD);
  else
      motorl.run(BACKWARD);
  if(stat&2)
      motorr.run(FORWARD);
  else
      motorr.run(BACKWARD);
  if(speed_l<50)        motorl.run(RELEASE);
  if(speed_r<50)        motorr.run(RELEASE);
  motorl.setSpeed(speed_l);
  motorr.setSpeed(speed_r);
 
  /*
  while (Serial.available()>0)
    myserial.write(Serial.read());
    */
   MSG_TYPE t;
   String msg="",recv;
  t=esp.TCP_LOOP();
  static int c=0;
  char a;
  int i=0;
  unsigned char check;
  switch(t)
  {
    case NOTHING:
    c++;
     break;
    case CONNECTED:
      DB("CONNECT")
      DB((int)esp.getTCP_CILENT());
     break;
    case DISCONNECTED:
      DB("CLOSE")
      DB((int)esp.getTCP_CILENT());
     break;
    case RECIEVE:
      recv=esp.tmp_recv;
      i=recv.indexOf("mm");
      if(i==-1)
      {
        esp.sendMSG(recv[7],"mm?\n");
        break;
      }
    //消息格式：mm(1/2)(数值)(数值)(1/2)(数值)(数值)（校验） 7位
    //1表示后退，2表示前进,迫于js的关系char类型（0-255）必须使用2位来表示
    if(recv.length()<i+2+7){
     DB("too short");
     esp.sendMSG(recv[7],"sl:"+String(speed_l)+"sr:"+String(speed_r)+"s:"+String(stat));
    // esp.sendMSG(recv[7],"short\n");
      break;
     }
     
     for(int j=i+2;j<i+2+7;j++)
     {
      a=recv[j];
      msg+=a;
     }
    
    if(esp.sendMSG(recv[7],msg+"\n"))
      DB("send suc!")
    else
      DB("send fail!")
    
   check=0;
    for(int j=0;j<6;j++)
      check^=(unsigned char)msg[j];
     check|=1;
    if(check!=(unsigned char)(msg[6]))
    {
      DB("check wrong!"+(String)check);
      break;
    }
      stat=0;
     stat=(unsigned char)(msg[0]-1)|(unsigned char)(msg[3]-1)<<1;
     
      speed_l=msg[1]+msg[2];
      speed_r=msg[4]+msg[5];
      DB("sl:"+String(speed_l)+"sr:"+String(speed_r));
      DB(stat);
     // Serial.println("speed:"+String((int)msg[0])+","+String((unsigned char)msg[1]));
     break;
    default :
    //Serial.println(c);
    c=0;
   // Serial.println(esp.tmp_recv);
    
    break;
  }
   // Serial.println(esp.TCP_recv(0));
    
}

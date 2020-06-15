#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"


SMSGSM sms;


String a,b,phone,message;

int numdata;
boolean started=false;
char smsbuffer[160];
char n[20];
char stat;
char temp[1];
char position;


void SendSMS(char *Number, char *Message)
{   
     if ((int)(sms.SendSMS(Number, Message)) == 1)
     {
       Serial.print("SMS- 1: ");
       Serial.print(Number);
       Serial.print(" Msg: ");
       Serial.println(Message);
     }
     else
     {
       Serial.print("SMS- 0: ");
       Serial.print(Number);
       Serial.print(" Msg: ");
       Serial.println(Message);
     }
     started=true;
}

void ListUNREAD_SMS()
{
  position = sms.IsSMSPresent(SMS_UNREAD);
      if(position)
          {
           if(sms.GetSMS(position,n,smsbuffer,160))
           {
              Serial.print("NMS: ");
              Serial.print(n);
              Serial.print(" Msg: ");
              Serial.println(smsbuffer);
           }
           sms.DeleteSMS(position);
      }
      started=true;
}

void ListALL_SMS()
{
position = sms.IsSMSPresent(SMS_ALL);
      if(position)
      {
       if(sms.GetSMS(position,n,smsbuffer,160))
       {
          Serial.print("OMS: ");
          Serial.print(n);
          Serial.print(" Msg: ");
          Serial.println(smsbuffer);
       }
       
      }
      started=true;
}

void DeleteALL_SMS()
{
  position = sms.IsSMSPresent(SMS_ALL);
  if(position)
  {
   if(sms.DeleteSMS(position)) Serial.println("STS: Deleted All Memory");
   else Serial.println("STS: Can't Delete");
  }
  started=true;
}

void CheckSignal()
{
  Serial.println("STS: SIGNAL_QLTY ");
  gsm.SimpleWriteln("AT+CSQ");
  started=true;
}

void CustomCmd(char *Cmd)
{
  Serial.print("STS: SENT_CMD: ");
  Serial.println(Cmd);
  gsm.SimpleWriteln(Cmd);
  started=true;
}

void setup()
{
     Serial.begin(115200);     
     Serial.println("STS: Connecting to GSM...");
     if (gsm.begin(4800)) {
          Serial.println("STS: Arduino READY, Uploaded Firmware: SMS_Official_Firmware_v2.1.2");
          started=true;
     } else Serial.println("STS: Arduino IDLE");
};

void loop()
{
  if(started){
while(Serial.available()) {
  a= Serial.readString();// read the incoming data as string
  b = a.substring(0,1);
  phone = a.substring(1,12);
  message = a.substring(12);
 // Serial.print("STS: ");
  //Serial.println(a);
  char PhoneNum[phone.length() + 1];
  char Messages[message.length() + 1];
  
  phone.toCharArray(PhoneNum,phone.length() + 1);
  message.toCharArray(Messages,message.length() + 1);
  b.toCharArray(temp,b.length() + 1);
  stat = temp[0];
     
          switch(stat)
          {
            case 'a':
            {
              //Serial.println("Sending SMS..");
              SendSMS(PhoneNum,Messages);
            }break;
            case 'b':
            {
            }break;
            case 'c':
            {
              for(int x=0;x<5;x++)
              {
                DeleteALL_SMS();
              }
            }break;
            case 'd':
            {
              ListALL_SMS();
            }break;
            case 'e':
            {
              Serial.println("STS: Checked!");
              started = true;
            }break;
            case 'f':
            {
              CheckSignal();
            }break;
            case 'g':
            {
              CustomCmd(Messages);
            }break;
            case 'h':
            {
              Serial.println("STS: Uploaded Firmware: SMS_Official_Firmware_v2.1.2");
            }break;
            default:
            {
              Serial.println("STS: This function is not Supported");
            }break;
         }
}
Serial.println("STS: OK!");
delay(1000);
ListUNREAD_SMS();
  }
};

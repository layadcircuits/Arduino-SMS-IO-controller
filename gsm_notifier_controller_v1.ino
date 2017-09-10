/***************************************************************************
 Example Code to control 3 LEDs on an Arduino/Saleng Uno via SMS commands.
 The project has 3 buttons that trigger 3 different SMS messages, each sent to a different number.
 The busy LED indicates if the Saleng GSM is busy.
  
 This software is free provided that this notice is not removed and proper attribution 
 is accorded to Layad Circuits and its Author(s).
 Layad Circuits invests resources in producing free software. By purchasing Layad Circuits'
 products or utilizing its services, you support the continuing development of free software
 for all.
  
 Author(s): C.D.Malecdan for Layad Circuits Electronics Engineering
 Revision: 1.0 - 2017/08/17 - initial creation
 Layad Circuits Electronics Engineering Supplies and Services
 B314 Lopez Bldg., Session Rd. cor. Assumption Rd., Baguio City, Philippines
 www.layadcircuits.com
 general: info@layadcircuits.com
 sales: sales@layadcircuits.com
 +63-916-442-8565
 ***************************************************************************/
#include <LayadCircuits_SalengGSM.h>
#include <SoftwareSerial.h>

#define PHONE1 "09164428564"
#define PHONE2 "09164428564"
#define PHONE3 "09164428564"

#define MSG1 "message1: Saleng GSM test message"
#define MSG2 "message2: Saleng GSM test message"
#define MSG3 "message3: Saleng GSM test message"

#define MSG1_ALREADY_OFF "LED 1 is Already Off"
#define MSG2_ALREADY_OFF "LED 2 is Already Off"
#define MSG3_ALREADY_OFF "LED 3 is Already Off"

#define MSG1_ALREADY_ON "LED 1 is Already On"
#define MSG2_ALREADY_ON "LED 2 is Already On"
#define MSG3_ALREADY_ON "LED 3 is Already On"


#define OFF1COMMAND "red off"
#define OFF2COMMAND "green off"
#define OFF3COMMAND "blue off"

#define ON1COMMAND "red on"
#define ON2COMMAND "green on"
#define ON3COMMAND "blue on"

#define PIN_LED1 5 // red
#define PIN_LED2 6 // green
#define PIN_LED3 7 // blue
#define PIN_BUSY 4

#define PIN_BUTTON1 10
#define PIN_BUTTON2 9
#define PIN_BUTTON3 8

SoftwareSerial gsmSerial(2,3);
LayadCircuits_SalengGSM salengGSM = LayadCircuits_SalengGSM(&gsmSerial);

unsigned long busytimer;

void setup()
{
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_LED3, OUTPUT);
  pinMode(PIN_BUSY, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_BUTTON1, INPUT_PULLUP);
  pinMode(PIN_BUTTON2, INPUT_PULLUP);
  pinMode(PIN_BUTTON3, INPUT_PULLUP);
  
  salengGSM.begin(9600); // this is the default baud rate
  Serial.begin(9600);
  Serial.print(F("Preparing Saleng GSM Shield.Pls wait for 10 seconds..."));
  
  // allow 10 seconds for modem to boot up and register
  for(byte i=0;i<100;i++)
  {
    delay(100);
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  }
  
  salengGSM.initSalengGSM();
  Serial.println(F("Done"));
  digitalWrite(LED_BUILTIN,HIGH);
  
  //salengGSM.sendSMS("09164428565","Hi, this is a test SMS from the Layad Circuits' Saleng GSM Shield. Have a nice day!");

}

void loop()
{
  salengGSM.smsMachine(); // we need to pass here as fast as we can. this allows for non-blocking SMS transmission

  if(digitalRead(PIN_BUTTON1) == LOW)
  {
    if(!salengGSM.isFreeToSend())
    {
       digitalWrite(LED_BUILTIN,LOW);delay(500);  
       digitalWrite(LED_BUILTIN,HIGH);delay(500);  
       digitalWrite(LED_BUILTIN,LOW);delay(500);  
       digitalWrite(LED_BUILTIN,HIGH);
    }
    else
    {
      delay(500);
      while(digitalRead(PIN_BUTTON1) == LOW){};//do nothing if hand is still pressed
      salengGSM.sendSMS(PHONE1,MSG1);
    }
  }

  if(digitalRead(PIN_BUTTON2) == LOW)
  {
    if(!salengGSM.isFreeToSend())
    {
       digitalWrite(LED_BUILTIN,LOW);delay(500);  
       digitalWrite(LED_BUILTIN,HIGH);delay(500);  
       digitalWrite(LED_BUILTIN,LOW);delay(500);  
       digitalWrite(LED_BUILTIN,HIGH);
    }
    else
    {
      delay(500);
      while(digitalRead(PIN_BUTTON2) == LOW){};//do nothing if hand is still pressed
      salengGSM.sendSMS(PHONE2,MSG2);
    }
  }

  if(digitalRead(PIN_BUTTON3) == LOW)
  {
    if(!salengGSM.isFreeToSend())
    {
       digitalWrite(LED_BUILTIN,LOW);delay(500);  
       digitalWrite(LED_BUILTIN,HIGH);delay(500);  
       digitalWrite(LED_BUILTIN,LOW);delay(500);  
       digitalWrite(LED_BUILTIN,HIGH);
    }
    else
    {
      delay(500);
      while(digitalRead(PIN_BUTTON3) == LOW){};//do nothing if hand is still pressed
      salengGSM.sendSMS(PHONE3,MSG3);
    }
  }
  
  
  if(salengGSM.isFreeToSend())
  {
    digitalWrite(PIN_BUSY,HIGH);
  }
  else
  {
    if(millis() - busytimer >= 250)
    {
      digitalWrite(PIN_BUSY,!digitalRead(PIN_BUSY));
      busytimer = millis();
    }
  }

  
  if(salengGSM.isSMSavailable()) 
  {
     salengGSM.readSMS(); // updates the read flag
     Serial.print("Sender=");
     Serial.println(salengGSM.smsSender);
     Serial.print("Whole Message=");
     Serial.println(salengGSM.smsRxMsg); // if we receive an SMS, print the contents of the receive buffer

     if(strstr(salengGSM.smsRxMsg,OFF1COMMAND))
     {
         Serial.println("RED ");
        if(digitalRead(PIN_LED1) == LOW)
        {
          salengGSM.sendSMS(salengGSM.smsSender,MSG1_ALREADY_OFF);
        }
        else
        {
          Serial.println("OFF");
          digitalWrite(PIN_LED1,LOW);
        }
     }

     if(strstr(salengGSM.smsRxMsg,OFF2COMMAND))
     {
        if(digitalRead(PIN_LED2) == LOW)
        {
          salengGSM.sendSMS(salengGSM.smsSender,MSG2_ALREADY_OFF);
        }
        else
        {
          digitalWrite(PIN_LED2,LOW);
        }
     }

     if(strstr(salengGSM.smsRxMsg,OFF3COMMAND))
     {
        if(digitalRead(PIN_LED3) == LOW)
        {
          salengGSM.sendSMS(salengGSM.smsSender,MSG3_ALREADY_OFF);
        }
        else
        {
          digitalWrite(PIN_LED3,LOW);
        }
     }


     if(strstr(salengGSM.smsRxMsg,ON1COMMAND))
     {
       Serial.println("RED ");
        if(digitalRead(PIN_LED1) == HIGH)
        {
          salengGSM.sendSMS(salengGSM.smsSender,MSG1_ALREADY_ON);
        }
        else
        {
           Serial.println("ON");
          digitalWrite(PIN_LED1,HIGH);
        }
     }
     
     if(strstr(salengGSM.smsRxMsg,ON2COMMAND))
     {
        if(digitalRead(PIN_LED2) == HIGH)
        {
          salengGSM.sendSMS(salengGSM.smsSender,MSG2_ALREADY_ON);
        }
        else
        {
          digitalWrite(PIN_LED2,HIGH);
        }
     }   
     
     if(strstr(salengGSM.smsRxMsg,ON3COMMAND))
     {
        if(digitalRead(PIN_LED3) == HIGH)
        {
          salengGSM.sendSMS(salengGSM.smsSender,MSG3_ALREADY_ON);
        }
        else
        {
          digitalWrite(PIN_LED3,HIGH);
        }
     } 
  }
}

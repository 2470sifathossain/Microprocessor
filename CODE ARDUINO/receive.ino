                                                         //Mert Arduino and Tech YouTube Channel -- https://goo.gl/ivcZhW

//Add the necessary libraries
//You can find all the necessary library links in the video description
//#include <SPI.h>      //SPI library for communicate with the nRF24L01+
//#include "RF24.h"     //The main library of the nRF24L01+
#include <DataCoder.h>
#include <VirtualWire.h>


const int rx_pin = 11;
const int baudRate = 1000;

//Define enable pins of the Motors
const int enbA = 3;
const int enbB = 5;

//Define control pins of the Motors
//If the motors rotate in the opposite direction, you can change the positions of the following pin numbers
const int IN1 = 2;    //Right Motor (-)
const int IN2 = 4;    //Right Motor (+)
const int IN3 = 7;    //Left Motor (+)
const int IN4 = 6;    //Right Motor (-)

//Define variable for the motors speeds
//I have defined a variable for each of the two motors 
//This way you can synchronize the rotation speed difference between the two motors
int RightSpd = 130;
int LeftSpd = 130;

//Define packet for the direction (X axis and Y axis)
//int data[2];

//Define object from RF24 library - 9 and 10 are a digital pin numbers to which signals CE and CSN are connected
//RF24 radio(9,10);

//Create a pipe addresses for the communicate
//const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(){

  //SetupRFDataTxnLink(transmit_pin, baudRate);
  
  //Define the motor pins as OUTPUT
  pinMode(enbA, OUTPUT);
  pinMode(enbB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(9600);

  SetupRFDataRxnLink(rx_pin, baudRate);
  //radio.begin();                    //Start the nRF24 communicate            
  //radio.openReadingPipe(1, pipe);   //Sets the address of the transmitter to which the program will receive data.
  //radio.startListening();             
  }

void loop(){

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  union RFData inDataSeq;
  
  float inData[RF_DATA_ARRAY_SIZE];
  
  if(vw_get_message(buf,&buflen))
  {
        //digitalWrite(led_pin, HIGH);
        
        for(int i =0; i< buflen; i++)
        {
          inDataSeq.s[i] = buf[i];
        }
        
        //Serial.println();
        
        //digitalWrite(led_pin, LOW);
      
        DecodeRFData(inData, inDataSeq);
        
        //Do something with the elements that have been received
       
        //Serial.print("Incoming Data:");
        
        for (int i=0; i < RF_DATA_ARRAY_SIZE; i++)
          {
            Serial.print(inData[i]);
            Serial.println(", ");
          }
        //Serial.println();
        //Serial.print("Bytes Used:");
        //Serial.println(RF_DATA_ARRAY_SIZE * 2);
        //Serial.println(inArray[0]);


        if(inData[0] > 380){
          //forward            
          analogWrite(enbA, RightSpd);
          analogWrite(enbB, LeftSpd);
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
        }
    
        if(inData[0] < 310){
          //backward              
          analogWrite(enbA, RightSpd);
          analogWrite(enbB, LeftSpd);
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
        }
     
        if(inData[1] > 180){
          //left
          analogWrite(enbA, RightSpd);
          analogWrite(enbB, LeftSpd);
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
        }

        if(inData[1] < 110){
          //right
          analogWrite(enbA, RightSpd);
          analogWrite(enbB, LeftSpd);
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
        }

        if(inData[0] > 330 && inData[0] < 360 && inData[1] > 130 && inData[1] < 160){
          //stop car
          analogWrite(enbA, 0);
          analogWrite(enbB, 0);
        }
  }






  /*
  if (radio.available()){
    radio.read(data, sizeof(data));

    if(data[0] > 380){
      //forward            
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    
    if(data[0] < 310){
      //backward              
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
     
    if(data[1] > 180){
      //left
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }

    if(data[1] < 110){
      //right
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }

    if(data[0] > 330 && data[0] < 360 && data[1] > 130 && data[1] < 160){
      //stop car
      analogWrite(enbA, 0);
      analogWrite(enbB, 0);
    }
  }*/

  delay(1000);
}

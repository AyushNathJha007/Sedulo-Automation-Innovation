//I2C communication(slave only)--All done
//best till now


#include <Wire.h>
#include<string.h>
#define Frame_Size 24
#define interruptPin 2
const int address = 1;
int VP=3;
String MA;
String SA=(String)address;
String NVP=(String)VP;
String d1;
String d2;
String d3;
String d4;
String d5;
String d6;
String d7;
String d8;
String d9;
String d10;
int msgno;
int msg_trans_length=120;
String slave_rxv;
String dataSend(String SA1, String NVP1, String set[],int VP1) //Encoder, with 6 parameters
{int ij;
  String start="#";
  String seperate="%";
  String endloop="@";
  String garbage="&";
  String encoded=start+SA1+seperate+NVP1;
  for(ij=0;ij<(2*VP1);ij++)
  {
    encoded=encoded+seperate+set[ij];
  }
  
  if((VP1*2)<=10)
  while(ij<10)
  {
  encoded=encoded+seperate+garbage;
  ij++;
  }
  encoded=encoded+endloop;
  while(encoded.length()<msg_trans_length)
  encoded=encoded+endloop;
  return encoded; //encoded will now have fixed 120 bytes
}
int i=0;
String DSet[5];//Standard Data Set--5*24=120
String set1[6]={"11.21","23.43","67.42","45.97","42.12","19.11"};
String encoded1;
 
void setup() {
Serial.begin(9600);
encoded1=dataSend(SA,NVP,set1,VP);
Serial.println(encoded1);
 
for(int zz=0,k=0;zz<=96&&k<5;zz=zz+24,k++)//Every data frame is of 24 bytes and last data frame starts from 96
 DSet[k]=encoded1.substring(zz,zz+24);

 
 Wire.begin(address);
Wire.onRequest(requestEvent);
Wire.onReceive(receiveEvent);


 for(int m=0;m<5;m++)
 {
  Serial.println(DSet[m]);
 }

  }

  void receiveEvent(int howMany) {
    Serial.println("Request Received");
String invit_rxv = "";
  
  while (Wire.available()>0) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    invit_rxv = invit_rxv + String(c);
  }
   Serial.println(invit_rxv);
if(invit_rxv == "start")
{i = 0;
Serial.println("i=0 now ");
}
  
}
void loop() {
  // put your main code here, to run repeatedly:
  
 
}
void requestEvent() {
  Wire.flush();
  if(i ==5)
  {i = 0;
 }
String local = DSet[i];
  int ii =0;
  while(local[ii]!='\0')
  {
  
    Serial.print("letter transmitted     :");//Debug
    Wire.write(local[ii]);
    delay(100);
    Serial.print(local[ii]);
    Serial.print("\n ");
    ii++;}
i++;

  
  }


/*

void send_warning()
{
String DSet[5];//Standard Data Set--5*24=120
String set1[6]={"11.21","23.43","67.42","45.97","42.12","19.11"};
String encoded1;
encoded1=dataSend(SA,NVP,set1,VP);

Serial.println(encoded1);
 
for(int zz=0,k=0;zz<=96&&k<5;zz=zz+24,k++)//Every data frame is of 24 bytes and last data frame starts from 96
 DSet[k]=encoded1.substring(zz,zz+24);
  
    Wire.flush();
  if(i ==5)
  {i = 0;
 }

 Dset[]
String local = DSet[i];
  int ii =0;
  while(local[ii]!='\0')
  {
  
    Serial.print("letter transmitted     :");//Debug
    Wire.beginTransmission(2)
    Wire.write(local[ii]);
    delay(100);
    Serial.print(local[ii]);
    Serial.print("\n ");
    ii++;}
i++;

  
  
  }
*/

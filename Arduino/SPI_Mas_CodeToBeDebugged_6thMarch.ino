/*Code for SPI Master*/

#include <SPI.h>
#define SS2 4 //YEIH WAALA USE KIYE HAIH 
#define SS1 8

#include <Wire.h>
#include<string.h>

#define i_Frame_Size 24
#define i_interruptPin 2


#define screen_intr 26
#define screen_intr_2 28


const int i_address = 1;
int i_VP=7;               // 00000000000000000000000000
String i_MA;
String i_SA=(String)i_address;
String i_NVP=(String)i_VP;
String i_d1;
String i_d2;
String i_d3;
String i_d4;
String i_d5;
String i_d6;
String i_d7;
String i_d8;
String i_d9;
String i_d10;
String i_d11;
String i_d12;
String i_d13;
String i_d14;
int i_msgno;
int i_msg_trans_length=120;


String i_slave_rxv;






  
String SA;
String NVP;
String a1;
String a2;
String b1;
String b2;
String c1;
String c2;
String d1;
String d2;
String e1;
String e2;
String f1;
String f2;
String g1;
String g2;
char e;
String data_frame_received;

#define totalspi 1
#define tempunit 4
#define smokeunit 5
int SPI_addr[totalspi] = {tempunit};  //,smokeunit};
// contain the pin on which ss pin of other arduios are connected
int addr = 0;

//_______________________________________________________________________
int i_i=0;
String i_DSet[5];//Standard Data Set--5*24=120
String i_set1[14]={a1,a2,b1,b2,c1,c2,d1,d2,e1,e2,f1,f2,g1,g2};
String i_encoded1;
//_________________________________________________________________________

//_______________________________________________________________________________________
String i_dataSend(String i_SA1, String i_NVP1, String i_set[],int i_VP1) //Encoder, with 6 parameters
{int i_ij;
  String i_start="#";
  String i_seperate="%";
  String i_endloop="@";
  String i_garbage="&";
  String i_encoded = i_start +  i_SA1 + i_seperate + i_NVP1;
  for(i_ij=0;i_ij<(2*i_VP1);i_ij++)
  {
    i_encoded=i_encoded+i_seperate+i_set[i_ij];
    Serial.println(i_ij);
    Serial.println(i_set[i_ij]);
  }
  
  if((i_VP1*2)<=14)
  while(i_ij<14)
  {
  i_encoded=i_encoded+i_seperate+i_garbage;
  i_ij++;
  }
  i_encoded=i_encoded+i_endloop;
  while(i_encoded.length()<i_msg_trans_length)
  i_encoded=i_encoded+i_endloop;
  return i_encoded; //encoded will now have fixed 120 bytes
}



void receiveEvent(int i_howMany) {

    Serial.println("Request Received");
String i_invit_rxv = "";
  
  while (Wire.available()>0) { // loop through all but the last
    char i_c = Wire.read(); // receive byte as a character
    i_invit_rxv = i_invit_rxv + String(i_c);
  }
 
 digitalWrite(screen_intr , HIGH);  // gets LOW IN LOOP@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   
   Serial.println(i_invit_rxv);
if(i_invit_rxv == "start")
{i_i = 0;
Serial.println("i=0 now ");

}
  

//delay(100);

}





void requestEvent() {
  Wire.flush();
  if(i_i ==5)
  {i_i = 0;
 }
String i_local = i_DSet[i_i];
  int i_ii =0;
  while(i_local[i_ii]!='\0')
  {
  
    Serial.print("letter transmitted     :");//Debug
    Wire.write(i_local[i_ii]);
    delay(100);
    Serial.print(i_local[i_ii]);
    Serial.print("\n ");
    i_ii++;}
i_i++;

  
  }





//_______________________________________________________________________________________















void spi_init()
{ int i = 0;
  for(i = 0 ;i<totalspi ; i++ )
    {
      pinMode(SPI_addr[i],OUTPUT);
      digitalWrite(SPI_addr[i], HIGH);
      }
  }



 
//______________________________________________________________________________

void setup (void) {
spi_init();
  //pinMode(SS2,OUTPUT);
   Serial.begin(9600); //set baud rate to 115200 for usart
   /*digitalWrite(SS, HIGH); // disable Slave Select
   digitalWrite(SS2, HIGH); // disable Slave Select
   digitalWrite(SS1, HIGH); // disable Slave Select
   */
   SPI.begin ();
   SPI.setClockDivider(SPI_CLOCK_DIV16);//divide the clock by 8
 
pinMode(screen_intr,OUTPUT);
Serial.println("screen_intr pin set to output");
 
pinMode(screen_intr_2,INPUT_PULLUP);
Serial.println("screen_intr pin set to input pullup");

digitalWrite(screen_intr , HIGH);
Serial.println("screen_intr pin set to input pullup");
 
 Wire.begin(i_address);
Wire.onRequest(requestEvent);
Wire.onReceive(receiveEvent);


 for(int i_m=0;i_m<5;i_m++)
 {
  Serial.println(i_DSet[i_m]);
 }

}
 //______________________________________________________________________________



//***********************************************************************************************************************

void loop (void)
{


while( digitalRead(screen_intr_2) == LOW )
{Serial.println("wait");

delay(10);
}  
frame_received();


// hard coded 

i_set1[0]=a1;
i_set1[1]=a2;
i_set1[2]=b1;
i_set1[3]=b2;
i_set1[4]=c1;
i_set1[5]=c2;
i_set1[6]=d1;
i_set1[7]=d2;
i_set1[8]=e1;
i_set1[9]=e2;
i_set1[10]=f1;
i_set1[11]=f2;
i_set1[12]=g1;
i_set1[13]=g2;


i_encoded1=i_dataSend(SA,NVP,i_set1,i_VP);
Serial.println(i_encoded1);
 
for(int i_zz=0,i_k=0;    i_zz<=96&&i_k<5   ;    i_zz = i_zz+24  ,   i_k++)//Every data frame is of 24 bytes and last data frame starts from 96
 i_DSet[i_k]=i_encoded1.substring(i_zz,i_zz+24);


digitalWrite(screen_intr , LOW);  // gets high when recieve  start from the screen//
delay(10000);

}


//***********************************************************************************************************************





  void getData(String &SA,String &NVP,String &a,String &b, String &c, String &d, String &e, String &f,String &g,String &h, String &i, String &z,String &z1,String &z2,String &z3,String &z4,String s)
{
  //the data frame, as received
  String set[16];//string array,in which the 6 values will be stored at 6 indices
  int j,k=0;
  for(int i=0;i<16;i++)
  {
  for(j=1;;j++)
     {
     k++;
     if(s.charAt(j)=='%'||s.charAt(j)=='@')
     break;
     }
  set[i]=s.substring(1,k);//extraction of required substring
  s.remove(0,j);
  k=0;
  
  
  }
    //Value of 6 parameters, stored in pre defined addresses---extension can be possibly made, but only after expanding the address variable list
  
  SA=set[0];
  NVP=set[1];
    a=set[2];
    b=set[3];
    c=set[4];
    d=set[5];
    e=set[6];
    f=set[7];
    g=set[8];
  h=set[9];
  i=set[10];
  z=set[11];
  z1=set[12];
  z2=set[13];
  z3=set[14];
  z4=set[15];
 }












void frame_received()
{
  
  //SPDR = 0;
   delay(100);
    Serial.println("start frame ____________________________________________");
trans_spi(SPI_addr[addr]);
addr++;
if(addr>=totalspi)
   {addr=0;}

Serial.println(data_frame_received);
delay(2000);
getData(SA,NVP,a1,a2,b1,b2,c1,c2,d1,d2,e1,e2,f1,f2,g1,g2,data_frame_received);
Serial.println("getdata executed");
delay(2000);
Serial.println(SA);
Serial.println(NVP);
Serial.println(a1);
Serial.println(a2);
Serial.println(b1);
Serial.println(b2);
Serial.println(c1);
Serial.println(c2);
Serial.println(d1);
Serial.println(d2);
Serial.println(e1);
Serial.println(e2);
Serial.println(f1);
Serial.println(f2);
Serial.println(g1);
Serial.println(g2);

   delay(1000);
   Serial.println("completed frame ____________________________________________");
data_frame_received="";//Emptied frame for welcoming new data
/*
digitalWrite(SS1, LOW);// enable Slave Select
   // send test string
   for (const char * p = "we will we will rock you !olala" ; c = *p; p++) {
      SPI.transfer (c);
      Serial.print("what is transfered:");
      Serial.println(c);
      delay(100); // delay value need to control as per length of communication
      d = SPDR;
      Serial.print("what is recieverd:");
      Serial.println(d);
   }
   digitalWrite(SS1, HIGH); // disable Slave Select
   delay(2000);
//++++++++++++++++++++++++++++++++++++++++++++++++++
   digitalWrite(SS2, LOW);// enable Slave Select
   // send test string
   for (const char * p = "jai ho jai ho, world!olala" ; c = *p; p++) {
      SPI.transfer (c);
      Serial.print("what is transfered:");
      Serial.println(c);
      delay(100); // delay value need to control as per length of communication
      d = SPDR;
      Serial.print("what is recieverd:");
      Serial.println(d);
   }
   digitalWrite(SS2, HIGH); // disable Slave Select
   delay(2000);
   */
  
  
  }

   void trans_spi(int SS_Pin)
   {
   char dd;
  
   
   
   digitalWrite(SS_Pin, LOW);// enable Slave Select
   delay(1000);
   // send test string
  // l1:
   SPI.transfer('~');delay(200);
   delay(200);
   //e=SPDR;
   //if(e=='~')
   //{
digitalWrite(SS_Pin, HIGH);
delay(50);
 
    const char c = '!'; 
    Serial.println("Loop about to start");
    //for (const char * p = "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" ; c = *p; p++) {
      l1:for(int ii  = 0 ; ii<120 ; ii++ ){
        
        delay(200);
      digitalWrite(SS_Pin, LOW);
      delay(250);// 
      SPI.transfer (c);
      //Serial.print("what is recieverd:");
      //Serial.println(c);
      
      
      delay(100);
      //Serial.println("not UPDATE ERROR");
      //ii--;
      // delay value need to control as per length of communication
      if(SPDR!='!') {dd = SPDR;
      }
      
      if(((char)dd=='#'&&ii==0)||(ii>0))  // how to overcome reset problem
      ;
      else
      {Serial.println((char)dd);
        digitalWrite(SS_Pin,HIGH);
      Serial.println("Encountered!!! Going to l1");
      delay(50);
      goto l1;}
      delay(200);
      Serial.print("what is recieverd:");
      Serial.println(dd);
      if((char)dd!='\0')
      data_frame_received=data_frame_received+(char)dd;
      dd='\0';
      digitalWrite(SS_Pin, HIGH);
      delay(50);// 
   }
   //}
  // else
   //goto l1;
   digitalWrite(SS_Pin, HIGH);
   
   // disable Slave Select
   delay(2000);
   }

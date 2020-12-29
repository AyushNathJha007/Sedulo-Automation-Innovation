/*Code is reponsible for-
1. Receiving the steering command from processing unit.
2. Mapping the received raw values to meaningful values
3. Taking input from encoder attached to the actuator to estimate current angle.
4. Switching modes between autonomous and remote-controlled.
5. Accepting commands from RF controller.
6. Controlling acceleration and braking.*/

//Combined ML+Remote


// 0-359 the motor will move ckw
// 360-719 the motor will move ackw
#include <Servo.h>
#include <NewPing.h>
#include <Wire.h>

#define fromLow 900
#define fromHigh 1900
#define toLow 0
#define toHigh 180
#define toLow_2 0
#define toHigh_2 5
#define toLow_Ch6 1000
#define toHigh_Ch6 2000
#define esc_pin 6
#define servo_pin 5

byte last_channel_1, last_channel_2, last_channel_3, last_channel_4, last_channel_5, last_channel_6;
int receiver_input_channel_1, receiver_input_channel_2, receiver_input_channel_3, receiver_input_channel_4, receiver_input_channel_5, receiver_input_channel_6;
int channel_1, channel_2, channel_3, channel_4, channel_5, channel_6;
int esc, servo;
unsigned long timer_channel_1, timer_channel_2, timer_channel_3, timer_channel_4, timer_channel_5, timer_channel_6;
unsigned long timer_1, timer_2, timer_3, timer_4, timer_5, timer_6, current_time;

volatile unsigned int temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder
int user_input_degree=0;
int user_input_degree_2=0;
int initial_angle=90;
int global_initial=90;          //// this is the initial position
int final_angle=0;
int diff=0;

int state_1 = 0;
int state_2 = 0;

int   flag = 0 ;


int calculated_output_rot=0;
String read_String; 
void setup() {
  Serial.begin (9600);
  Serial.begin(9600);
  PCICR |= (1 << PCIE2);
 
  PCMSK2 |= (1 << PCINT16); //Set PCINT16 (Analog input A8) to trigger an interrupt on state change.
  PCMSK2 |= (1 << PCINT17); //Set PCINT17 (Analog input A9) to trigger an interrupt on state change.
  PCMSK2 |= (1 << PCINT18); //Set PCINT18 (Analog input A10) to trigger an interrupt on state change.
  PCMSK2 |= (1 << PCINT19); //Set PCINT19 (Analog input A11) to trigger an interrupt on state change.
  PCMSK2 |= (1 << PCINT20); //Set PCINT20 (Analog input A12) to trigger an interrupt on state change.
  PCMSK2 |= (1 << PCINT21); //Set PCINT21 (Analog input A13) to trigger an interrupt on state change.

  pinMode(4,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(2, INPUT_PULLUP); // internal pullup input pin 2 
  
  pinMode(3, INPUT_PULLUP); // internal pullup input pin 3
//Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(0, ai0, RISING);
   
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(1, ai1, RISING);

  Serial.println("Interrupt attached.");
  
  }

void loop() {
  // put your main code here, to run repeatedly:
channel_6 = map(receiver_input_channel_6, fromLow, fromHigh, toLow_Ch6, toHigh_Ch6);
channel_5 = map(receiver_input_channel_6, fromLow, fromHigh, toLow_Ch6, toHigh_Ch6);

//For Remote Part
if(channel_6<1500&&channel_5<1500)
{

  
  channel_1 = map(receiver_input_channel_1, 1070, 1790, toLow, toHigh);
  channel_2 = map(receiver_input_channel_2, 1405, 1800, 0, 255);
  channel_3 = map(receiver_input_channel_3, fromLow, fromHigh, toLow, toHigh);
  //channel_4 = map(receiver_input_channel_4, fromLow, fromHigh, toLow, toHigh);

Serial.print("Channel 1 = ");
  Serial.print(channel_1);
  Serial.print('\t');
  /*Serial.print("Channel 2 = ");
  Serial.print(channel_2);
  Serial.print('\t');
  Serial.print("Channel 3 = ");*/
  Serial.print(channel_3);
  Serial.print('\t');/*
  Serial.print("Channel 4 = ");
  Serial.print(channel_4);
  Serial.print('\t');
  Serial.print("Channel 5 = ");
  Serial.print(channel_5);
  Serial.print('\t');
  Serial.print("Channel 6 = ");
  Serial.print(channel_6);
  Serial.print('\n');*/
if(channel_2>2)
analogWrite(10,channel_2);
else
analogWrite(10,0);

  // 87 to 92 oscillate karta haih mid position par toh jav value rahega toh kya hoga toh steering nahi hoga jis positionn seih gaari shuru hua tha usi par chala jayega 
  if(channel_1>84&&channel_1<90)
  {
    Serial.println("channel_1 Not Active");
    diff=global_initial-initial_angle;  //Bringing the motor back to its original position(Wheels straight)
    if(global_initial>initial_angle)  
    {
      state_1=0;
      state_2=1;
      flag=1;
      initial_angle=global_initial;
    }
    else if(global_initial<initial_angle)
    {
      state_1=1;
      state_2=0;
      flag=0;
      initial_angle=global_initial;
    }
    else
    {
      flag=-1;
      initial_angle=global_initial;
    }
    //flag=-1;
  }
  else if(channel_1>0&&channel_1<=84) //Code for negative half
 {
  if(channel_1>initial_angle)
  {
    state_1=0;
    state_2=1;
    flag=1;
  }
  else{
  state_1 = 1;
  state_2 = 0;
  }
  final_angle=channel_1;
  diff=final_angle-initial_angle;
  initial_angle=channel_1;
 }
else if(channel_1>=90&&channel_1<180)  //Code for positive half
 {
  if(channel_1<initial_angle)
  {
    state_1=1;
    state_2=0;
  }
  else
  {
  state_1 = 0;
  state_2 = 1;
  flag=1;
  }
  final_angle=channel_1;
  diff=final_angle-initial_angle;
  initial_angle=channel_1;

    delay(10);
  }

diff=abs(diff);
 
calculated_output_rot=map(diff,0,360,0,800);
Serial.println("Calculated output rot:");
Serial.println(calculated_output_rot);
//initial_angle=final_angle;
  //Serial.println("second if ends");
counter=0;
delay(10);
if(flag!=-1)
{
  while(  ((counter)<calculated_output_rot) || ( (counter)>(65535-calculated_output_rot) ))
  {
    //Serial.print("After_Mod:");
    //Serial.println(counter%799);
    //Serial.println(counter);
    //Serial.println("Hello");

    //Serial.println(calculated_output_rot);
    delay(1);
    
    if(flag == 0 )
    { digitalWrite(4,HIGH);  // normally yeih high tha
      digitalWrite(7,LOW);
    }
    else if(flag==1)
    {
      digitalWrite(4,LOW);  // normally yeih high tha
      digitalWrite(7,HIGH);
      }

      //------------------------------------------------------


  //----------------------------------------------------------------
    
    delay(1);
  }
}

  digitalWrite(4,LOW);
    digitalWrite(7,LOW);
    //user_input_degree=0;
    user_input_degree=0;
    counter=0;
    flag = 0 ; 
    calculated_output_rot=0;
    read_String="";

/*if(channel_3<1400)
{
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  delay(2000);
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  delay(2000);
}
else
{
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
}*/
    
    Serial.flush();
    //delay(1000);

  
  
}


//////////////////////////////////////////////////////////////////////////////////

else if(channel_6>=1500&&channel_5>=1500)
{
  while(Serial.available()) { //Check if the serial data is available.
//    String read_String = Serial.readString();  // storing input data
    char c = Serial.read();
    delay(3);                  // a small delay
    read_String += c;         // accumulate each of the characters in readString
      }
//Serial.println(read_String);// returnn to python
    //Serial.println(read_String);
    delay(10);
  //Serial.println("outside if variable");
//Serial.println(read_String.length() );


  
  if (read_String.length() >0) { //Verify that the variable contains information
  //Serial.println("inside second if");  
   //Serial.println(read_String.toInt());  //printing the input data in integer form
user_input_degree_2 = read_String.toInt();   // here input data is store in integer form*/
user_input_degree=map(user_input_degree_2,0,320,0,180);          //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
if(user_input_degree<90)
 {
  if(user_input_degree>initial_angle)
  {
    state_1=0;
    state_2=1;
    flag=1;
  }
  else{
  state_1 = 1;
  state_2 = 0;
  }
  final_angle=user_input_degree;
  diff=final_angle-initial_angle;
  initial_angle=user_input_degree;
 }
else if(user_input_degree>=90&&user_input_degree<180)
 {
  if(user_input_degree<initial_angle)
  {
    state_1=1;
    state_2=0;
  }
  else
  {
  state_1 = 0;
  state_2 = 1;
  flag=1;
  }
  final_angle=user_input_degree;
  diff=final_angle-initial_angle;
  initial_angle=user_input_degree;
  
  //Serial.println("negative value recieved ");
  //Serial.println(user_input_degree);
    //user_input_degree = user_input_degree - 60;
//Serial.println("updated value");
 //Serial.println(user_input_degree);
    

    delay(10);
  }
//final_angle=user_input_degree;
//diff=final_angle-initial_angle;

diff=abs(diff);
 
calculated_output_rot=map(diff,0,360,0,800);
Serial.println("Calculated output rot:");
Serial.println(calculated_output_rot);
//initial_angle=final_angle;
  //Serial.println("second if ends");
  
  }



    
//while (user_input_degree<=360)

//(counter)>(65535-calculated_output_rot))
  counter=0;
  //Serial.println(calculated_output_rot);
delay(10);
  while(  ((counter)<calculated_output_rot) || ( (counter)>(65535-calculated_output_rot) ))
  {
    //Serial.print("After_Mod:");
    //Serial.println(counter%799);
    Serial.println(counter);
    //Serial.println("Hello");

    //Serial.println(calculated_output_rot);
    delay(1);
    
    if(flag == 0 )
    { digitalWrite(4,HIGH);  // normally yeih high tha
      digitalWrite(7,LOW);
    }
    else
    {
      digitalWrite(4,LOW);  // normally yeih high tha
      digitalWrite(7,HIGH);
      }

      //------------------------------------------------------


  //----------------------------------------------------------------
    
    delay(1);
  }

  digitalWrite(4,LOW);
    digitalWrite(7,LOW);
    //user_input_degree=0;
    user_input_degree=0;
    counter=0;
    flag = 0 ; 
    calculated_output_rot=0;
    read_String="";
    Serial.flush();
    //delay(1000);

}



















  

}

void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(3)==LOW) {
    //Serial.println("Entered Pin 3 LOW");
  //if(counter<134)
  counter++;
  }else{
    //Serial.println("Entered Pin 3 HIGH");
  //if(counter>65401)
  counter--;
  }
  }
   
  void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(2)==LOW) {
    //Serial.println("Entered Pin 2 LOW");
    //if(counter>65401)
  counter--;
  }else{
    //Serial.println("Entered Pin 2 HIGH");
  //if(counter<134)
  counter++;
  }
  }

  ISR(PCINT2_vect)
{
  current_time = micros();
  //Channel 1=========================================
  if(PINK & B00000001)
  {                                        //Is input 8 high?
  if(last_channel_1 == 0)
  {                                   //Input 8 changed from 0 to 1
    last_channel_1 = 1;                                      //Remember current input state
    timer_1 = current_time;                                  //Set timer_1 to current_time
  }
  }
  else if(last_channel_1 == 1)
  {                                //Input 8 is not high and changed from 1 to 0
    last_channel_1 = 0;                                        //Remember current input state
    receiver_input_channel_1 = current_time - timer_1;         //Channel 1 is current_time - timer_1
  }
  //Channel 2=========================================
  if(PINK & B00000010 )
  {                                       //Is input 9 high?
    if(last_channel_2 == 0)
    {                                   //Input 9 changed from 0 to 1
      last_channel_2 = 1;                                      //Remember current input state
      timer_2 = current_time;                                  //Set timer_2 to current_time
    }
  }
  else if(last_channel_2 == 1)
  {                                //Input 9 is not high and changed from 1 to 0
    last_channel_2 = 0;                                        //Remember current input state
    receiver_input_channel_2 = current_time - timer_2;         //Channel 2 is current_time - timer_2
  }
  //Channel 3=========================================
  if(PINK & B00000100 )
  {                                       //Is input 10 high?
    if(last_channel_3 == 0)
    {                                   //Input 10 changed from 0 to 1
      last_channel_3 = 1;                                      //Remember current input state
      timer_3 = current_time;                                  //Set timer_3 to current_time
    }
  }
  else if(last_channel_3 == 1)
  {                                //Input 10 is not high and changed from 1 to 0
    last_channel_3 = 0;                                        //Remember current input state
    receiver_input_channel_3 = current_time - timer_3;         //Channel 3 is current_time - timer_3

  }
  //Channel 4=========================================
  if(PINK & B00001000 )
  {                                       //Is input 11 high?
    if(last_channel_4 == 0)
    {                                   //Input 11 changed from 0 to 1
      last_channel_4 = 1;                                      //Remember current input state
      timer_4 = current_time;                                  //Set timer_4 to current_time
    }
  }
  else if(last_channel_4 == 1)
  {                                //Input 11 is not high and changed from 1 to 0
    last_channel_4 = 0;                                        //Remember current input state
    receiver_input_channel_4 = current_time - timer_4;         //Channel 4 is current_time - timer_4
  }
  //Channel 5=========================================
  if(PINK & B00010000)
  {
    if(last_channel_5 == 0)
    {
      last_channel_5 = 1;
      timer_5 = current_time;       
    }   
  }
  else if(last_channel_5 == 1)
  {
      last_channel_5 = 0;
      receiver_input_channel_5 = current_time - timer_5;
  }
  //Channel 6=========================================
  if(PINK & B00100000)
  {
    if(last_channel_6 == 0)
    {
      last_channel_6 = 1;
      timer_6 = current_time;
    }   
  }
  else if(last_channel_6 == 1)
  {
    last_channel_6 = 0;
    receiver_input_channel_6 = current_time - timer_6;   
  }
}

#include <Wire.h>
#include <Stepper.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);//LCD位址0x27
Stepper myStepper(200,8,11,9,12);//轉一圈的步數=200 ,A=8 ,A'=11 ,B=9 ,B'=12  

int forward = 5;//前進鈕
int back = 6;//後退鈕
int zero = 4;//歸零鈕
int FL = 3;//前極限
int BL = 2;//後極限
int motorA= 10;//A線圈致能
int motorB= 13;//B線圈致能

float data = 0;//原始值
float dataval = 0;//目前距離值
long s;//速度值
int p;//步數值
char val=0;
int x=0;

void setup()
{
 lcd.init();//LCD初始化
 lcd.backlight();//LCD背光亮
  
 myStepper.setSpeed(100);//設定初始速度
  
 pinMode(forward, INPUT); 
 pinMode(back, INPUT);  
 pinMode(zero, INPUT);
 pinMode(FL, INPUT);
 pinMode(BL, INPUT);
 pinMode(motorA, OUTPUT);
 pinMode(motorB, OUTPUT);
 
 Serial.begin(9600);
 lcd.setCursor(0,0);
 lcd.print("Distance");
 lcd.setCursor(9,1);
 lcd.print(0.0);
 lcd.setCursor(14,1);
 lcd.print("mm"); 
}

/*馬達動作*/
void Move(long s , int p)
{
 if(p>0)//馬達正轉
 {
   for(x=0 ; x<p ; x++)
  {
   if(digitalRead(FL) == LOW)
   {
    x=0;
    p=0;
    digitalWrite(motorA,LOW);
    digitalWrite(motorB,LOW);    
    if(digitalRead(back) == HIGH)
    {
     digitalWrite(motorA,HIGH);
     digitalWrite(motorB,HIGH);
    }    
   } 
   else
   {
    digitalWrite(motorA,HIGH);
    digitalWrite(motorB,HIGH);
    myStepper.setSpeed(s);
    myStepper.step(1); 
   }
  }
 }
 
 if(p<0)//馬達反轉
 {
  for(x=0 ; x>p ; x--)
  {
   if(digitalRead(BL) == LOW)
   {
    x=0;
    p=0;
    digitalWrite(motorA,LOW);
    digitalWrite(motorB,LOW);    
    if(digitalRead(forward) == HIGH)
    {
     digitalWrite(motorA,HIGH);
     digitalWrite(motorB,HIGH);
    }
   }
   else
   {
    digitalWrite(motorA,HIGH);
    digitalWrite(motorB,HIGH);
    myStepper.setSpeed(s);
    myStepper.step(-1);
   }
  }
 }
}

/*數值讀取*/ 
String read_Data() 
{ 
 delay(10);
 String comdata = "";
 while (Serial.available() > 0)  
  {
   char c;
   c=Serial.read();
   int v=c;
   if(v==44)break;//,
   comdata += c;
   delay(3);
  }
 if (comdata.length() > 0)//ddd
 {
  return comdata; 
 }  
}

void loop()
{
/*前進動作*/
if(digitalRead(forward) == HIGH && digitalRead(back) == LOW)//前進鈕按下後執行
 {
   if(digitalRead(FL) == HIGH)
   {
    Serial.println("forward");
    s = 150;
    p = 20;
   }
   Move(s , p);//正轉200步,速度20rpm
   p=0;
 } 
/*後退動作*/
else if(digitalRead(forward) == LOW && digitalRead(back) == HIGH)//後退鈕按下後執行
 {
   if(digitalRead(BL) == HIGH)
   {
    Serial.println("back");
    s = 150;
    p = -20;
   }
   Move(s , p);//反轉200步,速度20rpm 
   p=0;
 } 
/*極限狀態*/
if(digitalRead(FL) == LOW)
   {
    delay(100);
    Serial.println("FL"); 
   }
if(digitalRead(BL) == LOW)
   {
    delay(100);
    Serial.println("BL"); 
   }
/*歸零動作*/
if(digitalRead(zero) == HIGH)//歸零鈕按下後歸零
 {
  delay(100);
  Serial.println("zero");
  data=0;//資料清除歸零
  dataval=0;
  lcd.setCursor(9,1);
  lcd.print(dataval); 
  Serial.println(dataval);
 } 
 
/*電腦數值輸入*/
String comdata = "";//清空
val=Serial.read();
if(val>0)
 {    
  if(val == 83)//S 速度
  {
   s=read_Data().toInt();
   Serial.print("S");
   Serial.println(s);
  }
 
  if(val==80)//P 步數
  {
   p=read_Data().toInt(); 
   Serial.print("P"); 
   Serial.println(p);  
  } 
 
  if(val == 77)//M 移動
  {
   Move(s,p);
   Serial.println("Move");
  } 
 
  if(val == 72)//H 回原點
  {
   if(digitalRead(FL) == HIGH)
   {
    p=20000;
    Move(150, p);       
   }  
   delay(100);
   Move(100 , -100);//到前極限後退0.5圈
   x=0;
   Serial.println("HOME");
  }
 
  if(val == 79)//O 歸零
  {
   Serial.println("Zero");
   data=0;//資料清除歸零
   dataval=0;
   lcd.setCursor(9,1);
   lcd.print(dataval);
   Serial.println(dataval);
  }  
 }

/*資料處理*/ 
 if(x!=0)
 {
  data = data + x;
  dataval = data * (1.0/200);//將步數轉為mm,螺桿節徑=1.0,轉一圈的步數=200
  Serial.println(dataval); 
  /*LCD顯示*/
  lcd.setCursor(0,0);
  lcd.print("Distance");
  lcd.setCursor(9,1);
  lcd.print(dataval);
  lcd.setCursor(14,1);
  lcd.print("mm"); 
  x=0;
 }
}

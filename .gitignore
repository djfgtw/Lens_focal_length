#include <Wire.h>
#include <Stepper.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);//LCD位址0x27
Stepper myStepper(200,8,9,10,11);//轉一圈的步數=200 ,A=8 ,A'=9 ,B=10 ,B'=11

int forward = 4;//前進鈕
int back = 5;//後退鈕
int zero = 6;//歸零鈕
float data = 0;//原始值
float dataval = 0;//目前距離值
volatile int FLstate = 0;
volatile int BLstate = 0;
long Speed = 20;//速度
int STEP = 0;//步數

void setup()
{
 lcd.init();//LCD初始化
 lcd.backlight();//LCD背光亮
  
 myStepper.setSpeed(20);//設定初始速度
  
 pinMode(forward, INPUT); 
 pinMode(back, INPUT);
 pinMode(zero, INPUT);
 attachInterrupt(0,FL,RISING);//前極限=2
 attachInterrupt(1,BL,RISING);//後極限=3
 
 Serial.begin(9600);
}

void loop()
{
/*前進動作*/
if(forward==HIGH && back==LOW)//前進鈕按下後執行
 {
   delay(100);
   while(FLstate=0)//當前極限=0執行,前極限=1不執行
   {
    Move(20 , 200);//正轉200步,速度20rpm
   }
   FLstate=0;
 }
 
/*後退動作*/
if(forward==LOW && back==HIGH)//後退鈕按下後執行
 {
   delay(100);
   while(BLstate=0)//當後極限=0執行,後極限=1不執行
   {
    Move(20 , -200);//反轉200步,速度20rpm
   }
   BLstate=0;
 } 
 
/*歸零動作*/
if(zero==HIGH)//歸零鈕按下後歸零
 {
  delay(100);
  data=0;//資料清除歸零
  STEP=0;
 } 

/*電腦數值輸入
while(Serial.available()>0)//
 {
  =Serial.read()
  {
    
  }
 }*/
}

    
void Move(long Speed , int SETP)
{
 /*馬達動作*/
 myStepper.setSpeed(Speed);
 myStepper.step(STEP);//正數為正方向,負數為反方向
 delay(500);
 
 /*資料處理*/
 data=data+STEP;
 Serial.println(data);
 dataval = data * (1.5 /200);//將步數轉為mm,螺桿節徑=1.5,轉一圈的步數=200
 Serial.println(dataval);
 
 /*LCD顯示*/
 lcd.setCursor(0,0);
 lcd.print("Distance");
 lcd.setCursor(14,1);
 lcd.print("mm");
 lcd.setCursor(9,1);
 lcd.print(dataval);
 delay(200);
}

void FL()//前極限狀態
{
  FLstate=1;
}
void BL()//後極限狀態
{
  BLstate=1;
}

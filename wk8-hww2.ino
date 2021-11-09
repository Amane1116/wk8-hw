#include <Wire.h>
#include "SSD1306Wire.h"
#include "pictures.h"
#include "pitches.h"
#define Button_Pin 32
SSD1306Wire  display(0x3c, 21, 22);
bool btn1_Pressed = false;

void Drawbackground(){
  display.drawXbm(0, 0, bg_width, bg_height, pic[0]);
  display.display();
}//畫出背景
void Drawvirus(int b_x,int b_y){
    display.drawXbm(b_x, b_y, virus_width, virus_height, pic[1]);
    display.display();
}//畫出角色

int freq = 2000;


int channel = 0;


int resolution = 8;

int tonePin = 12;

int melody[] = {
  0, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_DS3, 0, NOTE_F3, NOTE_F3,
  NOTE_F3,NOTE_D3
};
int noteDurations[] = {
  8, 8, 8, 8, 2, 8, 8, 8, 8, 2
};

unsigned long currentTime = 0, tonePreviousTime = 0, picPreviousTime = 0;
int pauseBetweenNotes = 0;
int noteDuration = 0;
int thisNote = 0;
boolean nextTone = true;
int dropY = -virus_height;
boolean nextPic = true;

int count = 0;

void setup() {
   Serial.begin(115200);
   pinMode(Button_Pin,INPUT);
   ledcSetup(channel, freq, resolution);
   ledcAttachPin(tonePin, channel);

   //OLED setting
   display.init();
   display.flipScreenVertically();
   display.clear(); //清除螢幕和緩衝區(clear)
}

void loop() {
    //buzzer
    if(nextTone){
      noteDuration = 1300 / noteDurations[thisNote];
      ledcWriteTone(channel, melody[thisNote]);
      pauseBetweenNotes = noteDuration * 1.3;
      nextTone = false;
    }
    if(digitalRead(Button_Pin) == HIGH && btn1_Pressed == false){
      btn1_Pressed = true;
      count++;
    } else if(digitalRead(Button_Pin) == LOW && btn1_Pressed == true){
      btn1_Pressed = false;
    }

    //畫出角色與其位置
    //if(nextPic){
      //  Drawbackground(); //畫出背景}
    display.clear(); //清除螢幕和緩衝區(clear)
    Serial.println(count);
    if(count%2==0){
      Drawbackground();
      Drawvirus(64 - virus_width / 2, dropY); //畫出角色
      dropY = dropY + 1;
      if(dropY > 64) dropY = 0 - virus_height;
      nextPic = false;
    }else{
     //display.init();
     //display.flipScreenVertically();
     
     Drawbackground();
      Drawvirus(64 - virus_width / 2, dropY + 0); //畫出角色
        dropY = dropY + 0;
        if(dropY > 64) dropY = 0 - virus_height;
        nextPic = false; 
    }
    display.display();
    //deley

    currentTime = millis(); //記錄現在時間
    if((currentTime - tonePreviousTime) > noteDuration){
      ledcWriteTone(channel, 0);
    } //buzzer停止播放
    if((currentTime - tonePreviousTime) > (pauseBetweenNotes)){
      nextTone = true;
      thisNote++;
      if(thisNote == 10) thisNote = 0;
      tonePreviousTime = currentTime; 
    } //buzzer播放下一個音
    if((currentTime - picPreviousTime) > 150 ){
       nextPic = true;
       picPreviousTime = currentTime;
    } //角色每150毫秒移動一次
}

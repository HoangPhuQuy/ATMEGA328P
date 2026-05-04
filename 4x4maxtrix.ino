// ======================== CONFIG ========================

// 16 LED chính

int ledPins[16] =
{
  2,3,4,5,6,7,8,9,
  10,11,12,13,
  A0,A1,A2,A3
};


// 3 LED phụ chung chân

#define EXTRA_LED A5


// nút đổi tần số

#define BUTTON_PIN A4



// ======================== SETTINGS ========================


// chuỗi LED

String bitString="0000000000000000";


// tần số LED chính

int freqMain=1000;


// tần số LED phụ

int freqExtra=2000;


// timing

unsigned long prevMain=0;
unsigned long prevExtra=0;

bool blinkStateMain=0;
bool blinkStateExtra=0;


// random pattern timing

unsigned long lastPatternChange=0;

unsigned long patternInterval=300; // ms



// debounce button

unsigned long lastButtonPress=0;



// ======================== SETUP ========================

void setup()
{

Serial.begin(115200);


for(int i=0;i<16;i++)
{
pinMode(ledPins[i],OUTPUT);
}


pinMode(EXTRA_LED,OUTPUT);

pinMode(BUTTON_PIN,INPUT_PULLUP);


// seed random

randomSeed(analogRead(A0));


Serial.println("Random pattern mode started");

}



// ======================== LOOP ========================

void loop()
{

updatePattern();

updateButton();

updateMainLED();

updateExtraLED();

}



// ======================== RANDOM PATTERN ========================

void updatePattern()
{

if(millis()-lastPatternChange>=patternInterval)
{

lastPatternChange=millis();


bitString="";


for(int i=0;i<16;i++)
{

bitString+=String(random(0,2));

}


// hiển thị chuỗi mới

Serial.print("Pattern: ");

Serial.println(bitString);

}

}



// ======================== BUTTON CONTROL ========================

void updateButton()
{

if(digitalRead(BUTTON_PIN)==LOW)
{

if(millis()-lastButtonPress>200)
{

lastButtonPress=millis();


freqMain+=1000;


if(freqMain>5000)
freqMain=1000;


Serial.print("Freq main = ");

Serial.println(freqMain);

}

}

}



// ======================== MAIN LED CONTROL ========================

void updateMainLED()
{

unsigned long interval=1000000UL/(2*freqMain);


if(micros()-prevMain>=interval)
{

prevMain=micros();


blinkStateMain=!blinkStateMain;


for(int i=0;i<16;i++)
{

if(bitString[i]=='1')

digitalWrite(ledPins[i],HIGH);

else

digitalWrite(ledPins[i],blinkStateMain);

}

}

}



// ======================== EXTRA LED CONTROL ========================

void updateExtraLED()
{

unsigned long interval=1000000UL/(2*freqExtra);


if(micros()-prevExtra>=interval)
{

prevExtra=micros();


blinkStateExtra=!blinkStateExtra;


digitalWrite(EXTRA_LED,blinkStateExtra);

}

}
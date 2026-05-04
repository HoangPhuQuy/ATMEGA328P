// ======================== CONFIG ========================

// 16 LED chính

int ledPins[16] =
{
  11,10,2,12,
  8,9,A2,13,
  6,4,A1,A0,
  7,5,3,A3
};


// 3 LED phụ chung chân

#define EXTRA_LED A5


// nút đổi tần số

#define BUTTON_PIN A4

// ======================== SETTINGS ========================


// chuỗi điều khiển LED

String bitString = "1010101010101010";


// tần số LED chính

int freqMain = 1000;


// tần số LED phụ

int freqExtra = 2000;



// timing

unsigned long prevMain = 0;
unsigned long prevExtra = 0;


bool blinkStateMain = 0;
bool blinkStateExtra = 0;


// debounce button

unsigned long lastButtonPress = 0;



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


  Serial.println("=================================");
  Serial.println("Nhap chuoi 16 bit (VD: 1010101010101010)");
  Serial.println("1 = sang lien tuc");
  Serial.println("0 = nhap nhay");
  Serial.println("=================================");

}



// ======================== LOOP ========================

void loop()
{

  readSerial();

  updateButton();

  updateMainLED();

  updateExtraLED();

}



// ======================== SERIAL INPUT ========================

void readSerial()
{

  if(Serial.available())
  {

    String input = Serial.readStringUntil('\n');

    input.trim();


    if(input.length()==16)
    {

      bitString = input;


      Serial.println("");
      Serial.println("Da nhan chuoi moi:");

      Serial.println(bitString);


      Serial.println("Trang thai tung LED:");

      for(int i=0;i<16;i++)
      {

        Serial.print("LED ");
        Serial.print(i+1);
        Serial.print(" (pin ");
        Serial.print(ledPins[i]);
        Serial.print(") = ");

        if(bitString[i]=='1')
        Serial.println("SANG");

        else
        Serial.println("NHAP NHAY");

      }


      Serial.print("Tan so LED chinh: ");
      Serial.print(freqMain);
      Serial.println(" Hz");


      Serial.print("Tan so 3 LED phu: ");
      Serial.print(freqExtra);
      Serial.println(" Hz");


      Serial.println("---------------------------------");

    }


    else
    {

      Serial.println("LOI: Nhap dung 16 bit!");

    }

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

      freqMain += 1000;


      if(freqMain>5000)
      freqMain=1000;


      Serial.print("Tan so moi LED chinh = ");

      Serial.print(freqMain);

      Serial.println(" Hz");

    }

  }

}



// ======================== MAIN LED CONTROL ========================

void updateMainLED()
{

  unsigned long interval = 1000000UL/(2*freqMain);


  if(micros()-prevMain >= interval)
  {

    prevMain = micros();


    blinkStateMain = !blinkStateMain;


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

  unsigned long interval = 1000000UL/(2*freqExtra);


  if(micros()-prevExtra >= interval)
  {

    prevExtra = micros();


    blinkStateExtra = !blinkStateExtra;


    digitalWrite(EXTRA_LED,blinkStateExtra);

  }

}
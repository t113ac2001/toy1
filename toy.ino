const int RledPin = 9;    
const int GledPin = 10;   
const int BledPin = 11;   
const int buttonPin = 7;  

int mood = 10;                   
unsigned long lastPressTime = 0; 
unsigned long lastUpdateTime = 0;
unsigned long pressStartTime = 0;
bool buttonPressed = false;
bool breathingMode = false;      
int breathingValue = 0;          
int breathingDirection = 1;     

const int maxMood = 20;
const int minMood = 0;

void setup() {
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); 
  Serial.begin(9600);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  unsigned long currentMillis = millis();

  if (buttonState == LOW && !buttonPressed) {
    buttonPressed = true;
    pressStartTime = currentMillis;
  }

  if (buttonState == HIGH && buttonPressed) {
    unsigned long pressDuration = currentMillis - pressStartTime;
    buttonPressed = false;

    if (pressDuration >= 500) {
      breathingMode = !breathingMode;
      Serial.println(breathingMode ? "Breathing Mode ON" : "Breathing Mode OFF");
      delay(200); // 防抖
    } else {

      if (!breathingMode) {
        mood++;
        if (mood > maxMood) mood = maxMood;
        lastPressTime = currentMillis;
      }
    }
  }

  
  if (breathingMode) {
    if (currentMillis - lastUpdateTime > 15) {
      breathingValue += breathingDirection;
      if (breathingValue >= 255 || breathingValue <= 0) {
        breathingDirection = -breathingDirection;
      }
      lastUpdateTime = currentMillis;
    }

    
    int R, G, B;
    getColorByMood(mood, R, G, B);
    
    analogWrite(RledPin, 255 - (R * breathingValue / 255));
    analogWrite(GledPin, 255 - (G * breathingValue / 255));
    analogWrite(BledPin, 255 - (B * breathingValue / 255));
  }

  if (currentMillis - lastPressTime > 5000) {
    if (currentMillis - lastUpdateTime > 1000) {
      if (mood > minMood) mood--;
      lastUpdateTime = currentMillis;
    }
  }

  setColorByMood(mood);
}

void setColorByMood(int mood) {
  int R, G, B;
  getColorByMood(mood, R, G, B);

  analogWrite(RledPin, 255 - R);
  analogWrite(GledPin, 255 - G);
  analogWrite(BledPin, 255 - B);
}


void getColorByMood(int mood, int &R, int &G, int &B) {
  float brightnessInterval = 255 / 10.0;

  if (mood <= 10) {
    R = 255;
    G = brightnessInterval * mood;
    B = 0;
  } else {
    R = 255 - brightnessInterval * (mood - 10);
    G = 255;
    B = brightnessInterval * (mood - 10);
  }
}
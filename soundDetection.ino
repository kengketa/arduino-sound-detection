extern volatile unsigned long timer0_millis;
const int SOUND_SENSOR = 5;
const int ALARM_PORT = 3;
const int TOUCH_BUTTON = 9;
const int DETECTION_RANGE = 10 * 1000; // msseconds
const int DETECTION_DELAY = 0.8 * 1000; // msseconds
const int DETECT = 0;
const int COUNTER_LIMIT = 3;

int detectionCounter = 0;
long startDetection = 0;
boolean detected = false;
boolean alam = false;


void setup() {
  Serial.begin(9600);
  pinMode(SOUND_SENSOR, INPUT);
  pinMode(TOUCH_BUTTON, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ALARM_PORT, OUTPUT);
}

void loop() {
  
    int resetPush = digitalRead(TOUCH_BUTTON);
    if(resetPush == 1){
      reset();
    }
    
    int statusSensor = digitalRead(SOUND_SENSOR);
    if (statusSensor == DETECT && alam == false){
        detected = true;
    }
    if (detected){
      delay(DETECTION_DELAY);
      detected = false;
      detectionCounter++;
      Serial.print("heard:");
      Serial.print(detectionCounter);
      Serial.print("\n");
    }
    if(detectionCounter == 1){
      if(startDetection == 0){
        startDetection = millis();
      }
      if(millis() - startDetection > DETECTION_RANGE){
         reset();
      }
    }
    if(detectionCounter != 0 && millis() - startDetection > DETECTION_RANGE && alam == false){
      reset();
    }
    if(detectionCounter >= COUNTER_LIMIT && alam == false){
      setAlam();
    }
    if(millis() > 14400000){ // 4 * 60 * 60 * 1000  reset every 4 hours
      timer0_millis = 0;
    }
}

void reset(){
  startDetection = 0;
  detectionCounter = 0;
  alam = false;
  digitalWrite(ALARM_PORT, LOW);
  Serial.println("reset");
}

void setAlam(){
  alam = true;
  digitalWrite(ALARM_PORT, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Alam go offf!");
}

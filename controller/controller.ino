#include <Servo.h>

const char SERIAL_HORIZONTAL = 'H'; // Lettre de position Front pour le calibrage des moteurs
const char SERIAL_VERTICAL = 'V';
char serial_action;
String serial_number_value = ""; // TODO check number of index for security
int indexAction = 0;
unsigned long last_serial_millis;

const unsigned long DELAY_NO_SERIAL_GO_RANDOM = 10000;

int HorizontalMovementRange[7] = {120, 110, 100, 90, 80, 70, 60};
int HorizontalLeft = 60;
int HorizontalMiddle = 90;
int HorizontalRight = 120;
int HorizontalTarget = HorizontalMiddle;
int HorizontalCurrent = -1;
const int SERVO1_PIN = 3;
Servo servo1;

int VerticalMovementRange[7] = {100, 90, 80, 70, 60, 50, 40};
int VerticalTop = 100;
int VerticalMiddle = 70;
int VerticalBottom = 40;
int VerticalTarget = VerticalMiddle; // TODO
int VerticalCurrent = -1;
const int SERVO2_PIN = 5;
Servo servo2;

const int SERVO3_PIN = 6;
const int SERVO4_PIN = 9;
const int SERVO3_CLOSE_POSITION = 60; // TODO
const int SERVO3_OPEN_POSITION = 120;  // TODO
const int SERVO4_OPEN_POSITION = 40;  // TODO
const int SERVO4_CLOSE_POSITION = 110; // TODO
Servo servo3;
Servo servo4;

const int SERVO5_PIN = 10;
const int SERVO6_PIN = 11;
const int SERVO5_CLOSE_POSITION = 120;
const int SERVO5_OPEN_POSITION = 60;
const int SERVO6_CLOSE_POSITION = 40;
const int SERVO6_OPEN_POSITION = 110;
Servo servo5;
Servo servo6;
bool pendingBlink = false;
bool eyesClosed = false;
unsigned long delay_blink_millis;
const unsigned long DELAY_BLINK = 200;
unsigned long delay_between_blink_millis;
unsigned long DELAY_BETWEEN_BLINK = 3000;

unsigned long delay_between_random_move_millis;
const unsigned long DELAY_BETWEEN_RANDOM_MOVE = 3000;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
  servo4.attach(SERVO4_PIN);
  servo5.attach(SERVO5_PIN);
  servo6.attach(SERVO6_PIN);

  openEyes();
  performMovments();
  
  last_serial_millis = millis();
  delay_between_blink_millis = millis();
  delay_between_random_move_millis = millis();
}

void loop()
{
  // put your main code here, to run repeatedly:

  manageSerialMovements();

  if (millis() - last_serial_millis > DELAY_NO_SERIAL_GO_RANDOM)
  {
    
    if (millis() - delay_between_blink_millis > DELAY_BETWEEN_BLINK && !pendingBlink)
    {
      Serial.println("blink random");
      blinkEyes();
      delay_between_blink_millis = millis();
      refreshRandomBlinkDelay();
    }

    if (pendingBlink == false && millis() - delay_between_random_move_millis > DELAY_BETWEEN_RANDOM_MOVE)
    {
      Serial.println("move random");
      setRandomTargets();
      delay_between_random_move_millis = millis();
    }
  }

  performBlinkEyes();

  performMovments();
}

void manageSerialMovements()
{
  while (Serial.available() > 0)
  {
    char storageActionChar = Serial.read();
    Serial.print("serial received");
    Serial.println(storageActionChar);
    Serial.println(indexAction);
    Serial.println(storageActionChar);
    if (storageActionChar == SERIAL_HORIZONTAL || storageActionChar == SERIAL_VERTICAL)
    {
      serial_action = storageActionChar;
    }
    else if (storageActionChar == ';')
    {
      setSerialTargetMovements();

      clearSerialActionData();
    }
    else
    {
        serial_number_value += (char)storageActionChar;
    }

    last_serial_millis = millis();
  }
}

void setSerialTargetMovements()
{
  int valueNumeric = serial_number_value.toInt();
  Serial.print("serial action");
  Serial.println(valueNumeric);
  if(valueNumeric < 7 && valueNumeric > -1){
    
    if (serial_action == SERIAL_HORIZONTAL)
    {
      Serial.println("horizontal serial");
      setHorizontalTargetFromRange(valueNumeric);
    }
    else if (serial_action == SERIAL_VERTICAL)
    {
      Serial.println("vertical serial");
      setVerticalTargetFromRange(valueNumeric);
    }
  }
}

void clearSerialActionData()
{
  serial_number_value = "";

  indexAction = 0;
  serial_action = -1;
}

void blinkEyes()
{
  pendingBlink = true;
}

void performBlinkEyes()
{
  if (pendingBlink)
  {
    if (eyesClosed && (millis() - delay_between_blink_millis > DELAY_BLINK))
    {
      openEyes();
      pendingBlink = false;
    }
    else if(eyesClosed == false)
    {
      closeEyes();
      delay_between_blink_millis = millis();
    }
  }
}

void setRandomTargets()
{
  int horizontalRandom = random(0, 6);
  int verticalRandom = random(0, 6);

  setHorizontalTargetFromRange(horizontalRandom);
  setVerticalTargetFromRange(verticalRandom);
}

void setHorizontalTargetFromRange(int index){

  HorizontalTarget = HorizontalMovementRange[index];

  Serial.print("H: ");
  Serial.println(HorizontalTarget);
}

void setVerticalTargetFromRange(int index){

  VerticalTarget = VerticalMovementRange[index];

  Serial.print("V: ");
  Serial.println(VerticalTarget);
}

void performMovments()
{
  moveHorizontal();

  moveVertical();
}

void moveHorizontal()
{
  if (HorizontalTarget != HorizontalCurrent)
  {
    servo1.write(HorizontalTarget);
    HorizontalCurrent = HorizontalTarget;
  }
}

void moveVertical()
{
  if (VerticalTarget != VerticalCurrent)
  {
    servo2.write(VerticalTarget);
    VerticalCurrent = VerticalTarget;
  }
}

void refreshRandomBlinkDelay(){
  DELAY_BETWEEN_BLINK = random(3000, 6000);
}

void openEyes()
{
  servo3.write(SERVO3_OPEN_POSITION);
  servo4.write(SERVO4_OPEN_POSITION);

  servo5.write(SERVO5_OPEN_POSITION);
  servo6.write(SERVO6_OPEN_POSITION);

  eyesClosed = false;
}

void closeEyes()
{
  servo3.write(SERVO3_CLOSE_POSITION);
  servo4.write(SERVO4_CLOSE_POSITION);

  servo5.write(SERVO5_CLOSE_POSITION);
  servo6.write(SERVO6_CLOSE_POSITION);

  eyesClosed = true;
}

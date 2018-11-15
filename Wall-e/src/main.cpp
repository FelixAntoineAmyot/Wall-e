#include <Arduino.h>
#include <LibRobus.h>
#include <TimedAction.h>
#include <QTRSensors.h>

const int LED_R = 2;
const int LED_G = 3;
const int LED_B = 5 ;
const int BOUTON_PIN = 37;
const int M_GAUCHE = 0;
const int M_DROITE = 0;

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(LED_R, red);
  analogWrite(LED_G, green);
  analogWrite(LED_B, blue);  
}

void stop()
{
  setColor(255,0,0);
  MOTOR_SetSpeed(M_GAUCHE,0);
  MOTOR_SetSpeed(M_DROITE,0);
}
void waitBouton()
{
  setColor(255,0,0);
  while (digitalRead(BOUTON_PIN) == 0){}
  while(digitalRead(BOUTON_PIN) == 1){} 
  delay(50); // bouncing buffer
 setColor(0,255,0);
}

void t_bouton()
{
  if (digitalRead(BOUTON_PIN) == 1)
  {
    while(digitalRead(BOUTON_PIN) == 1){}     // attend le relachement
    stop();
    delay(50); // bouncing buffer                             
    waitBouton();                             // attend pour repartir
  }
  digitalWrite(LED_R,LOW);
}

void t_Couleur()
{
  //pet pet
}

void calibration()
{
  setColor(0,0,0);
  for (int i =0; i<10;i++)
  {
    if(i%2 == 0)
    {
     setColor(0,0,255);
    }
    else
    setColor(0,0,0);
    delay(500);
  }

} 
TimedAction threadBouton = TimedAction(50,t_bouton);
TimedAction threadCouleur = TimedAction(500,t_Couleur);

void setup() {
  
  BoardInit();
  Serial.begin(9600);
  Serial.println("start");
  pinMode(LED_R,OUTPUT);
  digitalWrite(LED_R,LOW);
  pinMode(LED_G,OUTPUT);
  digitalWrite(LED_G,LOW);
  pinMode(LED_B,OUTPUT);
  digitalWrite(LED_B,HIGH);
  pinMode(BOUTON_PIN, LOW);

  waitBouton();
  calibration();
  waitBouton();
  
}

void loop() {
  threadBouton.check();
}
#include <Arduino.h>
#include <LibRobus.h>
#include <TimedAction.h>
#include <QTRSensors.h>

const int LED_V = 34;
const int LED_J = 35;
const int LED_R = 39;
const int BOUTON_PIN = 37;
const int M_GAUCHE = 0;
const int M_DROITE = 0;


void stop()
{
  digitalWrite(LED_R,HIGH);
  MOTOR_SetSpeed(M_GAUCHE,0);
  MOTOR_SetSpeed(M_DROITE,0);
}
void waitBouton()
{
  digitalWrite(LED_V,LOW);
  digitalWrite(LED_R,HIGH);
  while (digitalRead(BOUTON_PIN) == 0){}
  while(digitalRead(BOUTON_PIN) == 1){} 
  delay(50); // bouncing buffer
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_V,HIGH);
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
  digitalWrite(LED_V,LOW);
  for (int i =0; i<10;i++)
  {
    if(i%2 == 0)
    {
      digitalWrite(LED_J,HIGH);
    }
    else
    digitalWrite(LED_J,LOW);
    delay(500);
  }

} 
TimedAction threadBouton = TimedAction(50,t_bouton);
TimedAction threadCouleur = TimedAction(500,t_Couleur);

void setup() {
  
  BoardInit();
  pinMode(LED_V,OUTPUT);
  digitalWrite(LED_V,LOW);
  pinMode(LED_J,OUTPUT);
  digitalWrite(LED_J,LOW);
  pinMode(LED_R,OUTPUT);
  digitalWrite(LED_R,HIGH);
  pinMode(BOUTON_PIN, LOW);

  waitBouton();
  calibration();
  waitBouton();
  
}

void loop() {
  threadBouton.check();
}
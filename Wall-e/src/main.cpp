#include <Arduino.h>
#include <LibRobus.h>
#include <TimedAction.h>
#include <QTRSensors.h>
#include <ADJDS311.h>

#define NUM_SENSORS             8  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN             2  // emitter is controlled by digital pin 2

// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsAnalog qtra((unsigned char[]) {3,4,5,6,7,8,9,10},
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

const int vitesse = 0.5;
const int LED_R = 47  ;
const int LED_G = 48;
const int LED_B = 49;
const int BOUTON_PIN = 46;
const int M_GAUCHE = 0;
const int M_DROITE = 1;
const int maxCapacity = 3;
const int pinServo = 8;

#define stp 11
#define dir 12
#define ena 10
#define ledPin = 42;
ADJDS311 color(42);

const int pinA = 31;
const int pinB = 27;
const int pinC = 24;
const int pinD = 23;
const int pinE = 22;
const int pinF = 30;
const int pinG = 25;
const int D1 =32;
const int D2 = 29;
const int D3 = 28;
const int D4 = 26;
const int D5 = 33;


int nbRouge = 0;
int nbJaune = 0;
int nbBleu = 0;

void servo(int pulse) // 1 = ouvert     2 = ferme
{
  digitalWrite(pinServo,1);
  delay(pulse);
  digitalWrite(pinServo,0);
  delay(20-pulse);
}
void delayServo(int milli,int pulse) /// pas precis
{
  for (int t =0;t<milli;t+=1)
  {
    servo(pulse);
  }
}
void step(int angle, bool direction)
{
  long nb_pulse = angle / 0.225;
  Serial.println(nb_pulse);


  if(direction == true){
    digitalWrite(dir, HIGH); //Set direction of motor to CW.
    digitalWrite(ena, LOW); //Set Enable pin to low (Enabled)
    for(int count = 1; count < nb_pulse; count++){
      
      digitalWrite(stp, HIGH); //Set Step Pin to HIGH for half of period.
      delay(1);
      digitalWrite(stp, LOW); //Set Step pin to LOW for half of period.
      delay(1);
    }
  }

  else if(direction == false){
    digitalWrite(dir, LOW); //Set direction of motor to CCW.
    for(int count = 1; count < nb_pulse; count++){
      
      digitalWrite(stp, HIGH); //Set Step Pin to HIGH for the period duration.
      delay(1);
      digitalWrite(stp, LOW);
      delay(1);
    }
    digitalWrite(ena, HIGH); //Set Enable pin to high (Disabled)
  }
    
}
void displayChar(int position, int abcdefg[7])//position 1 a 5          thread pt pas nessecaire
{
  switch(position)
  {
    case 1:position = D1;break;
    case 2:position = D2;break;
    case 3:position = D3;break;
    case 4:position = D4;break;
    case 5:position = D5;break;
    default: position = D5;break;
  }
  
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(position, 0);
  for(int pin = pinA; pin <= pinG; pin ++)
  {
    digitalWrite(pin, abcdefg[pin-pinA]);
  }
  delay(1);
}
int changeC(int ch[],int a,int b,int c,int d,int e,int f,int g)
{
  ch[0] = a;
  ch[1] = b;
  ch[2] = c;
  ch[3] = d;
  ch[4] = e;
  ch[5] = f;
  ch[6] = g;
}
void displayString(char characters[])//juste minuscule
{
  int ch[7];
  for(int i = 0; i <5; i++)
  {
    switch (characters[i])
    {
      case 'a':case'A':changeC(ch,1,1,1,0,1,1,1);break;
      case 'b':changeC(ch,0,0,1,1,1,1,1);break;      case 'B':changeC(ch,1,1,1,1,1,1,1);break;
      case 'c':case'C':changeC(ch,1,0,0,1,1,1,0);break;
      case 'd':changeC(ch,0,1,1,1,1,0,1);break;      case 'D':changeC(ch,1,1,1,1,1,1,0);break;
      case 'e':case'E':changeC(ch,1,0,0,1,1,1,1);break;
      case 'f':case'F':changeC(ch,1,0,0,0,1,1,1);break;
      case 'g':changeC(ch,1,1,1,1,0,1,1);break;      case 'G':changeC(ch,1,0,1,1,1,1,1);break;
      case 'h':changeC(ch,0,0,1,0,1,1,1);break;      case 'H':changeC(ch,0,1,1,0,1,1,1);break;
      case 'i':case'I':changeC(ch,0,1,1,0,0,0,0);break;
      case 'j':case'J':changeC(ch,0,1,1,1,0,0,0);break;
      case 'k':case'K':changeC(ch,0,1,1,0,1,1,1);break;
      case 'l':case'L':changeC(ch,0,0,0,1,1,1,0);break;
      case 'n':changeC(ch,0,0,1,0,1,0,1);break;      case 'N':changeC(ch,1,1,1,0,1,1,0);break;
      case 'm':changeC(ch,0,0,1,0,1,0,1);break;      case 'M':changeC(ch,1,1,1,0,1,1,0);break; // doit ecrire 2 m
      case 'o':changeC(ch,0,0,1,1,1,0,1);break;      case 'O':changeC(ch,1,1,1,1,1,1,0);break;
      case 'p':case'P':changeC(ch,1,1,0,0,1,1,1);break;
      case 'q':case'Q':changeC(ch,1,1,1,0,0,1,1);break;
      case 'r':changeC(ch,0,0,0,0,1,0,1);break;      case 'R':changeC(ch,1,1,1,0,1,1,1);break;
      case 's':case'S':changeC(ch,1,0,1,1,0,1,1);break;
      case 't':changeC(ch,0,0,0,1,1,1,1);break;      case 'T':changeC(ch,1,0,0,0,1,1,0);break; // doit ecrire tT
      case 'u':changeC(ch,0,0,1,1,1,0,0);break;      case 'U':changeC(ch,0,1,1,1,1,1,0);break;
      case 'v':changeC(ch,0,0,1,1,1,0,0);break;      case 'V':changeC(ch,0,1,1,1,1,1,0);break;
      case 'w':changeC(ch,0,0,1,1,1,0,0);break;      case 'W':changeC(ch,0,1,1,1,1,1,0);break; // doit ecrire 2 w
      case 'x':case'X':changeC(ch,0,1,1,0,1,1,1);break;
      case 'y':case'Y':changeC(ch,0,1,1,1,0,1,1);break;
      case 'z':case'Z':changeC(ch,1,1,0,1,1,0,1);break;

      case '0':changeC(ch,1,1,1,1,1,1,0);break;
      case '1':changeC(ch,0,1,1,0,0,0,0);break;
      case '2':changeC(ch,1,1,0,1,1,0,1);break;
      case '3':changeC(ch,1,1,1,1,0,0,1);break;
      case '4':changeC(ch,0,1,1,0,0,1,1);break;
      case '5':changeC(ch,1,0,1,1,0,1,1);break;
      case '6':changeC(ch,1,0,1,1,1,1,1);break;
      case '7':changeC(ch,1,1,1,0,0,0,0);break;
      case '8':changeC(ch,1,1,1,1,1,1,1);break;
      case '9':changeC(ch,1,1,1,0,0,1,1);break;
      default : changeC(ch,0,0,0,0,0,0,0);break;
    }
    displayChar(i+1,ch);
  }
  digitalWrite(D5,1);
}
void displayCompteur()
{
  char compteur[5] = "+++++";
  compteur[0] = nbRouge +'0';
  compteur[2] = nbJaune +'0';
  compteur[4] = nbBleu +'0';
  displayString(compteur);
}
void setColor(int red, int green, int blue)
{
  digitalWrite(LED_R, red);
  Serial.print(red);

  digitalWrite(LED_G, green);
  Serial.print(green);
  digitalWrite(LED_B, blue); 
  Serial.println(blue); 
}
void stop()
{
  setColor(1,0,0);
  MOTOR_SetSpeed(M_GAUCHE,0);
  MOTOR_SetSpeed(M_DROITE,0);
}
int bacPlein()
{
  if(nbRouge >= maxCapacity) return 1;
  else if (nbJaune >= maxCapacity) return 2;
  else if (nbBleu >= maxCapacity) return 3;
  
  return 0;
}
void viderBac(char couleur[])
{
  stop();
  while (digitalRead(BOUTON_PIN) == 0)
  {
    displayString(couleur);
  }
  delay(1000);
}

void waitBouton()
{
  Serial.println("stop");
  setColor(1,0,0);
  while (digitalRead(BOUTON_PIN) == 0){displayString("Stop+");}
  Serial.println("peser");
  while(digitalRead(BOUTON_PIN) == 1){} 
  delay(50); // bouncing buffer
  setColor(0,1,0);
}

void t_bouton()
{
  Serial.println("bouton");
  if (digitalRead(BOUTON_PIN) == 1)
  {
    while(digitalRead(BOUTON_PIN) == 1){}     // attend le relachement
    stop();
    delay(50); // bouncing buffer                             
    waitBouton();                             // attend pour repartir
  }
  digitalWrite(LED_R,LOW);
}
int couleur()
{
  int coul=0;
    int tab[10];
    for (int k=0;k<10;k++){
      tab[k]=0;
    }
    int i=0;
      while (i<10){
        int red=color.readRed();
        int green=color.readGreen();
        int blue=color.readBlue();
      if (red>green){
        if (green>blue){
          if (blue>350){
          tab[i]=1;
          }
        }
      }
      if (green>red){
        if (red>blue){
          if (blue>350){
          if (red>400){
            tab[i]=5;
          }
          }
        }
      }
      if (green>blue){
        if(blue>red){
          if (blue>350){
          tab[i]=10;
          }
        }
      }
      else{
            tab[i]=0;
      }
      i++;
      }
      int somme=0;
      for (int j=0;j<10;j++){
        somme+=tab[j];
      }
      switch (somme){

      case 8: coul=1;break;
      case 9: coul=1;break;
      case 10: coul=1;break;
      case 40: coul=2;break;
      case 45: coul=2;break;
      case 50: coul=2;break;
      case 80: coul=3;break;
      case 90: coul=3;break;
      case 100: coul=3;break;

    default:coul=0;
  }
  return coul;
}
void compteur(int couleur)
{
  switch(couleur)
  {
    case 1:nbRouge++;break;
    case 2:nbJaune++;break;
    case 3:nbBleu++;break;
  }
}

void rammasser(int couleur)
{
  if (couleur == 0)return;
  stop();
  setColor(1,1,1);
  delayServo(50,2);
  step(180,true);
  delayServo(50,1);
  step(180,false);
  compteur(couleur);
  switch(bacPlein())
  {
    case 1:nbRouge =0; viderBac("rouGe");break;
    case 2:nbJaune = 0; viderBac("Jaune");break;
    case 3:nbBleu = 0; viderBac("BleU");break;
  }
  setColor(0,1,0);
}
void t_Couleur()
{
  Serial.println("couleur");
  rammasser(couleur());
}
int isLine()
{
  qtra.read(sensorValues);
  for(int i = 1; i < NUM_SENSORS+1;i++)
  {
    if(sensorValues[i-1] > 800) return i;
  }
  return 0;
}
void uTurn(int line)
{
  if (line <=4)
  {
    MOTOR_SetSpeed(1,0);
  }
  else
  {
    MOTOR_SetSpeed(0,0);
  }
}
void straight()
{
  MOTOR_SetSpeed(0,vitesse);
  MOTOR_SetSpeed(1,vitesse);
}
void t_bouger()
{
  Serial.println("bouger");
  int line = isLine();
  if(line != 0) uTurn(line);
  else straight();
}
void calibration()
{
  setColor(0,0,255);
  color.init();
  color.ledOn();
  color.calibrate();

} 
TimedAction threadBouton = TimedAction(50,t_bouton);
TimedAction threadCouleur = TimedAction(500,t_Couleur);
TimedAction threadBouger = TimedAction(100,t_bouger);


void setup() {
  
  BoardInit();
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  Serial.begin(9600);
  Serial.println("start");
  pinMode(LED_R,OUTPUT);
  digitalWrite(LED_R,LOW);
  pinMode(LED_G,OUTPUT);
  digitalWrite(LED_G,LOW);
  pinMode(LED_B,OUTPUT);
  digitalWrite(LED_B,LOW);
  pinMode(BOUTON_PIN, LOW);
  pinMode(ena,OUTPUT);
  digitalWrite(ena, HIGH);
  pinMode(dir,OUTPUT);
  digitalWrite(dir, LOW);
  pinMode(stp,OUTPUT);
  digitalWrite(stp, LOW);
  // initialize the digital pins as outputs.
  pinMode(pinA, OUTPUT);     
  pinMode(pinB, OUTPUT);     
  pinMode(pinC, OUTPUT);     
  pinMode(pinD, OUTPUT);     
  pinMode(pinE, OUTPUT);     
  pinMode(pinF, OUTPUT);     
  pinMode(pinG, OUTPUT);   
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT); 
  pinMode(D5,OUTPUT); 
  pinMode(pinServo,OUTPUT);
  
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);

  waitBouton();
  Serial.println("test");
  calibration();
  Serial.println("1");
  waitBouton();
  Serial.println("2");
  
  
}

void loop() {
  //setColor(1,0,0);
  //delay(1000);
  //setColor(0,1,0);
  //delay(1000);
  //setColor(0,0,1);
 // delay(1000);
 threadBouton.check();
  threadCouleur.check();
  threadBouger.check();
  Serial.println("loop");
  displayCompteur();
  /*
  digitalWrite(31,HIGH);
digitalWrite(27,LOW);
digitalWrite(24,HIGH);
digitalWrite(23,HIGH);
digitalWrite(22,LOW);
digitalWrite(30,HIGH);
digitalWrite(25,HIGH);

digitalWrite(32,HIGH);
digitalWrite(29,HIGH);
digitalWrite(28,HIGH);
digitalWrite(26,LOW);
digitalWrite(33,LOW);
*/
}
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
const int maxCapacity = 3;

const int pinA = 2;
const int pinB = 3;
const int pinC = 4;
const int pinD = 5;
const int pinE = 6;
const int pinF = 7;
const int pinG = 8;
const int D1 = 9;
const int D2 = 10;
const int D3 = 11;
const int D4 = 12;
const int D5 = 13;

int nbBleu = 0;
int nbRouge = 0;
int nbVert = 0;

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
  delay(4);
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
}
void displayCompteur()
{
  char compteur[5] = "+++++";
  compteur[0] = nbBleu +'0';
  compteur[2] = nbVert +'0';
  compteur[4] = nbRouge +'0';
  displayString(compteur);
}
int bacPlein()
{
  if(nbBleu >= maxCapacity) return 1;
  else if (nbVert >= maxCapacity) return 2;
  else if (nbRouge >= maxCapacity) return 3;
  
  return 0;
}
void viderBac(char couleur[])
{
  //LED
  while (true)
  {
    displayString(couleur);
  }
}
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
  
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);

  waitBouton();
  calibration();
  waitBouton();
  
}

void loop() {
  threadBouton.check();
  threadCouleur.check();

  switch(bacPlein())
  {
    case 1: viderBac("BleU");break;
    case 2: viderBac("VeRt");break;
    case 3: viderBac("RouGe");break;
    default:displayCompteur();
  }
}

#include <Pixy2.h>
#include <PIDLoop.h>
#include <Servo.h>

Servo myservo;

Pixy2 pixy;
Servo myServo;
PIDLoop panLoop(400, 0, 400, true);
PIDLoop tiltLoop(500, 0, 500, true);

long invertServoAngle(long angle);
int shotuganServoPos = 0;
int radarStatus = 0;
int visione = 0;


  const int motore1      = 7;  
  const int motore2      = 2; 
  const int motore3      = 5; 
  const int motore4      = 4;
  const int motorev12  = 6; 
  const int motorev34  = 3;
  const int sinistro =A5;
  const int destro =A4;
  int vel = 100;        
  int velc = 230;     
  int d = 10;
  int IFS;
  int IFD;
   int MAX = 1000;
 int MIN =0;
 int MED = 500;
 int AGG = 10;
 int POSX=500;
 int posServo = 0;  
int Cooldown = 1; 

void setup() {
    Serial.begin(115200);
  Serial.print("Starting...\n");
 
  pixy.init();
 
  pixy.changeProg("color_connected_components");

    myservo.attach(8); 

  
    pinMode(motore1, OUTPUT);
  pinMode(motore2, OUTPUT);
  pinMode(motore3, OUTPUT);
  pinMode(motore4, OUTPUT);
  myservo.attach(8);

  delay(3000);
}

bool radar()
{
 


pixy.setServos(POSX,MED);

   while (POSX < MAX && pixy.ccc.getBlocks() == false)
   {
    
     POSX = POSX+ AGG ;
     pixy.setServos(POSX,MED);
     delay(30);
     
   }
  
     if ( POSX = MAX)
    {
      
        while (POSX>MIN &&  pixy.ccc.getBlocks() == false)
       {
        
      
        POSX = POSX-AGG;
        pixy.setServos(POSX,MED);
        delay(30);
        
       }
     
     }
     if (pixy.ccc.getBlocks() == true){
      return true;
     }
     else{
      return false;
     }
  }
  
 
 
void movimento() {
 
IFS = analogRead(sinistro);
IFD = analogRead(destro);



if(IFD > 100 && IFS < 100)
{

  digitalWrite(motore1,LOW);
  digitalWrite(motore2,HIGH);                       
  digitalWrite(motore3,LOW);
  digitalWrite(motore4,HIGH);


  analogWrite (motorev12, vel);
  analogWrite (motorev34, velc);
  delay(d);
  
  }
  
if(IFD < 100 && IFS > 100)
{
  
  
  digitalWrite(motore1,HIGH);
  digitalWrite(motore2,LOW);                       
  digitalWrite(motore3,HIGH);
  digitalWrite(motore4,LOW);


  analogWrite (motorev12, velc);
  analogWrite (motorev34, vel);


  delay(d);
  }



if(IFD >100 && IFS > 100)
{ 
  
  analogWrite (motorev12, 0);
  analogWrite (motorev34, 0);
  
  }

else
{
  
  digitalWrite(motore1,HIGH);
  digitalWrite(motore2,HIGH);                       
  digitalWrite(motore3,LOW);
  digitalWrite(motore4,LOW);


  analogWrite (motorev12, vel);
  analogWrite (motorev34, vel);


  delay(d);
  }
 }

 void torretta(){
 
  static int i = 0;
  int j;
  char buf[64]; 
  int32_t panOffset, tiltOffset;
  
  // trova gli oggetti 
  pixy.ccc.getBlocks();
  
  if (pixy.ccc.numBlocks)
  {        
    i++;
    
    if (i%60==0)
      Serial.println(i);   
    
    // calcolare l'errore rispettoa blocks[0]
 // cioè l'oggetto più grande
    panOffset = (int32_t)pixy.frameWidth/2 - (int32_t)pixy.ccc.blocks[0].m_x;
    tiltOffset = (int32_t)pixy.ccc.blocks[0].m_y - (int32_t)pixy.frameHeight/2;  
  
    // aggiorna i loops
    panLoop.update(panOffset);
    tiltLoop.update(tiltOffset);
  
    // imposta pan e tilt 
    pixy.setServos(panLoop.m_command, invertServoAngle(tiltLoop.m_command));
   
#if 0 // per debugging
    sprintf(buf, "pan: %ld tilt: %ld", panLoop.m_command, tiltLoop.m_command);
    Serial.println(buf);   
#endif

  }  
  else // non ci sono oggetti trovati, torna nella posizione iniziale
  {
    panLoop.reset();
    tiltLoop.reset();
    pixy.setServos(panLoop.m_command, invertServoAngle(tiltLoop.m_command));
  }
}

void grilletto(){
  while (posServo <= 30 and Cooldown == 1){ 
    posServo = posServo +1;
    myservo.write(posServo);              
   
    }
    delay(300);
    Cooldown == 0;
    myservo.write(0);
  
}
// ho aggiunto una funzione che inverte il funzionamento del tilt per farlo funzionare corretamente
long invertServoAngle(long angle)
{
  return map(angle, 0, 1000, 1000, 0);
  
}


 
void loop() {
  while(radar() == false){
    movimento();    
    }
  if (radar() == true){
    Cooldown == 1;
    torretta();
    delay(5000);
    grilletto();
    
    }
    


}

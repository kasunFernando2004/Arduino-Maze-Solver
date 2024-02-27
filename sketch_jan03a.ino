//Code author: Kasun Fernando
//Code purpose: Implementation of the hand on wall rule for the arduino car to solve a simple maze
//pins to control the motor power
int motor1Pin=6;
int motor2Pin=11;
//pins to control the motors direction
int motor2Anti=10;
int motor2Clok =9;
int motor1Anti=7;
int motor1Clok =8;

//initialising the initial distance the car needs to remain from the wall, it is updated every time the bot turns
float remainDist=0;
//detects what type of turn has occured turn has occured 
int RightTicker=0;
int RightTickerFlag=0;
int LeftTicker=0;
int LeftTickerFlag=0;

//decides which side, left or right, the bot should maintain its distance from
float sideToFollow;
int UTurnTurnFlag=0;
int ExtremeFlag=0;
int TURNFLAG=0;   
int thingothingo =0;

//variables to store ultrasonic sensor data
float CurRightdist=0;
float CurLeftdist=0;
float CurFrontdist=0;
int moveforward;

//variables to control the three ultrasonic sensors
int FronttrigPin = 5;
int FrontechoPin = 4; 
long Frontduration;
float Frontdistance=0;

int RighttrigPin = 13;
int RightechoPin = 12;
long Rightduration;
//to determine if the distance changes suddenly, indicating that there is the absence of a wall in that direction
float Rightdistance[2]={0,0};

int LefttrigPin = 2;
int LeftechoPin = 3;
long Leftduration;
float Leftdistance[2]={0,0};
int RightTurnFlag=0;
int LeftTurnFlag=1;
int UTurnFlag=0;
void setup() {
  //setting up pins
  pinMode(motor1Pin,OUTPUT);
  pinMode(motor2Pin,OUTPUT);

  pinMode(motor2Anti,OUTPUT);
  pinMode(motor2Clok,OUTPUT);
  
  pinMode(motor1Anti,OUTPUT);
  pinMode(motor1Clok,OUTPUT);

  pinMode(FronttrigPin,OUTPUT);
  pinMode(FrontechoPin,INPUT);

  pinMode(LefttrigPin,OUTPUT);
  pinMode(LeftechoPin,INPUT);

  pinMode(RighttrigPin,OUTPUT);
  pinMode(RightechoPin,INPUT);
  
  digitalWrite(motor2Anti,HIGH);
  digitalWrite(motor2Clok,LOW);

  digitalWrite(motor1Anti,LOW);
  digitalWrite(motor1Clok,HIGH);
  Serial.begin(9600); //for debugging, used for printing distance data to the screen
  //the intial side the bot will follow is the right, below code determines the initial distance it needs to remain from the wall
  for (int i=0;i<=10;i++)
  {
      digitalWrite(RighttrigPin, LOW);
      delayMicroseconds(2);

      digitalWrite(RighttrigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(RighttrigPin, LOW);

      Rightduration = pulseIn(RightechoPin, HIGH);
      Rightdistance[0] = Rightdistance[1];
      Rightdistance[1] = Rightduration * 0.034 / 2;
      delay(100);

  }
  remainDist=Rightdistance[1];
}

void loop() 
{
  //accuring distance inputs
  digitalWrite(FronttrigPin,LOW);
    delayMicroseconds(2);
  
    digitalWrite(FronttrigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(FronttrigPin,LOW);
  
    Frontduration = pulseIn(FrontechoPin,HIGH);
    Frontdistance = Frontduration*0.034/2;
    

      digitalWrite(RighttrigPin,LOW);
      delayMicroseconds(2);
    
      digitalWrite(RighttrigPin,HIGH);
      delayMicroseconds(10);
      digitalWrite(RighttrigPin,LOW);
    
      Rightduration = pulseIn(RightechoPin,HIGH);
      Rightdistance[0]=Rightdistance[1];
      Rightdistance[1] = Rightduration*0.034/2;

    Leftdistance[0]=Leftdistance[1];
      digitalWrite(LefttrigPin,LOW);
      delayMicroseconds(2);
    
      digitalWrite(LefttrigPin,HIGH);
      delayMicroseconds(10);
      digitalWrite(LefttrigPin,LOW);
      
      Leftduration = pulseIn(LeftechoPin,HIGH);
      Leftdistance[1] = Leftduration*0.034/2;

      //determining which side the bot should follow based on the hand-on-wall-maze solving technique
      if (LeftTurnFlag == 1 && UTurnFlag==0)
      {
        sideToFollow=Rightdistance[1];
      }
      else if (RightTurnFlag==1 &&UTurnFlag==0)
      {
        sideToFollow=Leftdistance[1];
      }
      else if (UTurnFlag==1) //if a U turn occurs, the bot switches walls (because of its new orientation, left becomes right, right becomes left)
      {
        if (RightTurnFlag==1 && LeftTurnFlag==0)
        {
          sideToFollow=Rightdistance[1];
        }
        else if (RightTurnFlag==0 && LeftTurnFlag==1)
        {
          sideToFollow=Leftdistance[1];
        }
      }

       if (sideToFollow==Rightdistance[1] && ExtremeFlag==0)
      {
        if (Rightdistance[1]>30 && Rightdistance[0]>30)
        {
          ExtremeFlag=1;
          RightRightTicker=0;
          analogWrite(motor1Pin,0);
          analogWrite(motor2Pin,0);
          delay(3000);
          for (int i=0;i<=5;i++)
          {
            digitalWrite(motor2Anti,LOW);
            digitalWrite(motor2Clok,HIGH);
                
            digitalWrite(motor1Anti,HIGH);
            digitalWrite(motor1Clok,LOW);
    
            analogWrite(motor1Pin,71);
            analogWrite(motor2Pin,62);
            delay(50);
            if (i==0||i==2||i==4)
            {
              digitalWrite(motor2Anti,HIGH);
              digitalWrite(motor2Clok,LOW);
                  
              digitalWrite(motor1Anti,HIGH);
              digitalWrite(motor1Clok,LOW);
      
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(15);
            }
            else if (i==1||i==3||i==5)
            {
              digitalWrite(motor2Anti,LOW);
              digitalWrite(motor2Clok,HIGH);
                  
              digitalWrite(motor1Anti,LOW);
              digitalWrite(motor1Clok,HIGH);
      
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(15);
            }
            Leftdistance[0]=Leftdistance[1];
            digitalWrite(LefttrigPin,LOW);
            delayMicroseconds(2);
          
            digitalWrite(LefttrigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(LefttrigPin,LOW);
            
            Leftduration = pulseIn(LeftechoPin,HIGH);
            Leftdistance[1] = Leftduration*0.034/2;
          }
            remainDist=Leftdistance[1];
        }
      }
     if (sideToFollow==Leftdistance[1] && ExtremeFlag==0)
      {
        if (Leftdistance[1]>40 && Leftdistance[0]>40)
        {
          ExtremeFlag=2;
          analogWrite(motor1Pin,0);
          analogWrite(motor2Pin,0);
          delay(3000);
          for (int i=0;i<=5;i++)
          {
            digitalWrite(motor2Anti,LOW);
            digitalWrite(motor2Clok,HIGH);
                
            digitalWrite(motor1Anti,HIGH);
            digitalWrite(motor1Clok,LOW);
    
            analogWrite(motor1Pin,71);
            analogWrite(motor2Pin,62);
            delay(50);
            if (i==0||i==2||i==4)
            {
              digitalWrite(motor2Anti,HIGH);
              digitalWrite(motor2Clok,LOW);
                  
              digitalWrite(motor1Anti,HIGH);
              digitalWrite(motor1Clok,LOW);
      
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(15);
            }
            else if (i==1||i==3||i==5)
            {
              digitalWrite(motor2Anti,LOW);
              digitalWrite(motor2Clok,HIGH);
                  
              digitalWrite(motor1Anti,LOW);
              digitalWrite(motor1Clok,HIGH);
      
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(15);
            }
            Rightdistance[0]=Rightdistance[1];
            digitalWrite(RighttrigPin,LOW);
            delayMicroseconds(2);
          
            digitalWrite(RighttrigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(RighttrigPin,LOW);
            
            Rightduration = pulseIn(RightechoPin,HIGH);
            Rightdistance[1] = Rightduration*0.034/2;
          }
            remainDist=Rightdistance[1];
        }
      }
     if (ExtremeFlag==1)
     {
      sideToFollow=Leftdistance[1];
     }
     else if (ExtremeFlag==2)
     {
      sideToFollow=Rightdistance[1];
     }
     Serial.print(Leftdistance[1]);
     Serial.print("----");
     Serial.print(ExtremeFlag);
     Serial.print("----");
     Serial.print(Rightdistance[1]);
     Serial.print("-----");
     Serial.print(thingothingo);
     Serial.print('\n');
      if (Frontdistance>12)
      {
        digitalWrite(motor2Anti,LOW);
        digitalWrite(motor2Clok,HIGH);
      
        digitalWrite(motor1Anti,HIGH);
        digitalWrite(motor1Clok,LOW);
        //analogWrite(motor1Pin,ceil(exp((8+40.459)/10.903)));
        //analogWrite(motor2Pin,floor(exp((8+31.717)/9.1475)-1));
        analogWrite(motor1Pin,71);
        analogWrite(motor2Pin,62);
        delay(50);
          if (sideToFollow==Rightdistance[1])
          {
            if (sideToFollow<remainDist && RightTicker<=2)
            {
              digitalWrite(motor2Anti,LOW);
              digitalWrite(motor2Clok,HIGH);
            
              digitalWrite(motor1Anti,LOW);
              digitalWrite(motor1Clok,HIGH);
              //analogWrite(motor1Pin,ceil(exp((8+40.459)/10.903)));
              //analogWrite(motor2Pin,floor(exp((8+31.717)/9.1475)-1));
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(5);
              LeftTickerFlag=0;
              RightTickerFlag=1;
              
            }
            else if (sideToFollow>remainDist && LeftTicker<=2)
            {
              digitalWrite(motor2Anti,HIGH);
              digitalWrite(motor2Clok,LOW);
            
              digitalWrite(motor1Anti,HIGH);
              digitalWrite(motor1Clok,LOW);
              //analogWrite(motor1Pin,ceil(exp((8+40.459)/10.903)));
              //analogWrite(motor2Pin,floor(exp((8+31.717)/9.1475)-1));
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(5);
              RightTickerFlag=0;
              LeftTickerFlag=1;
            }
          }
        else if (sideToFollow==Leftdistance[1])
          {
            if (sideToFollow<remainDist && LeftTicker<=2)
            {
              digitalWrite(motor2Anti,HIGH);
              digitalWrite(motor2Clok,LOW);
            
              digitalWrite(motor1Anti,HIGH);
              digitalWrite(motor1Clok,LOW);
              //analogWrite(motor1Pin,ceil(exp((8+40.459)/10.903)));
              //analogWrite(motor2Pin,floor(exp((8+31.717)/9.1475)-1));
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(5);
              LeftTickerFlag=1;
              RightTickerFlag=0;
              
            }
            else if (sideToFollow>remainDist && RightTicker<=2)
            {
              digitalWrite(motor2Anti,LOW);
              digitalWrite(motor2Clok,HIGH);
            
              digitalWrite(motor1Anti,LOW);
              digitalWrite(motor1Clok,HIGH);
              //analogWrite(motor1Pin,ceil(exp((8+40.459)/10.903)));
              //analogWrite(motor2Pin,floor(exp((8+31.717)/9.1475)-1));
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(5);
              RightTickerFlag=1;
              LeftTickerFlag=0;
            }
          }

      }
      else if (Frontdistance<12)
      {
        analogWrite(motor1Pin,0);
        analogWrite(motor2Pin,0);
        delay(1000);
        ExtremeFlag=0;
        if (thingothingo!=0)
        {
         thingothingo++;
        }

      if (abs(Rightdistance[1]-Leftdistance[1])<20)// U TURN
      {
        LeftTicker=0;
        RightTicker=0;
        UTurnFlag=1;
        ExtremeFlag=0;
        CurRightdist = Rightdistance[1];
        CurLeftdist = Leftdistance[1];
        while (!(Frontdistance>30 && Rightdistance[1]<CurLeftdist+5 && Leftdistance[1]<CurRightdist+5))
        {
          digitalWrite(motor2Anti,LOW);
          digitalWrite(motor2Clok,HIGH);
        
          digitalWrite(motor1Anti,LOW);
          digitalWrite(motor1Clok,HIGH);
          analogWrite(motor1Pin,71);
          analogWrite(motor2Pin,62);
          delay(20);
          digitalWrite(FronttrigPin,LOW);
          delayMicroseconds(2);
            
          digitalWrite(FronttrigPin,HIGH);
          delayMicroseconds(10);
          digitalWrite(FronttrigPin,LOW);
        
          Frontduration = pulseIn(FrontechoPin,HIGH);
          Frontdistance = Frontduration*0.034/2;
          
      
            digitalWrite(RighttrigPin,LOW);
            delayMicroseconds(2);
          
            digitalWrite(RighttrigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(RighttrigPin,LOW);
          
            Rightduration = pulseIn(RightechoPin,HIGH);
            Rightdistance[0]=Rightdistance[1];
            Rightdistance[1] = Rightduration*0.034/2;
      
          Leftdistance[0]=Leftdistance[1];
            digitalWrite(LefttrigPin,LOW);
            delayMicroseconds(2);
          
            digitalWrite(LefttrigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(LefttrigPin,LOW);
            
            Leftduration = pulseIn(LeftechoPin,HIGH);
            Leftdistance[1] = Leftduration*0.034/2;
      }

     
        analogWrite(motor1Pin,0);
        analogWrite(motor2Pin,0);
      for (int i=0;i<10;i++)
      {
            digitalWrite(RighttrigPin,LOW);
            delayMicroseconds(2);
          
            digitalWrite(RighttrigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(RighttrigPin,LOW);
          
            Rightduration = pulseIn(RightechoPin,HIGH);
            Rightdistance[0]=Rightdistance[1];
            Rightdistance[1] = Rightduration*0.034/2;
      
          Leftdistance[0]=Leftdistance[1];
            digitalWrite(LefttrigPin,LOW);
            delayMicroseconds(2);
          
            digitalWrite(LefttrigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(LefttrigPin,LOW);
            
            Leftduration = pulseIn(LeftechoPin,HIGH);
            Leftdistance[1] = Leftduration*0.034/2;
            delay(100);
      }
          if (RightTurnFlag==1 && LeftTurnFlag==0)
          {
            remainDist=Rightdistance[1];
          }
          else if (RightTurnFlag==0 && LeftTurnFlag==1)
          {
            remainDist=Leftdistance[1];
          }
          UTurnTurnFlag=1;
    }
    else if (Rightdistance[1]>(Leftdistance[1]+20))//need to turn Right // (Rightdistance>(Leftdistance+15))
    {
      RightTurnFlag=1;
      UTurnFlag=0;
      LeftTurnFlag=0;
      CurFrontdist=Frontdistance;

      while (!(Frontdistance>30 && Leftdistance[1]<(CurFrontdist+5) && Rightdistance[1]>50 ))
      {
            digitalWrite(motor2Anti,HIGH);
            digitalWrite(motor2Clok,LOW);
          
            digitalWrite(motor1Anti,HIGH);
            digitalWrite(motor1Clok,LOW);
            analogWrite(motor1Pin,71);
            analogWrite(motor2Pin,62);
            delay(20);
          digitalWrite(FronttrigPin,LOW);
          delayMicroseconds(2);
            
          digitalWrite(FronttrigPin,HIGH);
          delayMicroseconds(10);
          digitalWrite(FronttrigPin,LOW);
        
          Frontduration = pulseIn(FrontechoPin,HIGH);
          Frontdistance = Frontduration*0.034/2;
          
      
            digitalWrite(RighttrigPin,LOW);
            delayMicroseconds(2);
          
            digitalWrite(RighttrigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(RighttrigPin,LOW);
          
            Rightduration = pulseIn(RightechoPin,HIGH);
            Rightdistance[0]=Rightdistance[1];
            Rightdistance[1] = Rightduration*0.034/2;
      
          Leftdistance[0]=Leftdistance[1];
            digitalWrite(LefttrigPin,LOW);
            delayMicroseconds(2);
          
            digitalWrite(LefttrigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(LefttrigPin,LOW);
            
            Leftduration = pulseIn(LeftechoPin,HIGH);
            Leftdistance[1] = Leftduration*0.034/2;
            Serial.print(Leftdistance[1]);
            Serial.print("------");
            Serial.print(Frontdistance);
            Serial.print("------");
            Serial.print(Rightdistance[1]);
            Serial.print('\n');
      }
            analogWrite(motor1Pin,0);
            analogWrite(motor2Pin,0);
      for (int i=0;i<10;i++)
      {
        Leftdistance[0]=Leftdistance[1];
        digitalWrite(LefttrigPin,LOW);
        delayMicroseconds(2);
      
        digitalWrite(LefttrigPin,HIGH);
        delayMicroseconds(10);
        digitalWrite(LefttrigPin,LOW);
        
        Leftduration = pulseIn(LeftechoPin,HIGH);
        Leftdistance[1] = Leftduration*0.034/2;
        delay(100);
      }
      remainDist=Leftdistance[1];
    }
    else if (Leftdistance[1]>(Rightdistance[1]+20))//need to turn LEft (Leftdistance+15>Rightdistance)
    {
      RightTurnFlag=0;
      LeftTurnFlag=1;
      UTurnFlag=0;
      CurFrontdist=Frontdistance;
      while (!(Frontdistance>30 && Leftdistance[1]>30 && Rightdistance[1]<(CurFrontdist+5)))
      {
            digitalWrite(motor2Anti,HIGH);
            digitalWrite(motor2Clok,LOW);
          
            digitalWrite(motor1Anti,HIGH);
            digitalWrite(motor1Clok,LOW);
            analogWrite(motor1Pin,71);
            analogWrite(motor2Pin,62);
            delay(20);

            
          digitalWrite(FronttrigPin,LOW);
          delayMicroseconds(2);
            
          digitalWrite(FronttrigPin,HIGH);
          delayMicroseconds(10);
          digitalWrite(FronttrigPin,LOW);
        
          Frontduration = pulseIn(FrontechoPin,HIGH);
          Frontdistance = Frontduration*0.034/2;
          
      
            digitalWrite(RighttrigPin,LOW);
            delayMicroseconds(2);
          
            digitalWrite(RighttrigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(RighttrigPin,LOW);
          
            Rightduration = pulseIn(RightechoPin,HIGH);
            Rightdistance[0]=Rightdistance[1];
            Rightdistance[1] = Rightduration*0.034/2;
      
          Leftdistance[0]=Leftdistance[1];
            digitalWrite(LefttrigPin,LOW);
            delayMicroseconds(2);
          
            digitalWrite(LefttrigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(LefttrigPin,LOW);
            
            Leftduration = pulseIn(LeftechoPin,HIGH);
            Leftdistance[1] = Leftduration*0.034/2;
      }
      analogWrite(motor1Pin,0);
      analogWrite(motor2Pin,0);
      for (int i=0;i<10;i++)
      {
        digitalWrite(RighttrigPin,LOW);
        delayMicroseconds(2);
      
        digitalWrite(RighttrigPin,HIGH);
        delayMicroseconds(10);
        digitalWrite(RighttrigPin,LOW);
      
        Rightduration = pulseIn(RightechoPin,HIGH);
        Rightdistance[0]=Rightdistance[1];
        Rightdistance[1] = Rightduration*0.034/2;
        delay(100);
      }
      remainDist=Rightdistance[1];
    }
  }

   if (UTurnTurnFlag==1 && Rightdistance[1]>40 && Rightdistance[0]>40 && (thingothingo==0 || thingothingo==3))
  {
      analogWrite(motor1Pin,0);
      analogWrite(motor2Pin,0);
      delay(1000);
      if (thingothingo==3)
      {
         moveforward  = 50;
      }
      else
      {
         moveforward =36;
      }
     for (int i=0;i<moveforward;i++)
          {
            digitalWrite(FronttrigPin,LOW);
            delayMicroseconds(2);
          
            digitalWrite(FronttrigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(FronttrigPin,LOW);
          
            Frontduration = pulseIn(FrontechoPin,HIGH);
            Frontdistance = Frontduration*0.034/2;
            
        
              digitalWrite(RighttrigPin,LOW);
              delayMicroseconds(2);
            
              digitalWrite(RighttrigPin,HIGH);
              delayMicroseconds(10);
              digitalWrite(RighttrigPin,LOW);
            
              Rightduration = pulseIn(RightechoPin,HIGH);
              Rightdistance[0]=Rightdistance[1];
              Rightdistance[1] = Rightduration*0.034/2;
        
            Leftdistance[0]=Leftdistance[1];
              digitalWrite(LefttrigPin,LOW);
              delayMicroseconds(2);
            
              digitalWrite(LefttrigPin,HIGH);
              delayMicroseconds(10);
              digitalWrite(LefttrigPin,LOW);
              
              Leftduration = pulseIn(LeftechoPin,HIGH);
              Leftdistance[1] = Leftduration*0.034/2;

              if (i==0)
              {
                remainDist=Leftdistance[1];
              }
              sideToFollow=Leftdistance[1];
            digitalWrite(motor2Anti,LOW);
            digitalWrite(motor2Clok,HIGH);
            
              digitalWrite(motor1Anti,HIGH);
              digitalWrite(motor1Clok,LOW);
    
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(50);
                if (sideToFollow==Rightdistance[1])
                {
                  if (sideToFollow<remainDist && RightTicker<=2)
                  {
                    digitalWrite(motor2Anti,LOW);
                    digitalWrite(motor2Clok,HIGH);
                  
                    digitalWrite(motor1Anti,LOW);
                    digitalWrite(motor1Clok,HIGH);
                    //analogWrite(motor1Pin,ceil(exp((8+40.459)/10.903)));
                    //analogWrite(motor2Pin,floor(exp((8+31.717)/9.1475)-1));
                    analogWrite(motor1Pin,71);
                    analogWrite(motor2Pin,62);
                    delay(5);
                    LeftTickerFlag=0;
                    RightTickerFlag=1;
                    
                  }
                  else if (sideToFollow>remainDist && LeftTicker<=2)
                  {
                    digitalWrite(motor2Anti,HIGH);
                    digitalWrite(motor2Clok,LOW);
                  
                    digitalWrite(motor1Anti,HIGH);
                    digitalWrite(motor1Clok,LOW);
                    //analogWrite(motor1Pin,ceil(exp((8+40.459)/10.903)));
                    //analogWrite(motor2Pin,floor(exp((8+31.717)/9.1475)-1));
                    analogWrite(motor1Pin,71);
                    analogWrite(motor2Pin,62);
                    delay(5);
                    RightTickerFlag=0;
                    LeftTickerFlag=1;
                  }
                }
              else if (sideToFollow==Leftdistance[1])
                {
                  if (sideToFollow<remainDist && LeftTicker<=2)
                  {
                    digitalWrite(motor2Anti,HIGH);
                    digitalWrite(motor2Clok,LOW);
                  
                    digitalWrite(motor1Anti,HIGH);
                    digitalWrite(motor1Clok,LOW);
                    //analogWrite(motor1Pin,ceil(exp((8+40.459)/10.903)));
                    //analogWrite(motor2Pin,floor(exp((8+31.717)/9.1475)-1));
                    analogWrite(motor1Pin,71);
                    analogWrite(motor2Pin,62);
                    delay(5);
                    LeftTickerFlag=1;
                    RightTickerFlag=0;
                    
                  }
                  else if (sideToFollow>remainDist && RightTicker<=2)
                  {
                    digitalWrite(motor2Anti,LOW);
                    digitalWrite(motor2Clok,HIGH);
                  
                    digitalWrite(motor1Anti,LOW);
                    digitalWrite(motor1Clok,HIGH);
                    //analogWrite(motor1Pin,ceil(exp((8+40.459)/10.903)));
                    //analogWrite(motor2Pin,floor(exp((8+31.717)/9.1475)-1));
                    analogWrite(motor1Pin,71);
                    analogWrite(motor2Pin,62);
                    delay(5);
                    RightTickerFlag=1;
                    LeftTickerFlag=0;
                  }
                }
          }
      if (Frontdistance>20)
      {
       
       UTurnTurnFlag=0;
        analogWrite(motor1Pin,0);
        analogWrite(motor2Pin,0);
        delay(1000);
         while (!(Frontdistance>40 && Leftdistance[1]>30 && Leftdistance[0]>30 && Rightdistance[1]>30 && Rightdistance[0]>30)) 
        {
              digitalWrite(motor2Anti,HIGH);
              digitalWrite(motor2Clok,LOW);
            
              digitalWrite(motor1Anti,HIGH);
              digitalWrite(motor1Clok,LOW);
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(20);
            digitalWrite(FronttrigPin,LOW);
            delayMicroseconds(2);
              
            digitalWrite(FronttrigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(FronttrigPin,LOW);
          
            Frontduration = pulseIn(FrontechoPin,HIGH);
            Frontdistance = Frontduration*0.034/2;
            
        
              digitalWrite(RighttrigPin,LOW);
              delayMicroseconds(2);
            
              digitalWrite(RighttrigPin,HIGH);
              delayMicroseconds(10);
              digitalWrite(RighttrigPin,LOW);
            
              Rightduration = pulseIn(RightechoPin,HIGH);
              Rightdistance[0]=Rightdistance[1];
              Rightdistance[1] = Rightduration*0.034/2;
        
            Leftdistance[0]=Leftdistance[1];
              digitalWrite(LefttrigPin,LOW);
              delayMicroseconds(2);
            
              digitalWrite(LefttrigPin,HIGH);
              delayMicroseconds(10);
              digitalWrite(LefttrigPin,LOW);
              
              Leftduration = pulseIn(LeftechoPin,HIGH);
              Leftdistance[1] = Leftduration*0.034/2;
        }
              digitalWrite(motor2Anti,HIGH);
              digitalWrite(motor2Clok,LOW);
            
              digitalWrite(motor1Anti,HIGH);
              digitalWrite(motor1Clok,LOW);
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(300);
        analogWrite(motor1Pin,0);
        analogWrite(motor2Pin,0);
        delay(1000);
        if (thingothingo==0)
        {
          moveforward =20;
        }
        else if (thingothingo==3)
        {
          moveforward=24;
        }
         for (int i=0;i<moveforward;i++)
            {
              digitalWrite(motor2Anti,LOW);
              digitalWrite(motor2Clok,HIGH);
                  
              digitalWrite(motor1Anti,HIGH);
              digitalWrite(motor1Clok,LOW);
      
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(100);
              if (i%2==0)
              {
                digitalWrite(motor2Anti,HIGH);
                digitalWrite(motor2Clok,LOW);
                    
                digitalWrite(motor1Anti,HIGH);
                digitalWrite(motor1Clok,LOW);
        
                analogWrite(motor1Pin,71);
                analogWrite(motor2Pin,62);
              
                  delay(5);
             
              }
              else if (i%2!=0)
              {
                digitalWrite(motor2Anti,LOW);
                digitalWrite(motor2Clok,HIGH);
                    
                digitalWrite(motor1Anti,LOW);
                digitalWrite(motor1Clok,HIGH);
        
                analogWrite(motor1Pin,71);
                analogWrite(motor2Pin,62);
                  delay(5);

              }
                   Rightdistance[0]=Rightdistance[1];
                  digitalWrite(RighttrigPin,LOW);
                  delayMicroseconds(2);
                
                  digitalWrite(RighttrigPin,HIGH);
                  delayMicroseconds(10);
                  digitalWrite(RighttrigPin,LOW);
                  
                  Rightduration = pulseIn(RightechoPin,HIGH);
                  Rightdistance[1] = Rightduration*0.034/2;
            }
       
        analogWrite(motor1Pin,0);
        analogWrite(motor2Pin,0); 



                digitalWrite(motor2Anti,HIGH);
              digitalWrite(motor2Clok,LOW);
            
              digitalWrite(motor1Anti,HIGH);
              digitalWrite(motor1Clok,LOW);
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(300);
                              digitalWrite(motor2Anti,LOW);
              digitalWrite(motor2Clok,HIGH);
            
              digitalWrite(motor1Anti,HIGH);
              digitalWrite(motor1Clok,LOW);
              analogWrite(motor1Pin,71);
              analogWrite(motor2Pin,62);
              delay(300);
                      analogWrite(motor1Pin,0);
        analogWrite(motor2Pin,0); 

        if (thingothingo!=3)
        {
          for (int i=0;i<9;i++)
          {
             Leftdistance[0]=Leftdistance[1];
                    digitalWrite(LefttrigPin,LOW);
                    delayMicroseconds(2);
                  
                    digitalWrite(LefttrigPin,HIGH);
                    delayMicroseconds(10);
                    digitalWrite(LefttrigPin,LOW);
                    
                    Leftduration = pulseIn(LeftechoPin,HIGH);
                    Leftdistance[1] = Leftduration*0.034/2;
                    delay(100);
          }
            UTurnFlag=0;
            remainDist=Leftdistance[1];
            RightTurnFlag=1;
            LeftTurnFlag=0;
        }
        else if (thingothingo==3)
        {
          for (int i=0;i<10;i++)
          {
             Rightdistance[0]=Rightdistance[1];
            digitalWrite(RighttrigPin,LOW);
            delayMicroseconds(2);
                  
            digitalWrite(RighttrigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(RighttrigPin,LOW);
            
            Rightduration = pulseIn(RightechoPin,HIGH);
            Rightdistance[1] = Rightduration*0.034/2;
            delay(100);
          }
        UTurnFlag=0;
        remainDist=Rightdistance[1];
        RightTurnFlag=0;
        LeftTurnFlag=1;
        }
      }
      else if (Frontdistance<=20)
      {
        thingothingo=1;
      }
      
      
  }
  else if (UTurnTurnFlag==1 && Leftdistance[1]>30 && Leftdistance[0]>30 && (thingothingo==0) || (thingothingo==3))
  {

     analogWrite(motor1Pin,0);
      analogWrite(motor2Pin,0);
      delay(1000);
      if (thingothingo==3)
      {
        moveforward =28;
      }
      else
      {
        moveforward=10;
      }
       while (!(Frontdistance<18))
      {
        digitalWrite(FronttrigPin,LOW);
        delayMicroseconds(2);
      
        digitalWrite(FronttrigPin,HIGH);
        delayMicroseconds(10);
        digitalWrite(FronttrigPin,LOW);
      
        Frontduration = pulseIn(FrontechoPin,HIGH);
        Frontdistance = Frontduration*0.034/2;
        
    
          digitalWrite(RighttrigPin,LOW);
          delayMicroseconds(2);
        
          digitalWrite(RighttrigPin,HIGH);
          delayMicroseconds(10);
          digitalWrite(RighttrigPin,LOW);
        
          Rightduration = pulseIn(RightechoPin,HIGH);
          Rightdistance[0]=Rightdistance[1];
          Rightdistance[1] = Rightduration*0.034/2;
    
        Leftdistance[0]=Leftdistance[1];
          digitalWrite(LefttrigPin,LOW);
          delayMicroseconds(2);
        
          digitalWrite(LefttrigPin,HIGH);
          delayMicroseconds(10);
          digitalWrite(LefttrigPin,LOW);
          
          Leftduration = pulseIn(LeftechoPin,HIGH);
          Leftdistance[1] = Leftduration*0.034/2;
          sideToFollow=Rightdistance[1];
        digitalWrite(motor2Anti,LOW);
        digitalWrite(motor2Clok,HIGH);
        
          digitalWrite(motor1Anti,HIGH);
          digitalWrite(motor1Clok,LOW);
  
          analogWrite(motor1Pin,71);
          analogWrite(motor2Pin,62);
          delay(100);
            if (sideToFollow==Rightdistance[1])
            {
              if (sideToFollow<remainDist && RightTicker<=2)
              {
                digitalWrite(motor2Anti,LOW);
                digitalWrite(motor2Clok,HIGH);
              
                digitalWrite(motor1Anti,LOW);
                digitalWrite(motor1Clok,HIGH);
                //analogWrite(motor1Pin,ceil(exp((8+40.459)/10.903)));
                //analogWrite(motor2Pin,floor(exp((8+31.717)/9.1475)-1));
                analogWrite(motor1Pin,71);
                analogWrite(motor2Pin,62);
                delay(5);
                LeftTickerFlag=0;
                RightTickerFlag=1;
                
              }
              else if (sideToFollow>remainDist && LeftTicker<=2)
              {
                digitalWrite(motor2Anti,HIGH);
                digitalWrite(motor2Clok,LOW);
              
                digitalWrite(motor1Anti,HIGH);
                digitalWrite(motor1Clok,LOW);
                //analogWrite(motor1Pin,ceil(exp((8+40.459)/10.903)));
                //analogWrite(motor2Pin,floor(exp((8+31.717)/9.1475)-1));
                analogWrite(motor1Pin,71);
                analogWrite(motor2Pin,62);
                delay(5);
                RightTickerFlag=0;
                LeftTickerFlag=1;
              }
            }
          else if (sideToFollow==Leftdistance[1])
            {
              if (sideToFollow<remainDist && LeftTicker<=2)
              {
                digitalWrite(motor2Anti,HIGH);
                digitalWrite(motor2Clok,LOW);
              
                digitalWrite(motor1Anti,HIGH);
                digitalWrite(motor1Clok,LOW);
                //analogWrite(motor1Pin,ceil(exp((8+40.459)/10.903)));
                //analogWrite(motor2Pin,floor(exp((8+31.717)/9.1475)-1));
                analogWrite(motor1Pin,71);
                analogWrite(motor2Pin,62);
                delay(5);
                LeftTickerFlag=1;
                RightTickerFlag=0;
                
              }
              else if (sideToFollow>remainDist && RightTicker<=2)
              {
                digitalWrite(motor2Anti,LOW);
                digitalWrite(motor2Clok,HIGH);
              
                digitalWrite(motor1Anti,LOW);
                digitalWrite(motor1Clok,HIGH);
                //analogWrite(motor1Pin,ceil(exp((8+40.459)/10.903)));
                //analogWrite(motor2Pin,floor(exp((8+31.717)/9.1475)-1));
                analogWrite(motor1Pin,71);
                analogWrite(motor2Pin,62);
                delay(5);
                RightTickerFlag=1;
                LeftTickerFlag=0;
              }
            }
      }
  if (Frontdistance>20)
  {
    UTurnTurnFlag=0;
    analogWrite(motor1Pin,0);
    analogWrite(motor2Pin,0);
    delay(1000);
     while (!(Frontdistance>40 && Leftdistance[1]>30 && Leftdistance[0]>30 && Rightdistance[1]>30 && Rightdistance[0]>30))
    {
          digitalWrite(motor2Anti,LOW);
          digitalWrite(motor2Clok,HIGH);
        
          digitalWrite(motor1Anti,LOW);
          digitalWrite(motor1Clok,HIGH);
          analogWrite(motor1Pin,71);
          analogWrite(motor2Pin,62);
          delay(20);
        digitalWrite(FronttrigPin,LOW);
        delayMicroseconds(2);
          
        digitalWrite(FronttrigPin,HIGH);
        delayMicroseconds(10);
        digitalWrite(FronttrigPin,LOW);
      
        Frontduration = pulseIn(FrontechoPin,HIGH);
        Frontdistance = Frontduration*0.034/2;
        
    
          digitalWrite(RighttrigPin,LOW);
          delayMicroseconds(2);
        
          digitalWrite(RighttrigPin,HIGH);
          delayMicroseconds(10);
          digitalWrite(RighttrigPin,LOW);
        
          Rightduration = pulseIn(RightechoPin,HIGH);
          Rightdistance[0]=Rightdistance[1];
          Rightdistance[1] = Rightduration*0.034/2;
    
        Leftdistance[0]=Leftdistance[1];
          digitalWrite(LefttrigPin,LOW);
          delayMicroseconds(2);
        
          digitalWrite(LefttrigPin,HIGH);
          delayMicroseconds(10);
          digitalWrite(LefttrigPin,LOW);
          
          Leftduration = pulseIn(LeftechoPin,HIGH);
          Leftdistance[1] = Leftduration*0.034/2;
    }
          digitalWrite(motor2Anti,LOW);
          digitalWrite(motor2Clok,HIGH);
        
          digitalWrite(motor1Anti,LOW);
          digitalWrite(motor1Clok,HIGH);
          analogWrite(motor1Pin,71);
          analogWrite(motor2Pin,62);
          delay(300);
    analogWrite(motor1Pin,0);
    analogWrite(motor2Pin,0);
    delay(1000);
    
     for (int i=0;i<15;i++)
        {
          digitalWrite(motor2Anti,LOW);
          digitalWrite(motor2Clok,HIGH);
              
          digitalWrite(motor1Anti,HIGH);
          digitalWrite(motor1Clok,LOW);
  
          analogWrite(motor1Pin,71);
          analogWrite(motor2Pin,62);
          delay(100);
          if (i==0||i==2||i==4||i==6||i==8||i==10||i==12||i==14)
          {
            digitalWrite(motor2Anti,HIGH);
            digitalWrite(motor2Clok,LOW);
                
            digitalWrite(motor1Anti,HIGH);
            digitalWrite(motor1Clok,LOW);
    
            analogWrite(motor1Pin,71);
            analogWrite(motor2Pin,62);
            delay(15);
          }
          else if (i==1||i==3||i==5||i==7||i==9||i==11||i==13)
          {
            digitalWrite(motor2Anti,LOW);
            digitalWrite(motor2Clok,HIGH);
                
            digitalWrite(motor1Anti,LOW);
            digitalWrite(motor1Clok,HIGH);
    
            analogWrite(motor1Pin,71);
            analogWrite(motor2Pin,62);
            delay(15);
          }
        }
   
    analogWrite(motor1Pin,0);
    analogWrite(motor2Pin,0); 
    for (int i=0;i<9;i++)
    {
       Rightdistance[0]=Rightdistance[1];
              digitalWrite(RighttrigPin,LOW);
              delayMicroseconds(2);
            
              digitalWrite(RighttrigPin,HIGH);
              delayMicroseconds(10);
              digitalWrite(RighttrigPin,LOW);
              
              Rightduration = pulseIn(RightechoPin,HIGH);
              Rightdistance[1] = Rightduration*0.034/2;
              delay(100);
    }
    UTurnFlag=0;
    remainDist=Rightdistance[1];
    RightTurnFlag=0;
    LeftTurnFlag=1;
  }
  else if (Frontdistance<=20)
  {
    thingothingo=1;
  }
  

  }

  
    if (RightTickerFlag==1)
    {
      RightTicker+=1;
    }
    else if (RightTickerFlag==0)
    {
      RightTicker=0;
    }
    else if (LeftTickerFlag==1)
    {
      LeftTicker+=1;
    }
    else if (LeftTickerFlag==0)
    {
      LeftTicker=0;
    }


//102
   //62
   
}

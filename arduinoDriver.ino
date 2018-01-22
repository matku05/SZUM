//
// Limit Switches
//
#define X_MIN_PIN           3
#ifndef X_MAX_PIN
  #define X_MAX_PIN         2
#endif
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19


//
// Steppers
//
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_CS_PIN           53

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_CS_PIN           49

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_CS_PIN           40

/************ custom variables *****************/

int buttonX;
int buttonY;
int buttonZ;

char buforstring[30];
int bufori = 0;
char bufor = ' ';
char axis = ' ';

char dir = ' ';
char dirX = ' ';
char dirY = ' ';
char dirZ = ' ';

long numberOfSteps = 0;
long numberOfStepsX = 30000;
long numberOfStepsY = 30000;
long numberOfStepsZ = 30000;

bool newCommand = 0;
bool calibrated = 0;

/************ setup *****************/

void setup() {
  // put your setup code here, to run once:
  pinMode(X_MAX_PIN,INPUT);
  digitalWrite(X_MAX_PIN,HIGH);
  pinMode(Y_MAX_PIN,INPUT);
  digitalWrite(Y_MAX_PIN,HIGH);
  pinMode(Z_MAX_PIN,INPUT);
  digitalWrite(Z_MAX_PIN,HIGH);

  pinMode(X_STEP_PIN,OUTPUT);
  digitalWrite(X_STEP_PIN,LOW);
  pinMode(X_DIR_PIN,OUTPUT);
  digitalWrite(X_DIR_PIN,LOW);
  pinMode(X_ENABLE_PIN,OUTPUT);
  digitalWrite(X_ENABLE_PIN,LOW);
  pinMode(X_CS_PIN,OUTPUT);
  digitalWrite(X_CS_PIN,HIGH);

  pinMode(Y_STEP_PIN,OUTPUT);
  digitalWrite(Y_STEP_PIN,LOW);
  pinMode(Y_DIR_PIN,OUTPUT);
  digitalWrite(Y_DIR_PIN,LOW);
  pinMode(Y_ENABLE_PIN,OUTPUT);
  digitalWrite(Y_ENABLE_PIN,LOW);
  pinMode(Y_CS_PIN,OUTPUT);
  digitalWrite(Y_CS_PIN,HIGH);

  pinMode(Z_STEP_PIN,OUTPUT);
  digitalWrite(Z_STEP_PIN,LOW);
  pinMode(Z_DIR_PIN,OUTPUT);
  digitalWrite(Z_DIR_PIN,LOW);
  pinMode(Z_ENABLE_PIN,OUTPUT);
  digitalWrite(Z_ENABLE_PIN,LOW);
  pinMode(Z_CS_PIN,OUTPUT);
  digitalWrite(Z_CS_PIN,HIGH);

  Serial.begin(9600);
  bufori = 0;
}

/************ main loop ****************************************************************/

void loop() {
  // put your main code here, to run repeatedly:
  
  // odczyt krancowek
  buttonX = digitalRead(X_MAX_PIN);
  buttonY = digitalRead(Y_MAX_PIN);
  buttonZ = digitalRead(Z_MAX_PIN);

  /********** odczyt komendy *****************/

  while( Serial.available() )
  {
    bufor = Serial.read();
    if( bufor!='\n' )
    {
        buforstring[ bufori ] = bufor;
        bufori++;      
    }
    else
    {
      //Serial.print(buforstring);

      sscanf( buforstring, "%c%c%ld\n",&axis,&dir,&numberOfSteps );

      for(int k=0;k<30;k++)
      {
        buforstring[k] = '\0';  
      }

/*      Serial.print('\n');
      Serial.println(axis);
      Serial.println(dir);
      Serial.println(numberOfSteps);
*/
      if (axis == 'x')
        {
          numberOfStepsX = numberOfSteps;
          dirX = dir;
        }
        
      else if (axis == 'y')
        {
          numberOfStepsY = numberOfSteps;
          dirY = dir;
        }
      else if (axis == 'z')
        {
          numberOfStepsZ = numberOfSteps;
          dirZ = dir;
        }
      
      bufori = 0;
      newCommand = 1;
    }
  }

  /************ interpretacja ***************/
  
    
      if (numberOfStepsX >0)
      {
        numberOfStepsX--;
        
        // change direction
        if(dirX=='d')
          digitalWrite(X_DIR_PIN,HIGH);
        else 
          // if dir is not 'd', set it to up
          digitalWrite(X_DIR_PIN,LOW);
        
        if(!buttonX)
        {
          // move only if krancowka is not pressed
          digitalWrite(X_STEP_PIN,HIGH);
        }
        else
        {
          // go down until krancowka is no longer pressed
          digitalWrite(X_DIR_PIN,HIGH);
          digitalWrite(X_STEP_PIN,HIGH);

          // go down another 100 steps after krancowka is released
          if (!calibrated)
          {
            numberOfStepsX = 12000;
          }
          else
          {
            numberOfStepsX = 2000;
          }

          dirX = 'd';
        }
        
      }
    
    if (numberOfStepsY >0)
      {
        numberOfStepsY--;
        
        // change dir
        if(dirY=='d')
          digitalWrite(Y_DIR_PIN,HIGH);
        else 
          digitalWrite(Y_DIR_PIN,LOW);
    
        // move only if krancowka is not pressed
        if(!buttonY)
        {
          digitalWrite(Y_STEP_PIN,HIGH);
        }
        else
        {
          digitalWrite(Y_DIR_PIN,HIGH);
          digitalWrite(Y_STEP_PIN,HIGH);

          if (!calibrated)
          {
            numberOfStepsY = 12000;
          }
          else
          {
            numberOfStepsY = 2000;
          }
          
          dirY = 'd';
        }
        
      }
  
    
    if (numberOfStepsZ >0)
      {
        numberOfStepsZ--;
        
        // change dir
        if(dirZ=='d')
          digitalWrite(Z_DIR_PIN,HIGH);
        else 
          digitalWrite(Z_DIR_PIN,LOW);
    
        // move only if krancowka is not pressed
        if(!buttonZ)
        {
          digitalWrite(Z_STEP_PIN,HIGH);
        }
        else
        {
          digitalWrite(Z_DIR_PIN,HIGH);
          digitalWrite(Z_STEP_PIN,HIGH);

          if (!calibrated)
          {
            numberOfStepsZ = 12000;
          }
          else
          {
            numberOfStepsZ = 2000;
          }
          
          dirZ = 'd';
        }
        
      } 

      delayMicroseconds(300);

      digitalWrite(X_STEP_PIN,LOW);
      digitalWrite(Y_STEP_PIN,LOW);
      digitalWrite(Z_STEP_PIN,LOW);
    
      delayMicroseconds(300);

      if (numberOfStepsX == 0 && numberOfStepsY == 0 && numberOfStepsZ == 0 && calibrated == 0)
      {
        calibrated = 1;
        Serial.println("OK");
      }

      if (numberOfStepsX == 0 && numberOfStepsY == 0 && numberOfStepsZ == 0 && newCommand == 1)
      {
        Serial.println("OK");
        newCommand = 0;
      }

}




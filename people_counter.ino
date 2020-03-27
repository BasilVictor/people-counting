#define ldr_1 A0
#define ldr_2 A1
#define ldr_light A2

#define trigPinA 10
#define echoPinA 9
#define trigPinB 12
#define echoPinB 11

#define relay 13

const int LDR_Min = 500;
const int Doorway = 16;

int lightLdr = 0;
int lightStatus = 0;
int A = 0;
int B = 0;
int people = 0;

bool LDR1 = LOW;
bool LDR2 = LOW;
bool flag = false;

long dura_a;
int a;
long dura_b;
int b;
int pDM = 1;
int p = 0;

void setup() {
  pinMode(ldr_light, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(trigPinA, OUTPUT);
  pinMode(echoPinA, INPUT);
  pinMode(trigPinB, OUTPUT);
  pinMode(echoPinB, INPUT);
  Serial.begin(9600);
  delay(2000);
}

void lightChange()
{
  if(people>0 && lightStatus==0 && (digitalRead(relay) == 0))
  {
    digitalWrite(relay, HIGH);
    lightStatus = 1;
  }
  else if(people>0 && lightStatus==0 && (digitalRead(relay) == 1))
  {
    digitalWrite(relay, LOW);
    lightStatus = 1;
  }
  else if(people<=0 && lightStatus==1 && (digitalRead(relay) == 1))
  {
    digitalWrite(relay, LOW);
    lightStatus = 0;
    delay(500);
    //Serial.println("zxc");
  }
  else if(people<=0 && lightStatus==1 && (digitalRead(relay) == 0))
  {
    digitalWrite(relay, HIGH);
    lightStatus = 0;
    delay(500);
    //Serial.println("asd");
  }  
  if(people<0)
  {
    people = 0;
  }
}

int peopleDistanceMeasurement()
{
  pDM = 1;
  digitalWrite(trigPinA, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPinA, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinA, LOW);

  dura_a = pulseIn(echoPinA, HIGH);

  digitalWrite(trigPinB, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinB, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinB, LOW);
  dura_b = pulseIn(echoPinB, HIGH);
  
  a = dura_a*0.034/2;
  b = dura_b*0.034/2;

  Serial.print("A: ");
  Serial.println(a);
  Serial.print("B: ");
  Serial.println(b);

  if((a+b)>(Doorway-(0.2*Doorway)) && (a+b)<(Doorway+(0.2*Doorway)))
  {
    pDM = 1;
  }
  else if((a+b)>Doorway)
  {
    
  }
  else
  {
    pDM = 2;
  }  
  Serial.println("pDM");
  Serial.println(pDM);
  return pDM;
}

void loop() {
  lightLdr = analogRead(ldr_light);
  if(lightLdr>LDR_Min)
  {
    //Light On
    lightStatus = 1;
  }
  else
  {
    lightStatus = 0;
  }

  if(analogRead(ldr_1)>LDR_Min && analogRead(ldr_2)<LDR_Min && LDR1==LOW && LDR2==LOW)
  {
    LDR1 = HIGH;
    Serial.println("A");
  }
  if(analogRead(ldr_1)<LDR_Min && analogRead(ldr_2)>LDR_Min && LDR1==HIGH && LDR2==LOW) 
  {
    p = peopleDistanceMeasurement();
    while(analogRead(ldr_2)>LDR_Min)
    {
    }
    Serial.println("People In");
    LDR1 = LOW;
    LDR2 = LOW;
    people += p;
    lightChange();
    Serial.print("Current num: ");
    Serial.println(people);
  }
  if(analogRead(ldr_1)<LDR_Min && analogRead(ldr_2)>LDR_Min && LDR1==LOW && LDR2==LOW)
  {
    LDR2 = HIGH;
    Serial.println("B");
  }
  if(analogRead(ldr_1)>LDR_Min && analogRead(ldr_2)<LDR_Min && LDR1==LOW && LDR2==HIGH) 
  {
    p = peopleDistanceMeasurement();
    while(analogRead(ldr_1)>LDR_Min)
    {
    }
    Serial.println("People Out");
    LDR1 = LOW;
    LDR2 = LOW;
    if(people!=0)
    {
      people -= p;
    }
    lightChange();
    Serial.print("Current num: ");
    Serial.println(people);
  }
}

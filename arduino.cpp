// C++ code
//
float r = 0,v=0;
void setup()
{
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop()
{
  
  r = analogRead(A0);
  v = r / 204.8;
  delay(10);
  Serial.println(v);
}

#define vmA 13
#define amA 12
#define vdA 11
#define vmpdA 10
#define vdpdA 9
#define vmB 8
#define amB 7
#define vdB 6
#define vmpdB 5
#define vdpdB 4

void setup()
{
  pinMode(vmA, OUTPUT);
  pinMode(amA, OUTPUT);
  pinMode(vdA, OUTPUT);
  pinMode(vmpdA, OUTPUT);
  pinMode(vdpdA, OUTPUT);
  pinMode(vmB, OUTPUT);
  pinMode(amB, OUTPUT);
  pinMode(vdB, OUTPUT); 
  pinMode(vmpdB, OUTPUT); 
  pinMode(vdpdB, OUTPUT);
}

void loop()
{
  leds(1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 10);
  leds(1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 5);
  leds(0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 10);
  leds(0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 5);
}

void leds (bool e1, bool e2, bool e3, bool e4, bool e5, 
           bool e6, bool e7, bool e8, bool e9, bool e10, 
           int tempo)
  
{ 
  digitalWrite(vmA, e1); 
  digitalWrite(amA, e2); 
  digitalWrite(vdA, e3); 
  digitalWrite(vmpdA, e4); 
  digitalWrite(vdpdA, e5); 
  digitalWrite(vmB, e6); 
  digitalWrite(amB, e7); 
  digitalWrite(vdB, e8); 
  digitalWrite(vmpdB, e9); 
  digitalWrite(vdpdB, e10); 
  delay(tempo * 1000);
}

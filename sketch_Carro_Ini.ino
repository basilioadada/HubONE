int PinoVelocidade12 = 3; //Ligado ao pino 1 do L293D 
int PinoVelocidade34 = 10; //Ligado ao pino 1 do L293D 
int Entrada1 = 2; //Ligado ao pino 2 do L293D  
int Entrada2 = 7; //Ligado ao pino 7 do L293D 
int Entrada3 = 5; //Ligado ao pino 2 do L293D  
int Entrada4 = 6; //Ligado ao pino 7 do L293D 
   
void setup()  
{  
  //Define os pinos como saida  
  pinMode(PinoVelocidade12, OUTPUT);  
  pinMode(PinoVelocidade34, OUTPUT);  
  pinMode(Entrada1, OUTPUT);  
  pinMode(Entrada2, OUTPUT);
  pinMode(Entrada3, OUTPUT);  
  pinMode(Entrada4, OUTPUT);  
}  
   
void loop()  
{  
  //Define a velocidade de rotacao  
  int velocidade = 500;  
  analogWrite(PinoVelocidade12, velocidade);   

  //Aciona o motor  
  digitalWrite(Entrada1, LOW);  
  digitalWrite(Entrada2, HIGH);  
  delay(3000);  

  //Chama a rotina de parada do motor  
  para_motor();  

  //Aciona o motor no sentido inverso
  analogWrite(PinoVelocidade34, velocidade);
  digitalWrite(Entrada3, HIGH);  
  digitalWrite(Entrada4, LOW);  
  delay(3000);  
  para_motor();
  
}  
    
void para_motor()  
{  
  digitalWrite(Entrada1, LOW);  
  digitalWrite(Entrada2, LOW);
  digitalWrite(Entrada3, LOW);  
  digitalWrite(Entrada4, LOW);
  delay(3000);  
}

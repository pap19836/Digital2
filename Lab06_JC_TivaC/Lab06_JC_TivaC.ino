  int J0 = 0;
  int J1 = 0;
  int J0AR = 0;
  int J1AR = 0;
  int push0 = 31;
  int push1 = 17;
  int phase = 0;
  int winner;
void setup() {
  // put your setup code here, to run once:
  pinMode(push0, INPUT_PULLUP);
  pinMode(push1, INPUT_PULLUP);
  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(18, OUTPUT);

  pinMode(30, OUTPUT);  //RED LED
  pinMode(39, OUTPUT);  //GREEN LED
  pinMode(40, OUTPUT);  //BLUE LED
}

void loop() {
  // put your main code here, to run repeatedly:

  if(phase == 0){
    digitalWrite(30, HIGH);
    delay(1000);
    digitalWrite(39, HIGH);
    delay(1000);
    digitalWrite(30, LOW);
    phase++;
  }
  
  while(phase == 1){
     if(digitalRead(push0) == 0){
      if(J0AR ==  0){
        J0 ++;
      }
      J0AR  = 1;
    }
    if(digitalRead(push0) ==  1){
      J0AR  = 0;
    }
    switch(J0){
      case 1:
        digitalWrite(3, HIGH);
        break;
      case 2:
        digitalWrite(3, LOW);
        digitalWrite(4, HIGH);
        break;
      case 3:
        digitalWrite(4, LOW);
        digitalWrite(5, HIGH);
        break;
      case 4:
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        break;
      case 5:
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        break;
      case 6:
        digitalWrite(7, LOW);
        digitalWrite(8, HIGH);
        break;
      case 7:
        digitalWrite(8, LOW);
        digitalWrite(9, HIGH);
        break;
      case 8:
        digitalWrite(9, LOW);
        digitalWrite(10, HIGH);
        winner  = 0;              //J0 is the winner
        digitalWrite(30, HIGH);    //Turn banner the color of J0
        digitalWrite(39, LOW);
        digitalWrite(40, LOW);
        phase++;
        break;
      default:
        digitalWrite( 3, LOW);
        digitalWrite( 4, LOW);
        digitalWrite( 5, LOW);
        digitalWrite( 6, LOW);
        digitalWrite( 7, LOW);
        digitalWrite( 8, LOW);
        digitalWrite( 9, LOW);
        digitalWrite( 10, LOW);
        J0  = 0;
    }
  
        //Player 2
    if(digitalRead(push1) == 0){
      if(J1AR ==  0){
        J1 ++;
      }
      J1AR  = 1;
    }
    if(digitalRead(push1) ==  1){
      J1AR  = 0;
    }
    switch(J1){
      case 1:
        digitalWrite(11, HIGH);
        break;
      case 2:
        digitalWrite(11, LOW);
        digitalWrite(12, HIGH);
        break;
      case 3:
        digitalWrite(12, LOW);
        digitalWrite(13, HIGH);
        break;
      case 4:
        digitalWrite(13, LOW);
        digitalWrite(14, HIGH);
        break;
      case 5:
        digitalWrite(14, LOW);
        digitalWrite(15, HIGH);
        break;
      case 6:
        digitalWrite(15, LOW);
        digitalWrite(36, HIGH);
        break;
      case 7:
        digitalWrite(36, LOW);    //16 is reset
        digitalWrite(37, HIGH);   //17 is push
        break;
      case 8:
        digitalWrite(37, LOW);
        digitalWrite(18, HIGH);
        winner  = 1;              //J1 is the winner
        digitalWrite(30, LOW);    //Turn banner the color of J1
        digitalWrite(39, LOW);
        digitalWrite(40, HIGH);
        phase++;
        break;
      default:
        digitalWrite( 11, LOW);
        digitalWrite( 12, LOW);
        digitalWrite( 13, LOW);
        digitalWrite( 14, LOW);
        digitalWrite( 15, LOW);
        digitalWrite( 36, LOW);
        digitalWrite( 37, LOW);
        digitalWrite( 18, LOW);
        J1  = 0;
    }
  }
 
}

const int OUT_PIN = A0;
unsigned long millisElapsed = 0;
unsigned long currTime = 0;
int max = 0;
int timeSet = 0;

//array for the different digits
int num_array[10][8] = {  { 1,0,0,0,0,1,0,0 },    // 0
                          { 1,0,1,1,1,1,1,0 },    // 1
                          { 1,1,0,0,1,0,0,0 },    // 2
                          { 1,0,0,1,1,0,0,0 },    // 3
                          { 1,0,1,1,0,0,1,0 },    // 4
                          { 1,0,0,1,0,0,0,1 },    // 5
                          { 1,0,0,0,0,0,0,1 },    // 6
                          { 1,0,1,1,1,1,0,0 },    // 7
                          { 1,0,0,0,0,0,0,0 },    // 8
                          { 1,0,0,1,0,0,0,0 }};   // 9

void setup() {
  //pins for the different displays
  pinMode(8, OUTPUT);
  pinMode(2, OUTPUT);   
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(9, OUTPUT);
  //pins for multiplexing
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

//mic gain evealuatiuon 
bool start = 0;
int thresh = 200;
void evalGain(int gain){
  if(gain > thresh && !start){
    start = !start;
    timeSet = millis();
    
  }else if(gain < thresh && start){
    start = !start;
  }
  if(start){
    push((millis()-timeSet));
  }
}

//writes the actual number on all displays
void Num_Write(int number) 
{
  int pin= 0;
  for (int j=0; j <= 7; j++) {
    digitalWrite(pin, !num_array[number][j]);
    pin++;
    }
}

//writes teh number to the number of the tube
void Disp_Write(int num_disp, int number){
  
  digitalWrite(num_disp + 7, HIGH);
  Num_Write(number);
  delay(2);
  digitalWrite(num_disp + 7, LOW);
}

//pushes teh numbers to the display
void push(int time){
  if (time >= max){
    time = max;
  }
  //writes the current tiem that you have screamed
  Disp_write(1, time/100 % 10);
  Disp_Write(2, time/1000 % 10);
  Disp_Write(3, time/10000 % 10);

  //writes the max amount of time anybody has screamed 
  Disp_Write(4, max/100 % 10);
  Disp_Write(5, max/1000 % 10);
  Disp_Write(6, max/10000 % 10);   
}

//main loop for the mix input and then gain evaluation
int gain = 0;
void loop() {
  int x = analogRead(A0)-510;
  x = abs(x);
  gain += x;
  if(millis() - currTime > millisElapsed){
    gain -= 240;
    currTime = millis();
  }
  gain = gain < 0 ? 0 : gain;
  gain = gain > 500 ? 500 : gain;
  delay(50);
  evalGain(gain);
}
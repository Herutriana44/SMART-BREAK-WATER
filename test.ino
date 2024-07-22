// const byte adc = A0;

// const byte trig = D0;
// const byte echo = D1;

// int var;
// int data[50];
// int maxx, meann, minn;

// unsigned long preTime = millis();

// void shiftRegister(int input)
// {
//   data[0] = input;
//   for(int i=49; i>0; i--)
//   {
//     data[i] = data[i-1];
//   }
// }

// int measure(const byte trigPin, const byte echoPin)
// {
//   digitalWrite(trigPin, 0);
//   delayMicroseconds(1);
//   digitalWrite(trigPin, 1);
//   delayMicroseconds(1);
//   digitalWrite(trigPin, 0);

//   int time = pulseIn(echoPin, 1);
//   int distance = (time / 2) * 0.0344;

//   return distance;
// }

// void setup()
// {
//   Serial.begin(9600);
//   pinMode(adc, INPUT);
//   pinMode(trig, OUTPUT);
//   pinMode(echo, INPUT);
// }

// void loop()
// {
//   var = analogRead(adc); //measure(trig, echo);
//   if(millis() - preTime > 100)
//   {
//     shiftRegister(var);
//     for(int i=0; i<50; i=i+2) {meann = meann + data[i] + data[i+1]; /*Serial.print(String(data[i]) + " ");*/} Serial.println();

//     meann = meann / 50;
//     if(var > maxx) {maxx = var;}
//     if(var < minn) {minn = var;}

//     Serial.print(" var ");
//     Serial.print(var);
//     Serial.print(" maxx ");
//     Serial.print(maxx);
//     Serial.print(" meann ");
//     Serial.print(meann);
//     Serial.print(" minn ");
//     Serial.print(minn);
//     Serial.println();

//     preTime = millis();
//   }
// }
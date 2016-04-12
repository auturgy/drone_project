unsigned short roll, pitch, throttle, yaw, aux_1, aux_2;

void setup() {

  Serial.begin(9600);

  pinMode(2,INPUT); // white - roll (좌우) 
  pinMode(3,INPUT); // yellow - pitch 
  pinMode(4,INPUT); // green - throttle 
  pinMode(5,INPUT); // blue - yaw (Rudder)
  pinMode(6,INPUT); // aux_1
  pinMode(7,INPUT); // aux_2

}

void loop() {

  // put your main code here, to run repeatedly:
  roll      = pulseIn(2,HIGH);
  pitch     = pulseIn(3,HIGH);
  throttle  = pulseIn(4,HIGH);
  yaw       = pulseIn(5,HIGH);
  aux_1     = pulseIn(6,HIGH);
  aux_2     = pulseIn(7,HIGH);
  
  Serial.print(roll);     Serial.print("\t");  
  Serial.print(pitch);    Serial.print("\t");
  Serial.print(throttle); Serial.print("\t");
  Serial.print(yaw);      Serial.print("\n");
  //Serial.print(aux_1);    Serial.print("\t");
  //Serial.print(aux_2);    Serial.print("\n");

}


// RC Singal for 6 channels structure 
//
// This structure is defined in ../../common/utils.hpp 
// so as to use on all applications in the drone project.
// However, I did not find how to include extenal header file 
// into aruino build system, so I just copy and use it here. 
// 
//////////////////////////////////////////////////////////////////
struct __attribute__ ((packed)) RC_SIGNAL
{
  unsigned short pin_1_;
  unsigned short pin_2_;
  unsigned short pin_3_;
  unsigned short pin_4_;
  unsigned short pin_5_;
  unsigned short pin_6_;
};

// not used 
//////////////////////////////////////////////////////////////////
void sendStructure( char *structurePointer, int structureLength)
{
  int i;

  for (i = 0 ; i < structureLength ; i++)
    Serial.println(structurePointer[i]);
}

// http://forum.arduino.cc/index.php?topic=258524.0
//////////////////////////////////////////////////////////////////
template< typename T >
union Serialize{
   T data;
   uint8_t bytes[ sizeof( T ) ];
   
   const uint8_t *begin(){ return bytes; }
   const uint8_t *end(){ return bytes + sizeof( T ); }
};

// global variables 
//////////////////////////////////////////////////////////////////
//unsigned short roll, pitch, throttle, yaw, aux_1, aux_2;
typedef Serialize< RC_SIGNAL > rcs;
rcs rc_signal_g;

// function setup() 
//////////////////////////////////////////////////////////////////
void setup() {

  Serial.begin(9600);

  pinMode(2,INPUT); // white - roll (좌우) 
  pinMode(3,INPUT); // yellow - pitch 
  pinMode(4,INPUT); // green - throttle 
  pinMode(5,INPUT); // blue - yaw (Rudder)
  pinMode(6,INPUT); // aux_1
  pinMode(7,INPUT); // aux_2

}

// function loop() 
//////////////////////////////////////////////////////////////////
void loop() {
 
  /*
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
  */
  
  rc_signal_g.data.pin_1_  = pulseIn(2,HIGH);
  rc_signal_g.data.pin_2_  = pulseIn(3,HIGH);
  rc_signal_g.data.pin_3_  = pulseIn(4,HIGH);
  rc_signal_g.data.pin_4_  = pulseIn(5,HIGH);
  rc_signal_g.data.pin_5_  = pulseIn(6,HIGH);
  rc_signal_g.data.pin_6_  = pulseIn(7,HIGH);

  for( auto i : rc_signal_g ) {
    Serial.write( i );
  }
  
  Serial.write('\n');    
  //Serial.flush();
 
  //delay(10); 
}

//  Lubrification Controller for SIP-600 Cedric Billeter
//  For Arduino  MINI

// Pin assignment
        int pushButtonPin  = 2 ;
        int oilPressurePin = 3 ;
        int pumpPin        = 4 ;
        int redLampPin     = 5 ;
        int greenLampPin   = 6 ;
    
// Event tracker
unsigned long PumpTime     = 0 ;    
unsigned long ErrorTime    = 0 ;  
unsigned long PressureTime = 0 ;
unsigned long AlarmTime    = 0 ;
          int ErrorCount   = 0 ;
         bool ErrorFlag    = false ;
         bool AlarmFlag    = false ;
         
// Initial Pin States
  bool greenLampState      = false ;
  bool redLampState        = false ;
  bool pumpState           = false ;
  bool oilPressureState    = false ;
  bool pushButtonState     = false ;

// Timer intervals
unsigned long tPumpOn      = 6000 ;    
unsigned long tPumpDelay   = 12000 ;   // 24'000'000 for 400 min
unsigned long tPressureOK  = 2000 ;
unsigned long tAlarm       = 2000 ;

// Setup 
void setup() 
{
//  
// initialize serial communication at 9600 bits per second:
  Serial.begin ( 9600 ) ;
//
  
// Pin Setups:
  pinMode(pushButtonPin ,  INPUT) ;  
  pinMode(oilPressurePin , INPUT) ;
  pinMode(pumpPin ,        OUTPUT) ;  
  pinMode(redLampPin ,     OUTPUT) ; 
  pinMode(greenLampPin ,   OUTPUT) ; 
  
// Set initial state of pins
  digitalWrite(pumpPin ,      pumpState) ; 
  digitalWrite(greenLampPin , greenLampState) ;
  digitalWrite(redLampPin ,   redLampState) ;
  }
  
// Main Loop
void loop() 
{

//*  Activate for Test Function  
//  Serial rutine  
  Serial.print   ( "  Button " ) ; 
  Serial.print   ( pushButtonState ) ;
  
  Serial.print   ( "  - pump " ) ; 
  Serial.print   ( pumpState ) ;
  
  Serial.print   ( "   - green " ) ;
  Serial.print   ( greenLampState ) ;
  
  Serial.print   ( "   - Oil " ) ; 
  Serial.print   ( oilPressureState ) ;
  
  Serial.print   ( "   - red " ) ; 
  Serial.print   ( redLampState ) ;

  Serial.print   ( "   - Err " ) ; 
  Serial.print   ( ErrorCount ) ;
  
  Serial.print   ( "   - Alm " ) ; 
  Serial.println ( AlarmFlag ) ;  

//*/
  
 // grab current time
 unsigned long currentMillis = millis() ;


// Pump OFF
    if ( pumpState == true && ( unsigned long )( currentMillis - PumpTime ) >= tPumpOn ) 
                        {
                         pumpState = false ; 
                         PumpTime  = currentMillis ;
                         ErrorFlag = false ;
                        }                         
    
// Pump ON 
    if ( pumpState == false && ( unsigned long )( currentMillis - PumpTime ) >= tPumpDelay )                           
                          {
                          if ( AlarmFlag == false )  {pumpState    = true ; } 
                          PumpTime     = currentMillis ;
                          PressureTime = currentMillis ;
                          }
    
// Pump Pressure check
    if( pumpState == true && oilPressureState == false && ( unsigned long )( currentMillis - PressureTime ) >= tPressureOK )
                          {
                          if ( AlarmFlag == false )  { redLampState = true ; }
                          if ( ErrorFlag == false )  { ErrorCount ++ ; }
                          ErrorFlag       = true ;
                          }

// Check if ErrorCount >= 3 if so turn On AlarmFlag
    if ( ErrorCount >= 3)
                          {
                          ErrorCount = 3 ;
                          AlarmFlag  = true ;
                          }

// Red Lamp Alarm rountine
    if ( AlarmFlag == true )
                         { 
                         if (( unsigned long )( currentMillis - AlarmTime ) >= tAlarm  )
                                         {
                                          redLampState = !redLampState ;
                                          AlarmTime    =  currentMillis ;
                                         }
                         }
  
// Write pumpState to greenLampState
    greenLampState = pumpState ;

// Write to pump and lamps outputs    
    digitalWrite ( pumpPin      , pumpState) ; 
    digitalWrite ( greenLampPin , greenLampState) ;
    digitalWrite ( redLampPin   , redLampState) ;

    
// Check if olio pressure is ON
   oilPressureState = digitalRead(oilPressurePin) ;
   
   if ( oilPressureState == true )
                         {
                         redLampState = false ;
                         PressureTime = currentMillis ;
                         ErrorCount   = 0 ;
                         AlarmFlag    = false ;
                         }

 // Check if pushButton is pressed
   pushButtonState = digitalRead ( pushButtonPin ) ;
  
  if ( pushButtonState == true )
  {
   pumpState = true ;
   PumpTime  = currentMillis ;
   }
}

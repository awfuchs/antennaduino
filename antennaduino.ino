
//
//  ANTENNADUINO! Source code by A W Fuchs.
//
//

// ===== CONSTANTS, MACROS, etc. =====
#define DEBUG false
#define YELL(x) if(DEBUG)Serial.println(x);

// Relay combinations to select antenna 1, 3, 2(north), 2(omni), 2(south)
#define ant1_relays {false, false, false, false}
#define ant3_relays {true, false, false, false}
#define ant2n_relays {true, true, false, false}
#define ant2s_relays {true, true, true, false}
#define ant2o_relays {true, true, true, true}

// Relay output pins (A0-A5 are D14-19 on output...)
#define relay1Pin 14
#define relay2Pin 15
#define relay3Pin 16
#define relay4Pin 17
#define RELAYMIN 14
#define RELAYMAX 17

// Input pins corresponding to antenna selector buttons
#define ant1Pin  2
#define ant2Pin  3
#define ant3Pin  4
#define northPin 5
#define omniPin  6
#define southPin 7
#define BUTTONMIN 2
#define BUTTONMAX 7
// Also the interrupt vector depends on which bank of pins
#define BUTTONVECTOR PCINT2_vect

// Output pins for button lights
#define ant1Led  8
#define ant2Led  9
#define ant3Led  10
#define northLed 11
#define omniLed  12
#define southLed 13
#define LEDMIN 8
#define LEDMAX 13

// Abstractions for button press states
#define DEPRESSED LOW
#define UNPRESSED HIGH


// ===== Data type definitions =====
 
typedef struct {
  bool ant1;
  bool ant2;
  bool ant3;
  bool north;
  bool omni;
  bool south; } radio_buttons;

typedef enum {
  north,
  omni,
  south } phase_direction;

typedef struct {
  bool r1;
  bool r2;
  bool r3;
  bool r4; } relay_set;


// ===== Global storage =====

// Button state and phase direction:
// Default antenna is Antenna 1
// Ant 2's default phasing is north
radio_buttons buttonState = { true, false, false, false, false, false };
phase_direction phaseDir = north;

// ===== Utility functions =====

// setRelays() outputs the provided relay_set values onto the control relays
void setRelays(relay_set rs) {
  if(rs.r1) digitalWrite(relay1Pin, LOW); else digitalWrite(relay1Pin, HIGH);
  if(rs.r2) digitalWrite(relay2Pin, LOW); else digitalWrite(relay2Pin, HIGH);
  if(rs.r3) digitalWrite(relay3Pin, LOW); else digitalWrite(relay3Pin, HIGH);
  if(rs.r4) digitalWrite(relay4Pin, LOW); else digitalWrite(relay4Pin, HIGH);
}

// setPushButton(b) takes a button state b, stores it as the new state, and sets LEDs accordingly 
#define MATCH_LIGHT(X,Y) if( X ) digitalWrite(Y, HIGH); else digitalWrite(Y, LOW);
void setPushButton(radio_buttons b) {
  buttonState=b;
  MATCH_LIGHT(b.ant1,ant1Led) 
  MATCH_LIGHT(b.ant2,ant2Led) 
  MATCH_LIGHT(b.ant3,ant3Led) 
  MATCH_LIGHT(b.north,northLed) 
  MATCH_LIGHT(b.omni,omniLed) 
  MATCH_LIGHT(b.south,southLed) 
}

// pushPhaseButton() does handling for phase direction buttons
void pushPhaseButton(phase_direction p) {
  phaseDir=p;
  switch (p) {
    case north:
      setPushButton( {0, 1, 0, 1, 0, 0} );
      setRelays( ant2n_relays );
      break;
    case omni:
      setPushButton( {0, 1, 0, 0, 1, 0} );
      setRelays( ant2o_relays );
      break;
    case south:
      setPushButton( {0, 1, 0, 0, 0, 1} );
      setRelays( ant2s_relays );
      break;
  }
}

// ===== Interrupt setup and handling =====

// Install Pin change interrupt for a pin, can be called multiple times 
void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}
 
// Interrupt routine for changed state of A0-A5 (pushbuttons)
ISR (BUTTONVECTOR) {
    YELL("Pin change interrupt")

    // ant1 and ant3 have simple select processing
    if( digitalRead(ant1Pin) == DEPRESSED )  {
      setPushButton( { 1, 0, 0, 0, 0, 0} );
      setRelays( ant1_relays );
    }
    else if( digitalRead(ant3Pin) == DEPRESSED ) {
        setPushButton( { 0, 0, 1, 0, 0, 0} );
        setRelays( ant3_relays );
    }
    // ant2 can have a direction select or retain the old one
    else if( digitalRead(ant2Pin) == DEPRESSED ) {
      pushPhaseButton(phaseDir);
    }
    else if( digitalRead(northPin) == DEPRESSED ) {
        pushPhaseButton(north);
    }
    else if( digitalRead(omniPin) == DEPRESSED ) {
        pushPhaseButton(omni);
    }
    else if( digitalRead(southPin) == DEPRESSED ) {
        pushPhaseButton(south);
    }       
 }  

// ===== The Arduino setup and loop routines =====

void setup() {  
int i;

if(DEBUG)  Serial.begin(9600);
 
// set pushbutton inputs with pullups, enable interrupt on pins
  for (i=BUTTONMIN; i<=BUTTONMAX; i++) {
      pinMode(i,INPUT_PULLUP);
      pciSetup(i);
  }
 
// set LED outputs
  for (i=LEDMIN; i<=LEDMAX; i++)
      pinMode(i,OUTPUT);
 
// set relay outputs
  for (i=RELAYMIN; i<=RELAYMAX; i++)
      pinMode(i,OUTPUT);
 
  // display initial state
  setPushButton( buttonState );
  setRelays( ant1_relays );
}
 
void loop() {
  // Nothing needed
}

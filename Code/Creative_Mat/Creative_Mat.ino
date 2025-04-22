// Button Pins
#define B1 0   // D0 (RX)
#define B2 1   // D1 (TX)
#define B3 2   // D2
#define B4 3   // D3
#define B5 A0  // A0 (digital)

// Red LED Pins
#define LR1 9  // D9
#define LR2 10 // D10
#define LR3 11 // D11
#define LR4 12 // D12
#define LR5 13 // D13 (built-in LED)

// Green LED Pins
#define LG1 4  // D4
#define LG2 5  // D5
#define LG3 6  // D6
#define LG4 7  // D7
#define LG5 8  // D8

void setup() {
  // Configure buttons (enable internal pull-up resistors)
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(B4, INPUT_PULLUP);
  pinMode(B5, INPUT_PULLUP);

  // Configure LEDs as outputs
  pinMode(LR1, OUTPUT);
  pinMode(LR2, OUTPUT);
  // ... Repeat for all LEDs ...
}

void loop() {
  // Example: Turn on LG1 when B1 is pressed
  if (digitalRead(B1) == LOW) { // Button pressed (pull-up logic)
    digitalWrite(LG1, HIGH);
  } else {
    digitalWrite(LG1, LOW);
  }
  // Add logic for other buttons/LEDs here...
}
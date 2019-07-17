
OneWire ds(A0);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// push buttons
const char Button_up    = A2; //*UP   -> Primeiro botão
const char Button_dn    = A3; //*DOWN -> Segundo botão
const char Button_start = A4; //*AUTO
const char Button_enter = A5; //*SETUP/OK

// outputs
const byte HeatLed = A1;

const byte Heat = 9;
const byte ZeroCross = 10;

const byte Buzz = 11;
const byte Pump = 12;
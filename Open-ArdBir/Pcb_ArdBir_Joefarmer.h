// sensor and lcd
OneWire ds(7);
LiquidCrystal lcd(A5, A4, 2, 3, 4, 5);

// push buttons
const char Button_dn    = A0;
const char Button_up    = A1;
const char Button_enter = A2;
const char Button_start = A3;

// outputs
const byte Pump = 8;
const byte Buzz = 9;
const byte Heat = 6;



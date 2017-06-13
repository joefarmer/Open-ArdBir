char *PIDName[]   ={"Use ", "Constant  kP", "Constant  kI", "Constant  kD", "SampleTime  ", "WindowSet ms", "Heat in Boil", "Calibration " , "Hysteresi  "};
char *stageName[] ={"Mash In   ", "Phytase   ", "Glucanase ", "Protease  ", "\xE2""Amylase  ", "\xE0""Amylase1 ", "\xE0""Amylase2 ", "Mash Out  ", "Boil      ", "Cooling   ", "Whirlpool "};
char *unitName[]  ={"Set Degrees", "Sensor     ", "Temp Boil  ", "Temp Boil  ", "Pump Cycle ", "Pump Rest  ", "Pmp PreMash", "Pmp on Mash", "Pmp MashOut", "Pmp on Boil", "Pump Stop  ", "Pump Stop  ", "PID Pipe   ", "Skip Add   ", "Skip Remove", "Skip Iodine", "IodineTime " , "Whirlpool "};

byte HeatONOFF[8]    = {B00000, B01010, B01010, B01110, B01110, B01010, B01010, B00000};  // [5] HEAT symbol
byte RevHeatONOFF[8] = {B11111, B10101, B10101, B10001, B10001, B10101, B10101, B11111};  // [6] reverse HEAT symbol
byte Language[8]     = {B11111, B00010, B01000, B11111, B00000, B10001, B10101, B11111};  // [7] EN symbol


void LCDClear(byte Riga){
  lcd.setCursor(0,Riga);
  LCDSpace(20);
}

void PrintTemp(byte PosX, byte PosY, float Temp, byte dec){
  if (PosY<4) lcd.setCursor(PosX,PosY);

  FormatNumeri(Temp, -1);

  lcd.print(Temp, dec);
  lcd.write((byte)0);
}

void Clear_2_3(){
  LCDClear(2);
  LCDClear(3);
}

void Version(byte locX, byte locY){
  lcd.setCursor(locX, locY);
  lcd.print(F("2.8.3""\xE0"));
  lcd.write(7);
}

void Intestazione(){
  lcd.setCursor(0,0);
  lcd.print(F("Open ArdBir "));
  Version(12,0);
}

void LCD_Procedo(){
  lcd.setCursor(1,3);
  lcd.print(F("Continue:  Yes  No "));
}


void LCD_Default(float Temp){
  Intestazione();

  PrintTemp(6,1,Temp,2);

  LCDClear(2);

  lcd.setCursor(1,3);
  lcd.print(F("---  MAN  --- SETUP"));
}

void CntDwn(int Time){
  CountDown(Time,11,2,2);
}

void Watch(int Time){
  CountDown(Time,6,2,2);
}

void PauseScreen(){
  lcd.setCursor(0,0);
  lcd.print(F("-----  "));
  lcd.setCursor(14,0);
  lcd.print(F(" -----"));

  lcd.setCursor(1,2);
  lcd.print(F("----"));
  lcd.setCursor(14,2);
  lcd.print(F(" ----"));
}

void LCD_QQSO(){
  lcd.setCursor(1,3);
  lcd.print(F("UP* *DWN Skip   Ok"));
}
void LCD_QQxO(){
  lcd.setCursor(1,3);
  lcd.print(F("UP* *DWN  ---   Ok"));
}
void LCD_SGEO(){
  lcd.setCursor(1,3);
  lcd.print(F("Up   Dwn  Quit  Ok"));
}
void LCD_xGEO(){
  lcd.setCursor(1,3);
  lcd.print(F("---  Dwn  Quit  Ok"));
}

void LCD_SxEO(){
  lcd.setCursor(1,3);
  lcd.print(F("Up   ---  Quit  Ok"));
}


void Menu_1(){
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print(F("MANUAL  MODE"));
}

void Manuale(float Set, float Temp,float TempBoil){
  PrintTemp(1,1,Temp,2);

  lcd.setCursor(12,1);
  if (Set<100)LCDSpace(1);
  lcd.print(Set);
  lcd.write(2);

  lcd.setCursor(1,3);
  lcd.print(F("UP* *DWN Heat Pmp"));
}

void Temp_Wait(float Temp){
  PrintTemp(1,1,Temp,2);
}

void Boil(float Heat, float Temp, byte Tipo){
  if (Tipo==1){
    lcd.setCursor(0,0);
    lcd.print(F(" AUTO --> Boil      "));
  }

  PrintTemp(1,1,Temp,2);

  lcd.setCursor(1,2);
  lcd.print(F("PWM="));    //Display output%
  FormatNumeri(Heat, -1);
  lcd.print(Heat,0);       //Display output%
  lcd.print(F("% "));
}

void NoBoil(){
  lcd.setCursor(1,2);
  LCDSpace(9);
}

void Menu_3(){
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(F("     SETUP MENU    "));
}
void Menu_3_1(){
  lcd.setCursor(2,1);
  lcd.print(F("   PID -- PWM   "));

  LCDClear(2);

  LCD_xGEO();
}
void Menu_3_1_x(byte i){
  display_lcd (1,2,PIDName[i],0);
  LCD_QQxO();
}

void PidSet(int pidSet, byte i){
  if (i == 0) {
    lcd.setCursor(10, 2);

    if (pidSet == 0) lcd.print(F("  Electric"));
    else             lcd.print(F("       Gas"));
  } else {
    lcd.setCursor(12, 2);
    LCDSpace(2);

    if (i > 0 && i <= 6) {
      if (i < 6) LCDSpace(1);

      if (i  < 4)           pidSet = pidSet - 100;
      if (i == 4 || i == 5) pidSet = pidSet * 250;

      FormatNumeri(pidSet, 0);
    }

    if(i >= 7){
      float OffSet = pidSet;
      if (i == 7) OffSet = (OffSet - 50.0) / 10.0;
      if (i == 8) OffSet =  OffSet / 10.0;

      FormatNumeri(OffSet, -2);
      lcd.print(OffSet);
      return;
    }

    lcd.print(pidSet);

    if (i == 6) lcd.print(F("%"));
  }
}

void Menu_3_2(){
  lcd.setCursor(1,1);
  lcd.print(F(" Unit  Parameters "));

  LCDClear(2);

  LCD_SGEO();
}

void Menu_3_2_x(byte i){
  display_lcd(1,2,unitName[i],0);
  LCD_QQxO();
}

void UnitSet(byte unitSet, byte i){
  lcd.setCursor(12,2);
  switch(i){

    case(0):// Scala Temp
      LCDSpace(6);
      lcd.write((byte)0);
      break;

    case(1)://Sensore
      if (unitSet == 0) lcd.print(F(" Inside"));
      else              lcd.print(F("Outside"));
      break;

    case( 2):
    case( 3): // Boil Temp
    case(10):
    case(11): //Temp to stop the pump at
      LCDSpace(3);
      PrintTemp(9, 9, unitSet, 0);
      break;

    case(4):// Pump Duty Cycle
      LCDSpace(4);
      FormatNumeri(unitSet, -2);
      lcd.print(unitSet);
      lcd.print(F("'"));
      break;

    case(5)://Duration to Pause Pump
      LCDSpace(5);
      lcd.print(unitSet);
      lcd.print(F("'"));
      break;

 default:
     LCDSpace(4);
      if (unitSet == 0) lcd.print(F("Off"));
      if (unitSet == 1) lcd.print(F(" On"));
      break;

    case(12)://Pipe
      if (unitSet == 0) lcd.print(F("Passive"));
      else              lcd.print(F(" Active"));
      break;

    case(13):
    case(14):
    case(15):
      LCDSpace(4);
      if (unitSet == 0) lcd.print(F(" No"));
      if (unitSet == 1) lcd.print(F("Yes"));
      break;

    case(16): //Iodio
      if (unitSet == 0) {
        lcd.setCursor(12, 2);
        lcd.print(F("    Off"));
      } else CountDown(unitSet * 60, 12, 2, 1);
      break;

    case(17):
      if (unitSet == 0) lcd.print(F("    Off"));
      if (unitSet == 1) lcd.print(F("   Cold"));
      if (unitSet == 2) lcd.print(F("    Hot"));
      break;
  }
}


void Inizializzazione(){
  lcd.setCursor(2,2);
  lcd.print(F("  Initialization"));

  LCD_Procedo();
}
void Inizializza(){
  LCDClear(2);

  lcd.setCursor(1,3);
  lcd.print(F(" Initialize EEPROM"));
  delay(1500);

  Buzzer(3,75);
  lcd.setCursor(1,3);
  lcd.print(F("  EEPROM   ready  "));
  delay(1500);
}

void MemoriaPiena(){
  LCDClear(2);

  Buzzer(3,125);

  lcd.setCursor(1,3);
  lcd.print(F("    ATTENZIONE    "));
  delay(1500);

  lcd.setCursor(1,3);
  lcd.print(F("   FULL  MEMORY   "));
  delay(2000);
}

void prompt_for_water (){
  lcd.setCursor(2,1);
  lcd.print(F("  Water  Added? "));
  LCD_Procedo();
}

void ledHeatON(){
  lcd.setCursor(0,2);
  lcd.write(6);
}

void ledHeatStatus(boolean mheat){
  lcd.setCursor(0,2);
  if (mheat)lcd.write(5);
  else LCDSpace(1);
}

void ledPumpON(){
  lcd.setCursor(19,2);
  lcd.write(4);
}

void ledPumpStatus(boolean mpump){
  lcd.setCursor(19,2);
  if (mpump)lcd.write(3);
  else LCDSpace(1);
}


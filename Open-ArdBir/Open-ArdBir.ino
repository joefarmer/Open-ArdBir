// ==============================================
// ATTENTION!!!!!
// YOU MUST SET ONLY THIS SECTION
// ==============================================

//SET PCB
// 5 ArdBir Joefarmer 
#define PCBType 5

// SET LCD and Language
#define LCDType 20

// LANGUAGE
// 1 English
#define LCDLanguage 1

// ==============================================
// END OF SETTING SECTION
// ==============================================

/*
brauduino semi automated single vessel RIMS
created by s.mathison
Copyright (C) 2012  Stephen Mathison

 Modified by Mike Wilson
 - Added INPUT_PULLUP to button inputs
 - swapped I/O ports for Buzz and Pump
 - changed to faherenheit
 - added heat and pump (H and P) indicators to the LCD
 - turned off pump during boil stage
 - added direct output% control and display during boil
 - saved boil heat % in EEPROM
 - 2v5 fixed hop addition when hoptime == boiltime
 - changed boil stage temp to 201 deg F.

 Modified by Massimo Nevi (2014)
 - Added Buzzer Modulation (NumBeep, Period)
 - Added Stage CountDown in Automatic Mode
 - Added Stage Watch in Manual Mode
 - Modified Max Number of Hops (10)
 - Modified stage Wait in Active Pause (PID Control On)
 - Modified H e P indicator for ON-OFF and Active-Disactive Modulation
 - Added Some Control Parameter in Configuration Menu 
 - Added Temp Pump Stop in Manual Mode
 - Dual Scale Temp (°C-°F)
 - Reorganized EEPROM
 - Reorganized Configuration
 - Added Delayed Start
 - Added Alert before Mash In
 - Jump Step whit Button Press in Automatic Mode
 - Added Cooling Procedure
 - Added Whirlpool Procedure
 
 Added more configuration on PID Menu
 - Added Use Gas section
 - Set Histeresy for Gas Use
 - Set Calibration Temp
 - Modified PID on BOIL
 
 - Added Second Menu Configuration
   - Set Scale Temp (°C-°F)
   - Set Location of Temp Sensor
   - Set Temp of Boil (°C-°F)
   - Set Set Time Pump Cycle and Time Rest
   - Set Pump ON-OFF before Mash In
   - Set Pump ON-OFF during Mash
   - Set Pump ON-OFF at Mash Out
   - Set Pump ON-OFF at Boil 
   - Set Temp Pump Stop (°C-°F)
   - Set Skip Pause at Add Malt
   - Set Skip Pause at Remove Malt
   - Set Skip Pause at Iodine Test
   - Set Time Max Iodine Test 
   - Set Manage Whirlpool
   
 - Reorganized Automation
   - Removed Number of Stages Setting
   - Fixed Name of Stages (8)
   - Dual A-Amylase Stage
   - Correct Mash In Stage
   - Auto Mash Design
   - Correct Time Reset of Pump
   - Added Iodine Test (Active Pause)
   - Added WHIRLPOOL control
   - Added COOLING control
   
 - Stage Managing
   - Load Stage Set
   - Save Stage Set
   - Delete Stage Set
   - Initialize EEPROM

 - LCD 16x2 wiew
 - LCD 20x4 wiew
 - Italian Language     (Both)
 - English Language     (Both)
 - Spanish Language     (Both)
 - Portuguese Language  (Both)
 - Russian Language     (20x4)
 - Norwegian Language   (20x4)
 
 compiled on Arduino V1.6

EEPROM MAP
  PID MENU
      0       Use Gas
      1       kP
      2       kI
      3       kD
      4       SampleTime
      5       WindowSize
      6       Boil Heat %
      7       Offset
      8       Hysteresi 
      9       [ SPACE ]

  UNIT MENU  
     10       Scale Temp
     11       Sensor Type
     12       Temp Boil °C
     13       Temp Boil °F
     14       Time Pump Cycle
     15       Time Pump Rest
     16       Pump PreMash
     17       Pump on Mash
     18       Pump on MashOut
     19       Pump on Boil
     20       Temp Pump Rest °C
     21       Temp Pump Rest °F
     22       PID Pipe
     23       Skip Add Malt
     24       Skip Remove Malt
     25       Skip Iodine Test
     26       Iodine Time
     27       Whirlpool
     28 -  31 [ SPACE ]

  RUN  (HTemp °C - LTemp °C - HTemp °F - LTemp °F - Time)
    32 -  36 MashIn
    37 -  41 Fitasi
    42 -  46 Glucanasi
    47 -  51 Proteasi
    52 -  55 B-Amilasi
    57 -  61 A-Amilasi 1
    62 -  66 A-Amilasi 2
    67 -  71 Mash Out

    72       Numbers of Hops
    73       Boil Time 
    74       Time Hop  1
    75       Time Hop  2
    76       Time Hop  3
    77       Time Hop  4
    78       Time Hop  5
    79       Time Hop  6
    80       Time Hop  7
    81       Time Hop  8
    82       Time Hop  9
    83       Time Hop 10

    84       FLAG Automode Started

  RESUME
    85       HANDLE Stage
    86       HANDLE Time Rest
    87       Hop Add

    88 -  89 [ SPACE ]

  RECIPE
    90 -  99 Index 1-10
   100 - 151 Recipe Data  1

   152 - 203 Recipe Data  2
   204 - 255 Recipe Data  3
   256 - 307 Recipe Data  4
   308 - 359 Recipe Data  5
   360 - 411 Recipe Data  6
   412 - 463 Recipe Data  7
   464 - 515 Recipe Data  8
   516 - 567 Recipe Data  9
   568 - 619 Recipe Data 10

   620 - 629 Recipe Name  1
   630 - 639 Recipe Name  2
   640 - 649 Recipe Name  3
   650 - 659 Recipe Name  4
   660 - 669 Recipe Name  5
   670 - 679 Recipe Name  6
   680 - 689 Recipe Name  7
   690 - 699 Recipe Name  8
   700 - 709 Recipe Name  9
   710 - 719 Recipe Name 10



 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 */

//libraries
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <PID_v1.h>

// SETTING PCB*****
#include "Pcb_ArdBir_Joefarmer.h"

// Porzioni di codice
#include "Funzioni.h"


// SETTING LCD*****
#include "LCD20x4_ENG.h"
 

// global variables
unsigned long TimeLeft;
unsigned long TimeSpent;
unsigned long w_StartTime;
unsigned long start;
unsigned long Timer; 

byte WindowSize;

double Setpoint;
double Input;
double Output;

boolean Conv_start   = false;
boolean mpump        = false;
boolean mheat        = false;
boolean b_Enter      = false;
boolean tempReached  = false;
boolean pumpRest     = false;
boolean resume       = false;
boolean IodineTest   = false;
boolean DelayedMode  = false;


//float mset_temp;
float stageTemp;
float boilStageTemp;
float Temp_Now;


byte x;
byte ScaleTemp       = EEPROM.read(10);;
byte SensorType      = EEPROM.read(11);;
byte UseGAS          = EEPROM.read( 0);

byte stageTime;
byte mainMenu        = 0;
byte pumpTime           ;
byte data[9]            ; // Sensor Temp with crc version
byte second             ;
byte Busy            = 0;
byte StageAddr          ;
byte blhpAddr           ;
byte boil_output        ;  // boil output %

byte Verso              ;

//Setting 8 Stages
float p_C[]    ={  75.00, 20.00, 0.25,   55.00, 25.00, 0.25,   50.00, 35.00, 0.25,   60.00,  45.00, 0.25,   70.00,  50.00, 0.25,   76.00,  60.00, 0.25,  76.00,  60.00, 0.25,  80.00,  75.00, 0.25 }; 
float p_F[]    ={ 167.00, 68.00, 0.25,  131.00, 77.00, 0.25,  122.00, 95.00, 0.25,  140.00, 113.00, 0.25,  158.00, 122.00, 0.25,  168.75, 140.00, 0.25, 176.00, 167.00, 0.25, 176.00, 167.00, 0.25 }; 

byte  p_PID[]  ={   1, 0, 1,   200, 0, 1,   255, 0, 1,   200, 0, 1,   14, 6, 1,   30, 0, 1,   100, 0, 1,   100, 0, 1,   100, 0, 1 }; 
//                  Use Gas       kP           kI           kD       SampleTIme   Win Size    %PWM Boil    Calibration   Hysteresi 

byte  p_Unit[] ={   1, 0, 1,   1, 0, 1,   105, 90, 1,   221, 194, 1,   15, 5, 1,   5, 0, 1,   1, 0, 1,   1, 0, 1,   1, 0, 1,   1, 0, 1,   105, 80, 1,   221, 176, 1,   1, 0, 1,   1, 0, 1,   1, 0, 1,   1, 0, 1,   90, 0, 1,   2, 0, 1};
//                   Scala °   Sensore      Boil °C       Boil °F      Ciclo Pmp  Pausa Pmp   PreMash    on Mash    MashOut    on Boil     Fermo °C       Fermo °F     PID Pipe    Sk.Add   Sk.Remove   Iodine     TimeIodio   Whirlpool


//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, 100, 40, 0, DIRECT);
unsigned long serialTime; //this will help us know when to talk with processing

byte degC[8]         = {B01000, B10100, B01000, B00111, B01000, B01000, B01000, B00111};  // [0] degree c sybmol 
byte degF[8]         = {B01000, B10100, B01000, B00111, B00100, B00110, B00100, B00100};  // [0] degree f symbol

byte SP_Symbol[8]    = {B11100, B10000, B11100, B00111, B11101, B00111, B00100, B00100};  // [2] SP Symbol
byte PumpONOFF[8]    = {B00000, B01110, B01010, B01110, B01000, B01000, B01000, B00000};  // [3] Pump Symbol 
byte RevPumpONOFF[8] = {B11111, B10001, B10101, B10001, B10111, B10111, B10111, B11111};  // [4] Reverse PUMP Symbol

// ****************************************
// ******** start of  the funtions ******** 
// ****************************************

void Gradi() {
  if (ScaleTemp == 0) lcd.createChar(0, degC); // Celsius  
  else                lcd.createChar(0, degF); // Faherenheit
}


void dsInizializza() {
  ds.reset();
  ds.skip();
}

void Temperature() { // reads the DS18B20 temerature probe 
  dsInizializza();
  
  // start conversion and return
  if (!(Conv_start)) {
    ds.write(0x44, 0);
    Conv_start = true;
    return;
  }
  if (Conv_start) { // check for conversion if it isn't complete return if it is then convert to decimal
    Busy = ds.read_bit();
    if (Busy == 0) return;

    dsInizializza();
    
    ds.write(0xBE);  
    for ( byte i = 0; i < 9; i++) {           // with crc we need 9 bytes
      data[i] = ds.read();
    } 
    /* add this routine for crc version */
    if ( OneWire::crc8(data, 8) != data[8]) {  //if checksum fails start a new conversion right away
      dsInizializza();
      
      ds.write(0x44, 0);
      Conv_start = true;
      return;
      /*Fine Routine crc*/
    }

    unsigned int raw = (data[1] << 8) + data[0];
    
    Temp_Now = (raw & 0xFFFC) * 0.0625;
    if (ScaleTemp == 1) Temp_Now = Temp_Now * 1.8 + 32.0;
    
    Temp_Now = Temp_Now + ((float)((r_set(7) - 50) / 10.0));
    
    Conv_start = false;
    return;
  } 
}

// PID_HEAT modified to use SetMode(MANUAL)
void PID_HEAT (boolean autoMode) {
  //autoMode = TRUE  PID Control
  //autoMode = FALSE PWM Control
  
  float DeltaPID;
  float Rapporto, Delta, IsteresiProporzionale;
  
  if (UseGAS == 1) {
    DeltaPID = EEPROM.read(8) / 10;
    
    IsteresiProporzionale = DeltaPID / Input;
    myPID.SetSampleTime(8000);
    WindowSize = 160;
  } else {  
    if (ScaleTemp == 0)       DeltaPID = 3.50;
    else                      DeltaPID = 6.30;
    
    if (Input >= boilStageTemp - DeltaPID) DeltaPID = 1;
    
    IsteresiProporzionale = 0.0;
  }  
  
  Delta    = Setpoint - (Input + IsteresiProporzionale);
  Rapporto = Delta / DeltaPID;
  
  unsigned long now = millis();
    
  if (autoMode){
    if (Rapporto < 1.00){
      //IL VALORE VA MODULATO 
      if (UseGAS == 1) {
        //SEZIONE GAS
        Output = Arrotonda025(Rapporto) * 255;
        if (Rapporto < 0.20) Output = 40.00;
        if (Rapporto < 0.10) Output =  0.00;
      } else {
        //SEZIONE ELETTRICA
        myPID.Compute();   // was 6, getting close, start feeding the PID -mdw
      }
      
    //IL VALORE E' DIRETTO
    } else {
      Output = 255;      // was 5, ignore PID and go full speed -mdw  // set the output to full on
    }  
  }


  //BOIL STAGE REACHED
  // Output value is reassigned
  if (Input >= Setpoint && Setpoint >= boilStageTemp) Output = boil_output * 255 / 100;
  
// PWM  
  if (now - w_StartTime > (unsigned int) WindowSize * 250) {
    w_StartTime += (unsigned int)WindowSize * 250; //time to shift the Relay Window
  }
  
  if ((Output / 255) * ((unsigned int)WindowSize * 250) > now - w_StartTime) {
    //***********
    mheat = true;
    //***********
    heat_on();
  }
  else {
    heat_off(mheat);
  }

}

void load_pid_settings () {
  myPID.SetTunings(r_set(1) - 100, (double)((r_set(2) - 100.00) / 250.00), r_set(3) - 100); // send the PID settings to the PID
  
  WindowSize = r_set(5);

  myPID.SetSampleTime(r_set(4) * 250);
}  

boolean wait_for_confirm (byte Stato, byte Tipo, byte Display) { 
  // Stato   == 1 Pause with    PID
  // Stato   == 2 Pause without PID
  
  // Tipo    == 1 YES
  // Tipo    == 2 YES NO
  
  // Display == 1 Yes
  // Display == 2 No
  
  boolean wtBtn = true;
  
  byte Attesa   = 0;
  
  Buzzer(1, 750);
  
  while (wtBtn) {                             // wait for comfirmation 
    Temperature();
    Input = Temp_Now;
     
    if (Display == 1) Temp_Wait(Temp_Now);

    if (Stato == 1) {                         // Pausa ATTIVA
      if (SensorType == 1) heat_off(mheat);
      else                 PID_HEAT(true);
    } else {                                  // Pausa PASSIVA
      heat_off(mheat);
    }
        
    if (btn_Press(Button_start, 50)) {
      return true;
      wtBtn = false;
    }
    
    if (Tipo == 2) {
      if (btn_Press(Button_enter, 50)) {
        return false;
        wtBtn = false;
        lcd.clear();
      }
    } else {
      if (Attesa == 255) {
        Buzzer(2, 25);
        Attesa = 0;
      }
    }
    
    Attesa++;  
  }
}

void quit_mode (boolean& processLoop) {
  if ((digitalRead(Button_dn) == 0) && (digitalRead(Button_up) == 0)) {
    delay(350);
    if ((digitalRead(Button_dn) == 0) && (digitalRead(Button_up) == 0)) {
      mheat = false;
      mpump = false;
      
      allOFF();
      
      processLoop = false;
      Clear_2_3();
    }
  }
}

void allOFF() {
  heat_off(mheat);
  pump_off(mpump);
}

void heat_on() {
  digitalWrite (Heat, HIGH);
  ledHeatON();
}

void heat_off(boolean mheat) {
  digitalWrite (Heat, LOW);
  ledHeatStatus(mheat);
}


void heat_control() {
  //turns heat on or off      
  if (btn_Press(Button_start, 50)) {
    if (mheat == false){
      mheat = true;
      w_StartTime = millis();
    } else {
      mheat = false;
      heat_off(mheat);
    }
  }
}

void pump_on() {
  digitalWrite(Pump, HIGH);
  ledPumpON(); 
}

void pump_off(boolean mpump) {
  digitalWrite(Pump, LOW);
  ledPumpStatus(mpump);
}

void pump_rest (byte stage) {
  byte TimePumpCycle = r_set(14);
  byte TimePumpRest  = r_set(15);
  byte PumpPreMash   = r_set(16);
  byte PumpOnMash    = r_set(17);
  byte PumpOnMashOut = r_set(18);
  byte PumpOnBoil    = r_set(19);
  byte TempPumpRest;
  
  if (ScaleTemp == 0) TempPumpRest = r_set(20);
  else                TempPumpRest = r_set(21);
  
  
  //Condizioni per il ripristino a POMPA ON
  float DeltaTemp;//Stabilisce il Delta in base al sensore
  
  if (SensorType == 0) { 
    //Sensore INTERNO
    DeltaTemp = TimePumpRest * stageTemp / 70;
  } else {
    //Sensore ESTERNO
    DeltaTemp = TimePumpRest * stageTemp / 35;
  }
  
  if (Temp_Now >= TempPumpRest) {
    if (SensorType == 0) { 
      //Sensore Interno
      pump_off(mpump);
      return;
    
    } else {
      // Modifica per sensore sulla pompa
    
      //Determina il Delta in cui tenere spento
      if (tempReached){
        if (Temp_Now < (boilStageTemp - (DeltaTemp * 2))) {
          // ************
          mpump = true;
          // ************
          pump_on();
          pumpRest = false;
        } else { 
          pump_off(mpump);
        }
      } else {
        // ************
        mpump = true;
        // ************
        pump_on();
        pumpRest = false;
      }
    }
  } else {
    if (stage == 8) {
      //Siamo in fase di BOIL
      if (PumpOnBoil == 0) {
        pump_off(mpump); // Turn OFF the pump in BOIL stage
      } else {
        // ************
        mpump = true;
        // ************
        pump_on();
        pumpRest = false;
      }
    } else {//Durante le fasi 
      
      if (stage == 9 || stage == 99) {
        return; // Non effettua i controlli perchè siamo in WHIRlPOOL o in MANUAL MODE
      }
      
      //Se non viene raggiunto il limite di tempo POMPA ON
      if ((pumpTime < TimePumpCycle)) { // starts pumps and heat
        // ************
        mpump = true;
        // ************
        pump_on();
        pumpRest = false; 
      } else {//Se viene raggiunto il limite di tempo POMPA OFF
        allOFF();
        
        pumpRest = true;

        //Viene raggiunta la fine della PAUSA POMPA
        //Viene raggiunta la soglia di abbassamento Temperatura
        //Si azzera il PumpTime per cominciare un altro ciclo
        if (pumpTime >= (TimePumpCycle + TimePumpRest) || ((stageTemp - Temp_Now) > DeltaTemp)) pumpTime = 0;
      } 
    }
  }
}

void pump_control() {//ON-OFF Manuale
  //turns the pump on or off
  if (btn_Press(Button_enter, 50)) {
    if (mpump == false) {
      mpump = true;
      pump_on();
    } else {
      mpump = false;
      pump_off(mpump);
    }
  }
}

void start_time () {
  start = millis();
  second = 0;
}

void Timing(byte stage, boolean Test, byte Type) {
  //Type == 0 AUTOMATIC
  //Type == 1 MANUAL
  //Type == 2 IODINE TEST
  //Type == 3 START DELAYED

  if ((millis() - start) > 1000) {  // timing routine
    start = millis();
    second++;

    if (!(Test)) second = 0;// starts counting down when temp reached
    else {
      TimeLeft--;
      TimeSpent++;
    }
    
    if (Type == 2) {
      if (IodineTest) second = 0;// starts counting down when temp reached
     }
    
    if (second > 59) {
      second = 0;
      if (stage == 0) pumpTime = 0;
      else            pumpTime++;

      stageTime--;
    }
  }
}

void manual_mode () {
  float mset_temp;
   
  if (ScaleTemp == 0)  mset_temp = 35.0;
  else                 mset_temp = 95.0;
  
  boolean manualLoop    = false;
  boolean reachedBeep   = false;
  
  boolean FlagSpentLeft = false;
  boolean SpentLeft     = false;
  
  if (UseGAS == 0)    load_pid_settings();

  boil_output = r_set(6);
  
  prompt_for_water();
  manualLoop = wait_for_confirm(2, 2, 2);

  Menu_1();//Pulisce lo schermo
  
  tempReached = false;
  TimeSpent   = 0;
  TimeLeft    = 0;
  
  byte cntTime = 1;
  
  
  float               DeltaSetPoint = 2.0;
  if (ScaleTemp == 1) DeltaSetPoint = 3.6;
  
  while (manualLoop) {            // manual loop
    Temperature();
    Setpoint = mset_temp;
    Input = Temp_Now;
    
    if (tempReached == false) {
      if (Input >= Setpoint) {
        tempReached = true;
      }
    } else {
      if ((Input + DeltaSetPoint) < Setpoint && Verso == 1) {
        tempReached   = false;
        reachedBeep   = false;
        
        FlagSpentLeft = false;
        SpentLeft     = false;
        TimeSpent     = 0;
        TimeLeft      = 60;
        
      }
    }
    
    if (tempReached) {
      if (reachedBeep == false) {
        Buzzer(3, 250);
        reachedBeep = true;
      }
    } 
    
    
    Timing(8, tempReached, 1);

    // START + ENTER = COUNTDOWN MODE
    if ((digitalRead(Button_start) == 0) && (digitalRead(Button_enter) == 0)) {
      delay(350);
      if ((digitalRead(Button_start) == 0) && (digitalRead(Button_enter) == 0)) {
        Buzzer(2, 35);
        FlagSpentLeft = true;
        if (SpentLeft) SpentLeft = false;
        else           SpentLeft =  true;
      }
    }
   
    quit_mode(manualLoop);
    heat_control();
    pump_control();  

    if (SpentLeft) {
      CntDwn(cntTime * 60);
      LeggiPulsante(Verso, Timer);
    
      Set(cntTime, 240, 1, 1, Timer, Verso);
      TimeLeft = cntTime * 60;
    } else {
      LeggiPulsante(Verso, Timer);
      Manuale(mset_temp, Temp_Now, boilStageTemp);
    }
    
    if (Setpoint >= boilStageTemp && Temp_Now >= Setpoint) { 
      Set(boil_output, 100, 0, 1, Timer, Verso);

      Boil(boil_output, Temp_Now, 0);

      if (mheat) PID_HEAT(false); //set heat in PWM Boil
      if (mpump) pump_rest(8);    //Forced Boil Stage for Pump Control
      
    } else {
      
      NoBoil();
      
      if (ScaleTemp == 0) Set(mset_temp, 110, 20, 0.25, Timer, Verso);
      else                Set(mset_temp, 230, 68, 0.25, Timer, Verso);

      if (mheat) PID_HEAT(true); //set heat in auto mode
      if (mpump) pump_rest(99);  //Forced normal Stage for jump Pump Control
    }

    if (FlagSpentLeft) {
      CntDwn( TimeLeft);
        
      if (TimeLeft < 6 ) Buzzer(1, 135);
      if (TimeLeft == 0){ 
        Buzzer(3, 850);
        
        FlagSpentLeft = false;
        SpentLeft     = false;
        TimeSpent     = 0;
        TimeLeft      = 60;
      }
    } else CntDwn(TimeSpent);  
  } lcd.clear();
}


void set_PID () {
  boolean pidLoop = true;
  byte    pidSet;
  byte    Min;
  pidLoop = false;
  
  //Indirizzo di memorizzazione corrisponde al ciclo
  
  for(byte i = 0; i < 9; i++) {
 
    pidSet = r_set(i);
    
    if (i == 8 && UseGAS == 0) pidLoop = false;
    else                       pidLoop = true;

    while (pidLoop) {
      Menu_3_1_x(i);
      
      LeggiPulsante(Verso, Timer);
      
      PidSet(pidSet, i);

      if (i == 5) Min = r_set(i - 1) + 2;
      else        Min = p_PID[i * 3 + 1];
      
      Set(pidSet, p_PID[i * 3], Min, p_PID[i * 3 + 2], Timer, Verso); 
      
      
      quit_mode(pidLoop);
      if (!pidLoop)i = 9;

      if (btn_Press(Button_enter, 50)) {     
        s_set(i, pidSet);
        if (i == 0 ) {
          UseGAS = pidSet;
          if (UseGAS == 1) i = 5;  // Va alla Calibrazione
        }
        pidLoop = false;
      }
   }  
 }Clear_2_3();
}

void set_Unit () {
  byte unitSet;
  boolean unitLoop = false;

  for (byte i = 10; i < 28; i++){
  
    // SKIP BLOCK PUMP SENSOR IF OUTSIDE  
    if ((i >= 16 || i <= 19) && SensorType == 1) unitLoop = false;
    else                                         unitLoop = true;
    
    // WATER TIME IODIEN SE SKIP IODINE ACTIVE
    if (i == 26 && EEPROM.read(i - 1) == 1)      unitLoop = false;
    else                                         unitLoop = true;


    if (i != 12 && i != 20)                      unitSet = r_set(i);
    else {
      if (ScaleTemp == 0)                        unitSet = r_set(i);
      else                                       unitSet = r_set(i + 1);
    }
    
    while (unitLoop) {
      Menu_3_2_x(i - 10);  
      if (i == 10) {
        ScaleTemp = unitSet;
        Gradi();
      }
    
      UnitSet(unitSet, i - 10);
  
      LeggiPulsante(Verso, Timer);
      Set(unitSet, p_Unit[(i - 10) * 3], p_Unit[(i - 10) * 3 + 1], p_Unit[(i - 10) * 3 + 2], Timer, Verso);      

      quit_mode(unitLoop);
      if (!unitLoop) break;

      if (btn_Press(Button_enter, 50)) {
        
        if (i != 12 && i != 20) {
          s_set(i, unitSet);

          if (i == 10) { 
            ScaleTemp = unitSet;
            Gradi();
          }
          if (i == 11) SensorType = unitSet;    

          if (i == 16 && SensorType == 1) { 
            //Il SENSORE E' ESTERNO
            for (byte ii = i; ii < i + 4; ii++) s_set(ii, 1);
            
            //Il Pump Rest viene settato a 0
            // °C = 105; //°F = 221
            s_set(i + 4, 110);
            s_set(i + 5, 230);

            // Il PID Pipe è ON
            s_set(i + 6, 1);

            // Ciclo e setAddr vengono allineati al PID Pipe
            i = 22;
          }


          if (i == 19) {
            if (r_set(i) == 0) { // Controllo sul fermo Pompa
              //°C = 80; //°F = 176
              s_set(i + 1,  80);
              s_set(i + 2, 176);

              // Ciclo e setAddr vengono allineati al FermoPompa
              i = 21;
            }
          }

        } else {
          //FERMO POMPA  
          if (ScaleTemp == 0) {
              s_set(i,     unitSet);
              s_set(i + 1, (byte)((unitSet * 1.8) + 32));
            } else {
              s_set(i,     (byte)((unitSet - 32) / 1.8));
              s_set(i + 1, unitSet);
            }
            
          if (i == 12) boilStageTemp = unitSet;
          i = i + 1;
        }
        unitLoop = false;
      }
    }
  }Clear_2_3(); 
}



void setup_mode () {
  boolean setupLoop = true;
  byte setupMenu    = 0;

  while (setupLoop) {
    switch (setupMenu) { 
      
      case(0):
      Menu_3_1();
      if (btn_Press(Button_start, 50)) setupLoop = false;
      if (btn_Press(Button_dn, 50))    setupMenu = 1;
      if (btn_Press(Button_enter, 50)) set_PID();  
      break;

      case(1):
      Menu_3_2();
      if (btn_Press(Button_start, 50)) setupLoop = false;
      if (btn_Press(Button_up, 50))    setupMenu = 0;
      if (btn_Press(Button_enter, 50)) set_Unit();  
      break;

    }
  } lcd.clear();
}   

void setup() {
  
  // SETTING LCD*****
  #if   LCDType == 16
    lcd.begin(16,2);
  #elif LCDType == 20
    lcd.begin(20,4);
  #endif

  pinMode (Button_up,    INPUT_PULLUP);
  pinMode (Button_dn,    INPUT_PULLUP);
  pinMode (Button_start, INPUT_PULLUP);
  pinMode (Button_enter, INPUT_PULLUP);
  pinMode (Heat, OUTPUT);
  pinMode (Pump, OUTPUT);
  pinMode (Buzz, OUTPUT);

  //tell the PID to range between 0 and the full window size
  myPID.SetMode(AUTOMATIC);

  allOFF();
 
  if (ScaleTemp == 0) boilStageTemp = r_set_float(12);
  else                boilStageTemp = r_set_float(13);
  
  Gradi();  // write custom symbol to LCD
  lcd.createChar(2, SP_Symbol);    // Set Point
  lcd.createChar(3, PumpONOFF);    // Pump
  lcd.createChar(4, RevPumpONOFF); // Pump
  lcd.createChar(5, HeatONOFF);    // Resistor ON-OFF
  lcd.createChar(6, RevHeatONOFF); // Heat On-OFF
  lcd.createChar(7, Language);     // Language Symbol 
}

void loop() {
    
  pumpTime   = 0;
  TimeSpent  = 0;
  TimeLeft   = 0;
  
  IodineTest = false;
 
  switch(mainMenu) {

    case (1):
    Menu_1();
    manual_mode();
    mainMenu = 0;
    break;

    case (3):   
    Menu_3();
    setup_mode();
    mainMenu = 0;    
    break;

  default: 
    DelayedMode = false;
    mheat =       false;
    mpump =       false;  
    
    allOFF();
    
    Temperature();
    LCD_Default(Temp_Now);

    if (btn_Press(Button_dn, 500))    mainMenu = 1;
    if (btn_Press(Button_enter, 500)) mainMenu = 3;
    break;    
  }
}




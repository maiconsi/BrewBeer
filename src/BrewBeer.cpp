#include <Arduino.h>
// ==============================================
// ATTENTION!!!!!
// YOU MUST SET ONLY THIS SECTION
// ==============================================

//SET PCB
// 1 Brauduino Original (Matho's PCB)
// 2 Brauduino by DanielXan
// 3 ArdBir by DanielXan
// 4 ArdBir by Maicon/GTO
#define PCBType 4

// SET LCD and Language
// LCD 16 or 20
#define LCDType 20

// LANGUAGE
// 1 English
// 2 Italian
// 3 Spanish
// 4 Portuguese
// 5 Russian    (only 20 x 4)
// 6 Norwegian  (only 20 x 4)
#define LCDLanguage 4

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

/// FOR DEBUGGING ///
#define ToneOnBuzzer  false

#define SerialPID     false
#define PID_FE        false


#define ReadWrite     false
#define TestMemoria   false
/// ------------- ///


//libraries
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <PID_v1.h>

// SETTING PCB*****
// #include "Pcb_ArdBir_DanielXan.h"
#include "Pcb_ArdBir_GTO.h"

// Porzioni di codice
#include "Funzioni.h"

// SETTING LCD*****
#include "LCD20x4_POR.h"

// Função para utilizar a interrupção em qualquer porta
#include "AttachInterrupcaoPorta.h"

// inclui biblioteca Timer1
#include <TimerOne.h> 

// global variables
unsigned long TimeLeft;
unsigned long TimeSpent;
unsigned long w_StartTime;
unsigned long start;
unsigned long Timer; 

byte WindowSize;

double Setpoint;
double Input;
double Output = 0;

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
byte hopTime;
byte mainMenu        = 0;
byte pumpTime           ;
byte data[9]            ; // Sensor Temp with crc version
byte second             ;
byte Busy            = 0;
byte StageAddr          ;
byte blhpAddr           ;
byte hopAdd             ;
byte nmbrHops           ;
byte boil_output        ;  // boil output %

byte Verso              ;

//Setting 8 Stages
float p_C[]    ={  75.00, 20.00, 0.25,   55.00, 25.00, 0.25,   50.00, 35.00, 0.25,   60.00,  45.00, 0.25,   70.00,  50.00, 0.25,   76.00,  60.00, 0.25,  76.00,  60.00, 0.25,  80.00,  75.00, 0.25 }; 
float p_F[]    ={ 167.00, 68.00, 0.25,  131.00, 77.00, 0.25,  122.00, 95.00, 0.25,  140.00, 113.00, 0.25,  158.00, 122.00, 0.25,  168.75, 140.00, 0.25, 176.00, 167.00, 0.25, 176.00, 167.00, 0.25 }; 

//byte  p_PID[]  ={   1, 0, 1,   200, 0, 1,   255, 0, 1,   200, 0, 1,   30, 9, 1,   100, 0, 1,   100, 0, 1,   100, 0, 1 }; 
//                  Use Gas       kP           kI           kD        Win Size    %PWM Boil    Calibration   Hysteresi 
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

void Gradi();
void heat_on();
void heat_off(boolean mheat);
void allOFF();
void pauseStage();
void dsInizializza(); 
void Temperature();
void zero_crosss_int(); 
void gateTRIAC();
void PID_HEAT(boolean autoMode);
void load_pid_settings();
boolean wait_for_confirm(byte Stato, byte Tipo, byte Display); 
void quit_mode(boolean& processLoop);
void heat_control();
void pump_on(); 
void pump_off(boolean mpump);
void pump_prime();
void pump_rest (byte stage);
void pump_control();
void start_time();
void Timing(byte stage, boolean Test, byte Type);
void hop_add();
void stage_loop();
void add_malt();
void Iodine_Test(); 
void remove_malt();
void manual_mode();
void WaitStart();
void Temperatura_Raggiunta();
void auto_mode();
void Cooling();
void Whirlpool();
void set_PID();
void set_Unit(); 
void set_Stages(); 
void Congruita(byte& numRicetta, byte Verso);
void loadRecipe(); 
void saveRecipe();
void deleteRecipe();
void initializeRecipe();
void RecipeMenu(); 
void set_hops();
void TestRam();
void setup_mode();

void setup() {
  // Start up the library  
  #if   SerialMonitor == true
    Serial.begin(9600);
  #elif SerialPID     == true
    Serial.begin(9600);
  #elif PID_FE        == true
    Serial.begin(9600);
  #elif ReadWrite     == true
    Serial.begin(9600);
  #endif

  // SETTING LCD*****
  #if   LCDType == 16
    lcd.begin(16,2);
  #elif LCDType == 20
    lcd.begin(20,4);
  #endif

  #if SerialPID == true  
    Serial.println();
    Serial.println(F("CONTROLLO SPERIMENTALE ATTIVITA' P.I.D."));
    Serial.print (F("ScaleTemp:  "));
    if (ScaleTemp == 0)  Serial.println ("C");
    else                 Serial.println ("F");
    
    Serial.print (F("SensorType: "));
    if (SensorType == 0) Serial.println ("INTERNO");
    else                 Serial.println ("ESTERNO");
    
    Serial.print (F("UseGAS:     "));
    if (UseGAS == 0)     Serial.println ("ELETTRICO");
    else                 Serial.println  ("GAS");
  #endif
  
  pinMode (Button_up,    INPUT_PULLUP);
  pinMode (Button_dn,    INPUT_PULLUP);
  pinMode (Button_start, INPUT_PULLUP);
  pinMode (Button_enter, INPUT_PULLUP);
  pinMode (Heat, OUTPUT);
  pinMode (Pump, OUTPUT);
  pinMode (Buzz, OUTPUT);
  pinMode (HeatLed, OUTPUT);
  //w_StartTime = millis();

  // Inicializa interrupção. O número zero indica a porta 10 do Arduino,
  // zero_crosss_int é a função que será chamada toda vez que o pino 2
  // "subir" (RISING) de valor de 0 para 1.  
  PCattachInterrupt(ZeroCross, zero_crosss_int, RISING);
  
  // Initialize the interruption time Timer1
  Timer1.initialize(); // Initialize TimerOne library for the freq we need
  
  //tell the PID to range between 0 and the full window size
  myPID.SetMode(AUTOMATIC);

  allOFF();
 
  //if (ScaleTemp == 0) boilStageTemp = EEPROM.read(12);
  //else                boilStageTemp = EEPROM.read(13);
  
  //if (ScaleTemp == 0) boilStageTemp = ((r_set_float(12) - 32) / 1.8);
  //else                boilStageTemp = ((r_set_float(13) * 1.8) + 32) ;

  if (ScaleTemp == 0) boilStageTemp = r_set(12);
  else                boilStageTemp = r_set(13);

  Serial.print("boilStageTemp: ");
  Serial.println(boilStageTemp);
  
//  Sprite screen
//  ArdBir();
  
  Gradi();
  // write custom symbol to LCD
  //lcd.createChar(0, deg);        // Celsius
  //lcd.createChar(1, degF);       // Faherenheit
  lcd.createChar(2, SP_Symbol);    // Set Point
  lcd.createChar(3, PumpONOFF);    // Pump
  lcd.createChar(4, RevPumpONOFF); // Pump
  lcd.createChar(5, HeatONOFF);    // Resistor ON-OFF
  lcd.createChar(6, RevHeatONOFF); // Heat On-OFF
  lcd.createChar(7, Language);     // Language Symbol 
}

void loop() {
  //boolean StartNow = false;
    
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

    case (2):
    Menu_2();
    
    PartenzaRitardata();
    //wait_for_confirm(StartNow, 2, 2, 2);
    
    if (!(wait_for_confirm(2, 2, 2))) DelayedMode = true;
    
    Menu_2();
    auto_mode(); 
    mainMenu = 0;
    break;

    case (3):   
    Menu_3();
    setup_mode();
    mainMenu = 0;    
    break;

    case (4):
    #if TestMemoria == true
      Menu_4();
      TestRam();
      mainMenu = 0;    
      break;
    #endif


  default: 
    DelayedMode = false;
    mheat =       false;
    mpump =       false;  
    
    allOFF();
    
    Temperature();
    LCD_Default(Temp_Now);

    if (btn_Press(Button_dn, 500))    mainMenu = 1;
    if (btn_Press(Button_start, 500)) mainMenu = 2;
    if (btn_Press(Button_enter, 500)) mainMenu = 3;
    #if TestMemoria == true
      if (btn_Press(Button_up, 2500)) mainMenu = 4;
    #endif  
    break;    
  }
}

void Gradi() {
  if (ScaleTemp == 0) lcd.createChar(0, degC); // Celsius  
  else                lcd.createChar(0, degF); // Faherenheit
}

void pauseStage(){
  boolean stage_pause = false;
  if (btn_Press(Button_start, 250)) {
    Buzzer(3, 100);
    stage_pause = true;
    
    allOFF();
 
    while (stage_pause) {
      Temperature();
      Pause_Stage(Temp_Now, TimeLeft);
      delay(75);
      if (btn_Press(Button_start, 250)) stage_pause = false;
    }
    Buzzer(3, 100);
    Menu_2();
  } 
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
    
    //byte Correzione;
    //r_set(Correzione, 7);
    //Temp_Now = Temp_Now + ((float)((Correzione - 50) / 10.0));
    
    Temp_Now = Temp_Now + ((float)((r_set(7) - 50) / 10.0));
    
    
    //Temp_Now = Temp_Now + ((EEPROM.read(6) - 50) / 10.0);
    
    Conv_start = false;
    return;
  } 
}

void zero_crosss_int()  
{
  if (mheat)
  {
//    digitalWrite(Heat, HIGH); //Teste
    
//  lcd.setCursor(3,2); //Teste
//  lcd.print(Output);

    if(Output > 240)
      Output = 240;
 //   else if (Output < 30)
 //     Output = 30;    
    
    // Cálculo do ângulo de disparo: 60Hz-> 8.33ms (1/2 ciclo)
    // (8333us - 8.33us) / 256 = 32 (aprox).
    // Ou seja, cada 1 unidade em power corresponde a 32 microssegundos
    // na onda de meio ciclo.
    int powertime = (32*(256-Output));      
      
    // Mantém o circuito desligado por powertime microssegundos 
    Timer1.attachInterrupt(gateTRIAC, powertime);  //associa a funcao gateTRIAC com Interrupcao do TIMER1
    Timer1.start();  // inicia contagem TIMER1
  }else{
    digitalWrite(Heat, LOW);   // mantem gate do TRIAC desativado.
    Timer1.stop();
  }
}

void gateTRIAC ()
{  // trata interrupcao do TIMER1 gerando pulso no gate do TRIAC
  digitalWrite(Heat, HIGH);  // dispara o Triac
  delayMicroseconds(8.33);  // aguarda 5 microsegundos para garantir disparo do TRIAC
  digitalWrite(Heat, LOW);   // desabibilta gate do TRIAC
  Timer1.stop();
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
  
  #if SerialPID == true
    byte Ore, Minuti, Secondi;
    unsigned int Millesimi;
    Ore       = (byte) ((now/1000)/3600);
    Minuti    = (byte)(((now/1000)%3600)/60);
    Secondi   = (byte) ((now/1000)%60);
    Millesimi = (unsigned int)    now%1000;
  
    Serial.print(F("Tempo: "));
    if (Ore < 10)     Serial.print("0"); Serial.print(Ore);
    Serial.print(":");    
  
    if( Minuti < 10)  Serial.print("0"); Serial.print(Minuti);
    Serial.print(F(":"));    
  
    if (Secondi < 10) Serial.print("0"); Serial.print(Secondi);
    Serial.print(F("."));
  
    if (Millesimi <   10) Serial.print("0");
    if (Millesimi <  100) Serial.print("0");
    Serial.print(Millesimi);
    
    Serial.print(F("  Temperatura: "));
    if(Temp_Now <  10 && Temp_Now >= 0) Serial.print(F("  "));
    if(Temp_Now < 100 && Temp_Now >=10) Serial.print(F(" "));
    Serial.print(Temp_Now);
    
    Serial.print(F("  Set Point: "));
    if(Setpoint <  10 && Setpoint >= 0) Serial.print(F("  "));
    if(Setpoint < 100 && Setpoint >=10) Serial.print(F(" "));
    Serial.print(Setpoint);
    
    Serial.print(F("  DeltaPID: "));
    Serial.print(DeltaPID);

  #endif
  
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
  
  //FASE DI BOIL RAGGIUNTA
  // Il valore di Output viene riassegnato
  if (Input >= Setpoint && Setpoint >= boilStageTemp) 
    Output = boil_output * 255 / 100;
  
// PWM  
  //if (now - w_StartTime > (unsigned int)(WindowSize * 250)) w_StartTime += (unsigned int)(WindowSize * 250); //time to shift the Relay Window
  
  if (now - w_StartTime > (unsigned int) WindowSize * 250) {
    w_StartTime += (unsigned int)WindowSize * 250; //time to shift the Relay Window
  }
  
  
  if ((Output / 255) * ((unsigned int)WindowSize * 250) > now - w_StartTime) {
  
  //if ((Output * ((unsigned int)(WindowSize * 250) / 255)) > now - w_StartTime) {
    //***********
    mheat = true;
    //***********
    heat_on();
  }
  else {
    heat_off(mheat);
  }
  
  #if PID_FE == true
    if(millis()>serialTime)  {
      SerialReceive();
      SerialSend();
      serialTime+=500;
    }
  #endif
  
  #if SerialPID == true
    Serial.print(F("  Output: "));
    if(Output <  10 && Output >= 0) Serial.print(F("  "));
    if(Output < 100 && Output >=10) Serial.print(F(" "));
    Serial.print(Output);
    
    Serial.print (F("  Now: "));
    Serial.print (now);
    Serial.print (F("  w_StartTime: "));
    Serial.print (w_StartTime);
    
    Serial.print(F("   | Stato Pompa: "));
    Serial.print(mpump);
    
    Serial.print(F("  Tempo: "));
    if(pumpTime <  10 && pumpTime >= 0) Serial.print(F("  "));
    Serial.println(pumpTime);
  #endif
}

void load_pid_settings () {
  //byte eepromKp   = r_set(1);
  //byte eepromKi   = r_set(2);
  //byte eepromKd   = r_set(3);  
  //byte SampleTime = r_set(4);
  
  //r_set(eepromKp, 1);
  //r_set(eepromKi, 2);
  //r_set(eepromKd, 3);
  
  //myPID.SetTunings(eepromKp - 100, (double)((eepromKi - 100.00) / 250.00), eepromKd - 100); // send the PID settings to the PID
  myPID.SetTunings(r_set(1) - 100, (double)((r_set(2) - 100.00) / 250.00), r_set(3) - 100); // send the PID settings to the PID
  
  //r_set(SampleTime, 4);
  //r_set(WindowSize, 5);
  WindowSize = r_set(5);
    
  //myPID.SetOutputLimits(0.0, 255.0);
  myPID.SetOutputLimits(20, 240);
  
  //myPID.SetSampleTime(SampleTime * 250);
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
  digitalWrite(HeatLed, HIGH);
  ledHeatON();
}

void heat_off(boolean mheat) {
  if (mheat) {
     digitalWrite (HeatLed, HIGH);
  } else {
     digitalWrite (HeatLed, LOW);
  }
  ledHeatStatus(mheat);
}


void heat_control() {
  //turns heat on or off      
  if (btn_Press(Button_start, 50)) {
    if (mheat == false){
      mheat = true;
      Output = 240; 
      myPID.Compute(); 
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

void pump_prime() {
  PumpPrime();
  
  for (byte i = 1; i < 6; i++) {
    pump_on();
    delay(750 + i * 250);
    pump_off(mpump);
    delay(350);
  }Menu_2(); 
}

void pump_rest (byte stage) {
  byte TimePumpCycle = r_set(14);
  byte TimePumpRest  = r_set(15);
  byte PumpPreMash   = r_set(16);
  byte PumpOnMash    = r_set(17);
  byte PumpOnMashOut = r_set(18);
  byte PumpOnBoil    = r_set(19);
  byte TempPumpRest;
  
  //r_set(TimePumpCycle, 14);
  //r_set(TimePumpRest,  15);
  //r_set(PumpPreMash,   16);
  //r_set(PumpOnMash,    17);
  //r_set(PumpOnMashOut, 18);
  //r_set(PumpOnBoil,    19);
  
  //if (ScaleTemp == 0) r_set(TempPumpRest, 20);
  //else                r_set(TempPumpRest, 21);
  
  if (ScaleTemp == 0) TempPumpRest = r_set(20);
  else                TempPumpRest = r_set(21);
  
  
  //mpump = true;
  if (stage == 0 && PumpPreMash == 0 || stage > 0 && stage < 7 && PumpOnMash == 0 || stage == 7 && PumpOnMashOut == 0) { 
    pump_off(true);
    return;
  }
  
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

      //if (Type == 0) EEPROM.write(86, stageTime);// saves stage time incase of interuption
      if (Type == 0) s_set(86, stageTime);        // saves stage time incase of interuption
    }
  }
}

void hop_add () {
  //r_set(nmbrHops, 72);
  nmbrHops = r_set(72);
  
  
  if (hopAdd < nmbrHops) {
    
    if (stageTime == hopTime) {
      Buzzer(4, 250);
      HopAdd(hopAdd);
      CntDwn(TimeLeft);

      if (TimeLeft < 6) Buzzer(1, 150);

      delay(2500);
      Buzzer(1, 750);
      hopAdd++;
      //EEPROM.write(87, hopAdd);
      s_set(87, hopAdd);
      blhpAddr++;
      //r_set(hopTime, blhpAddr);
      hopTime = r_set(blhpAddr);
    }
  } 
}



void stage_loop () {  
  byte lastminute;
  boolean tempBoilReached = false;
  
  while ((stageTime > 0) && (b_Enter)) {
    lastminute = stageTime;

    Timing((x - 1), tempReached, 0);

    Temperature();// get temp
    Setpoint = stageTemp;

    Input = Temp_Now;
    
    pauseStage();
      
    LeggiPulsante(Verso, Timer);
    
    if (pumpRest){
      PausaPompa(Temp_Now, TimeLeft);
      if (TimeSpent % 30 == 0) Buzzer(1, 65);
      delay (135);
    }else{
      if ((x - 1)  != 0) CntDwn(TimeLeft);
      if (TimeLeft  < 6) Buzzer(1, 150);
      if (TimeLeft == 0) Buzzer(1, 1000); 
      
      if ((x - 1) == 7 && IodineTest == false ) {
        //byte b_Iodine;
        //r_set(b_Iodine, 25);
        //if (b_Iodine == 0) Iodine_Test();
        if (r_set(25) == 0) Iodine_Test();
      }

      if ((x - 1) == 8 && tempBoilReached && Temp_Now >= boilStageTemp) {  //if temp reached during boil
      
        Set(boil_output, 100, 0, 1, Timer, Verso);
        
        Boil(boil_output, Temp_Now, 1);
        PID_HEAT(false); //set heat in Boil PWM

      } else { 
        
        float Max, Min;
        if (ScaleTemp == 0) {
          Max = p_C[(x - 1) * 3];
          Min = p_C[(x - 1) * 3 + 1];
        } else {
          Max = p_F[(x - 1) * 3];
          Min = p_F[(x - 1) * 3 + 1];
        }
        /*
        if ((x - 1) == 8) {
          //if (ScaleTemp == 0) Set(stageTemp, 110, EEPROM.read(12), 0.25, Timer, Verso);
          //else                Set(stageTemp, 230, EEPROM.read(13), 0.25, Timer, Verso);
          
          if (ScaleTemp == 0) Min = EEPROM.read(12);
          else                Min = EEPROM.read(13);
          Set(stageTemp, 230, Min, 0.25, Timer, Verso);
          
          NoBoil();
          tempBoilReached = false;

        } else                Set(stageTemp, Max, Min, 0.25, Timer, Verso);
        */
        
        if ((x - 1) == 8) {
          if (ScaleTemp == 0) {
            Max = 110;
            Min = EEPROM.read(12);
          } else {
            Max = 230;
            Min = EEPROM.read(13);
          }
          NoBoil();
          tempBoilReached = false;
        } 

        Set(stageTemp, Max, Min, 0.25, Timer, Verso);

        Stage((x - 1), stageTemp, Temp_Now);

        if (pumpRest == false) PID_HEAT(true);
      }  
    }
    if (Temp_Now >= stageTemp) {
      if ((x - 1) == 8 && tempBoilReached == false) tempBoilReached = true;
      if (tempReached == false) {
        tempReached = true; 
        Buzzer (3,250);
        
        //***** Aggiunta per salto tenuta Mash In
        if ((x - 1) == 0) stageTime = 0;
        //*****

        if ((x - 1) == 8) hop_add();  // check for hop add once temp reached
      }
    }
    
    pump_rest((x - 1));
    if ((x - 1) == 8) {
      //check for minute tick
      if (stageTime < lastminute) hop_add();  //check to add hops
    }
    quit_mode (b_Enter);
    
    // *********************************
    // Aggiunta controllo pompa on boil
    // if ((x - 1) == 8) pump_control();
    // *********************************
    
    if (btn_Press(Button_enter, 2500)) {
      if ((x - 1) > 0) {
        //boolean flag_SaltoStep;
        Buzzer(3, 50);
        SaltoStep();
        //wait_for_confirm(flag_SaltoStep, 1, 2, 1);
        if (wait_for_confirm(1, 2, 1)) {
          Clear_2_3();
          return;
        }
      }
    }  
  }
}

void add_malt () {
  //boolean malt;
  pump_off(mpump);
  
  AddMalt();

  //wait_for_confirm(malt, 1, 2, 1);
  if (wait_for_confirm(1, 2, 1) == false) {
    LCD_Default(Temp_Now);
    delay(50);
    mainMenu = 0;
    b_Enter  = false;
  }
}

void Iodine_Test () {
  // byte IodineTime   = r_set(26);
  byte TempoRimasto = stageTime;
  
  //r_set(IodineTime, 26);
  
  boolean Test = true;
  
  //if (ScaleTemp == 0) read_set(Setpoint, StageAddr - 5);
  //else                read_set(Setpoint, StageAddr - 3);
  //Setpoint = Setpoint / 16.0;

  if (ScaleTemp == 0) Setpoint = r_set_double(StageAddr - 5) / 16.0;
  else                Setpoint = r_set_double(StageAddr - 3) / 16.0;

  pump_on();
  pumpTime  = 0;
  TimeSpent = 0;
  
  while (Test) {
    Timing(6, true, 2);

    Temperature();
    Input = Temp_Now;

    if (pumpRest == false) PID_HEAT(true);
    pump_rest(6);

    Iodine(Temp_Now,TimeSpent);
    
    if (TimeSpent % 45 == 0) Buzzer(1, 65);
    delay (135);
    
    quit_mode(Test);  
    
    //if (btn_Press(Button_start, 50) || IodineTime != 0 && TimeSpent >= IodineTime * 60) {
    if (btn_Press(Button_start, 50) || r_set(26) != 0 && TimeSpent >= r_set(26) * 60) {  
      Test       = false;
      IodineTest = true;
      TimeSpent  = 0;
      stageTime  = TempoRimasto;
      TimeLeft   = stageTime * 60;
    }
  }
  pump_on();
  pumpRest = false;
  pumpTime = 0;
  Menu_2();
}

void remove_malt () {
  boolean malt;
  x = 8;               // used add to stage count on the final stage for the resume 
  //EEPROM.write(85, x); // stores the stage number for the resume
  s_set(85, x); // stores the stage number for the resume
  pump_off(mpump);

  //***** Reset della pompa al cambio step
  pumpRest = false;
  pumpTime = 0;

  RemoveMalt();

  // Pausa senza PID (gli enzimi ormai sono distrutti)
  //byte noPID;
  //r_set(noPID, 22);
  //if (noPID == 1 && SensorType == 0) malt = wait_for_confirm(1, 2, 1);
  
  if (r_set(22) == 1 && SensorType == 0) malt = wait_for_confirm(1, 2, 1);
  else                                   malt = wait_for_confirm(2, 2, 1);
  
  if (malt == false) {
    //r_set(stageTime, 73);
    stageTime = r_set(73);
    //EEPROM.write(86, stageTime);
    s_set(86, stageTime);
    LCD_Default(Temp_Now);
    delay(50);
    mainMenu = 0;
    b_Enter  = false;
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

  //r_set(boil_output, 6);
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

void WaitStart() {
  boolean Procedo = true;
  TimeLeft = 0;
  
  while (Procedo) {
    ImpostaTempo(stageTime);
    LeggiPulsante(Verso, Timer);
    
    Set((stageTime), 1440, 15, 15, Timer, Verso);
    
    if (btn_Press(Button_enter, 50)) Procedo = false;
    
    if (btn_Press(Button_start, 50)) {
      Procedo  = false;
      mainMenu = 0;
      b_Enter  = false;
      return;
    }
  }
  
  LCD_Procedo();
  Procedo = wait_for_confirm(2, 2, 2);
  
  Clear_2_3();
  
  if (Procedo) {
    TimeLeft = (unsigned long) stageTime * 60;
    
    start_time();
    
    while(stageTime > 0) {
      StartDelay(TimeLeft);
      Timing(0, true, 1);
      
      quit_mode(Procedo);    
      if (!Procedo) {
        Procedo  = false;
        mainMenu = 0;
        b_Enter  = false;
        return;
      }
  
    }
    Buzzer(5, 250);
    Clear_2_3();
  
  } else {
    Procedo  = false;
    mainMenu = 0;
    b_Enter  = false;
    return;
  }
}

void Temperatura_Raggiunta() {
  boolean TempRaggiunta = false;
  //byte pmpPreMash;
  //r_set(pmpPreMash, 16);
  
  while (!TempRaggiunta){
    TemperaturaRaggiunta();

    //if (pmpPreMash) == 1) pump_on();
    if (r_set(16) == 1) pump_on();
    else                ledPumpStatus(true);
    
    TempRaggiunta = wait_for_confirm(1, 1, 1);
  }
}

void auto_mode () {
  //byte AutoMode;
  //r_set(AutoMode, 84);
  
  StageAddr = 32;
  
  if (UseGAS == 0) load_pid_settings();

//  check_for_resume();
  
  //if (AutoMode) { // FLAG Automode Started
  if (r_set(84)) { // FLAG Automode Started
  
    Resume();

    resume = wait_for_confirm(2, 2, 2);
    if (resume) {
      //r_set(StageAddr, 85);
      //StageAddr = (StageAddr * 5) + 32;
      //StageAddr = r_set(85);
      //StageAddr = (StageAddr * 5) + 32;
      
      //r_set(x, 85);
      x = r_set(85);
      
      StageAddr = (x * 5) + 32;
      b_Enter = true;
    }
  } 
    
  Menu_2();

  if (!(resume)) {  // if starting a new process prompt for water
    prompt_for_water();
    b_Enter = wait_for_confirm(2, 2, 2);

    Menu_2();//pulisce lo schermo

    if (!(b_Enter)) return;
    hopAdd = 0; //reset hop count at teh start of the processes
    //EEPROM.write(87, hopAdd);
    s_set(87, hopAdd); 
    pumpTime = 0;
    pumpRest = false;
    pump_prime();
    x = 0;
  }

  if (DelayedMode) WaitStart();
  
// ************  
// x=9; //for TESTING WHIRLPOOL
// ************

// ************  
w_StartTime = millis();
// ************

  if (b_Enter) {                     // mash steps
    //EEPROM.write(84, 1);           // auto mode started
    s_set(84, 1);                    // auto mode started
    for (byte i = x; i < 8; i++) {
      //EEPROM.write(85, x); // stores the stage number for the resume
      s_set(85, x);                 // stores the stage number for the resume
      x++;                          // used to count the stages for the resume 
      tempReached = false;
 
      //***** Reset della pompa al cambio step
      pumpRest = false;
      pumpTime = 0;
      //*****

      //if (ScaleTemp == 0) read_set(stageTemp, StageAddr);
      //else                read_set(stageTemp, StageAddr + 2);
      //stageTemp = stageTemp / 16.0;

      if (ScaleTemp == 0) stageTemp = r_set_float(StageAddr) / 16.0;
      else                stageTemp = r_set_float(StageAddr + 2) / 16.0;
      
      
      if (resume) {                 // on the start of resume gets saved time
        stageTime = r_set(86);
        //r_set(stageTime, 86);
        resume = false;            // clears resume for next loop
      } else {
        stageTime = r_set(StageAddr + 4); // gets stage time
        //r_set(stageTime, StageAddr + 4); // gets stage time
        
        //EEPROM.write(86, stageTime);   // saves the intial stage time
        s_set(86, stageTime);   // saves the intial stage time
        
        if (x == 7 && stageTime == 0) IodineTest = true; 
      } 
      TimeLeft = stageTime * 60;
      
      start_time(); 
      stage_loop();
      
      if (!(b_Enter)) break;
      if (i == 0 && b_Enter) {    // at the end of the mashIn step pauses to add grain

//      INSERIMENTO PAUSA AGGIUNTIVA        
        Temperatura_Raggiunta();

        if (EEPROM.read(23) == 0) add_malt();
        if (!(b_Enter)) break;

        Menu_2();
      }
      if (i == 7 && b_Enter) {   // at the end of the last step pauses to remove the malt pipe before the boil
        if (EEPROM.read(24) == 0) remove_malt();
        if (!(b_Enter)) break;

        Menu_2();
      }          
      StageAddr += 5; // increase stage addresses  
    }
  }

  // start of the boil
  if (b_Enter) {

    start_time(); 
    
    stageTemp   = boilStageTemp; // was set the intital boil temp to 98 deg c -mdw
    tempReached = false;  

    //r_set(nmbrHops, 72);
    nmbrHops = r_set(72);
/*    
    if (resume) {
      if (x != 9) r_set(stageTime, 73);
      else        r_set(stageTime, 86);
    } else {
      r_set(stageTime, 73);
      //EEPROM.write(86, stageTime);
      s_set(86, stageTime);
    } 
*/    
    if (resume) {
      if (x != 9) stageTime = r_set(73);
      else        stageTime = r_set(86);
    } else {
      stageTime = r_set(73);
      //EEPROM.write(86, stageTime);
      s_set(86, stageTime);
    } 
    
    //r_set(hopAdd, 87);
    hopAdd = r_set(87);
    blhpAddr = hopAdd + 74;
    
    //r_set(hopTime, blhpAddr);
    //r_set(boil_output, 6);
    hopTime = r_set(blhpAddr);
    boil_output = r_set(6);
    
    TimeLeft = stageTime * 60;
    
    x = 9;
    stage_loop();

    if (b_Enter) {    // finishes the brewing process
      mheat = false;
      
      if (EEPROM.read(27) == 2) Whirlpool(); // Whirlpool a Caldo
      else                      Cooling();
      
      End();
      //EEPROM.write(84, 0); // sets auto start byte to 0 for resume
      //EEPROM.write(87, 0); // sets hop count to 0
      s_set(84, 0); // sets auto start byte to 0 for resume
      s_set(87, 0); // sets hop count to 0
      
      mainMenu = 0;
      b_Enter  = false;
      resume   = false;
    }
  } lcd.clear();
}

void Cooling() {
  boolean f_Cooling = false;
  
  allOFF();

  //Attende risposta per raffreddamento
  Raffreddamento();
  f_Cooling = wait_for_confirm(2, 2, 2);
  
  Menu_2();
  while (f_Cooling) {
    Temperature();
    
    pump_control();

    LeggiPulsante(Verso, Timer);

    if (ScaleTemp == 0) Set(stageTemp, 30, 10, 0.25, Timer, Verso);
    else                Set(stageTemp, 86, 50, 0.25, Timer, Verso);

    Stage(9, stageTemp, Temp_Now);

    quit_mode(f_Cooling);
    if ((!(f_Cooling)) || (Temp_Now <= stageTemp)) break;
  }
  //byte b_Whirlpool;
  //r_set(b_Whirlpool, 27);
  //if (b_Whirlpool == 1) Whirlpool();
  if (r_set(27) == 1) Whirlpool();
}

void Whirlpool () {  
  //stageTime =  3;
  TimeSpent =  0;
  TimeLeft  = 0;
  
  boolean f_Whirlpool = false;
  //byte b_Whirlpool;
  //r_set(b_Whirlpool, 27);
  
  //if (b_Whirlpool == 2) {
  if (r_set(27) == 2) {
    if (ScaleTemp == 0 ) stageTemp =  85;
    else                 stageTemp = 185;
  } else {
    if (ScaleTemp == 0 ) stageTemp =  30;
    else                 stageTemp =  86;
  }
  
  pump_off(true);
  
  //Attende risposta per whirlpool
  LCDWhirlpool();
  f_Whirlpool = wait_for_confirm(2, 2, 2);
  if (f_Whirlpool){
    boolean Procedo = true;
    byte TempoWhirlpool;
    
    //TimeLeft = 0;
  
    while (Procedo) {
      ImpostaWhirlpool(TempoWhirlpool);
      LeggiPulsante(Verso, Timer);
    
      Set((TempoWhirlpool), 10, 1, 1, Timer, Verso);
    
      if (btn_Press(Button_enter, 50)) Procedo = false;
    
      if (btn_Press(Button_start, 50)) {
        Procedo     = false;
        f_Whirlpool = false;
      }
    }  
    
    TimeLeft = TempoWhirlpool * 60;
    Menu_2();
  
    byte AppoggioTempo;  
    boolean Mulinello = false;
    mpump = true; 
    
    while (f_Whirlpool) {
      Temperature();
      if (mpump) Stage(10, stageTemp, Temp_Now);
      else       Stage(11, stageTemp, Temp_Now);
      
      //if (b_Whirlpool == 2) {
      if (r_set(27) == 2) {
        if (Temp_Now <=  stageTemp) break;
        else Mulinello = true;
      } else {
        if (Temp_Now <=  stageTemp && mpump) {
          Mulinello = true;
        }
      }
    
      if (Mulinello) {
        pump_on();
        Timing (0, true, 1);
      } 
      
      pump_control();
      Mulinello = mpump;
      
      if (!mpump) {
        if (btn_Press(Button_start, 50)) {
          AppoggioTempo = TempoWhirlpool;
          Procedo = true;
          lcd.clear();
        }
        
        while (Procedo) {
          ImpostaWhirlpool(TempoWhirlpool);
          LeggiPulsante(Verso, Timer);
    
          Set((TempoWhirlpool), 10, 1, 1, Timer, Verso);
    
          if (btn_Press(Button_enter, 50)) {
            Procedo     = false;
            Menu_2();
          }
          
          if (btn_Press(Button_start, 50)) {
            Procedo     = false;
            Menu_2();
            TempoWhirlpool = AppoggioTempo;
          }
        }
        TimeLeft = TempoWhirlpool * 60; 
      }
      
      
      if (TimeLeft <= 0) break;  
      CntDwn(TimeLeft);

      quit_mode(f_Whirlpool);
      if (!(f_Whirlpool))   break;
    }
  }
  
  //if (b_Whirlpool == 2) Cooling();
  if (r_set(27) == 2) Cooling();  
}

void set_PID () {
  boolean pidLoop = true;
  byte    pidSet;
  byte    Min;
  pidLoop = false;
  
  //Indirizzo di memorizzazione corrisponde al ciclo
  
  for(byte i = 0; i < 9; i++) {
 
    //r_set(pidSet, i);
    pidSet = r_set(i);
    
    if (i == 8 && UseGAS == 0) pidLoop = false;
    else                       pidLoop = true;

    while (pidLoop) {
      Menu_3_1_x(i);
      
      LeggiPulsante(Verso, Timer);
      
      PidSet(pidSet, i);
/*      
      if (i == 5) {
        //r_set(Min, i - 1);
        Min = r_set(i - 1);
        Set(pidSet, p_PID[i * 3], Min + 2, p_PID[i * 3 + 2], Timer, Verso); 
      } else {
        Set(pidSet, p_PID[i * 3], p_PID[i * 3 + 1], p_PID[i * 3 + 2], Timer, Verso); 
      }
*/

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
  
    // SALTA BLOCCHI POMPA SE SENSORE ESTERNO  
    if ((i >= 16 || i <= 19) && SensorType == 1) unitLoop = false;
    else                                         unitLoop = true;
    
    // SALTA TEMPO IODIO SE SKIP IODIO ATTIVO
    if (i == 26 && EEPROM.read(i - 1) == 1)      unitLoop = false;
    else                                         unitLoop = true;

/*    
    if (i != 12 && i != 20)                      r_set(unitSet, i);
    else {
      if (ScaleTemp == 0)                        r_set(unitSet, i);
      else                                       r_set(unitSet, i + 1);
*/

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
            //setAddr = 22;
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

void set_Stages () { 
  
  boolean TempTimeLoop    = false;
  boolean autoLoop        = false;
  boolean Control         = true;
  boolean FlagStart       = false;
  float   stagetempSet;
  int     w_stagetempSet;
  byte    stagetimeSet;

  float Min, Max,Step;
  float temp_stageTemp = 0.0;
  float DeltaTemp;
  boolean MashInFLAG   = true;
  float MashInTemp     = 0;

  StageAddr = 32;
  
  for (byte i = 0; i < 8; i++) { // loops for the number of stages
    TempTimeLoop = true;
    autoLoop     = true;

    if (ScaleTemp == 0) {
      
      //read_set(stagetempSet, StageAddr);
      //if (i > 0) read_set(temp_stageTemp, StageAddr - 5);
      //temp_stageTemp = temp_stageTemp / 16.0;
     
      stagetempSet = r_set_float(StageAddr);
      if (i > 0) temp_stageTemp = r_set_float(StageAddr - 5) / 16.0;

      Max       = p_C[i * 3];
      Min       = p_C[i * 3 + 1];
      Step      = p_C[i * 3 + 2];
      DeltaTemp = 3.5;
    } else { 
      //read_set(stagetempSet, StageAddr + 2);
      //if (i > 0) read_set(temp_stageTemp, StageAddr - 3);
      //temp_stageTemp = temp_stageTemp / 16.0;
      
      stagetempSet = r_set_float(StageAddr + 2);
      if (i > 0) temp_stageTemp = r_set_float(StageAddr - 3) / 16.0;
      
      Max       = p_F[i * 3];
      Min       = p_F[i * 3 + 1];
      Step      = p_F[i * 3 + 2];
      DeltaTemp = 6.3;    
    }
    if (!(MashInFLAG)) DeltaTemp = 0;

    stagetempSet = stagetempSet / 16.0;
    
    if (i > 0) {      
      if (Max < temp_stageTemp - DeltaTemp) {//La temepratura precedente e' sopra i limiti dello stage   
        //Salta lo Stage
        if (i != 6 && i != 7) { 
          stagetempSet   = temp_stageTemp * 16;
          w_stagetempSet = word(stagetempSet);

          if (ScaleTemp == 0) {
            // Salva il settaggio in °C
            save_set(StageAddr, w_stagetempSet);

            // Salva il settaggio in °F
            ConvertiCtoF(stagetempSet); 
            w_stagetempSet = word(stagetempSet);
            save_set(StageAddr + 2, w_stagetempSet);
          } else {
            // Salva il settaggio in °F
            save_set(StageAddr + 2, w_stagetempSet);

            // Salva il settaggio in °C
            ConvertiFtoC(stagetempSet);
            w_stagetempSet = word(stagetempSet);
            save_set(StageAddr, w_stagetempSet);
          }          
          s_set(StageAddr + 4, 0);
          autoLoop     = false;
          TempTimeLoop = false;
          FlagStart    = false;
        }
      } else {
        
        if (MashInFLAG) {
          if (MashInTemp - DeltaTemp >= Min) Min = MashInTemp - DeltaTemp;
        } else {
          if (Min < temp_stageTemp) Min = temp_stageTemp;
        }
      } 
    }
    while (TempTimeLoop) {  // loops for temp adjust
      Menu_3_3_x(i);
      StageSet(stagetempSet);

      quit_mode(TempTimeLoop);
      if (!TempTimeLoop) {
        Control = false;
        return;
      }

      if (btn_Press(Button_start, 50)) {
        
        if (ScaleTemp == 0) stagetempSet = p_C[i * 3 + 1];
        else                stagetempSet = p_F[i * 3 + 1];
        
        if (Min > stagetempSet) stagetempSet = Min;
        
        FlagStart = true;
      }
     
      LeggiPulsante(Verso, Timer);

      Set(stagetempSet, Max, Min, Step, Timer, Verso);
      
      // Vengono obbligati i tempi di: Mash In - A-Amilasi - Mash Out
      if (i == 0 || i == 6 || i == 7 ) FlagStart = false;      

      if (FlagStart || (btn_Press(Button_enter, 50))) {
        if (i == 0) MashInTemp = stagetempSet;
    
        stagetempSet   = stagetempSet * 16;
        w_stagetempSet = word(stagetempSet);

        if (ScaleTemp == 0) {
          // Salva il settaggio in °C
          save_set(StageAddr, w_stagetempSet);

          // Salva il settaggio in °F
          ConvertiCtoF(stagetempSet); 
        
          w_stagetempSet = word(stagetempSet);
          save_set(StageAddr + 2, w_stagetempSet);
        } else {
          // Salva il settaggio in °F
          save_set(StageAddr + 2, w_stagetempSet);

          // Salva il settaggio in °C
          ConvertiFtoC(stagetempSet);
          w_stagetempSet = word(stagetempSet);
          save_set(StageAddr, w_stagetempSet);
        }          
        if (FlagStart) {//      Viene memorizzato il tempo a 0
          
          s_set(StageAddr + 4, 0);
          autoLoop = false;
        }
        TempTimeLoop = false;
        FlagStart    = false;
      }
    }

    if (autoLoop)TempTimeLoop = true;

    //r_set(stagetimeSet, StageAddr + 4);
    stagetimeSet = r_set(StageAddr + 4);
    
    while (TempTimeLoop) { // loops to adjust time setting
      if (i != 0) {
        TimeSet(stagetimeSet);

        quit_mode(TempTimeLoop);
        if (!TempTimeLoop) {
          Control = false;
          return;
        }

        LeggiPulsante(Verso, Timer);   
        Set(stagetimeSet, 140, 1, 1, Timer, Verso);
       
        if (btn_Press(Button_enter, 50)) {
          if (stagetimeSet > 0) {
            s_set(StageAddr + 4, stagetimeSet);
            if (MashInFLAG) MashInFLAG = false;
            TempTimeLoop = false;
          }
        }
      }
      else { //Se si tratta del Mash In setta il tempo a 5 (fittizio)
        //EEPROM.write(StageAddr + 4, 5);
        s_set(StageAddr + 4, 5);
        
        TempTimeLoop = false;
      }
    } 
    StageAddr += 5;
  }
  if (Control) set_hops();
  Clear_2_3();
}  


void Congruita(byte& numRicetta, byte Verso) {  
  if (EEPROM.read(89 + numRicetta) == 0) {
    boolean Controllo = true;
    
    while(Controllo) {
      if (Verso == 1) {
        if (numRicetta < 10) 
          numRicetta++;
        else 
          Controllo = false; 
      }
        
      if (Verso == 2) {
        if (numRicetta > 1) 
          numRicetta--;
        else            
          Controllo = false;
      }
      
      if (EEPROM.read(89 + numRicetta) == 1) { 
        Controllo = false;
      }
    }
  }
}

void loadRecipe() {
  byte numRicetta     = 0;  
  boolean ricettaLoop = true;
  byte RicettaUp, RicettaDwn;

  RicettaUp  = 0;
  RicettaDwn = 0;
  
  for (byte i = 90; i < 100; i++) {//Assegna il limite di ricette registrate 
    if (EEPROM.read(i) == 1) {
      RicettaUp = (i - 89);
      if (RicettaDwn == 0) RicettaDwn = RicettaUp;
    }
  }
  if (RicettaUp == 0) {
    NoRecipe();
    return;
  }
  
  // byte NomeRicetta[10];
  byte pos = 0; 
  
  for (byte i = RicettaDwn + 89; i < RicettaUp + 89 + 1; i++) {//Trova la prima ricetta libera
    numRicetta = i - 89;
    
    while (ricettaLoop) {
      Ricetta(numRicetta,0);
      LeggiPulsante(Verso, Timer);
      Set(numRicetta, RicettaUp, RicettaDwn, 1, Timer, Verso);
      
      for (pos = 0; pos < 10; pos++) {
        LCD_NomeRicetta(pos, EEPROM.read(620 + pos + ((numRicetta - 1) * 10)));
      }
      
      Congruita(numRicetta, Verso);
      if (btn_Press(Button_enter, 50)) {
        Menu_3();
        Menu_3_4();
        ricettaLoop = false;  
        i = 100;
      }
      if (btn_Press(Button_start, 50)) {
        Clear_2_3();
        LeggoRicetta(numRicetta);
        
        int Da;
  
        //Parametri Ricetta
        Da = 100 + ((numRicetta - 1) * 52);
        for (int j = 30; j < 82; j++) { 
          //save_set (j, (byte)EEPROM.read(Da));
          s_set (j, EEPROM.read(Da));
          Da++;
        }
        ricettaLoop = false;  
      }
    }
  }
}


void saveRecipe() {
  //boolean saverecipeLoop;
  byte numRicetta = 0;
   
  for (byte i = 90; i < 100; i++) {//Trova la prima ricetta libera
    if (EEPROM.read(i) == 0) {
      numRicetta = (i - 89);
      i = 99;
    }
  }
  
  if (numRicetta == 0) MemoriaPiena();
  else {
    // Spc 32
    // 0-9 da  48 a  57
    // A-Z da  65 a  90
    // a-z da  97 a 122
  
    byte NomeRicetta[10];
    byte pos = 0; 
    NomeRicetta[pos] = 97;
  
    Clear_2_3();
  
    Ricetta(numRicetta,1);

    while (pos < 10) {
      LCD_NomeRicetta(pos, NomeRicetta[pos] );
      lcd.blink();
    
      LeggiPulsante(Verso, Timer);
      Set(NomeRicetta[pos], 122, 32, 1, Timer, Verso);
    
      if ((NomeRicetta[pos] > 32 && NomeRicetta[pos] < 48) && Verso == 1) NomeRicetta[pos] = 48;
      if ((NomeRicetta[pos] > 57 && NomeRicetta[pos] < 97) && Verso == 1) NomeRicetta[pos] = 97;
    
      if ((NomeRicetta[pos] < 97 && NomeRicetta[pos] > 57) && Verso == 2) NomeRicetta[pos] = 57;
      if ( NomeRicetta[pos] < 48 && Verso == 2)                           NomeRicetta[pos] = 32;
      
      if (btn_Press(Button_enter, 50)) {
        pos++;
        NomeRicetta[pos] = 97;
      }
    
      if ((digitalRead(Button_dn) == 0) && (digitalRead(Button_up) == 0)) {
        delay(350);
        if ((digitalRead(Button_dn) == 0) && (digitalRead(Button_up) == 0)) {
          for (byte j = pos; j < 10; j++) {
            NomeRicetta[pos] = 32;
            pos++;
          }pos=9;
        }
      }    
      
      if (btn_Press(Button_start, 50)) {
        if (pos > 0) pos--;
      }
    }
  
    lcd.noBlink();
  
    SalvataggioRicetta (numRicetta);
  
    //wait_for_confirm(saverecipeLoop, 2, 2, 2);
  
    if (wait_for_confirm(2, 2, 2) == false) {
      Menu_3();
      Menu_3_4();
      return;
    } else {
      SalvaRicetta();
      
      int Da;
  
      //Parametri Ricetta
      Da = 100 + ((numRicetta - 1) * 52);
      for (byte j = 30; j < 82; j++) { 
        //save_set (Da, (byte)EEPROM.read(j));
        s_set (Da, EEPROM.read(j));
        Da++;
      }
      
      
      //Nome Ricetta
      for (pos = 0; pos < 10; pos++) {
        //save_set(620 + pos + ((numRicetta - 1) * 10), NomeRicetta[pos]);
        s_set(620 + pos + ((numRicetta - 1) * 10), NomeRicetta[pos]);
      }
      //Byte di Controllo
      //save_set(89 + numRicetta, (byte)1);
      s_set(89 + numRicetta, 1);
    }
  }
}

void deleteRecipe() {
  byte numRicetta = 0;
  boolean ricettaLoop = true;
  byte RicettaUp, RicettaDwn;

  RicettaUp  = 0;
  RicettaDwn = 0;
  
  for (byte i = 90; i < 100; i++) {//Assegna il limite di ricette registrate 
    if (EEPROM.read(i) == 1) {
      RicettaUp = (i - 89);
      if (RicettaDwn == 0) RicettaDwn = RicettaUp;
    }
  }
  if (RicettaUp == 0) {
    NoRecipe();
    return;
  }
  
  for (byte i = RicettaDwn + 89; i < RicettaUp + 89 + 1; i++) { //Trova la prima ricetta libera
    numRicetta = i - 89;
    
    while (ricettaLoop) {
      CancelloRicetta(numRicetta);
      LeggiPulsante(Verso, Timer);
      Set(numRicetta, RicettaUp, RicettaDwn, 1, Timer, Verso);
      
      Congruita(numRicetta, Verso);
      if (btn_Press(Button_enter, 50)) {
        Menu_3(); 
        Menu_3_4();
        ricettaLoop = false;  
        i = 100;
      }
      if (btn_Press(Button_start, 50)) {
        Cancellazione(numRicetta);
        //save_set(89 + numRicetta, (byte)0);
        s_set(89 + numRicetta, 0);
        ricettaLoop = false;  
        i = 100;  
      }
    }
  }
}

void initializeRecipe() {
  //boolean initialize;
  
  Inizializzazione();
  
  //wait_for_confirm(initialize, 2, 2, 2);
  
  if (wait_for_confirm(2, 2, 2) == false) {
    Menu_3();
    Menu_3_4();
    return;
  } else {
    Inizializza();      	
    for (byte i = 1; i < 11; i++) {
      //save_set(89 + i,(byte)0);
      s_set(89 + i, 0);
    }	
  }
}

void RecipeMenu() {
  boolean recipeLoop = true;
  byte recipeMenu = 0;
  
  while (recipeLoop) {
    switch (recipeMenu) { 
      
      case(0):
        Menu_3_4_1();
        if (btn_Press(Button_start, 50)) recipeLoop = false;
        if (btn_Press(Button_dn, 50))    recipeMenu = 1;
        if (btn_Press(Button_enter, 50)) loadRecipe(); 
        break;

      case(1):
        Menu_3_4_2();
        if (btn_Press(Button_start, 50)) recipeLoop = false;
        if (btn_Press(Button_up, 50))    recipeMenu = 0;
        if (btn_Press(Button_dn, 50))    recipeMenu = 2;
        if (btn_Press(Button_enter, 50)) saveRecipe(); 
        break;

      case(2):
        Menu_3_4_3();
        if (btn_Press(Button_start, 50)) recipeLoop = false;
        if (btn_Press(Button_up, 50))    recipeMenu = 1;
        if (btn_Press(Button_dn, 50))    recipeMenu = 3;
        if (btn_Press(Button_enter, 50)) deleteRecipe();
        break;

      case(3):
        Menu_3_4_4();
        if (btn_Press(Button_start, 50)) recipeLoop = false;
        if (btn_Press(Button_up, 50))    recipeMenu = 2;
        if (btn_Press(Button_enter, 50)) initializeRecipe();
        break;
    }        
  }Menu_3_4();
}


void set_hops () {
  boolean hopLoop;
  byte hopSet;
  //r_set(nmbrHops, 72);
  nmbrHops = r_set(72);
  
  byte TimeUp;
  byte TimeDwn;
  
  blhpAddr = 73;

  hopLoop = true;

  while (hopLoop) {
    Menu_3_3_8();
    NumHops(nmbrHops);
    
    LeggiPulsante(Verso, Timer);
    
    Set(nmbrHops, 10, 0, 1, Timer, Verso);
    
    quit_mode(hopLoop);
    if (!hopLoop) return;

    if (btn_Press(Button_enter, 50)) {
      s_set(72, nmbrHops);
      hopLoop = false;
    }
  }

  nmbrHops+=1;

  for (byte i = 0; i < nmbrHops; i++) {
    hopLoop = true;
    //r_set(hopSet,blhpAddr);
    hopSet = r_set(blhpAddr);
    
    while (hopLoop){
      if (i == 0) {
        Menu_3_3_9();
        TimeHops(int(hopSet));
      }
      else {
        Menu_3_3_10(i);
        TimeHops(int(hopSet));
      }
     
      quit_mode(hopLoop);
      if (!hopLoop) return;

      if (i == 0) {
        TimeUp  = 180;
        TimeDwn =  30;
      } else {
        TimeUp = r_set(blhpAddr - 1);
        if (i != 1) TimeUp = TimeUp - 1;
        
        //if (i == 1) r_set(TimeUp, blhpAddr - 1);
        //if (i == 1) TimeUp = r_set(blhpAddr - 1);
        //else        TimeUp = EEPROM.read(blhpAddr - 1) - 1;
        TimeDwn =  0;
      }

      LeggiPulsante(Verso, Timer);
      Set(hopSet, TimeUp, TimeDwn, 1, Timer, Verso);
      
      if (btn_Press(Button_enter, 50)) {
        s_set(blhpAddr, hopSet);
        hopLoop = false;
      }
    } blhpAddr += 1;
  }
  
  for (byte i = nmbrHops; i < 11; i++) {
    s_set(blhpAddr, (byte)0);
    blhpAddr += 1;
  }
  
  Clear_2_3();
}



void TestRam() {  
#if TestMemoria == true
  Menu_4_1();
#endif
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
      if (btn_Press(Button_dn, 50))    setupMenu = 2;
      if (btn_Press(Button_enter, 50)) set_Unit();  
      break;

      case(2):
      Menu_3_3();
      if (btn_Press(Button_start, 50)) setupLoop = false;
      if (btn_Press(Button_up, 50))    setupMenu = 1;
      if (btn_Press(Button_dn, 50))    setupMenu = 3;
      if (btn_Press(Button_enter, 50)) set_Stages();
      break;

      case(3):
      Menu_3_4();
      if (btn_Press(Button_start, 50)) setupLoop = false;
      if (btn_Press(Button_up, 50))    setupMenu = 2;
      if (btn_Press(Button_enter, 50)) RecipeMenu();
      break;     
    }
  } lcd.clear();
}   
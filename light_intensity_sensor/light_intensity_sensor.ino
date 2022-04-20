#include <LiquidCrystal_I2C.h> // Library for LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
#define LDR_PIN A0 // analog pin0 , the sensor output is connected here.
#define MAX_ADC_READING           1023
#define ADC_REF_VOLTAGE           5.0
#define REF_RESISTANCE            9470  // measure this for best results
#define LUX_CALC_SCALAR           12518931
#define LUX_CALC_EXPONENT         -1.405
int ldrRawData;
float resistorVoltage, ldrVoltage;
float ldrResistance;
float ldrLux;
float safe_limit = 2000; //danger level, anything higher triggers an alarm (adjustable)

void setup() {
  Serial.begin(9600);
  //Initializing the LCD
  lcd.init();
  lcd.clear();
  //  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Light intensity detector");
  lcd.setCursor(0, 1);
  lcd.print("SBME24 Team 13");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Perform the analog to digital conversion
  ldrRawData = analogRead(LDR_PIN);

  // RESISTOR VOLTAGE_CONVERSION
  // Convert the raw digital data back to the voltage that was measured on the analog pin
  resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;

  // voltage across the LDR is the 5V supply minus the 5k resistor voltage
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;

  // LDR_RESISTANCE_CONVERSION
  // resistance that the LDR would have for that voltage
  ldrResistance = ldrVoltage / resistorVoltage * REF_RESISTANCE;

  // LDR_LUX
  // Change the code below to the proper conversion from ldrResistance to
  // ldrLux
  ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);

  if (ldrLux > safe_limit) {
    lcd.clear();
    lcd.setCursor(0, 0); // set cursor to column 0, line 1
    lcd.print("Alarm! high Int.");
    delay(1000);
  } else {
    // Print out the LDR Illuminance to the LCD
    lcd.setCursor(0, 0); // set cursor to column 0, line 1
    lcd.print("Lux Meter");

    lcd.setCursor(0, 1); // set cursor to column 0, line 2
    lcd.print(ldrLux);
    lcd.setCursor(7, 1); // set cursor to column 4, line 1
    lcd.print(" lux");
    delay(1000);
  }
}

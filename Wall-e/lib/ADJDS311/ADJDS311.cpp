/*
  Library for using the ADJD-S311 and ADJD-S371 color sensors.
  Created by Adam Meyer of bildr Jan 28th 2012
  Released as MIT license

  Modded by Jean-Samuel Lauzon & Serge Caron for Arduino-X, october 2018.
  Example on how to use the sensor at the end of the .h file.
*/

#include <Arduino.h>
#include <ADJDS311.h>

ADJDS311::ADJDS311(int led_pin) {
	_led_pin = led_pin;
}



/* calibrate() - This functions calibrate the sensor's gains.
   Make sure the sensor faces a white surface at focal point when calling this function.
*/
void ADJDS311::calibrate(){
	delay(2); //wait for LED to come full brintness if used
	calibrateColor();  // This calibrates R, G, and B int registers
	calibrateClear();  // This calibrates the C int registers
	calibrateCapacitors();  // This calibrates the RGB, and C cap registers
}

void ADJDS311::ledOn(){
	digitalWrite(_led_pin, HIGH);
}

void ADJDS311::ledOff(){
	digitalWrite(_led_pin, LOW);
}


/* init() - Initialize the sensor with some preset values.
   You should calibrate the sensor afterward, the S311 and S371's gains are different.
*/
void ADJDS311::init(){
	pinMode(_led_pin, OUTPUT);  // Set the sensor's LED as output
	
	Wire.begin();
	delay(1);  // Wait for ADJD reset sequence
	
	// write sensor gain registers, CAP_... to select number of capacitors.
  // all those values where originally at 15 in the Armus example
  setcapacitor(9, 9, 2, 5);

	//Write sensor gain registers INT_...
  setintegrationtime(2048, 2048, 2048, 2048);
}

/* setintegrationtime() - This function sets all integration registers of the
   ADJD. Integration times must be <= 4095
*/
void ADJDS311::setintegrationtime(unsigned int ired, unsigned int igreen, unsigned int iblue, unsigned int iclear)
{
  // save new values in object
	// selected integration time value must be <= 4095
	colorInt[RED] = ired;
	colorInt[GREEN] = igreen;
	colorInt[BLUE] = iblue;
	colorInt[CLEAR] = iclear;

	// Now write sensor gain registers INT_...
	writeRegister((unsigned char)colorInt[RED], INT_RED_LO);
	writeRegister((unsigned char)((colorInt[RED] & 0x1FFF) >> 8), INT_RED_HI);
	writeRegister((unsigned char)colorInt[BLUE], INT_BLUE_LO);
	writeRegister((unsigned char)((colorInt[BLUE] & 0x1FFF) >> 8), INT_BLUE_HI);
	writeRegister((unsigned char)colorInt[GREEN], INT_GREEN_LO);
	writeRegister((unsigned char)((colorInt[GREEN] & 0x1FFF) >> 8), INT_GREEN_HI);
	writeRegister((unsigned char)colorInt[CLEAR], INT_CLEAR_LO);
	writeRegister((unsigned char)((colorInt[CLEAR] & 0x1FFF) >> 8), INT_CLEAR_HI);
}

/* setcapacitor() - This function sets all capacitor registers of the
   ADJD. Capacitor values must be between 0 and 15
*/
void ADJDS311::setcapacitor(unsigned int cred, unsigned int cgreen, unsigned int cblue, unsigned int cclear)
{
  // save new values in object
  // selected values must be between 0 and 15
	colorCap[RED] = cred;
	colorCap[GREEN] = cgreen;
	colorCap[BLUE] = cblue;
	colorCap[CLEAR] = cclear;
	
	// write sensor gain registers, CAP_... to select number of capacitors.
	writeRegister(colorCap[RED] & 0xF, CAP_RED);
	writeRegister(colorCap[GREEN] & 0xF, CAP_GREEN);
	writeRegister(colorCap[BLUE] & 0xF, CAP_BLUE);
	writeRegister(colorCap[CLEAR] & 0xF, CAP_CLEAR);
}

/* calibrateClear() - This function calibrates the clear integration registers
of the ADJD-S311.
*/
int ADJDS311::calibrateClear(){
	int gainFound = 0;
	int upperBox=4096;
	int lowerBox = 0;
	int half;
	
	while (!gainFound){
		half = ((upperBox-lowerBox)/2)+lowerBox;
		//no further halfing possbile
		
		if (half==lowerBox){
			gainFound=1;
		}else{
			writeInt(INT_CLEAR_LO, half);
			performMeasurement();
			int halfValue = readRegisterInt(DATA_CLEAR_LO);
		
			if (halfValue>1000){
				upperBox=half;
			}else if (halfValue<1000){
				lowerBox=half;
			}else{
				gainFound=1;
			} 
		}
	}
	return half;
}

/* calibrateColor() - This function clalibrates the RG and B 
integration registers.
*/
int ADJDS311::calibrateColor(){
  int gainFound = 0;
  int upperBox=4096;
  int lowerBox = 0;
  int half;
  
  while (!gainFound)
  {
    half = ((upperBox-lowerBox)/2)+lowerBox;
    //no further halfing possbile
    if (half==lowerBox)
    {
      gainFound=1;
    }
    else {
      writeInt(INT_RED_LO, half);
      writeInt(INT_GREEN_LO, half);
      writeInt(INT_BLUE_LO, half);

      performMeasurement();
      int halfValue = 0;

      halfValue=max(halfValue, readRegisterInt(DATA_RED_LO));
      halfValue=max(halfValue, readRegisterInt(DATA_GREEN_LO));
      halfValue=max(halfValue, readRegisterInt(DATA_BLUE_LO));

      if (halfValue>1000) {
        upperBox=half;
      }
      else if (halfValue<1000) {
        lowerBox=half;
      }
      else {
        gainFound=1;
      }
    }
  }
  return half;
}

/* calibrateCapacitors() - This function calibrates each of the RGB and C
capacitor registers.
*/
void ADJDS311::calibrateCapacitors(){
  int  calibrationRed = 0;
  int  calibrationBlue = 0;
  int  calibrationGreen = 0;
  int calibrated = 0;

  //need to store detect better calibration
  int oldDiff = 5000;

  while (!calibrated){
    // sensor gain setting (Avago app note 5330)
    // CAPs are 4bit (higher value will result in lower output)
    writeRegister(calibrationRed, CAP_RED);
    writeRegister(calibrationGreen, CAP_GREEN);
    writeRegister(calibrationBlue, CAP_BLUE);

    // int colorGain = _calibrateColorGain();
    int colorGain = readRegisterInt(INT_RED_LO);
    writeInt(INT_RED_LO, colorGain);
    writeInt(INT_GREEN_LO, colorGain);
    writeInt(INT_BLUE_LO, colorGain);

    int maxRead = 0;
    int minRead = 4096;
    int red   = 0;
    int green = 0;
    int blue  = 0;
    
    for (int i=0; i<4 ;i ++)
    {
      performMeasurement();
      red   += readRegisterInt(DATA_RED_LO);
      green += readRegisterInt(DATA_GREEN_LO);
      blue  += readRegisterInt(DATA_BLUE_LO);
    }
    red   /= 4;
    green /= 4;
    blue  /= 4;

    maxRead = max(maxRead, red);
    maxRead = max(maxRead, green);
    maxRead = max(maxRead, blue);

    minRead = min(minRead, red);
    minRead = min(minRead, green);
    minRead = min(minRead, blue);

    int diff = maxRead - minRead;

    if (oldDiff != diff)
    {
      if ((maxRead==red) && (calibrationRed<15))
        calibrationRed++;
      else if ((maxRead == green) && (calibrationGreen<15))
        calibrationGreen++;
      else if ((maxRead == blue) && (calibrationBlue<15))
        calibrationBlue++;
    }
    else
      calibrated = 1;
      
    oldDiff=diff;

    int rCal = calibrationRed;
    int gCal = calibrationGreen;
    int bCal = calibrationBlue;
  }
  
}

/* writeInt() - This function writes a 12-bit value
to the LO and HI integration registers */
void ADJDS311::writeInt(int address, int gain){
  if (gain < 4096) 
  {
    byte msb = gain >> 8;
    byte lsb = gain;

    writeRegister(lsb, address);
    writeRegister(msb, address+1);
  }
}

/* performMeasurement() - This must be called before
reading any of the data registers. This commands the
ADJD-S311 to perform a measurement, and store the data
into the data registers.*/
void ADJDS311::performMeasurement(){  
  writeRegister(0x01, 0x00); // start sensing
  while(readRegister(0x00) != 0)
    ; // waiting for a result
}


uint16_t ADJDS311::readRed(){
  performMeasurement();
	return readRegisterInt(DATA_RED_LO);
}
uint16_t ADJDS311::readGreen(){
  performMeasurement();
	return readRegisterInt(DATA_GREEN_LO);
}
uint16_t ADJDS311::readBlue(){
  performMeasurement();
	return readRegisterInt(DATA_BLUE_LO);
}
uint16_t ADJDS311::readClear(){
  performMeasurement();
	return readRegisterInt(DATA_CLEAR_LO);
}

RGBC ADJDS311::read(){
	//Read data registers and return a RGBC var
	RGBC color = RGBC();
	
	performMeasurement();
	
	color.red = readRegisterInt(DATA_RED_LO);
	color.green = readRegisterInt(DATA_GREEN_LO);
	color.blue = readRegisterInt(DATA_BLUE_LO);
	color.clear = readRegisterInt(DATA_CLEAR_LO);
	
	return color;
}

/* getOffset() - This function performs the offset reading
and stores the offset data into the colorOffset[] array.
You can turn on data trimming by uncommenting out the 
writing 0x01 to 0x01 code.
*/
void ADJDS311::getOffset(){
  digitalWrite(_led_pin, LOW);  // turn LED off
  delay(10);  // wait a tic
  writeRegister(0x02, 0x00); // start sensing
  while(readRegister(0x00) != 0)
    ; // waiting for a result
  //writeRegister(0x01, 0x01);  // set trim
  //delay(100);
  for (int i=0; i<4; i++)
    colorOffset[i] = (signed char) readRegister(OFFSET_RED+i);
  digitalWrite(_led_pin, HIGH);
}

// Write a byte of data to a specific ADJD-S311 address
void ADJDS311::writeRegister(unsigned char data, unsigned char address){
  Wire.beginTransmission(ADJD_S311_ADDRESS);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

// read a byte of data from ADJD-S311 address
unsigned char ADJDS311::readRegister(unsigned char address){
  unsigned char data;
  
  Wire.beginTransmission(ADJD_S311_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(ADJD_S311_ADDRESS, 1);
  while (!Wire.available())
    ;  // wait till we can get data
  
  return Wire.read();
}

// Write two bytes of data to ADJD-S311 address and addres+1
int ADJDS311::readRegisterInt(unsigned char address){
  return readRegister(address) + (readRegister(address+1)<<8);
}

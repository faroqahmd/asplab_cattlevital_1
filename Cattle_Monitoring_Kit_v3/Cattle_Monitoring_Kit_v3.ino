#include <SoftwareSerial.h>  
#include <EEPROM.h>
#include "max6675.h"
#include "ph_grav.h" 
#include "stdio.h"                          
#include "AltSoftSerial.h"
 


//  SM //

//#define rx1 2                                          //define what pin rx is going to be
//#define tx1 3 
//SoftwareSerial portOne(rx1, tx1);

AltSoftSerial altser;
const int mybaud = 9600;
char inchar2 = 0; 
int i= 0;
int j= 0;
int k= 0;
unsigned long long int rec_pH =0;
unsigned long long int rec_TH =0;
unsigned long long int rec_Cond =0;

//   EC    //

#define rx2 4                                          //define what pin rx is going to be
#define tx2 5                                          //define what pin tx is going to be
  char inchar = 0;
  char sensorstring_array[30];                        //we make a char array
  char *EC;                                           //char pointer used in string parsing
  char *TDS;                                          //char pointer used in string parsing
  char *SAL;                                          //char pointer used in string parsing
  char *GRAV;                                         //char pointer used in string parsing
  float f_ec; 
  float GRAV_f =0;
SoftwareSerial portTwo(rx2, tx2);
String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product


////    TH   //
int thermoDO = 6;
int thermoCS = 7;
int thermoCLK = 10;
float temp_TH = 0;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);


//    PH    //
Gravity_pH pH = Gravity_pH(A0);
uint8_t user_bytes_received = 0;                
const uint8_t bufferlen = 32;                   
char user_data[bufferlen];                     
float volt =0;
float pH_value =0;
/////////////



unsigned long current_mills=0;
unsigned long instant_mills=0;
unsigned long time_mills=2000;
unsigned long time_elapsed=0;

//void parse_cmd(char* string)
//{                   
//  strupr(string);                                
//  if (strcmp(string, "CAL,7") == 0) 
//  {       
//    pH.cal_mid();                                
//    Serial.println("MID CALIBRATED");
//  }
//  else if (strcmp(string, "CAL,4") == 0) 
//  {            
//    pH.cal_low();                                
//    Serial.println("LOW CALIBRATED");
//  }
//  else if (strcmp(string, "CAL,10") == 0) 
//  {      
//    pH.cal_high();                               
//    Serial.println("HIGH CALIBRATED");
//  }
//  else if (strcmp(string, "CAL,CLEAR") == 0) 
//  { 
//    pH.cal_clear();                              
//    Serial.println("CALIBRATION CLEARED");
//  }
//}





void setup()
{
  // put your setup code here, to run once:
    Serial.begin(9600);
    while (!Serial) 
    {
    ; // wait for serial port to connect. Needed for native USB port only
    }
        
    portTwo.begin(9600);
    altser.begin(9600);
    inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
    sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
    
//    Serial.println(F("Use commands \"CAL,7\", \"CAL,4\", and \"CAL,10\" to calibrate the circuit to those respective values"));
//    Serial.println(F("Use command \"CAL,CLEAR\" to clear the calibration"));
    
    if (pH.begin()) 
    {                                     
    Serial.println("Loaded EEPROM");
    }
    delay(500);

}
void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}

void get_BT_data(void);
void get_EC_data(void);
void get_TH_data(void);
void get_pH_data(void);
void update_EC_data(void);
void print_EC_data(void);
void print_TH_data(void);
void print_pH_data(void);




void loop()
{

   instant_mills =  millis();

  //  EC // 

// Software Serial //
// EC //

////
//  if (input_string_complete == true)
//  {                //if a string from the PC has been received in its entirety
//    portTwo.print(inputstring);                      //send that string to the Atlas Scientific product
//    portTwo.print('\r');                             //add a <CR> to the end of the string
//    inputstring = "";                                 //clear the string
//    input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the PC
//  }
  ///////////////
  
// Software Serial //
  // pH
  
//if (Serial.available() > 0) {                                                      
//    user_bytes_received = Serial.readBytesUntil(13, user_data, sizeof(user_data));   
//  }
//
//  if (user_bytes_received) {                                                      
//    parse_cmd(user_data);                                                          
//    user_bytes_received = 0;                                                        
//    memset(user_data, 0, sizeof(user_data));                                         
//  }



//////////////

//Serial.println(inchar2);
//Serial.println(portOne.available());
//Serial.println();


//if (Serial.available()) {
//    int inByte = Serial.read();
//    Serial.write(inByte);
//  }
//////////////////////////////

  

//  SENSOR RECEIVE //

   get_EC_data();
   get_TH_data();
   get_pH_data();

/////////////////////////
   get_BT_data();

   if (inchar2 == '1')
   {
    
    altser.print("@");
    altser.print(","); 
    altser.print(i);
    altser.print(",");
    altser.print(pH_value);
//    altser.println("@"); 
    i++;
    inchar2 =0;
    
   }

   if (inchar2 == '2')
   {
    
    altser.print("#");
    altser.print(","); 
    altser.print(j);
    altser.print(",");
    altser.print(temp_TH);
//    altser.println("@"); 
    j++;
    inchar2 =0;
    
   }

   if (inchar2 == '3')
   {
    
    altser.print("$"); 
    altser.print(",");
    altser.print(k);
    altser.print(",");
    altser.print(EC);                                 //this is the EC value
    altser.print(',');
    altser.print(TDS);                                //this is the TDS value
    altser.print(',');
    altser.print(SAL);                                //this is the salinity value
    altser.print(',');
    altser.print(GRAV_f);
//    altser.println("@"); 
    k++;
    inchar2 =0;
    
   }


// if ((instant_mills - current_mills) >= time_mills)
// {
//
////  portOne.print("$"); 
////  portOne.print(string1);
////  portOne.print(',');
////  portOne.print(string2); 
////  portOne.print(',');
////  portOne.print(string3);                                 //this is the EC value
////  portOne.print(',');
////  portOne.print(string4);                                //this is the TDS value
////  portOne.print(',');
////  portOne.print(string5);                                //this is the salinity value
////  portOne.print(',');
////  portOne.print(string6);                               //this is the specific gravity
////   
//////  portOne.print("12,13");  
////  portOne.println("@");
//
//
// 
//  altser.print("$"); 
////  altser.print(temp_TH);
////  altser.print(',');
//  altser.print(pH_value); 
////  altser.print(',');
////  altser.print(EC);                                 //this is the EC value
////  altser.print(',');
////  altser.print(TDS);                                //this is the TDS value
////  altser.print(',');
////  altser.print(SAL);                                //this is the salinity value
////  altser.print(',');
////  altser.print(GRAV_f);                               //this is the specific gravity
////   altser.print(',');
//////  altser.print("12,13");  
//  altser.println("@"); 
//
//
//   
//  Serial.print("$"); 
//  print_TH_data();
//  print_pH_data();
//  print_EC_data();
//  Serial.print("12,13");  
//  Serial.println("@");
// 
//  current_mills=  millis();
// }
//
}





void get_BT_data(void)
{
  if (altser.available()) 
  {                     //if we see that the Atlas Scientific product has sent a character
                            //add the char to the var called sensorstring
//     Serial.println(" read   " );
     
     inchar2 = (char)altser.read();              //get the char we just received
     Serial.println(inchar2);
     inchar2 = inchar2++;
  }
}



void get_EC_data(void)
{
  if (portTwo.available() > 0) 
  {                     //if we see that the Atlas Scientific product has sent a character
    inchar = (char)portTwo.read();              //get the char we just received
    sensorstring += inchar;                           //add the char to the var called sensorstring
    if (inchar == '\r') 
    {                             //if the incoming character is a <CR>
      sensor_string_complete = true;                  //set the flag
    }
  }

  if (sensor_string_complete == true)
  {               //if a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(sensorstring[0]) == false) 
    {          //if the first character in the string is a digit
      Serial.println(sensorstring);                   //send that string to the PC's serial monitor
    }
    else                                              //if the first character in the string is NOT a digit
    {
      update_EC_data();                                //then call this function 
    }
    sensorstring = "";                                //clear the string
    sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }
  
}



void get_TH_data(void)
{
  temp_TH = thermocouple.readCelsius(); \
  //   Serial.println(thermocouple.readFahrenheit()); 
}


void get_pH_data(void)
{
  pH_value = pH.read_ph();
}



void update_EC_data(void)
{                            //this function will pars the string  

                                         //used to hold a floating point number that is the EC
  
  sensorstring.toCharArray(sensorstring_array, 30);   //convert the string to a char array 
  EC = strtok(sensorstring_array, ",");               //let's pars the array at each comma
  TDS = strtok(NULL, ",");                            //let's pars the array at each comma
  SAL = strtok(NULL, ",");                            //let's pars the array at each comma
  GRAV = strtok(NULL, ",");                           //let's pars the array at each comma
  GRAV_f = atof(GRAV);                                //this just makes the output easier to read
  
//f_ec= atof(EC);                                     //uncomment this line to convert the char to a float

  Serial.print("EC Updated");
}

void print_EC_data(void)
{
  Serial.println(""); 
  Serial.print("EC:");                                //we now print each value we parsed separately
  Serial.println(EC);                                 //this is the EC value

  Serial.print("TDS:");                               //we now print each value we parsed separately
  Serial.println(TDS);                                //this is the TDS value

  Serial.print("SAL:");                               //we now print each value we parsed separately
  Serial.println(SAL);                                //this is the salinity value

  Serial.print("GRAV:");                              //we now print each value we parsed separately
  Serial.println(GRAV_f);                               //this is the specific gravity
  
}

void print_TH_data(void)
{

  Serial.println(""); 
  Serial.print("C = "); 
  Serial.println(temp_TH);
  
//   Serial.print("F = ");

  
}

void print_pH_data(void)
{
  Serial.println(""); 
  Serial.print("pH  ");
  Serial.println(pH_value);
  
}

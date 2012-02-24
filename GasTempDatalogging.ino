/*GasTempDataLogger using merged code from ladyada
 for the DHT and Arduino Forum user smau
 
 Modified by Monica Bate and Melissa dela Merced
 23 February 2012
 
 This code uses the DHT22 Temp Sensor, MG811 CO2 Gas Sensor and the Sparkfun SD Shield
 
 
 */


#include "DHT.h"

#define DHTPIN 2  //DHT data pin on digital PIN 2

//Add the SdFat Libraries
/*#include <fat.h>
 #include <SdFatUtil.h> 
 #include <ctype.h>
 */
#include <byteordering.h>
#include <fat.h>
#include <fat_config.h>
#include <partition.h>
#include <partition_config.h>
#include <sd-reader_config.h>
#include <sd_raw.h>
#include <sd_raw_config.h>


//This is the for the GAS sensor
#define R 11 //led RED
#define L 12 //led YELLOW 1
#define G 13 //led GREEN
#define K 10 //led YELLOW 2
#define PIN 0 //analog PIN wired to co2 sensor
#define SOGLIA 1000 // 1000ppm OK value
#define DHTTYPE DHT22   // DHT 22  (AM2302) DHT sensor brand

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

//Create the variables to be used by SdFat Library
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;

//sd card test
char name[] = "Test.txt";     //Create an array that contains the name of our file.
char contents[256];           //This will be a data buffer for writing contents to the file.
char in_char=0;
int index=0;                  //Index will keep track of our position within the contents buffer.


unsigned int lettura = 0; //actual read value
unsigned int letturaOld = 0; //old read value
unsigned long time;



void reading(){ //led blinking while reading
  digitalWrite(R, LOW);
  digitalWrite(K, LOW);
  digitalWrite(G, LOW);
  int cont = 0;
  while (cont < 15){
    digitalWrite(L, HIGH);
    delay(500);
    digitalWrite(L, LOW);
    delay(500);
    cont++;
  }
}

void victory(){ //led blinking when SOGLIA read
  digitalWrite(R, LOW);
  digitalWrite(G, HIGH);
  int cont = 0;
  while (cont < 15){
    digitalWrite(L, HIGH);
    digitalWrite(K, LOW);
    delay(500);
    digitalWrite(L, LOW);
    digitalWrite(K, HIGH);
    delay(500);
    cont++;
  }
}

void rightWay(){ //led blinking when lettura < letturaOld
  digitalWrite(R, LOW);
  digitalWrite(K, LOW);
  digitalWrite(G, HIGH);
  int cont = 0;
  while (cont < 15){
    digitalWrite(L, HIGH);
    delay(500);
    digitalWrite(L, LOW);
    delay(500);
    cont++;
  }
}

void wrongWay(){ //led blinking when lettura > letturaOld
  digitalWrite(R, HIGH);
  digitalWrite(K, LOW);
  digitalWrite(G, LOW);
  int cont = 0;
  while (cont < 15){
    digitalWrite(L, HIGH);
    delay(500);
    digitalWrite(L, LOW);
    delay(500);
    cont++;
  }
}

void setup(){
  pinMode(PIN, INPUT);
  pinMode(R, OUTPUT);
  pinMode(L, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(K, OUTPUT);
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  pinMode(10,OUTPUT);
  card.init();               //Initialize the SD card and configure the I/O pins.
  volume.init(card);         //Initialize a volume on the SD card.
  root.openRoot(volume);     //Open the root directory in the volume. 

  dht.begin();
}

void loop(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();


  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } 
 / else {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
  }

  time = millis();
  if ( time <= 300000) {  //5 minutes heating time
    digitalWrite(R, HIGH);
    digitalWrite(L, HIGH);
    digitalWrite(K, HIGH);
    digitalWrite(G, HIGH);
    letturaOld = analogRead(PIN); //keeps on reading
    letturaOld = map(letturaOld,0,1023,350,10000);
    Serial.print(time/1000);
    Serial.println(" seconds elapsed. Heating in progress...");
    delay(10000);
  }
  else if ( time > 300000 && time < 305000 ) { // leds off
    digitalWrite(R, LOW);
    digitalWrite(L, LOW);
    digitalWrite(K, LOW);
    digitalWrite(G, LOW);
  }
  else{
    //blinking YELLOW led while reading
    reading();
    delay(1500);
    lettura = analogRead(PIN);
    lettura = map(lettura,0,1023,350,10000);
    Serial.print("Vecchia lettura: "); //old reading
    Serial.print(letturaOld);
    Serial.print(" | Nuova lettura: "); //new reading
    Serial.println(lettura);
    if ( lettura < SOGLIA ){
      victory(); //we're done! ding ding ding!
      delay(1500);
    }
    else{
      if (lettura < letturaOld)
        rightWay();
      else
        wrongWay();
    }
    letturaOld = lettura;
    lettura = 0;
    delay(1500);
  }

}





/*GasTempDataLogger using merged code from ladyada
for the DHT and Arduino Forum user smau

Modified by Monica Bate and Melissa dela Merced
23 February 2012

*/
#include "DHT.h"

#define DHTPIN 2  //DHT data pin on digital PIN 2
#define R 11 //led RED
#define L 12 //led YELLOW 1
#define G 13 //led GREEN
#define K 10 //led YELLOW 2
#define PIN 0 //analog PIN wired to co2 sensor
#define SOGLIA 1000 // 1000ppm OK value
#define DHTTYPE DHT22   // DHT 22  (AM2302) DHT sensor brand

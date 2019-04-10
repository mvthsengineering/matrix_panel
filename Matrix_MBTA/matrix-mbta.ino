// This #include statement was automatically added by the Particle IDE.
#include <neomatrix.h>

// Includes for parser, http local client and neomatrix
#include "JsonParserGeneratorRK.h"
#include "application.h"
#include "HttpClient.h"
#include "Particle.h"
#include "neomatrix.h"

#define PIXEL_PIN D2
#define PIXEL_TYPE WS2812B

// Global parser that supports up to 256 bytes of data and 20 tokens
JsonParserStatic<1000, 50> parser;

HttpClient http;
http_request_t request;
http_response_t response;

// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    //  { "Content-Type", "application/json" },
    //  { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

//Neo pixel orientation
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(7, 7, PIXEL_PIN,
  NEO_MATRIX_TOP    + NEO_MATRIX_RIGHT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  PIXEL_TYPE);
  
//int x    = matrix.width();

void tickTock() {
    int count = 0;
    int x,y;
    String strWellington = "Wellington";
   // String strVeterans = "Wellington via Veterans Senior Center";
  //  String strMiddlesex = "Middlesex Ave & Second St";
    String strHeading;
    String strHeadings[6];
    String strBustimes[6];

   //Create path for http address
    request.hostname = "10.1.41.85";
    request.port = 5000;
    request.path = "/";
  
    //Get http response 
    http.get(request, response, headers);
  
    parser.clear();
    parser.addString(response.body.c_str());
	if (!parser.parse()) {
		Serial.println("parse fail");
	} 
	else {
	    Serial.println("parse pass");
	}
	int jSize = parser.getReference().key("data").size();

	for (x=0; x < jSize; x++) {
	  	strHeading = parser.getReference().key("data").index(x).key("headsign").valueString();
        if (strHeading.compareTo(strWellington) == 0) {
            strBustimes[count++] = parser.getReference().key("data").index(x).key("time").valueString();
        }
	}
	//Serial.println(strBustimes[0].length(), DEC);
	 for (x = matrix.width(); x > (12 * -5); x--) {
            matrix.fillScreen(0);
            matrix.setCursor(x, 0);
          //  matrix.print(F("Next "));
            matrix.print(F(strWellington));
           // matrix.print(F(" bus in:"));
            delay(50);
            matrix.show();
        }
	for (y = 0; y < count; y++) {
       // for (x = matrix.width(); x > (strBustimes[y].length() * -5); x--) {
        for (x = matrix.width(); x > (4 * -5); x--) {
            matrix.fillScreen(0);
            matrix.setCursor(x, 0);
            matrix.print(F(strBustimes[y]));
            delay(100);
            matrix.show();
        }
	}
}

// create a software timer to get new prediction times every minute
Timer timer(5000, tickTock);
 
void setup() {
    timer.start();
    Serial.begin(9600);
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(100);
    matrix.setTextColor(matrix.Color(80,255,0));
}

void loop() {
    /*
    matrix.fillScreen(0);
    matrix.setCursor(x, 0);
  //matrix.print(F(c));
    matrix.print(F(strHeading[0]));
    if(--x < -60) {
        x = matrix.width();
   /// if(++pass >= 3) pass = 0;
   // matrix.setTextColor(colors[pass]);
    }
    matrix.show();
    delay(100);
    */
}
# Spotify Display - A Tutorial
Hello! This tutorial will be split into two parts, hardware and software. You will be learning about CAD, SPI communication, and Wifi!  

## Hardware
This device is relatively simple when it comes to hardware, you will need:  
* 1x ESP32 Board (any works, but we will be using the LOLIN C3 mini)
* 1x 1.8" TFT display (ST7735 or ILI9341)
* 3x Tactile switches (keyboard switches work too!)
* 4x M3 Heatset inserts
* Soldering iron (optional, or else you should use dupont cables)

For the case, it will be modelled in Fusion360! It is free for students [here](https://www.autodesk.com/education/edu-software/fusion)  

## Case
Download these parts to use in our CAD! 
* [Screen](https://grabcad.com/library/tft-display-1-8-1)
* [ESP32](https://grabcad.com/library/wemos-esp32-c3-mini-v1-0-0-1) (download the .step!)
* [Keyboard Switch](https://grabcad.com/library/outemu-mx-switch-1)

Start by creating a new project, then a new design. In this design, click on the upload button in the top left, and upload the downloaded assets above. Once uploaded, drag and drop the screen into your project. 
Create a new sketch by pressing this button:   
<img width="53" height="54" alt="image" src="https://github.com/user-attachments/assets/e810bddc-15bb-4efe-8a24-54b16a4da647" />  

Select the plane you want to work on (XY plane)  
Create a rectangle by pressing the button to the right of where the sketch button used to be and create a 50x70mm rectangle:   
<img width="800" height="571" alt="image" src="https://github.com/user-attachments/assets/2fe1a258-9618-4f2b-a35c-0544265ea5e4" />

Click on that new sketched rectangle and press e, extrude it to 60mm  
Next, create a new sketch on the front side of the extruded body. Create another rectangle from the edge to toe other edge of the screen: (43.5mm x  38mm)
<img width="691" height="495" alt="image" src="https://github.com/user-attachments/assets/fb92eccd-2d8b-4abd-ad66-e4f1d0b975bb" />



## Spotify API setup
1. Hop over to https://developer.spotify.com/ and click on your profile in the top left and click dashboard.
2. Press create new app and you'll be prompted with this screen:   
![image](https://github.com/user-attachments/assets/08ea6cbd-1e50-4130-96ad-a5f4a30cbff7)
3. Give it a name, write a small description, and add a random redirect URI for now! (we will change this later)
4.   Press save, then click on the app you just created, copy the Client ID and client secret and keep it aside for now. 


## Software
We will be using Arduino IDE, download it [here](https://www.arduino.cc/en/software/)  

This will be split into a few sections
* What you'll need
* Wifi connection
* Spotify library methods
* TFT screen setup
* Code example

### What you'll need
Before we start, a few things need to be installed. 
1. Follow [this](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) tutorial on how to install the ESP32 board in Arduino IDE
2. Install the following libraries
    * https://github.com/FinianLandes/SpotifyEsp32 (install as a .zip)
    * Adafruit_ST7735 
    * Adafruit_GFX  

Use the library manager to install the last two! Install all the pre-reqs when the pop-up shows after you click install.   

You're all done! In your new sketch, include the following:
```cpp
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>
#include <SPI.h>
```
Inside void setup(), include the following: 
```cpp
Serial.begin(115200);
```
This makes sure that the ESP32 can communicate with the computer over serial.

For general rule of thumb, if you would like to create a variable/object, it should go outside of any method, if you would like to setup something (e.g. it should only run once such as connecting to wifi), it should be in void setup(), if you would like something to run, it should be in void loop().

### Wifi Connection

We will need to store your WiFi credentials in the file, create two new lines under the included libraries: 

```cpp
char* SSID = "INSERT_SSID_HERE"
char* PASSWORD = "INSERT_PASSWORD_HERE"
```

These are two new variables that will hold your ssid/password

In the setup method (void setup), insert the following: 
```cpp
WiFi.begin(SSID, PASSWORD); // Attempt to connect to wifi 
Serial.print("Connecting to WiFi..."); // This will print into the console! 
while(WiFi.status() != WL_CONNECTED) // Checking if the wifi has connected
{
    delay(1000);
    Serial.print("."); // show a loading dot every second
}
Serial.printf("\nConnected!\n"); // Wifi connected!
```
Thats it! Now the ESP is connected to WiFi.  
If you would like to learn more behind the WiFi library, I would recommend checking out https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/

### Spotify Library Methods

Create the following variables:
```cpp
const char* CLIENT_ID = "CLIENT_ID";
const char* CLIENT_SECRET = "CLIENT_SECRET";
```
Create your spotify object as such: 
```cpp
Spotify sp(CLIENT_ID, CLIENT_SECRET);
```
You can rename sp to anything you would like! This is the name of your object. 

To setup the connection between your ESP and spotify, it's relatively similar to WiFi! This block of code will connect the ESP to the spotify client. 

```cpp
sp.begin();
white(!sp.is_auth())
{
    sp.handle_cleint();
}
Serial.println("Connected to Spotify!");
```

Remember to put a delay(ms) at the end of the loop, spotify's API has a rate limit! (change ms with 1000 for 1s or 2000 for 2s)
#### General Methods
```cpp
sp.current_artist_names(); - returns String of all artists seperated by a ','
sp.current_track_name(); - returns String of song name
sp.start_resume_playback(); - Calling this method will pause/play the track
sp.skip(); - skips current track
sp.previous(); - goes to previous track
sp.is_playing(); - returns a boolean if something is playing currently
```
We will use these in an example!

### TFT Screen 
In this project, we will be using the ST7735 Screen. 

We need to define the pin variables that we will be connecting our screen to! Refer to the hardware section of the tutorial for the proper pin assignments. Create the variables as such:

```cpp
#define TFT_CS 1
#define TFT_RST 2
#define TFT_DC 3
#define TFT_SCLK 4
#define TFT_MOSI 5
```
Again, the numbers 1-5 will be changed based on your pin assignments on the ESP.

Create your screen object:
```cpp
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
```
Similar to the spotify object setup, you can change "tft" to anything you would like.

In your setup, initiate the screen: 
```cpp
tft.initR(INITR_BLACKTAB); // the type of screen
tft.setRotation(1); // this makes the screen landscape! remove this line for portrait
Serial.println("TFT Initialized!");
tft.fillScreen(ST77XX_BLACK); // make sure there is nothing in the buffer
/*

INSERT WIFI SETUP HERE

*/

tft.setCursor(0,0); // make the cursor at the top left 
tft.write(WiFi.localIP().toString.c_str()); // print out IP on the screen
```
#### General Methods
```cpp
tft.write("STRING HERE"); - prints out the string 
tft.setCursor(x,y); - the screen is 160x128! Think of it as a x,y grid
tft.fillScreen(ST77XX_COLOR); - most of the main colors are here! 
tft.drawRect(Cursorx, Cursory, RectWidth, RectHeight, ST77XX_color); - draws a rectangle
tft.fillRect(Cursorx, Cursory, RectWidth, RectHeight, ST77XX_color); - fills in a rectangle
tft.setTextSize(integer); - try it out! Use 1,2,3, etc. 
```
Note that when printing strings from spotify, store the String into a variable, then properly format the string as such: 

```
tft.write(variable.toString.c_str());
```

It will not work otherwise and you will be met with an error!

### General Example

Now, we will combine everything to show a example!
Please keep in mind, this is a very bare bones program just to show the functionality of the code. 

```cpp
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>
#include <SPI.h>

#define TFT_CS 1
#define TFT_RST 2
#define TFT_DC 3
#define TFT_SCLK 4
#define TFT_MOSI 5

char* SSID = "YOUR WIFI SSID";
const char* PASSWORD = "YOUR WIFI PASSWORD";
const char* CLIENT_ID = "YOUR CLIENT ID FROM THE SPOTIFY DASHBOARD";
const char* CLIENT_SECRET = "YOUR CLIENT SECRET FROM THE SPOTIFY DASHBOARD";

String lastArtist;
String lastTrackame;

Spotify sp(CLIENT_ID, CLIENT_SECRET);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


void setup() {
    Serial.begin(115200);
    
    tft.initR(INITR_BLACKTAB); // the type of screen
    tft.setRotation(1); // this makes the screen landscape! remove this line for portrait
    Serial.println("TFT Initialized!");
    tft.fillScreen(ST77XX_BLACK); // make sure there is nothing in the buffer
    
    WiFi.begin(SSID, PASSWORD); 
    Serial.print("Connecting to WiFi..."); 
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.printf("\nConnected!\n");

    tft.setCursor(0,0); // make the cursor at the top left 
    tft.write(WiFi.localIP().toString.c_str()); // print out IP on the screen
    
    sp.begin();
    while(!sp.is_auth()){
        sp.handle_client();
    }
    Serial.println("Authenticated");
}

void loop()
{
    String currentArtist = sp.current_artist_names();
    String currentTrackname = sp.current_track_name();
    
    if (lastArtist != currentArtist && currentArtist != "Something went wrong" && !currentArtist.isEmpty()) {
        tft.fillScreen(ST77XX_BLACK);
        lastArtist = currentArtist;
        Serial.println("Artist: " + lastArtist);
        tft.setCursor(10,10);
        tft.write(lastArtist.toString().c_str());
    }
    
    if (lastTrackname != currentTrackname && currentTrackname != "Something went wrong" && currentTrackname != "null") {
        lastTrackname = currentTrackname;
        Serial.println("Track: " + lastTrackname);
        tft.setCursor(10,40);
        tft.write(lastTrackname.toString().c_str());
    }
    delay(2000);
}
```
Thats all! If you would like to learn more about what methods / customizations you can do, check out these pages: 
* https://github.com/FinianLandes/SpotifyEsp32/tree/main/examples
    * This has more examples!
* https://github.com/FinianLandes/SpotifyEsp32/blob/main/src/SpotifyEsp32.cpp
    * All the methods and what they return for the spotify library
* https://learn.adafruit.com/adafruit-gfx-graphics-library/overview
    * All the graphics that are possible with the ST7735 screen (fonts, colors, etc.)

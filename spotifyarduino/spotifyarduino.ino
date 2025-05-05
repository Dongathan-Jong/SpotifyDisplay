#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Arduino.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>

boolean oneTime = true;
boolean secondLine = false; 
boolean secondLineTrack = false;
int progressCheck = 0;
boolean playing;
boolean getTop = true;
boolean topPlayed = false;
String lastArtist;
String lastTrackname;

char* SSID = "NETGEAR80";
const char* PASSWORD = "tinybanana043";
const char* CLIENT_ID = "79ecbb261a8447bdb6242b1f2e2d8917";
const char* CLIENT_SECRET = "c5079e6132a64a8ab093b6a83102de54";
const char* REFRESH_TOKEN = "AQCTq_qs9uRW852th48W_uxMtp1EHJCnxBzfIpw2zPc9Blom1D7WtNz-zh1HaNHYZ_U121pGWsayGA976-hPeOT9Fms0VOtlfyEK8WWkuXla6pjv6O7Jw_SbqiUsOBMVdyw";

#define TFT_CS 5
#define TFT_RST 4
#define TFT_DC 3
#define TFT_SCLK 2
#define TFT_MOSI 1

const unsigned char spotifyLogo[] PROGMEM = {
  0x00, 0x07, 0x80, 0x00,
  0x00, 0x3f, 0xf0, 0x00,
  0x00, 0xff, 0xfc, 0x00,
  0x03, 0xff, 0xff, 0x00,
  0x07, 0xff, 0xff, 0x80,
  0x0f, 0xff, 0xff, 0xc0,
  0x1f, 0xff, 0xff, 0xe0,
  0x1f, 0xff, 0xff, 0xe0,
  0x3f, 0x80, 0x7f, 0xf0,
  0x38, 0x00, 0x03, 0xf0,
  0x78, 0x00, 0x00, 0xf8,
  0x78, 0xff, 0xc0, 0x78,
  0x7f, 0xff, 0xf8, 0x38,
  0xff, 0x80, 0x7f, 0x7c,
  0xfc, 0x00, 0x0f, 0xfc,
  0xfc, 0x00, 0x03, 0xfc,
  0xff, 0xff, 0xe0, 0xfc,
  0x7f, 0xff, 0xf8, 0xf8,
  0x7e, 0x00, 0x3f, 0xf8,
  0x7e, 0x00, 0x0f, 0xf8,
  0x3f, 0xff, 0x83, 0xf0,
  0x3f, 0xff, 0xf3, 0xf0,
  0x1f, 0xff, 0xff, 0xe0,
  0x1f, 0xff, 0xff, 0xe0,
  0x0f, 0xff, 0xff, 0xc0,
  0x07, 0xff, 0xff, 0x80,
  0x03, 0xff, 0xff, 0x00,
  0x00, 0xff, 0xfc, 0x00,
  0x00, 0x3f, 0xf0, 0x00,
  0x00, 0x07, 0x80, 0x00
};

const unsigned char playIcon[] PROGMEM = {
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0xf0, 0x00,
  0x03, 0xfc, 0x00,
  0x07, 0xff, 0x00,
  0x0f, 0xff, 0x00,
  0x1f, 0xff, 0x80,
  0x1e, 0x67, 0x80,
  0x3e, 0x67, 0xc0,
  0x3e, 0x67, 0xc0,
  0x3e, 0x67, 0xc0,
  0x3e, 0x67, 0xc0,
  0x1e, 0x67, 0x80,
  0x1f, 0xff, 0x80,
  0x0f, 0xff, 0x00,
  0x07, 0xfe, 0x00,
  0x03, 0xfc, 0x00,
  0x00, 0xf0, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00
};

const unsigned char pauseIcon[] PROGMEM = {
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0xf0, 0x00,
  0x03, 0xfc, 0x00,
  0x07, 0xff, 0x00,
  0x0f, 0xff, 0x00,
  0x1f, 0xff, 0x80,
  0x1f, 0x3f, 0x80,
  0x3e, 0x0f, 0xc0,
  0x3e, 0x07, 0xc0,
  0x3e, 0x07, 0xc0,
  0x3e, 0x0f, 0xc0,
  0x1f, 0x3f, 0x80,
  0x1f, 0xff, 0x80,
  0x0f, 0xff, 0x00,
  0x07, 0xfe, 0x00,
  0x03, 0xfc, 0x00,
  0x00, 0xf0, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00
};

const unsigned char prev[] PROGMEM = {
  0x00, 0xf0, 0x00,
  0x03, 0xfc, 0x00,
  0x0c, 0x03, 0x00,
  0x18, 0x01, 0x80,
  0x30, 0x00, 0xc0,
  0x20, 0x00, 0x40,
  0x40, 0x00, 0x20,
  0x42, 0x04, 0x20,
  0xc2, 0x1c, 0x30,
  0xc2, 0x64, 0x30,
  0xc2, 0x64, 0x30,
  0xc2, 0x1c, 0x30,
  0x42, 0x04, 0x20,
  0x40, 0x00, 0x20,
  0x20, 0x00, 0x40,
  0x30, 0x00, 0xc0,
  0x18, 0x01, 0x80,
  0x0c, 0x03, 0x00,
  0x03, 0xfc, 0x00,
  0x00, 0xf0, 0x00
};

const unsigned char next[] PROGMEM = {
  0x00, 0xf0, 0x00,
  0x03, 0xfc, 0x00,
  0x0c, 0x03, 0x00,
  0x18, 0x01, 0x80,
  0x30, 0x00, 0xc0,
  0x20, 0x00, 0x40,
  0x40, 0x00, 0x20,
  0x42, 0x04, 0x20,
  0xc3, 0x84, 0x30,
  0xc2, 0x64, 0x30,
  0xc2, 0x64, 0x30,
  0xc3, 0x84, 0x30,
  0x42, 0x04, 0x20,
  0x40, 0x00, 0x20,
  0x20, 0x00, 0x40,
  0x30, 0x00, 0xc0,
  0x18, 0x01, 0x80,
  0x0c, 0x03, 0x00,
  0x03, 0xfc, 0x00,
  0x00, 0xf0, 0x00
};


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

Spotify sp(CLIENT_ID, CLIENT_SECRET);

void setup() {
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  Serial.println("tft good");

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextSize(1.8);

  Serial.begin(115200);
  connect_to_wifi();

  tft.setCursor(30, 50);
  tft.write("SSID: ");
  tft.write(SSID);

  tft.setCursor(30, 70);
  tft.write("IP: ");
  tft.write(WiFi.localIP().toString().c_str());

  Serial.println(WiFi.localIP());

  sp.begin();
  while (!sp.is_auth()) {
    sp.handle_client();
  }


  Serial.println("Authenticated");
  tft.fillScreen(ST77XX_BLACK);

  playing = sp.is_playing();

  tft.drawBitmap(5, 2, spotifyLogo, 30, 30, ST77XX_GREEN);

  if (sp.is_playing()) {
    tft.drawBitmap(70, 100, playIcon, 20, 20, ST77XX_WHITE);
  } else {
    tft.drawBitmap(70, 100, pauseIcon, 20, 20, ST77XX_WHITE);
  }

  tft.drawBitmap(25, 100, prev, 20, 20, ST77XX_WHITE);
  tft.drawBitmap(115, 100, next, 20, 20, ST77XX_WHITE);
}

void loop()
{
  
    normalScreen();
  
}

void normalScreen() 
{

  checkInput();

  tft.setTextSize(1.5);
  String currentArtist = sp.current_artist_names();
  String currentTrackname = sp.current_track_name();
  checkInput();

  if (lastArtist != currentArtist && currentArtist != "null" && !currentArtist.isEmpty() && currentTrackname != "null" && lastTrackname != currentTrackname) {
    lastArtist = currentArtist;
    lastTrackname = currentTrackname;
    tft.setTextSize(1.9);

    checkInput();
    if (lastArtist.length() < 18) {
      Serial.println("Artist: " + lastArtist);
      tft.fillRect(40, 65, 140, 20, ST77XX_BLACK);
      tft.setCursor(40, 65);
      tft.write(lastArtist.c_str());
      checkInput();
    } else if (lastArtist.charAt(10) == ' ') {
      Serial.println("Artist: " + lastArtist);
      tft.fillRect(40, 65, 140, 20, ST77XX_BLACK);
      tft.setCursor(40, 65);
      tft.write(lastArtist.substring(0, 10).c_str());
      tft.setCursor(40, 75);
      tft.write(lastArtist.substring(11, lastArtist.length()).c_str());
      checkInput();
    } else {
      int tempCounter = 0;
      int tempCharCount = 0;

      //check # of words
      for (int i = 0; i < 18; i++) {
        if (lastArtist.charAt(i) == ' ') {
          tempCounter++;
        }
        checkInput();
      }

      int tempWordCount = 0;
      String firstLineArtist = "";

      for (int i = 0; i < 18; i++) {
        if (tempWordCount != tempCounter) {
          firstLineArtist += lastArtist.charAt(i);
          if (lastArtist.charAt(i) == ' ') {
            tempWordCount++;
          }
        }
        checkInput();
      }
      checkInput();
      Serial.println("Artist: " + lastArtist);
      tft.fillRect(40, 65, 140, 20, ST77XX_BLACK);
      tft.setCursor(40, 65);
      tft.write(firstLineArtist.c_str());
      tft.setCursor(40, 75);
      tft.write(lastArtist.substring(firstLineArtist.length(), lastArtist.length()).c_str());
    }

    checkInput();
    if (lastTrackname.length() <= 10) {
      Serial.println("Track: " + lastTrackname);
      tft.fillRect(35, 0, 145, 40, ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(40, 10);
      tft.write(lastTrackname.c_str());
      checkInput();
    } else if (lastTrackname.charAt(10) == ' ' && lastTrackname.length() <= 30) {
      tft.fillRect(35, 0, 145, 40, ST77XX_BLACK);
      tft.setTextSize(2);
      tft.setCursor(40, 3);
      tft.write(lastTrackname.substring(0, 10).c_str());
      tft.setTextSize(1.5);
      tft.setCursor(40, 20);
      tft.write(lastTrackname.substring(11, lastTrackname.length()).c_str());
      checkInput();
    } else {
      int counter = 0;
      int characterCount = 0;
      for (int i = 0; i < 10; i++) {
        if (lastTrackname.charAt(i) == ' ') {
          counter++;
          characterCount++;
        } else {
          characterCount++;
        }
      }

      int wordCount = 0;
      String theTrack = "";
      for (int i = 0; i < characterCount; i++) {
        if (wordCount != counter) {
          theTrack += lastTrackname.charAt(i);
          if (lastTrackname.charAt(i) == ' ') {
            wordCount++;
          }
        }
      }
      checkInput();
      tft.fillRect(35, 0, 145, 40, ST77XX_BLACK);
      tft.fillRect(0, 36, 180, 19, ST77XX_BLACK);


      String restOfTrack = lastTrackname.substring(theTrack.length(), lastTrackname.length());

      if (restOfTrack.length() <= 20) {
        tft.setTextSize(2);
        tft.setCursor(40, 5);
        tft.write(theTrack.c_str());
        tft.setTextSize(1.5);
        tft.setCursor(40, 22);
        tft.write(restOfTrack.c_str());
      } else if (restOfTrack.charAt(20) == ' ') {
        tft.setTextSize(2);
        tft.setCursor(40, 0);
        tft.write(theTrack.c_str());
        tft.setTextSize(1.5);
        tft.setCursor(40, 17);
        Serial.println(restOfTrack.substring(0, 19));
        tft.write(restOfTrack.substring(0, 20).c_str());
        tft.setCursor(40, 27);
        tft.write(restOfTrack.substring(21, restOfTrack.length()).c_str());
      } else {

        tft.setTextSize(2);
        tft.setCursor(40, 0);
        tft.write(theTrack.c_str());
        tft.setTextSize(1.5);
        tft.setCursor(40, 17);
        int restCounter = 0;
        int restCharacterCount = 0;

        for (int i = 0; i < 20; i++) {
          if (restOfTrack.charAt(i) == ' ') {
            restCounter++;
          }
        }

        int restWordCount = 0;
        String thirdLine = "";
        for (int i = 0; i < 20; i++) {
          if (restWordCount != restCounter) {
            thirdLine += restOfTrack.charAt(i);
            if (restOfTrack.charAt(i) == ' ') {
              restWordCount++;
            }
          }
        }

        tft.write(thirdLine.c_str());
        tft.setCursor(40, 27);
        tft.write(restOfTrack.substring(thirdLine.length(), restOfTrack.length()).c_str());
      }

      
    }
    tft.fillRect(0,124,180,4,ST77XX_BLACK);
    checkInput();
  }

  if (progressCheck == 2) {
    checkProgress();
    progressCheck = 0;
  } else {
    progressCheck++;
  }
}

void connect_to_wifi() {
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.printf("\nConnected to WiFi\n");
}

void checkProgress() {
  StaticJsonDocument<200> filter;
  filter["item"]["duration_ms"] = true;
  filter["progress_ms"] = true;

  response r = sp.current_playback_state(filter);

  int progressMs = r.reply["progress_ms"];
  int durationMs = r.reply["item"]["duration_ms"];
  
  double percentage = (double)progressMs/(double)durationMs;
  Serial.println(progressMs);
  Serial.println(durationMs);
  Serial.println(percentage);
  tft.setCursor(0,124);
  tft.fillRect(0,124,(int)(160*percentage),4, ST77XX_GREEN);
}

void checkInput() {
  if (digitalRead(8) == LOW) {
    Serial.println("pause/unpause!");
    if (playing) {
      tft.fillRect(70, 100, 20, 20, ST77XX_BLACK);
      tft.drawBitmap(70, 100, pauseIcon, 20, 20, ST77XX_WHITE);
      sp.pause_playback();
      playing = false;
    } else {
      tft.fillRect(70, 100, 20, 20, ST77XX_BLACK);
      tft.drawBitmap(70, 100, playIcon, 20, 20, ST77XX_WHITE);
      sp.start_resume_playback();
      playing = true;
    }
  }

  if (digitalRead(6) == LOW) {
    tft.drawBitmap(115, 100, next, 20, 20, ST77XX_GREEN);
    sp.skip();
    tft.drawBitmap(115, 100, next, 20, 20, ST77XX_WHITE);
  }
  
  if (digitalRead(10) == LOW) {
    tft.drawBitmap(25, 100, prev, 20, 20, ST77XX_GREEN);
    sp.previous();
    tft.drawBitmap(25, 100, prev, 20, 20, ST77XX_WHITE);
  }
}

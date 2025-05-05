# Spotify Display
![IMG_0356](https://github.com/user-attachments/assets/8f077db9-e3f1-42a3-9775-0f6178f8bcea)
### What is it / Inspiration

Ever see one of those Spotify Car Things? They look amazing, and I would love to have one... Only problem is that they discontinued it and now costs upwards of $200. Why not make one myself?   

### Features

- Displays song + artist in real time
- Shows progress bar in real time
- You can pause / play / skip / go back with the buttons
- Runs on 3v3, decently low power consumption
- Has word wrap!!! (makes it so that the song/artist doesn't get cut off and creates a new line)
   
### Build process

This project is relatively simple, using a WEMOS ESP32 C3 mini, ST7735 1.8" Screen, and 3 keyboard switches for buttons.   

### Customization

You can change the icons, I used this tutorial to make the spotify / play / skip icons: https://www.instructables.com/Converting-Images-to-Flash-Memory-Iconsimages-for-/  
It works great, and the Spotify logo is 30x30, while all the other icons are 20x20.   

If you would like to modify the code, I put all of the spotify stuff into one single method, and its called in void loop().   

You can connect the screen relatively any way you would like, but you can change the pins here:   
![image](https://github.com/user-attachments/assets/98197ecd-a51a-48e5-ac5b-82ddde55e18d)


As for the buttons, you can change the GPIO pins as well. Note that the LED pin for the c3 mini is 7 and the button is 9, so those basically cannot be used.   
![image](https://github.com/user-attachments/assets/a3ef2cc7-8dde-4f3d-aa97-271c0134608c)


### Case 

I whipped this up in under an hour, the screen and esp32 are just hot glued in place, while the case uses m3 heatsets with m3 bolts to hold it together.   
There are 100% more work that can be done on this, but if it works it works :)  

### Setup

Before we start, install these libraries!  
- https://github.com/FinianLandes/SpotifyEsp32  
- Adafruit_ST7735  
- Adafruit_GFX  
Next, we will set up the client for Spotify.
- Hop over to https://developer.spotify.com/ and click on your profile in the top left and click dashboard.   
- Press create new app and you'll be prompted with this screen:   
![image](https://github.com/user-attachments/assets/08ea6cbd-1e50-4130-96ad-a5f4a30cbff7)  
- Name it anything you'd like and give it some random description. Add a random redirect URI for now. (we will change the redirect URI once we plug the ESP32 in)  
- Now, click on your newly created app and copy the client id + client secret and paste it into the code. Also, fill in the SSID and PASSWORD fields!
- Upload the code to the ESP and power it on, you should be prompted with a IP address on the screen or in the serial monitor.   
- Head back to the developer page and edit the app, and add in that IP as the callback URI. For example: I have 192.168.1.18 as my IP, the callback URI would be https://192.168.1.18/callback/, make sure to include the last slash!   
- You should be all good to go! Reset the ESP32 again and it should work once you login. Note you do not need to fill out the REFRESH_TOKEN field, it will be filled out by the code once you login!   




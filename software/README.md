# Software

This section is everything you need to upload new firmware and begin developing your own.

There are two parts that you need

1. An Arduino Board definition for the Attiny84, configured to use Micronucleus as the programmer
2. Micronucleus to communicate with the Attiny84

## 1. Board Definition

Included are the group of files you need to move to the Arduino board definition folder. On Mac this is in

```~/Documents/Arduino/hardware/```

On Windows it should be in

```My Documents\Arduino\hardware```

You need to move the CCTV folder as well as it's contents. Your structure should look like:

```~/Documents/Arduino/hardware/CCTV/avr/boards.txt (etc)```

You can test that this worked by opening Arduino. Under `Tools --> Board` you should now see `CCTV - Cascadence (Attiny84 5V 8Mhz)`

## 2. Micronucleus

There's a few extra steps depending on what OS you're using

### OSX

Run the following command to install libusb

```brew install libusb-compat```

copy the included micronucleus executable to `~/Documents/Arduino/hardware/tools/`

*note* not the .exe file, just **micronucleus**

### Windows

 In Windows, copy the included **micronucleus.exe** to \/hardware/tools. 
 
 Unfortunately, Windows doesn't know what to do with the micronucleus bootloader on the ATtiny84. It comes up as an Unknown USB Device, so we'll fix that with a custom driver. Lucky for us, the micronucleus project already comes with one.

 Plug in the module, and run `zadig_2.1.2.exe` as administrator.
 
 In the interface, select **Unknown Device #1** from the dropdown menu, and make sure that **libusb-win32** is selected for the driver.

 Click Install Driver and let Zadig do its thing. Close out of Zadig once the installation is complete.
 
 ### Linux
 
 Refer to [this article](https://learn.sparkfun.com/tutorials/how-to-install-an-attiny-bootloader-with-virtual-usb/all), specifically the section titled **Build the Micronucleus Loader (Linux)**
 
 
This platform was created following the excellent guide from [Sparkfun](https://learn.sparkfun.com/tutorials/how-to-install-an-attiny-bootloader-with-virtual-usb/all) and if you have any problems, or if you want to explore tweaking the bootloader or board definitions you should definitely read their write up on Attiny84 USB.

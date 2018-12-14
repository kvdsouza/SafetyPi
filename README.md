# **Safety-Pi**

### *"A Remote Safety Check For Your Kitchen"*

## Description
Safety-Pi is a tool designed to keep your home and family safe, as well as keep an eye out for the potentially dangerous items around your kitchen. It has two main features:

 1. *Sensors that provide temperature redundancy (one for near and far detection, as well as flame and gas detection*

 2. *Website interface so one can view and detect any changes in their kitchen/stove area*

 The project is at a point where there are many exciting extensions that can take this work to the next step.

# Motivation / Problem Statement

When I was a little kid, I remember fondly (or unfondly) the memory of being ready to head out to soccer practice or a road trip, only have to run out of the car, sprint to the kitchen and check on Mom's orders if the stove and oven were both off.

There are an estimated 358,000 home structure fires in America, and about 7 die every day from home structure fires. The kitchen, being one of the few places with flammable material, is often the cause of these fires.

Thus, I wanted to achieve two goals in this project:

 1. Build something tangible to understand how I could build a product that met people's needs; and
 2. Gain experience working on a coding project from start to finish, as this was my first experience at that.



# How To Use Safety-Pi

## Setup / Prerequisites

 1. Breadboard
 2. ADC
 3. Sensors: Analog Temp. (NOP), Flame, Gas, and Second Ambient Temperature Sensor
 4. ESP-32 for connecting to WiFi
 5. Lots of jumper cables
 6. LEDs
 7. A working console
 8. In future, ArduCam camera implementation (hopefully) and more customizability of website.

## Technical Architecture
Main components:
 1. http://stanford.edu/~kvdsouza/cgi-bin/readMessage.py and http://stanford.edu/~kvdsouza/cgi-bin/sendMessage.py served as home bases for all on HTML side
 2. Light Display properly shows if heat is increasing or decreasing
 3. Console also displays important information regarding sensors and sensitivity.
 4. Arduino IDE code operates all things ESP32.

The project's technical workflow can be best explained below:

 - First, the user sets up their breadboard and wirings.
 - Next, initializes ESP-32 to Arduino code
 - Then, make install, being careful to be on different Ports.



## Technical Challenges and Solutions

 - How do you communicate over from one side to another?
	 - spent a long time (20+ hours working with ESP32 and figuring out many of the kinks). While I may not have figured it out fully in time for final presentation, I know I'm absolutely very close to getting a solid solution to have more robust printf interfacing to a website. Key issues here are synchronicity and keeping track of data and memory as it goes from device to device.
 - Debugging takes much longer on Arduino IDE and getting used to a new interface definitely took its time; however, it was a great experience diving into something new.
 - Design process was often SNAFU-filled - wanted to break things into smaller chunks but you can never go to small. (i.e. temperature sensor was a little funky on the edges, both required regression calibration and also wasn't often detecting very well).


# Future

As a first project, I definitely plan to make improvements on this in the coming weeks and months. I'd like to increase ability to communicate over the Web. Additionally, I'd like to sync up my ArduCam to work with the system. Finally, I'd like to sync up my Console or build another interface so that the sensor is useful both in emergency situations and otherwise.

# Acknowledgments

A big thank you to all TA's, classmates, and peers throughout the quarter. Special acknowledgment to Chris for providing the ESP32 framework and the readMessage, sendMessage logic to begin to communicate over the interface.

## Thank you.

### - Kyle D'Souza

# SeaSweepers2017

Sup. The Sea Sweepers are an underwater robotics team that competes in the MATE ROV competition. If you'd like to learn more about our team, check out our website at http://seasweepersrov.com

In this repository is the entirety of the Sea Sweepers' software for their ROV. For our system, we have four Arduinos and one Raspberry Pi.

Our system consists of two Arduinos in our control box. One of these Arduinos reads joystick and potentiometer values and sends them to the ROV through the tether to control the motors, lights, and servos (seen in the the MotorTX.ino code). This Arduino also sends these values to the Raspberry Pi which will be explained a little later. This Arduino converts these values of the joysticks and potentiometers into bytes and sends them through serial (RS-232) and another Arduino on the vehicle interprets these bytes and assigns them to the motor control boards accordingly (as seen in the MotorRX.ino code).
The other unique part of our ROV is our sensors and how we display our sensor data. Another Arduino in the vehicle's tube collects all the data from various sensors, such as a temperature sensor, depth sensor, and water leak sensor. This data is then converted to bytes and sent through serial (RS-232). Another Arduino in the control box interprets these bytes and sends them to the Raspberry Pi, which is using a Python script to display this data.
Our Python code (ROVCode.py) runs on our Raspberry Pi. Using a program written completely from scratch, we are able to clearly and easily display our sensor data with different types of graphical displays as well as organized data such as number displays.

For the 2017 season, we are going to experiment with vehicle stabilization as well in order to create an easier environment for our pilot to control the vehicle. We will achieve this with different sensors communicating directly with our motor code, and we are exctied to tackle this challenge.

We are proud of the work we have achieved, as this is the first year the Sea Sweepers have used any type of software on our vehicles before. This was a big leap for us and we've found it to be quite successful.

Feel free to use this code to help yourself, and if you have any questions about the code feel free to contact either Michael Georgariou (michael@seasweepersrov.com) OR Marco Pizarro-Silva (marco@seasweepersrov.com), the Sea Sweepers' two software engineers for the 2017 MATE ROV season.😀Yaay! boo hssh

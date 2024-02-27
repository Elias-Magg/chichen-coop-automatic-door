# chichen-coop-automatic-door
A weather proof solar powered automatic door for chicken coops
(This project was hastily put together and that is why it is lacking documentation)
(If there is time I might make a tutorial)

## Project parts
This project consists of 4 parts

### Solar cell

![image](/assets/images/solar.jpg)

### Motor and its housing
This is essentially just a 360 degree servo
connected to a 8mm metal shaft with bearings on each side.
The motor box is assembled with two 5mm threaded rods for extra rigidity

![image](/assets/images/close-up.jpg)

### Brain box and control interface
This is where the batteries (2x 18650), brain (arduino nano), the real time clock,
the power management circuitry and screen/ buttons lie.
The screen is weather proofed by adding an acetate film on the face of the box
and sealing its sides with sylicone.
This makes the whole assembly water tight while leaving the buttons fully accessible.

![image](/assets/images/ui.jpg)

### Door lock 
A simple servo that moves a metal pin in and out of the assembly.
Stopping the door at the open possition.
In this case the door used was metal and the static friction of 
the servo was not enough to hold it open.
If a lighter door is used this lock is not needed.

![image](/assets/images/lock.jpg)


![image](/assets/images/wide-shot.jpg)

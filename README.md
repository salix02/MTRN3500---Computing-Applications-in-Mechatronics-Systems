# MTRN3500---Computing-Applications-in-Mechatronics-Systems
This project is focussed on implementing an object-oriented c++ program.
An interprocess communication and process management have been implemented to interface with live streams of data and to use them to assist in the tele-operation of a UGV.

The modules in this project includes:
Shared Memory Module
Laser Module (to interface a data stream from an LMS151 laser rangefinder)
Camera Module (to interface to a data stream originating from a reversing camera)
GNSS Module (to interface to a data stream originating from an Novatel SMART-VI GPS receiver)
Vehicle Control Module (to control the unmanned ground vehicle)
Xbox Module (to issue commands using an Xbox game controller)
Display Module (to graphically display the x-y data of the laser range finder and GNSS data in a virtual world using openGL)
Process Management Module (to supervise the operation of all the above modules)

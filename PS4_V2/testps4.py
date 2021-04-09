from pyPS4Controller.controller import Controller
import time
import serial

ser=serial.Serial('/dev/ttyACM1',9600,timeout=1)
#Serial Address of Arduino still unstable. Please finalize and make the address default ACM0!


class MyController(Controller):

    def __init__(self, **kwargs):
        Controller.__init__(self, **kwargs)
        #INITIALIZES THE CONTROLLER MAPPING from pyPS4Controller.

    def on_x_press(self):
        #Applies Gel Application on hold. May need to assign delay on Arduino reading code.
        print("Hello world")
        ser.write('E')
        
    def on_x_release(self):
        #Stops Gel Application. DO NOT ASSIGN SERIAL.
        print("Goodbye world")

    def on_triangle_press(self):
        #
        print("on_triangle_press")

    def on_triangle_release(self):
        #
        print("on_triangle_release")

    def on_circle_press(self):
        #Vaccuum Pump Suction on but off on release.
        print("on_x_press")
        ser.write('HIGH')

    def on_circle_release(self):
        #Vaccuum On/ Off.
        #do nothing. DO NOT ASSIGN SERIAL.
        print("on_x_release")
    
    #LEFT JOYSTICKS = LEFT SERVO MOTOR MOVING FORWARD/BACKWARD/REST    
    def on_L3_up(self, value):
        print("Left Moving Forward...")
        ser.write('F050')
        

    def on_L3_down(self, value):
        print("Left Moving Backwards...")
        ser.write('B050')

    def on_L3_y_at_rest(self):
        #stops movement of left motor. DO NOT ASSIGN SERIAL.
        print("Stopped..")
        ser.write('F000')

    #RIGHT JOYSTICKS = RIGHT SERVO MOTOR MOVING FORWARD/BACKWARD/REST 
    def on_R3_up(self, value):
        print("Right Moving Forward....")
        ser.write('B')
        #print("on_R3_up: {}".format(value))

    def on_R3_down(self, value):
        print("Right Moving Backward...")
        ser.write('D')
        #print("on_R3_down: {}".format(value))

    def on_R3_y_at_rest(self):
        #do nothing. DO NOT ASSIGN SERIAL
        print("Stopped...")

    #D-PAD UP ARROW BUTTON
    def on_up_arrow_press(self):
        #Retracts Transducer
        print("on_up_arrow_press")
        ser.write('G')

    def on_up_arrow_release(self):
        print("on_up_arrow_release")
        #do nothing. DO NOT ASSIGN SERIAL
             
    def on_up_down_arrow_release(self):
        #do nothing. DO NOT ASSIGN SERIAL
        print("on_up_down_arrow_release")
    
    #D-PAD DOWN ARROW BUTTON
    def on_down_arrow_press(self):
        #Deploys Transducer
        print("on_down_arrow_press")
        ser.write('H')

    def on_down_arrow_release(self):
        #Deploying transducer
        #do nothing. DO NOT ASSIGN SERIAL
        print("on_down_arrow_release")

controller = MyController(interface="/dev/input/js0", connecting_using_ds4drv=False)
# you can start listening before controller is paired, as long as you pair it within the timeout window
controller.listen(timeout=60)
time.sleep(0.5)

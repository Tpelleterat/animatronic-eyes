#!/usr/bin/env python3

import serial,time
import sys
import cv2
import keyboard

def program():
    #ttyACM0
    with serial.Serial("/dev/ttyUSB0", 9600, timeout=1) as arduino:

        time.sleep(0.1) #wait for serial to open
        if arduino.isOpen():
            lastHorizontal = ""
            lastVertical = ""
        
            print("arduino connected")

            filePath = sys.path[0] + '/haarcascade_frontalface_default.xml'
            face_cascade = cv2.CascadeClassifier(filePath)

            vid = cv2.VideoCapture(0)
            screenPartWidth  = vid.get(cv2.CAP_PROP_FRAME_WIDTH) / 14
            screenPartHeight = vid.get(cv2.CAP_PROP_FRAME_HEIGHT) / 14

            try:

                while(True):
                    # Capture the video frame
                    # by frame
                    ret, img = vid.read()

                    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
                    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

                    for (x,y,w,h) in faces:

                        pointX = x + w/2
                        pointY = y

                        horizontalZone = int(pointX / screenPartWidth)
                        verticalZone = int(pointY / screenPartHeight)
                        
                        horizontalMessage = f"H{horizontalZone};"
                        verticalMessage = f"V{verticalZone};"
                        cmd=""
                        
                        if horizontalMessage != lastHorizontal:
                            cmd += horizontalMessage
                            lastHorizontal = horizontalMessage
                        
                        if verticalMessage != lastVertical:
                            cmd += verticalMessage
                            lastVertical = verticalMessage

                        if(cmd != ""):
                            #print(cmd)
                            arduino.write(cmd.encode())

                        break

            except KeyboardInterrupt:
            # clean up
                print("Stop program")
                
                #cv2.imshow('img',img)

                # the 'q' button is set as the
                # quitting button you may use any
                # desired button of your choice
                #if cv2.waitKey(1) & 0xFF == ord('q'):
                 #   break
                #if keyboard.is_pressed('q'):  # if key 'q' is pressed 
                 #   print('You Pressed A Key!')
                  #  break  # finishing the loop

            # After the loop release the cap object
            vid.release()
            # Destroy all the windows
            #cv2.destroyAllWindows()

def main():
    program()

if __name__ == '__main__':
    main()
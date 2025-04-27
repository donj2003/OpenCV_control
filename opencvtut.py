
#height = 480, width 640
import cv2
import numpy as np
from PIL import Image
import requests, threading

#height = 480, width 640
prev_direction = 0


def run_motor(val):

    global prev_direction
    if prev_direction == val:
        pass
    else:
        if val == 1:
            direction = 'forward'
       
        elif val == 0:
            direction = 'stop'
        prev_direction = val

        url = 'http://192.168.4.1/'
        print (direction)
        try:
            print(url+direction)
            response = requests.get(url + direction)
            response.raise_for_status()  # Raises HTTPError if the status code is 4xx or 5xx
            print('Response received successfully')

        except requests.exceptions.HTTPError as errh:
            print(f'HTTP Error: {errh}')
        except requests.exceptions.ConnectionError as errc:
            print(f'Error Connecting: {errc}')
        except requests.exceptions.Timeout as errt:
            print(f'Timeout Error: {errt}')
        except requests.exceptions.RequestException as err:
            print(f'An error occurred: {err}')
        

#'http://192.168.4.2:81/stream'
cap = cv2.VideoCapture('http://192.168.4.2:81/stream')
if not cap.isOpened():
    print("Error: Could not open video stream.")
    exit()
upper_limit = np.array([85,255,255])
lower_limit = np.array([35, 100, 100])

while True:

    ret, frame = cap.read()
    if not ret:
        print("Error: Could not read frame.")
        break
    if frame is None or frame.size == 0:
        print("Error: Empty frame received.")
        continue
    #height, width, channel = frame.shape
    #print(f"height{height}, width {width}")
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv_frame, lower_limit, upper_limit) 
    mask_ = Image.fromarray(mask)
    bbox = mask_.getbbox()
    #print(bbox)

    if bbox is not None:
        x1, y1, x2, y2 = bbox
        frame = cv2.rectangle (frame, (x1,y1), (x2,y2), (0,255,0), 4)
        area = (x2-x1)*(y2-y1)
        #print(area)

        if area  < 5000:
            val = 1
        else:
            val = 0

        run_motor(val)        

    cv2.imshow('frame', frame)
    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()

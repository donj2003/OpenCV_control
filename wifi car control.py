from pynput import keyboard
import requests

keys = [keyboard.Key.up, keyboard.Key.down, keyboard.Key.right, keyboard.Key.left]
moving = False
url = 'http://192.168.4.1/'
targ = ''

def on_press(key):
    
    global moving, url, targ

    if moving == True:
        print("hi")
        return True
    try:
        print(f'key {key.char} pressed')
    except AttributeError:
        print(f'special key{key} pressed')

    if key in keys:
        moving = True

    if key == keyboard.Key.up:
        print("moving forward")
        print(url+'forward')
        targ= url + 'forward'        

    elif key == keyboard.Key.down:
        print("moving back")
        print(url+'back')
        targ= url + 'back' 
        
    elif key == keyboard.Key.right:
        print("moving right")
        print(url+'right')
        targ= url + 'right' 
        
    elif key == keyboard.Key.left:
        print("moving left")
        print(url+'left')
        targ= url + 'left' 
        
    else:
        print("try again")
     
    

    try:
        response = requests.get(targ)
        targ = ''
        response.raise_for_status()  # Raises HTTPError if the status code is 4xx or 5xx
        print('Response received successfully')
        print(response.text)  # Print the content of the response
    except requests.exceptions.HTTPError as errh:
        print(f'HTTP Error: {errh}')
    except requests.exceptions.ConnectionError as errc:
        print(f'Error Connecting: {errc}')
    except requests.exceptions.Timeout as errt:
        print(f'Timeout Error: {errt}')
    except requests.exceptions.RequestException as err:
        print(f'An error occurred: {err}')

def on_release(key):
    global moving,url
    print(f'key {key} released')
    if key == keyboard.Key.esc:
        return False
    elif key in keys:
        print("stopped car")
    moving = False
    response = requests.get(url + 'stop')
    print(f'Status Code: {response.status_code}')
    print(f'Response Text: {response.text}')    

with keyboard.Listener(on_press=on_press, on_release= on_release) as listener:
    listener.join()


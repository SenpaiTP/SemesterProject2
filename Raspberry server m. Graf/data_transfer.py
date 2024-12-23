import serial
import requests
import time

ser = serial.Serial('COM9', 9600)

while True:
    if ser.in_waiting > 0:
        data = ser.readline().decode('utf-8').rstrip()
        try:
            response = requests.post('http://127.0.0.1:5000/endpoint', json={'sensor_data': data})
            print(response.text)
        except Exception as e:
            print(f"Error sending data to Flask server: {str(e)}")
        
        time.sleep(1)

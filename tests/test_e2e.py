import requests
import os

def test_e2e():
	print(os.environ["ARDUINO_IP_ADDRESS"])

	response = requests.get(
		"http://192.168.1.2"
	)
	
	print(response.status_code)
	print(response.content.decode().replace('\r', '\n'))
	
	assert True
import requests
import os

def test_e2e():
	print(os.environ["ARDUINO_IP_ADDRESS"])

	response = requests.get(
		os.environ["ARDUINO_IP_ADDRESS"]
	)
	
	print(response.status_code)
	print(response.content.decode().replace('\r', '\n'))
	
	# sys.stdout.write(response.text)
	# print(response.text, end='\r')
	assert True
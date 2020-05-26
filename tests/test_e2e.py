import requests
import os

def test_e2e():
	print(os.environ)
	print(os.environ["ARDUINO_IP_ADDRESS"])
	assert True
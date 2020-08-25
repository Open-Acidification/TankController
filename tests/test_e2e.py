import requests
import os
import re
import datetime
import json 
import csv
from collections import defaultdict

def split_iter(string):
    return (int(x.group(0)) for x in re.finditer(r"[0-9]+", string))

def test_uri_series():
    print(os.environ["ARDUINO_IP_ADDRESS"])

    payload = """
{
  "pH": {
    "value": [
        7
    ],
    "time": [
      0
    ],
    "interval": 10,
    "delay": 30
  },
  "temp": {
    "value": [
        2
    ],
    "time": [
      0
    ],
    "interval": 10,
    "delay": 0
  }
}
    """

    # upload new timeseries
    response = requests.post(
        "http://%s/series" % os.environ["ARDUINO_IP_ADDRESS"],
        data=payload
    )
    assert response.status_code == 200
    
    # check timeseries is uploaded
    response = requests.get(
        "http://%s/goal" % os.environ["ARDUINO_IP_ADDRESS"]
    )
    assert response.status_code == 200

    response_string = response.content.decode().replace('\r', '\n')
    series_list = list(split_iter(response_string))
    expected_series_list = [7,0,10,30,2,0,10,0]
    assert series_list == expected_series_list


def test_uri_data():
    # find latest hour
    today = datetime.datetime.today()
    url_address = "http://%s/data/" % os.environ["ARDUINO_IP_ADDRESS"] + today.strftime("%Y/%m/%d")
    response = requests.get(
        url_address
    )
    assert response.status_code == 200
    
    response_array = json.loads(response.content.decode())
    max_index = [int(e) for e in response_array].index(max([int(e) for e in response_array]))
    max_hour = response_array[max_index]

    # get last lines
    url_address += "/%s?last=10" % max_hour
    response = requests.get(
        url_address
    )
    assert response.status_code == 200

    response_string = response.content.decode().replace('\r', '\n')
    response_csv = csv.DictReader(response_string.split('\n'))
    columns = defaultdict(list) # each value in each column is appended to a list
    for row in response_csv: # read a row as {column1: value1, column2: value2,...}
        try:
            for (k,v) in row.items(): # go over each column name and value 
                columns[k].append(v) # append the value into the appropriate list
        except:
            pass

    assert all(int(float(e)) == 2 for e in columns['temp setpoint'])
    assert all(int(float(e)) == 7 for e in columns['pH setpoint'])

def test_uri_mac():
    # find latest hour
    today = datetime.datetime.today()
    url_address = "http://%s/mac" % os.environ["ARDUINO_IP_ADDRESS"]
    response = requests.get(
        url_address
    )
    assert response.status_code == 200

    response_string = response.content.decode().replace('\r', '\n')
    assert response_string.startsWith("90:a2:da:")

def test_uri_mac():
    # find latest hour
    url_address = "http://%s/mac" % os.environ["ARDUINO_IP_ADDRESS"]
    response = requests.get(
        url_address
    )
    assert response.status_code == 200

    response_string = response.content.decode().replace('\r', '\n')
    assert response_string.startsWith("90:a2:da:")

def test_uri_info():
    # find latest hour
    url_address = "http://%s/info" % os.environ["ARDUINO_IP_ADDRESS"]
    response = requests.get(
        url_address
    )
    assert response.status_code == 200

    response_string = response.content.decode().replace('\r', '\n')
    assert response_string.strip() == os.environ["ARDUINO_TANK_ID"]

def test_uri_lines():
    # TODO determine correct parameters to send for test.
    pass

def test_uri_test():
    # find latest hour
    url_address = "http://%s/test" % os.environ["ARDUINO_IP_ADDRESS"]
    response = requests.get(
        url_address
    )
    assert response.status_code == 200

def test_uri_time():
    # find latest hour
    url_address = "http://%s/time" % os.environ["ARDUINO_IP_ADDRESS"]
    response = requests.get(
        url_address
    )
    assert response.status_code == 200


def test_uri_device():
    # find latest hour
    url_address = "http://%s/device" % os.environ["ARDUINO_IP_ADDRESS"]
    response = requests.get(
        url_address
    )
    assert response.status_code == 200

    response_string = response.content.decode().replace('\r', '\n')
    device = json.loads(response_string)
    # Check json fields
    assert device.has_key("granularity")
    assert device.has_key("maxDataAge")
    assert device.has_key("macstr")
    assert device.has_key("tankid")

    # Check field data
    assert device["macstr"].startsWith("90:a2:da:")
    assert device["tankid"] == int(os.environ["ARDUINO_TANK_ID"])


def test_uri_config():
    # TODO determine correct parameters to send for test.
    pass
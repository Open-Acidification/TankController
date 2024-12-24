import csv
import secrets
from datetime import datetime, timedelta

# Function to generate a random float in a range
def secure_uniform(start, end, precision=2):
    scale = 10 ** precision
    return round(secrets.randbelow(int((end - start) * scale)) / scale + start, precision)

# Function to generate random integers
def secure_randint(start, end):
    return secrets.randbelow(end - start + 1) + start

# Function to generate a random PID gain
def generate_pid_gain():
    Kp = secure_uniform(0.5, 2.0)
    Ki = secure_uniform(0.1, 1.0)
    Kd = secure_uniform(0.1, 1.0)
    return Kp, Ki, Kd

# Function to generate the mockup data
def generate_mock_data():
    # Start time for the mock data (beginning of the day)
    start_time = datetime(2024, 12, 16, 0, 0, 0)
    tank_id = 99
    temp = 20.0  # Initial temperature in °C
    temp_setpoint = 20.0  # Initial temp setpoint
    pH = 7.0  # Initial pH value
    pH_setpoint = 7.0  # Initial pH setpoint

    # List to store data rows
    data = []

    # Generate data for one day (24 hours, 1440 minutes)
    for minute in range(1440):
        time_stamp = (start_time + timedelta(minutes=minute)).strftime("%d/%m/%Y %H:%M:%S")

        # Change temp setpoint and temp gradually
        if minute % 240 == 0:  # Every 4 hours change temp setpoint
            temp_setpoint = secure_uniform(18.0, 30.0, 1)  # Secure random temp setpoint between 18°C and 30°C

        # Gradual change in temp based on setpoint
        temp = round(temp + secure_uniform(-0.1, 0.3, 1), 1) if temp < temp_setpoint else round(temp + secure_uniform(-0.3, 0.1, 1), 1)

        # Change pH setpoint and pH gradually
        if minute % 300 == 0:  # Every 5 hours change pH setpoint
            pH_setpoint = secure_uniform(6.5, 8.5, 1)  # Secure random pH setpoint between 6.5 and 8.5

        # Gradual change in pH based on setpoint
        pH = round(pH + secure_uniform(-0.1, 0.3, 1), 1) if pH < pH_setpoint else round(pH + secure_uniform(-0.3, 0.1, 1), 1)

        # Generate random PID gains
        Kp, Ki, Kd = generate_pid_gain()

        # Construct data row
        data_row = [time_stamp, tank_id, temp, temp_setpoint, pH, pH_setpoint, secure_randint(0, 30), Kp, Ki, Kd]
        data.append(data_row)

    return data

# Write data to CSV file
def write_data_to_csv(filename, data):
    header = ['time', 'tankid', 'temp', 'temp setpoint', 'pH', 'pH setpoint', 'onTime', 'Kp', 'Ki', 'Kd']
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(header)
        writer.writerows(data)

# Generate and write mock data
data = generate_mock_data()
write_data_to_csv(r'extras\log_file_client\logs\test_data\tank_controller_data.csv', data)

print("CSV data for tank controller has been written.")
import secrets
import time

# Helper functions to replace random with secrets
def randint(a, b):
    return secrets.randbelow(b - a + 1) + a
def uniform(a, b):
    return secrets.randbelow(int((b - a) * 100)) / 100 + a
def random():
    return secrets.randbelow(1_000_000) / 1_000_000

# Constants for simulation
tank_id = randint(1, 100)  # Generate a random tank ID as an integer
version = "1.0"
seconds_in_24_hours = 24 * 60 * 60
log_interval = 60  # Generate a log every 60 seconds

# Set consistent targets that change once or twice a day
thermal_target = round(uniform(18.0, 35.0), 2)
ph_target = round(uniform(6.0, 8.0), 3)

# Helper functions to generate mock data
def generate_timestamp(start_time, offset):
    timestamp = time.localtime(start_time + offset)
    return f"{timestamp.tm_year:04d}-{timestamp.tm_mon:02d}-{timestamp.tm_mday:02d} {timestamp.tm_hour:02d}:{timestamp.tm_min:02d}:{timestamp.tm_sec:02d}"

def generate_thermal_data(target):
    fluctuation = uniform(-0.5, 0.5)  # Small fluctuations around the target
    mean = round(target + fluctuation, 2)
    std_dev = round(abs(fluctuation) / 2, 3)  # Standard deviation proportional to fluctuation
    return mean, std_dev

def generate_ph_data(target):
    fluctuation = uniform(-0.2, 0.2)  # Small fluctuations around the target
    return round(target + fluctuation, 3)

def generate_mac_address():
    return ":".join(f"{randint(0, 255):02X}" for _ in range(6))

# Generate log entries
def generate_information_log(timestamp, uptime):
    global thermal_target, ph_target

    # Change targets once or twice during the day
    if random() < 0.001:  # Very low probability to simulate rare target changes
        thermal_target = round(uniform(18.0, 35.0), 2)
        ph_target = round(uniform(6.0, 8.0), 3)
    if random() < 0.01:  # 1% chance of calibration mode
        thermal_mean, thermal_std_dev, ph = "C", "C", "C"
    else:
        thermal_mean, thermal_std_dev = generate_thermal_data(thermal_target)
        ph = generate_ph_data(ph_target)

    return f"{version}\t{tank_id}\tI\t{timestamp}\t\t{thermal_target}\t{thermal_mean}\t{thermal_std_dev}\t{ph_target}\t{ph}\t{uptime}"

def generate_warning_log(timestamp, uptime):
    mac_address = generate_mac_address()
    ph_slope = round(uniform(0.9, 1.1), 3)
    return f"{version}\t{tank_id}\tW\t{timestamp}\t\t\t\t\t\t\t{uptime}\t{mac_address}\t{ph_slope}\t"

# Main function to generate logs
def generate_logs():
    start_time = int(time.time())
    logs = []

    for offset in range(0, seconds_in_24_hours, log_interval):
        timestamp = generate_timestamp(start_time, offset)
        uptime = offset  # Uptime starts at 0 and increments by log_interval

        if random() < 0.01:  # Low probability to simulate rare warning logs
            log = generate_warning_log(timestamp, uptime)
            logs.append(log)
        
        log = generate_information_log(timestamp, uptime)
            
        logs.append(log)

    return logs

# Write logs to a file
def write_logs_to_file(filename, logs):
    with open(filename, "w") as file:
        file.write("\n".join(logs))

if __name__ == "__main__":
    logs = generate_logs()
    write_logs_to_file(r"extras\log_file_client\logs\test_data\mock_log_file.log", logs)
    print("Log file 'mock_log_file.log' has been generated.")

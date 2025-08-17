import socket
import csv
import time
import os

# UDP Server Configuration
HOST = '0.0.0.0'  # Listen on all network interfaces
PORT = 44432       # UDP port
BUFFER_SIZE = 1024  # Large enough for 112 bytes

# CSV file setup
csv_filename = "Dheeraj13.csv"

# Number of columns to write per row
NUM_COLUMNS =4   # Change this to set the number of columns dynamically

# Track the previous packet's timestamp
previous_receive_time_us = None

# Create a UDP socket and bind it to the host and port
with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
    s.bind((HOST, PORT))

    # Check if CSV file exists
    file_exists = os.path.exists(csv_filename)

    # Open CSV file in append mode
    with open(csv_filename, "w", newline="") as csvfile:
        csv_writer = csv.writer(csvfile)

        # Dynamically generate the header based on NUM_COLUMNS
        header = ["Timestamp"] + [f"Combined Value{i+1}" for i in range(NUM_COLUMNS)]
        if not file_exists:
            csv_writer.writerow(header)

        print(f"UDP Server listening on {HOST}:{PORT}...")

        while True:
            try:
                # Record current receive time
                receive_time = time.time()  # Seconds with high precision
                receive_time_us = int(receive_time * 1e6)  # Convert to microseconds

                # Receive data from UDP client
                data, addr = s.recvfrom(BUFFER_SIZE)
                if data:
                    decoded_data = data.decode(errors='ignore').strip()
                    values = decoded_data.split(",")

                    # Ensure we have exactly 112 values
                    if len(values) == 112:
                        try:
                            int_values = [int(v) for v in values]  # Convert all to integers

                            # Print the received values
                            print(f"Raw Input (112 values): {int_values}")

                            # Manually combine using given scheme
                            combined_values = [
                                 (int_values[i+2] << 24) | (int_values[i+3]<<16)|(int_values[i] << 8) | (int_values[i+1])
                                for i in range(0, 112, 4)
                            ]

                            # Compute timestamp difference
                            if previous_receive_time_us is None:
                                timestamp_increment = 0  # First packet, no difference
                            else:
                                timestamp_increment = (receive_time_us - previous_receive_time_us) // len(combined_values)

                            # Store current timestamp as previous for next iteration
                            previous_receive_time_us = receive_time_us

                            # Print processed combined values
                            print(f"Processed 32-bit Values ({len(combined_values)} total): {combined_values}")

                            # Write values in NUM_COLUMNS per row
                            for i in range(0, len(combined_values), NUM_COLUMNS):
                                timestamp = receive_time_us - ((len(combined_values) - 1 - i) * timestamp_increment)
                                row_values = [timestamp] + combined_values[i:i+NUM_COLUMNS]

                                # Pad row with empty values if needed
                                while len(row_values) < NUM_COLUMNS + 1:
                                    row_values.append("")

                                csv_writer.writerow(row_values)
                            csvfile.flush()  # Ensure immediate write

                        except ValueError:
                            print(f"⚠ Warning: Received non-integer data -> {decoded_data}")

                    else:
                        print(f"⚠ Warning: Expected 112 values but received {len(values)} -> {decoded_data}")

            except Exception as e:
                print(f"❌ Error: {e}")

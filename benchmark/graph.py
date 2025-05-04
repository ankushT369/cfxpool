import pandas as pd
import matplotlib.pyplot as plt

# Function to check if a row has the expected number of columns
def is_valid_row(row, expected_columns=10):
    return len(row) == expected_columns

# Read the file, manually handling rows to skip non-CSV lines
with open('benchmark_results.txt') as f:
    # Skip metadata at the beginning
    lines = f.readlines()
    # Skip over the first few lines that are part of the metadata
    data_lines = lines[8:]  # Adjust based on actual number of metadata lines
    
    # Filter only valid rows based on expected columns
    valid_rows = [line for line in data_lines if is_valid_row(line.split(','))]
    
# Create a DataFrame from valid rows
data = pd.DataFrame([line.strip().split(',') for line in valid_rows])

# Rename columns based on the expected header (adjust as needed)
data.columns = ["name", "iterations", "real_time", "cpu_time", "time_unit", "bytes_per_second", "items_per_second", "label", "error_occurred", "error_message"]

# Convert the relevant columns to numeric (real_time in this case)
data['real_time'] = pd.to_numeric(data['real_time'], errors='coerce')

# Display the first few rows to check if the data looks correct
print(data.head())

# Plot the time taken for each benchmark
plt.figure(figsize=(10, 6))
plt.bar(data['name'], data['real_time'], color='b', label='Real Time (ns)')
plt.xlabel('Benchmark')
plt.ylabel('Time (ns)')
plt.title('Benchmark Results')
plt.xticks(rotation=90)
plt.tight_layout()
plt.show()


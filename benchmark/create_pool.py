import pandas as pd
import matplotlib.pyplot as plt

# Read the benchmark output file
with open('benchmark_create_pool_results.txt') as f:
    lines = f.readlines()

# Filter out the benchmark data (skip metadata and header lines)
data_lines = [line.strip() for line in lines if line.startswith('BM_')]
data = []

# Parse each line to extract relevant fields
for line in data_lines:
    parts = line.split()
    if len(parts) < 5:  # Ensure the line has enough parts
        continue
    name = parts[0]  # e.g., BM_fxpool_creation/64/100000
    real_time = float(parts[1])  # e.g., 1744
    time_unit = parts[2]  # e.g., ns
    if time_unit != 'ns':  # Ensure we're dealing with nanoseconds
        continue
    data.append({'name': name, 'real_time': real_time})

# Create a DataFrame
df = pd.DataFrame(data)

# Extract benchmark type and data size from the name
df['benchmark_type'] = df['name'].apply(lambda x: x.split('/')[0])  # e.g., BM_fxpool_creation
df['data_size'] = df['name'].apply(lambda x: int(x.split('/')[1]))  # e.g., 64

# Sort by data size for smoother lines
df = df.sort_values('data_size')

# Plot the line graph
plt.figure(figsize=(10, 6))

# Plot for BM_fxpool_creation (first color: blue)
fxpool_data = df[df['benchmark_type'] == 'BM_fxpool_creation']
plt.plot(fxpool_data['data_size'], fxpool_data['real_time'], marker='o', color='b', label='BM_fxpool_creation')

# Plot for BM_bulk_malloc (second color: red)
bulk_malloc_data = df[df['benchmark_type'] == 'BM_bulk_malloc']
plt.plot(bulk_malloc_data['data_size'], bulk_malloc_data['real_time'], marker='o', color='r', label='BM_bulk_malloc')

# Use logarithmic scales for both axes due to large ranges
plt.xscale('log')  # Log scale for data size
plt.yscale('log')  # Log scale for time

# Labels and title
plt.xlabel('Data Size (bytes)')
plt.ylabel('Time (ns, log scale)')
plt.title('Benchmark Pool creation Results: Data Size vs Time (Create Pool)')
plt.grid(True, which="both", ls="--")
plt.legend()
plt.tight_layout()
plt.show()

import matplotlib.pyplot as plt

def read_data_from_file(filename):
    """Reads data from a file, assuming each line contains a single integer."""
    with open(filename, 'r') as file:
        data = [int(line.strip()) for line in file.readlines()]
    return data

def plot_data(data):
    """Plots the data on a line chart with iterations as the x-axis and steps as the y-axis."""
    plt.figure(figsize=(10, 6))
    plt.plot(range(1, len(data)+1), data, marker='o')
    plt.title('Step Count vs. Iterations')
    plt.xlabel('Number of Iterations')
    plt.ylabel('Steps')
    plt.grid(True)
    plt.show()

# Example usage
filename = 'Result.txt'  # Replace 'path_to_your_file.txt' with the path to your data file
data = read_data_from_file(filename)
plot_data(data)

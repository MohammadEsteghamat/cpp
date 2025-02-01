import matplotlib.pyplot as plt

# خواندن داده‌ها از فایل
x = []
y = []

with open('trajectory.txt', 'r') as file:
    for line in file:
        data = line.split()
        if len(data) >= 2:
            x.append(float(data[0]))
            y.append(float(data[1]))

# رسم مسیر
plt.figure(figsize=(10, 6))
plt.plot(x, y, label='Path')
plt.scatter(x[0], y[0], color='green', label='Start', marker='o')
plt.scatter(x[-1], y[-1], color='red', label='End', marker='x')
plt.xlabel('X (meters)')
plt.ylabel('Y (meters)')
plt.title('Robot Path')
plt.legend()
plt.grid(True)
plt.show()

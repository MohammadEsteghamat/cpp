import matplotlib.pyplot as plt

x = []
y = []
theta = []

with open('trajectory.txt', 'r') as file:
    for line in file:
        xi, yi, thetai = map(float, line.split())
        x.append(xi)
        y.append(yi)
        theta.append(thetai)

plt.figure(figsize=(10, 5))


plt.subplot(1, 2, 1)
plt.plot(x, y, marker='o')
plt.xlabel('X position (m)')
plt.ylabel('Y position (m)')
plt.title('Trajectory of Bicycle Model')
plt.grid(True)


plt.subplot(1, 2, 2)
plt.plot(range(len(theta)), theta, marker='o', color='r')
plt.xlabel('Time step')
plt.ylabel('Theta (radians)')
plt.title('Orientation of Bicycle Model')
plt.grid(True)

plt.tight_layout()
plt.show()
print(theta[-1])
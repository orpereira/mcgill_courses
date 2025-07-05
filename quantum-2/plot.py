import numpy as np
import matplotlib.pyplot as plt

''' Generic plotting code: '''
data = np.genfromtxt('y.dat', delimiter=' ')

fig = plt.figure(dpi=120)

plt.subplot(211)
plt.plot(data[:,0], data[:,1], label='numerov_test.dat')
plt.xlabel('x')
plt.ylabel('y')
plt.title('F(x)')
plt.legend()
plt.tight_layout()
plt.savefig('plot.pdf')
plt.show()

''' Code used to make example plots: '''
# # Plot the data for F(x) = -x
# data_x = np.genfromtxt('minus_x.dat', delimiter=' ')

# fig = plt.figure(dpi=120)

# plt.subplot(211)
# plt.plot(data_x[:,0], data_x[:,1], label='numerov_test.dat')
# plt.xlabel('x')
# plt.ylabel('y')
# plt.title('F(x) = -x')

# # Plot the data for F(x) = -1.0
# data_one = np.genfromtxt('minus_one.dat', delimiter=' ')
# plt.subplot(212)
# plt.plot(data_one[:,0], data_one[:,1], label='numerov_test.dat')
# plt.xlabel('x')
# plt.ylabel('y')
# plt.title('F(x) = -1.0')  
# plt.tight_layout()  
# plt.savefig('numerov_plots.pdf')
# plt.show()


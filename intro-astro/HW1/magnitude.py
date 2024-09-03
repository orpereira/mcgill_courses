# olivia pereira, 260985600
# phys 521 homework 1, question 4

import numpy as np
import matplotlib.pyplot as plt

'''
a. Write a python script, magnitude.py, that calculates the flux ratios for stars with a magnitude 
difference Δm of 1, 2, 3, 4, 5, 6, 10, 20, 30 mags. Present the results graphically. 
(Check out section 3.3 of Python for Astronomers if you aren’t sure where to start!)
'''

def calculate_flux_ratios(delta_mags):
    # equation 3.3 in Carroll & Ostlie: F1/F2 = 100^((m1 - m2) / 5)
    return 100**(delta_mags/5)  # unit: dimensionless

def plot_flux_ratios(delta_mags, flux_ratios):
    # plot the flux ratios vs. magnitude difference for inputted values

    plt.figure(dpi = 200, figsize = (6, 3))
    plt.plot(delta_mags, flux_ratios, 'ro') 

    # plot labelling
    plt.xlabel('Magnitude Difference')
    plt.ylabel('Flux Ratio')
    plt.title('Flux Ratios vs. Magnitude Difference')
    plt.yscale('log') # plotting on a log scale to better visualize the data

    # save figure to add to homework file, commented out for homework grading
    # plt.savefig('flux_ratios_vs_magnitude_difference.png', bbox_inches = 'tight')
    plt.show()

def parta():
    delta_mags = np.array([1, 2, 3, 4, 5, 6, 10, 20, 30])   # create an array of the magntiude differences 
    flux_ratios = calculate_flux_ratios(delta_mags)         # calculate the flux ratios
    plot_flux_ratios(delta_mags, flux_ratios)               # create the plot


'''
b. The faintest stars we can observe with our 16-inch telescope are about V=16, while for the recorded 
with the Hubble Space Telescope its about V= 30. What is the brightness ratio?
'''

def partb():
    delta_mags = 30 - 16    # from question, the difference in magnitudes between the two telescopes
    
    # Brightness ratio ~ flux ratio, so we can use our function from part a
    flux_ratio = calculate_flux_ratios(delta_mags)

    # print result, rounding to 2 decimal places
    print(f'The brightness ratio between the faintest stars observed with the 16-inch telescope and the Hubble Space Telescope is {flux_ratio:.2e}.')

# ------------------------------------------------------------------------------
if __name__ == '__main__':
    parta()
    partb()


# olivia pereira, 260985600
# phys 521 homework 1, question 5

import numpy as np
import matplotlib.pyplot as plt

# values from part a, calculated using https://irsa.ipac.caltech.edu/data/SPITZER/docs/dataanalysistools/tools/pet/magtojy/
dict = {
    # band: {'F_lambda': flux density (W/m^2/micron), 'lambda_c': central wavelength (microns), 'magnitude': magnitude}
    'U': {'F_lambda': 4.22e-8,  'lambda_c': 0.36, 'magnitude': -2.38e-5},
    'B': {'F_lambda': 6.7e-8,   'lambda_c': 0.43, 'magnitude': 1.49e-4},
    'V': {'F_lambda': 3.75e-8,  'lambda_c': 0.55, 'magnitude': -7.18e-5},
    'R': {'F_lambda': 1.8e-8,   'lambda_c': 0.7,  'magnitude': 3.69e-4},
    'I': {'F_lambda': 9.76e-9,  'lambda_c': 0.9,  'magnitude': -8.24e-5},
    'J': {'F_lambda': 3.08e-9,  'lambda_c': 1.25, 'magnitude': -7.9e-4},
    'H': {'F_lambda': 1.26e-9,  'lambda_c': 1.6,  'magnitude': -2.02e-4},
    'K': {'F_lambda': 4.06e-10, 'lambda_c': 2.22, 'magnitude': 3.78e-5},
    'L': {'F_lambda': 6.89e-11, 'lambda_c': 3.54, 'magnitude': 7.2e-4},
    'M': {'F_lambda': 2.21e-11, 'lambda_c': 4.8,  'magnitude': 1.74e-3}
}

# create the plot
plt.figure(dpi = 200, figsize = (6, 8))

# plot the magnitude of Vega in different bands
plt.subplot(211)
for band in dict:
    plt.plot(dict[band]['lambda_c'], dict[band]['magnitude'], 'b.')
    plt.annotate(band, (dict[band]['lambda_c'], dict[band]['magnitude']))
plt.xlabel('Wavelength (microns)')
plt.ylabel('Magnitude')
plt.title('Magnitude of Vega in Different Bands of the Johnson System')

# plot the flux density of Vega in different bands
plt.subplot(212)
for band in dict:
    plt.plot(dict[band]['lambda_c'], dict[band]['F_lambda'], 'r.')
    plt.annotate(band, (dict[band]['lambda_c'], dict[band]['F_lambda']))
plt.xlabel('Wavelength (microns)')
plt.ylabel(f'Flux Density ($W/m^2/micron$)')
plt.title('Flux Density of Vega in Different Bands of the Johnson System\n')
plt.tight_layout()

# save figure to add to homework file, commented out for homework grading
# plt.savefig('magnitude_flux_density.png', bbox_inches = 'tight')
plt.show()
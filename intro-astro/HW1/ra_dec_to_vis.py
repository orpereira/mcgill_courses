# olivia pereira, 260985600
# phys 521 homework 1, question 6

import numpy as np
import matplotlib.pyplot as plt
import astropy as ap
from astropy.coordinates import EarthLocation

'''
Catalogs of astronomical objects typically list their right ascension and 
declination. Using the astropy library, write a function that takes the right 
ascension and declination of an object as input and plots the visibility of 
that object as a function of time (think of what coordinate determines the 
visibility of an object at any given time), for a given place on Earth (see 
http://docs.astropy.org/en/stable/coordinates/index.html to learn how 
you can use astropy to convert coordinates). Use the function to plot the 
visibility of the Triangulum Galaxy (M33) from Montréal on the night of 
September 13, 2024. You’ll receive full marks for a correct plot of altitude 
vs. time. 
'''

def plot_visibility(ra, dec, location, date):

    # create array of times through the night of the given date
    # to be conservative, will code it from 4pm to 8am the next day 
    times = ap.time.Time(date) + np.linspace(16, 32, 50) * ap.units.hour

    # create the observer object using the inputtedd address
    observer = ap.coordinates.EarthLocation.of_address(location)

    # create the skycoord object using the inputted ra and dec
    skycoord = ap.coordinates.SkyCoord(ra, dec, unit = 'deg')

    # calculate the altitude of the object at each time
    # units: degrees
    altaz = skycoord.transform_to(ap.coordinates.AltAz(obstime = times, location = observer))

    # create the plot
    plt.figure(dpi = 200, figsize = (6, 4))
    plt.plot(times.datetime, altaz.alt, 'ro')  
    plt.xticks(rotation = 90) 
    plt.xlabel('Time (UTC)')
    plt.ylabel('Altitude (degrees)')
    plt.title(f'Visibility of object at ({ra}, {dec}) \nfrom {location} on {date}')
    plt.tight_layout()

    # save figure to add to homework file, commented out for homework grading
    # plt.savefig('visibility_plot.png', bbox_inches = 'tight')
    
    plt.show()

# ------------------------------------------------------------------------------
if __name__ == '__main__':

    # Location of M33, source: http://simbad.u-strasbg.fr/simbad/sim-id?Ident=M33
    ra = 23.4621
    dec = 30.6602

    # Location and date inputs from question
    location = 'Montreal'
    date = '2024-09-13'

    # create the plot
    plot_visibility(ra, dec, location, date)


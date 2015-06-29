# SNe
Riemann solver with cooling, for the Sedov and cooling phases of a supernova remnant
-------

Author: Eric Gentry   (gentry.e@gmail.com; egentry@ucsc.edu)   

Licensed under the GPLv3.

-------

##Main Objectives
  - Evolve a spherical symmetric blast wave, incorporating hydrodynamics and radiative cooling
    - Achieve accurate and stable results through Sedov and (thin shell) radiative phases
  - Incorporate pre-supernova feedback, to create more realistic initial conditions for the background medium
  - Incorporate multiple supernovae
  - Measure the energy and momentum injected into the surrounding medium, to be used as feedback prescriptions in lower resolution galaxy simulations


##Requires
  - gcc (makefile can be adapted to use other compilers)
  - mpicc (makefile assumes OpenMPI)
  - [grackle cooling](https://bitbucket.org/grackle/grackle)
  - For visualization:
    - Python (tested for v.3, probably backwards compatible)
    - IPython 3
    - Matplotlib
    - Numpy
    - Astropy
    - Pandas
    - Seaborn


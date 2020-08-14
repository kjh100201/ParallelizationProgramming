#!/bin/bash
#PBS -N mpi
#PBS -l walltime=01:00:00
### Select 2 nodes with 4 CPUs each for a total of 8 MPI processes
#PBS -l select=2:ncpus=4:mpiprocs=4
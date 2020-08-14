#!/bin/bash
#PBS -N openMP
#PBS -l walltime=01:00:00
### Request 4 CPUS for 4 threads
#PBS -l select=1:ncpus=4:ompthreads=4
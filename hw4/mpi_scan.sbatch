#!/bin/bash
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task=1
#SBATCH --time=1:30:00
#SBATCH --mem=2GB
#SBATCH --job-name=mpi_scan
#SBATCH --mail-type=END
#SBATCH --mail-user=weijian.feng@nyu.edu
#SBATCH --output=mpi_scan_%j.out
module purge
module load intel/19.1.2
module load openmpi/intel/4.1.1
mpirun ./mpi_scan
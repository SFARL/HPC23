#!/bin/bash
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --time=1:30:00
#SBATCH --mem=2GB
#SBATCH --job-name=int_ring_bw
#SBATCH --mail-type=END
#SBATCH --mail-user=weijian.feng@nyu.edu
#SBATCH --output=int_ring_bw_%j.out
module purge
module load intel/19.1.2
module load openmpi/intel/4.1.1
mpirun ./ring_bw 1000000
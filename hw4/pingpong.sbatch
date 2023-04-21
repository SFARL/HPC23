#!/bin/bash                                                                                                        
#SBATCH --nodes=4                                                                                                  
#SBATCH --ntasks-per-node=1                                                                                        
#SBATCH --cpus-per-task=1                                                                                          
#SBATCH --time=00:30:00                                                                                            
#SBATCH --mem=2GB                                                                                                  
#SBATCH --job-name=pingpong                                                                                        
#SBATCH --mail-type=END                                                                                            
#SBATCH --mail-user=weijian.feng@nyu.edu                                                                           
#SBATCH --output=pingpong_%j.out                                                                                   
module purge
module load openmpi/intel/4.1.1
module load intel/19.1.2
mpirun --mca opal_warn_on_missing_libcuda 0  ./pingpong 1 2
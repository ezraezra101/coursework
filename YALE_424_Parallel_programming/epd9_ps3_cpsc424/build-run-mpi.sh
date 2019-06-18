#!/bin/bash
#SBATCH --partition=cpsc424
# set total number of MPI processes
# set number of cpus per MPI process
#SBATCH --cpus-per-task=1
# set memory per cpu
#SBATCH --mem-per-cpu=6100mb
#SBATCH --job-name=MPI_RUN
#SBATCH --time=15:00
#SBATCH --ntasks-per-node=4
#SBATCH --ntasks=8

module load Langs/Intel/15 MPI/OpenMPI/2.1.1-intel15
pwd
# printf some environment variables
printf $SLURM_JOB_NODELIST
printf $SLURM_NTASKS_PER_NODE
# Do a clean build
make clean
# The following mpirun command will pick up required info on nodes and cpus from Slurm. 
# You can use mpirun's -n option to reduce the number of MPI processes started on the cpus. (At most 1 MPI proc per Slurm task.)
# You can use mpirun options to control the layout of MPI processes---e.g., to spread processes out onto multiple nodes
# In this example, we've asked Slurm for 4 tasks (2 each on 2 nodes), but we've asked mpirun for two MPI procs, which will go onto 1 node.
# (If "-n 2" is omitted, you'll get 4 MPI procs (1 per Slurm task)

# printf "\n\tTask 2a\n\n"
# make parallel2
# time mpirun -n 1 ./parallel2
# time mpirun -n 2 ./parallel2
# time mpirun -n 4 ./parallel2
# time mpirun -n 8 ./parallel2


#printf "\n\tTask 3a\n\n"
#make parallel3
#time mpirun -n 1 ./parallel3
#time mpirun -n 2 ./parallel3
#time mpirun -n 4 ./parallel3
#time mpirun -n 8 ./parallel3

# Include the following line in the header before performing part B:
#SBATCH --ntasks-per-node=2
# set number of MPI processes per node
# (number of nodes is calculated by Slurm)

nodes=2
tasks=8
# should set --SBATCH --ntasks-per-node= to be nodes/tasks
printf "\n\t Running $tasks on $nodes nodes \n\n"

printf "\n\tTask 2b\n\n"
make parallel2
time mpirun -n $tasks --map-by socket ./parallel2 b

printf "\n\tTask 3b\n\n"
make parallel3
time mpirun -n $tasks --map-by socket ./parallel3 b

printf "\n\tTask 4\n\n"
make parallel4
time mpirun -n $tasks --map-by socket ./parallel4 b

# printf "\n\tTask 5\n\n"
# time mpirun -n 7 --map-by socket ./parallel4 5


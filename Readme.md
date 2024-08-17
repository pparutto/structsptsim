To re-generate density simulations for the SPT fidelity 2024 paper launch the following scripts located in the scripts folder:

generate_simu_density_freespace_allDT.sh outdir
generate_simu_density_structure_allDT.sh structdir outdir

with the path to an output directory as argument. The structure generation script also takes the path to a directory containing structures as argument.

You will need to change the variable bin at the top of each file to point to the corresponding binary file.

You can find the structures used for the SPT fidelity 2024 paper in the examples folder.
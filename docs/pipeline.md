# Computational Pipeline

This repository archives the thesis workflow around the original C programs. The code is legacy scientific computing code, so filenames and output names follow the historical scripts.

```text
Raw Pokec dataset
    -> reciprocal_contact_cleaning.c
    -> cleaned reciprocal network
    -> random_walk_subgraph_sampler.c
    -> smaller subgraphs
    -> susceptibility_threshold_scan.c
    -> estimate lambda_c
    -> mutation_phase_diagram_scan.c
    -> estimate gamma_c(lambda)
    -> phase diagram

Cleaned network or subgraph
    -> mutant_trajectory_simulation.c
    -> time-evolution trajectories
```

## Workflow Notes

1. `reciprocal_contact_cleaning.c` reads the directed Pokec relationship edge list and keeps reciprocal contacts. It writes `nodes22.dat` and `aristes22.dat`.
2. `random_walk_subgraph_sampler.c` reads an edge list and generates a smaller sampled graph using a random-walk-like process. It writes `subg.dat`.
3. `susceptibility_threshold_scan.c` runs repeated non-mutant epidemic simulations across infection-rate values. It writes `suscep.dat`, which is used to estimate the epidemic threshold `lambda_c`.
4. `mutation_phase_diagram_scan.c` runs repeated mutant epidemic simulations across mutation-rate values for a fixed infection rate. It writes `fases2.dat`, which is used to estimate survival and endemic probabilities for the phase diagram.
5. `mutant_trajectory_simulation.c` runs a single mutant epidemic trajectory and writes temporal evolution to `Evol.dat`.

The input files are plain text edge lists with two integer columns. The programs prompt for the input filename at runtime.

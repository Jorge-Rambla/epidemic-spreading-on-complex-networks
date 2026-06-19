# Computational Pipeline

This repository archives the thesis workflow around the original C programs. The code is legacy scientific computing code, so filenames and output names follow the historical scripts.

```text
Raw Pokec dataset
    -> limpieza2.c
    -> cleaned reciprocal network
    -> subgraf.c
    -> smaller subgraphs
    -> suscep.c
    -> estimate lambda_c
    -> fases6.c
    -> estimate gamma_c(lambda)
    -> phase diagram

Cleaned network or subgraph
    -> Xarxes5.c
    -> time-evolution trajectories
```

## Workflow Notes

1. `limpieza2.c` reads the directed Pokec relationship edge list and keeps reciprocal contacts. It writes `nodes22.dat` and `aristes22.dat`.
2. `subgraf.c` reads an edge list and generates a smaller sampled graph using a random-walk-like process. It writes `subg.dat`.
3. `suscep.c` runs repeated non-mutant epidemic simulations across infection-rate values. It writes `suscep.dat`, which is used to estimate the epidemic threshold `lambda_c`.
4. `fases6.c` runs repeated mutant epidemic simulations across mutation-rate values for a fixed infection rate. It writes `fases2.dat`, which is used to estimate survival and endemic probabilities for the phase diagram.
5. `Xarxes5.c` runs a single mutant epidemic trajectory and writes temporal evolution to `Evol.dat`.

The input files are plain text edge lists with two integer columns. The programs prompt for the input filename at runtime.

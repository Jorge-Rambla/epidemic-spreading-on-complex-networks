# Mutant Epidemic Spreading on Complex Networks

This repository is a documented archive of undergraduate thesis code for studying mutant epidemic spreading on complex networks. The project uses the Pokec social network dataset from SNAP, cleans the directed relationship graph into reciprocal contacts, samples smaller subgraphs, and runs stochastic event-driven epidemic simulations with infection, recovery, and mutation events.

This repository is intended as a documented archive of the scientific computing code used in my undergraduate thesis, not as a production-ready epidemic simulation package.

## Motivation

Epidemic processes on social networks are shaped by contact structure. Real networks are heterogeneous: some nodes have many contacts, many nodes have few contacts, and local connectivity can change epidemic thresholds and persistence. This thesis code explores how a mutant epidemic process behaves on empirical and sampled complex networks, with special attention to phase-transition-like behavior.

## Scientific Background

The classical SIR model divides nodes into susceptible, infected, and recovered states. Infected nodes transmit the disease to susceptible neighbors, then recover and no longer participate in transmission.

The SIS model also uses susceptible and infected states, but recovered nodes immediately become susceptible again. This makes SIS dynamics useful for studying endemic persistence and epidemic thresholds on networks.

The mutant epidemic model extends epidemic dynamics by allowing infected strains to mutate. The implementation tracks multiple strains, active infection links, recovery events, and mutation events. This makes it possible to study when mutation allows continued epidemic survival even when individual strains die out.

Complex networks represent populations as nodes connected by edges. In this project, edges are social contacts derived from reciprocal Pokec relationships. Network topology matters because infection events occur across edges rather than through homogeneous mixing.

The simulations use a Gillespie-style event-driven stochastic process. Instead of updating all nodes at fixed time steps, the code samples the next event type and event time from rates associated with infection, recovery, and mutation.

## Dataset

The empirical network source is the Pokec social network from SNAP:

https://snap.stanford.edu/data/soc-Pokec.html

The original dataset is not included in this repository. It is large, externally maintained, and should be downloaded directly from SNAP. See `data/README.md` for where to place local data files.

## Repository Structure

```text
mutant-epidemics-complex-networks/
|-- README.md
|-- .gitignore
|-- Makefile
|-- paper/
|   `-- TFG_Rambla_Campesino_Jorge.pdf
|-- src/
|   |-- cleaning/
|   |   |-- limpieza.c
|   |   `-- limpieza2.c
|   |-- graph_sampling/
|   |   `-- subgraf.c
|   |-- simulation/
|   |   |-- Xarxes5.c
|   |   |-- suscep.c
|   |   `-- fases6.c
|-- data/
|   `-- README.md
|-- docs/
|   |-- pipeline.md
|   `-- program_inventory.md
|-- results/
|   `-- figures/
```

## Program Overview

- `limpieza2.c`: cleans Pokec by keeping reciprocal relationships, removing repeated edges and self-loops, and writing cleaned node and edge files.
- `limpieza.c`: alternative/preliminary cleaning script, including filtering of high-degree nodes and writing `nodes.dat` and `aristes.dat`.
- `subgraf.c`: generates smaller subgraphs using a random-walk-like sampling process.
- `Xarxes5.c`: runs a single mutant epidemic simulation and outputs temporal evolution.
- `suscep.c`: runs many non-mutant simulations over infection rates to estimate the epidemic threshold using susceptibility.
- `fases6.c`: runs many mutant simulations over mutation rates for a fixed infection rate to estimate survival/endemic probabilities and build the phase diagram.

## Main Result

The simulations support a critical mutation threshold of the form:

```text
gamma_c = 1 / (N (lambda - lambda_c)) + gamma_c(infinity)
```

Here `lambda` is the infection rate, `lambda_c` is the epidemic threshold estimated from non-mutant simulations, `N` is the network size, and `gamma_c(infinity)` represents the large-system limiting contribution.

## Compile

Build all programs:

```sh
make
```

Build individual programs:

```sh
make limpieza2
make subgraf
make xarxes5
make suscep
make fases6
```

Equivalent direct `gcc` examples:

```sh
mkdir -p bin
gcc -O2 -Wall -Wextra src/cleaning/limpieza2.c -o bin/limpieza2 -lm
gcc -O2 -Wall -Wextra src/graph_sampling/subgraf.c -o bin/subgraf -lm
gcc -O2 -Wall -Wextra src/simulation/Xarxes5.c -o bin/Xarxes5 -lm
gcc -O2 -Wall -Wextra src/simulation/suscep.c -o bin/suscep -lm
gcc -O2 -Wall -Wextra src/simulation/fases6.c -o bin/fases6 -lm
```

Remove compiled binaries:

```sh
make clean
```

## Run

The programs prompt for an input filename. Input files are edge lists with two integer columns:

```text
source_node target_node
```

Example:

```sh
./bin/limpieza2
./bin/subgraf
./bin/Xarxes5
./bin/suscep
./bin/fases6
```

Expected generated outputs include:

- `nodes22.dat`: node pointer and degree data from the reciprocal cleaning step.
- `aristes22.dat`: reciprocal edge list from the cleaning step.
- `nodes.dat` and `aristes.dat`: outputs from the preliminary high-degree-filter cleaning script.
- `subg.dat`: sampled subgraph edge list.
- `Evol.dat`: temporal evolution from a single mutant simulation.
- `suscep.dat`: susceptibility values across infection rates.
- `fases2.dat`: mutation scan output for phase-diagram analysis.

Generated `.dat` files are ignored by Git.

## Limitations

- The C programs are legacy undergraduate thesis code.
- Most scientific parameters are set with `#define` constants inside the source files.
- The original Pokec dataset is not included.
- Direct reproducibility is not guaranteed without the original data, compiler, operating system, random seeds, and computational environment.
- The code is archived with minimal structural changes so that the thesis methodology and simulation behavior remain intact.

## Interview-Friendly Summary

This project demonstrates stochastic simulation, complex-network analysis, C programming, dynamic memory management, phase transitions, and mathematical modelling. The code builds a workflow from empirical social-network data to event-driven epidemic simulations, then uses repeated experiments to estimate epidemic and mutation thresholds.

See `docs/pipeline.md` for the workflow and `docs/program_inventory.md` for details about each archived program.

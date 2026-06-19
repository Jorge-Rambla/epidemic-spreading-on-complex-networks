# Program Inventory

## `src/cleaning/limpieza2.c`

- Purpose: clean the directed Pokec relationship network by keeping reciprocal relationships and removing self-loops or repeated links.
- Inputs: text edge list with two integer columns.
- Outputs: `nodes22.dat` and `aristes22.dat`.
- Main parameters: inferred from the input file; no major compile-time scientific parameter is exposed.
- Role in the thesis: builds the reciprocal-contact network used as the basis for later sampling and simulations.
- Status: core archived program.

## `src/cleaning/limpieza.c`

- Purpose: preliminary cleaning script that removes repeated links and self-loops while filtering edges attached to very high-degree nodes.
- Inputs: text edge list with two integer columns.
- Outputs: `nodes.dat` and `aristes.dat`.
- Main parameters: high-degree filtering threshold hard-coded as `grad[i] > 1400 || grad[j] > 1400`.
- Role in the thesis: legacy or auxiliary cleaning workflow used before the reciprocal-contact cleaning variant.
- Status: legacy/auxiliary archived program.

## `src/graph_sampling/subgraf.c`

- Purpose: generate a smaller subgraph from a larger cleaned network.
- Inputs: text edge list with two integer columns.
- Outputs: `subg.dat`.
- Main parameters: `NODOS`, the target number of sampled nodes.
- Role in the thesis: creates computationally smaller networks for simulation experiments.
- Status: core archived program.

## `src/simulation/Xarxes5.c`

- Purpose: run one mutant epidemic simulation and record the temporal evolution of infected strains.
- Inputs: text edge list with two integer columns, usually a cleaned network or sampled subgraph.
- Outputs: `Evol.dat`; optional network diagnostics are present in commented code.
- Main parameters: `LAM` infection rate, `DEL` recovery rate, `GAM` mutation rate, `MAX` maximum number of strains, `RANG`, and `LIMIT`.
- Role in the thesis: produces representative stochastic trajectories for the mutant epidemic process.
- Status: core archived program.

## `src/simulation/suscep.c`

- Purpose: run many non-mutant simulations over infection-rate values and estimate the epidemic threshold through susceptibility.
- Inputs: text edge list with two integer columns.
- Outputs: `suscep.dat`.
- Main parameters: `DEL`, `GAM`, `MAX`, `RANG`, `LIMIT`, and `simu`; the infection rate is scanned internally.
- Role in the thesis: estimates the baseline epidemic threshold `lambda_c`.
- Status: core archived program.

## `src/simulation/fases6.c`

- Purpose: run many mutant epidemic simulations over mutation-rate values at fixed infection rate.
- Inputs: text edge list with two integer columns.
- Outputs: `fases2.dat`.
- Main parameters: `DEL`, `MAX`, `RANG`, `LIMIT`, `nodos`, `LAM`, `simulacions`, `ranggam`, `salt`, and `ini`.
- Role in the thesis: estimates survival and endemic probabilities used to build the mutation phase diagram.
- Status: core archived program.

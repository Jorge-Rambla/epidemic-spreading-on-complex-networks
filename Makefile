CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra
LDFLAGS ?= -lm
BIN_DIR := bin

.PHONY: all clean directories high-degree-clean reciprocal-clean subgraph-sampler mutant-trajectory susceptibility-scan mutation-phase-scan

all: high-degree-clean reciprocal-clean subgraph-sampler mutant-trajectory susceptibility-scan mutation-phase-scan

directories:
	mkdir -p $(BIN_DIR)

high-degree-clean: directories
	$(CC) $(CFLAGS) src/cleaning/high_degree_filter_cleaning.c -o $(BIN_DIR)/high_degree_filter_cleaning $(LDFLAGS)

reciprocal-clean: directories
	$(CC) $(CFLAGS) src/cleaning/reciprocal_contact_cleaning.c -o $(BIN_DIR)/reciprocal_contact_cleaning $(LDFLAGS)

subgraph-sampler: directories
	$(CC) $(CFLAGS) src/graph_sampling/random_walk_subgraph_sampler.c -o $(BIN_DIR)/random_walk_subgraph_sampler $(LDFLAGS)

mutant-trajectory: directories
	$(CC) $(CFLAGS) src/simulation/mutant_trajectory_simulation.c -o $(BIN_DIR)/mutant_trajectory_simulation $(LDFLAGS)

susceptibility-scan: directories
	$(CC) $(CFLAGS) src/simulation/susceptibility_threshold_scan.c -o $(BIN_DIR)/susceptibility_threshold_scan $(LDFLAGS)

mutation-phase-scan: directories
	$(CC) $(CFLAGS) src/simulation/mutation_phase_diagram_scan.c -o $(BIN_DIR)/mutation_phase_diagram_scan $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)

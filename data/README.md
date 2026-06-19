# Data

The original Pokec social network dataset is not included in this repository.

Download the relationship data from the SNAP Pokec dataset page:

https://snap.stanford.edu/data/soc-Pokec.html

Recommended local layout:

```text
data/
`-- raw/
    `-- soc-pokec-relationships.txt
```

The C programs expect edge-list input files with two integer node identifiers per line. Keep raw and generated data files out of version control. The repository `.gitignore` excludes `data/raw/`, compressed datasets, and generated `.dat` files.

Typical workflow:

1. Download the Pokec relationship file from SNAP.
2. Place it under `data/raw/`.
3. Run the cleaning program from a working directory where generated outputs can be written.
4. Use the cleaned reciprocal edge list or sampled subgraphs as inputs to the simulation programs.

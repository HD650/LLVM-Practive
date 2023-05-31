# Dominance Analysis (WIP)

Implement two variants of dominance analysis:
1. Simple Fast Dominator Algorithm (Keith D. Cooper)
2. Lengauer-Tarjan Algorithm

Implement a dominator tree that:
1. holds the analysis result
2. could efficiently determine if A dominates B
3. could be efficiently updated (future?)

Implement unittest system:
1. build and link llvm
2. run gtest suit to test the correctness of the analysis

## Simple Fast Dominator Algorithm (Keith D. Cooper)
See files `IterativeDominanceAnalysis.h`.


## build, debug build and run
Refer to HelloWorld pass.

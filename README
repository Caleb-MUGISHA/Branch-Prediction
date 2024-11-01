# Custom Branch Predictor Implementation in gem5

## Overview
This project implements a custom branch predictor in the gem5 simulator. The branch predictor uses a NAND-based prediction scheme that combines branch address bits with global history bits to make predictions about branch behavior.

## Configuration
The branch predictor is configured with the following parameters:
- **Predictor Size**: 64 entries (2^6)
- **Counter Bits**: 2 bits per counter (values 0-3)
- **Global History Bits**: 6 bits
- **Operation**: NAND between branch address and global history

### Branch Predictor Components

1. **Pattern History Table (PHT)**
   - Size: 64 entries
   - Each entry: 2-bit saturating counter
   - Counter values:
     * 00 (0): Strongly Not Taken
     * 01 (1): Weakly Not Taken
     * 10 (2): Weakly Taken
     * 11 (3): Strongly Taken

2. **Global History Register**
   - Size: 6 bits
   - Updated speculatively during prediction
   - Restored on misprediction

3. **Index Calculation**
   - Uses NAND operation between:
     * 6 bits from branch address
     * 6 bits from global history
   - Final index masked to fit PHT size

## Implementation Details

### Key Files
1. `custom.hh`: Header file containing class definition
2. `custom.cc`: Implementation of branch predictor functionality
3. `BranchPredictor.py`: Python configuration file

### Key Functions
1. `lookup`: Makes branch predictions
   - Uses global history and branch address
   - Updates history speculatively
   - Returns prediction (taken/not taken)

2. `update`: Updates predictor state
   - Updates counters based on actual branch outcome
   - Handles misprediction recovery
   - Updates global history with actual outcome

3. `getPHTIndex`: Calculates PHT index
   - Performs NAND operation
   - Ensures index fits within table size

## Building and Running

1. Place files in the correct directories:
```bash
gem5/src/cpu/pred/custom.hh
gem5/src/cpu/pred/custom.cc
```

2. Build gem5:
```bash
scons build/ARM/gem5.opt
```

3. Run benchmark:
```bash
./build/ARM/gem5.opt -d /path/to/output ./configs/example/se.py --cpu-type=DerivO3CPU --caches --l2cache --bp-type=CustomBP -c /path/to/benchmark
```

## Performance Analysis

Branch prediction accuracy can be calculated from stats.txt:
```
Accuracy = ((Predicted - Incorrect) / Predicted) * 100
```
Where:
- Predicted = system.cpu.branchPred.condPredicted
- Incorrect = system.cpu.branchPred.condIncorrect


## Testing
The implementation was tested using two benchmarks:
1. dijkstra_small
2. qsort_small

Each benchmark generates configuration files and statistics that can be used to analyze predictor performance.

## Design Decisions
1. Used simple unsigned integers for counters instead of SatCounter8 for stability
2. Implemented thorough error checking and bounds validation
3. Proper memory management for history objects
4. Dynamic mask calculations based on configuration parameters

## References
1. gem5 Documentation: http://www.gem5.org/documentation/
2. Branch Prediction Theory
3. GShare and Tournament Predictor Implementations

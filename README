# Custom Branch Predictor Implementation in gem5

This project implements a custom branch predictor in the gem5 simulator, utilizing a **NAND-based prediction scheme** that combines branch address bits with global history bits.

---

## Configuration

### Parameters

- **Predictor Size**: 64 entries (2<sup>6</sup>)
- **Counter Bits**: 2 bits per counter (values 0-3)
- **Global History Bits**: 6 bits
- **Operation**: NAND operation between branch address and global history

---

## Branch Predictor Components

1. **Pattern History Table (PHT)**
   - **Size**: 64 entries
   - **Each Entry**: 2-bit saturating counter
   - **Counter Values**:
     - `00` (0): Strongly Not Taken
     - `01` (1): Weakly Not Taken
     - `10` (2): Weakly Taken
     - `11` (3): Strongly Taken

2. **Global History Register**
   - **Size**: 6 bits
   - **Updated**: Speculatively during prediction; restored on misprediction

3. **Index Calculation**
   - **Uses**: NAND operation between:
     - 6 bits from branch address
     - 6 bits from global history
   - **Final Index**: Masked to fit PHT size

---

## Implementation Details

### Key Files

- **custom.hh**: Header file containing the class definition
- **custom.cc**: Implementation of branch predictor functionality
- **BranchPredictor.py**: Python configuration file

### Key Functions

- **`lookup`**: Makes branch predictions
  - Uses global history and branch address
  - Updates history speculatively
  - Returns prediction (taken/not taken)

- **`update`**: Updates predictor state
  - Updates counters based on actual branch outcome
  - Handles misprediction recovery
  - Updates global history with the actual outcome

- **`getPHTIndex`**: Calculates the PHT index
  - Performs NAND operation
  - Ensures index fits within table size

---

## Building and Running

### Steps

1. **Place files in correct directories**:
   ```bash
   gem5/src/cpu/pred/custom.hh
   gem5/src/cpu/pred/custom.cc
---



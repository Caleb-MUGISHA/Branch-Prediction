#include "cpu/pred/custom.hh"
#include "base/bitfield.hh"
#include "base/intmath.hh"
#include <cstdio>

CustomBP::CustomBP(const CustomBPParams &params)
    : BPredUnit(params),
      PredictorSize(params.PredictorSize),
      globalHistoryReg(params.numThreads, 0),
      globalHistoryMask(mask(params.globalHistoryBits)),
      PHTCtrBits(params.PHTCtrBits),
      predictorCounters(params.PredictorSize, SatCounter8(params.PHTCtrBits)),
      indexMask(mask(params.globalHistoryBits))
{
    if (!isPowerOf2(PredictorSize)) {
        fatal("Invalid predictor size!\n");
    }
}

inline
bool
CustomBP::getPrediction(uint8_t &count)
{
    return (count >> (PHTCtrBits - 1));
}

inline
unsigned
CustomBP::getPredictorIndex(Addr &branch_addr, unsigned historyReg)
{
    // NAND indexing
    return ~((branch_addr >> instShiftAmt) & historyReg) & indexMask;
}

void
CustomBP::updateGlobalHistReg(ThreadID tid, bool taken)
{
    globalHistoryReg[tid] = taken ? (globalHistoryReg[tid] << 1) | 1 :
                               (globalHistoryReg[tid] << 1);
    globalHistoryReg[tid] &= globalHistoryMask;
}

bool
CustomBP::lookup(ThreadID tid, Addr branch_addr, void * &bp_history)
{
    unsigned predictor_idx = getPredictorIndex(branch_addr, globalHistoryReg[tid]);
    uint8_t counter_val = predictorCounters[predictor_idx];
    bool prediction = getPrediction(counter_val);

    BPHistory *history = new BPHistory;
    history->globalHistoryReg = globalHistoryReg[tid];
    history->finalPred = prediction;
    bp_history = static_cast<void*>(history);

    updateGlobalHistReg(tid, prediction);

    return prediction;
}

void
CustomBP::update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                bool squashed, const StaticInstPtr & inst, Addr corrTarget)
{
    assert(bp_history);

    BPHistory *history = static_cast<BPHistory*>(bp_history);

    if (squashed) {
        if(taken)
            globalHistoryReg[tid] = (history->globalHistoryReg << 1) | 1;
        else
            globalHistoryReg[tid] = (history->globalHistoryReg << 1);
        globalHistoryReg[tid] &= globalHistoryMask;
    } else {
        unsigned predictor_idx = getPredictorIndex(branch_addr, history->globalHistoryReg);
        
        if (taken) {
            predictorCounters[predictor_idx]++;
        } else {
            predictorCounters[predictor_idx]--;
        }

        delete history;
    }
}

void
CustomBP::btbUpdate(ThreadID tid, Addr branch_addr, void * &bp_history)
{
    BPHistory *history = new BPHistory;
    history->globalHistoryReg = globalHistoryReg[tid];
    history->finalPred = false;
    bp_history = static_cast<void*>(history);

    globalHistoryReg[tid] &= (globalHistoryMask & ~ULL(1));
}

void
CustomBP::uncondBranch(ThreadID tid, Addr pc, void * &bp_history)
{
    BPHistory *history = new BPHistory;
    history->globalHistoryReg = globalHistoryReg[tid];
    history->finalPred = true;
    bp_history = static_cast<void*>(history);
    
    updateGlobalHistReg(tid, true);
}

void
CustomBP::squash(ThreadID tid, void *bp_history)
{
    if (!bp_history) return;

    BPHistory *history = static_cast<BPHistory*>(bp_history);
    globalHistoryReg[tid] = history->globalHistoryReg;
    delete history;
}

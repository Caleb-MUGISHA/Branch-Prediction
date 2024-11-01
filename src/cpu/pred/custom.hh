#ifndef __CPU_PRED_CUSTOM_HH__
#define __CPU_PRED_CUSTOM_HH__

#include <vector>
#include "base/sat_counter.hh"
#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/CustomBP.hh"

class CustomBP : public BPredUnit
{
  public:
    CustomBP(const CustomBPParams &params);

    virtual void uncondBranch(ThreadID tid, Addr pc, void * &bp_history);
    bool lookup(ThreadID tid, Addr branch_addr, void * &bp_history);
    void btbUpdate(ThreadID tid, Addr branch_addr, void * &bp_history);
    void update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                bool squashed, const StaticInstPtr & inst, Addr corrTarget);
    void squash(ThreadID tid, void *bp_history);

  private:
    struct BPHistory {
      unsigned globalHistoryReg;
      bool finalPred;
    };

    inline bool getPrediction(uint8_t &count);
    inline unsigned getPredictorIndex(Addr &branch_addr, unsigned historyReg);
    void updateGlobalHistReg(ThreadID tid, bool taken);

    const unsigned PredictorSize;
    std::vector<unsigned> globalHistoryReg;
    unsigned globalHistoryMask;
    const unsigned PHTCtrBits;
    std::vector<SatCounter8> predictorCounters;
    const unsigned indexMask;
    const unsigned instShiftAmt = 2;
};

#endif // __CPU_PRED_CUSTOM_HH__

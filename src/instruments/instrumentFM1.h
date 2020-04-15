#ifndef INSTRUMENTFM1
#define INSTRUMENTFM1

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentFM1: public upc::Instrument {
    EnvelopeADSR adsr;
    
    float phase ;
    float step;
    float F0;
  	float A;
    float I1;
    float phase_vib;
    float step_vib;
    float fm1;
  
  public:
    InstrumentFM1(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif

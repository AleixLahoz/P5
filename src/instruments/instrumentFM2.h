#ifndef INSTRUMENTFM2
#define INSTRUMENTFM2

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentFM2: public upc::Instrument {
    EnvelopeADSR adsr;
    
    float phase ;
    float step;
    float F0;
  	float A;
    float I1;
    float phase_vib;
    float step_vib;
    float fm;
    float N1;
    float N2;
  
  public:
    InstrumentFM2(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif

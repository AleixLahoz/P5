#ifndef WAV
#define WAV

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"
#include "sndfile.h"


namespace upc {
  class Wav: public upc::Instrument {
    EnvelopeADSR adsr;    
    std::string file_name;
    double *audioData;
    int readcount;
    SNDFILE *inFile;
    SF_INFO inFileInfo;
  public:
    Wav(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif

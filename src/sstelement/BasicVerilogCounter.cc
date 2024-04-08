//
// _BasicVerilogCounter_cc_
//

#include "BasicVerilogCounter.h"

#define LOW 0
#define HIGH 1

using namespace SST::VerilatorSST;

BasicVerilogCounter::BasicVerilogCounter(ComponentId_t id, Params& params)
  : Component(id) {
  out = new Output("", 1, 0, Output::STDOUT);
  const std::string clockFreq = params.find<std::string>("CLOCK_FREQ", "1GHz");
  uint16_t stop = params.find<std::uint16_t>("STOP", "20");

  verilatorSetup(stop);
  registerAsPrimaryComponent();
  primaryComponentDoNotEndSim();

  registerClock(clockFreq, new Clock::Handler<BasicVerilogCounter>(this, &BasicVerilogCounter::clock));
  out->output("Registering clock with frequency=%s\n", clockFreq.c_str());
  out->output("Counter set to stop=%u\n", stop);
}

BasicVerilogCounter::~BasicVerilogCounter(){
  delete out;
}

void BasicVerilogCounter::verilatorSetup(uint16_t stop){
  top = std::make_unique<VerilatorSST>();

  Signal init_low(1,LOW);
  Signal init_stop(8,stop);

  top->writePort("clk", init_low);
  top->writePort("reset_l", init_low);
  top->writePort("stop", init_stop);

  Signal reset_delay(1,HIGH);
  top->writePortAtTick("reset_l",reset_delay,5);
  top->tick(1);
}

bool BasicVerilogCounter::testBenchPass(){
  Signal done;
  top->readPort("done", done);

  bool pass = done.getUIntValue<uint8_t>() == HIGH;
  if(pass){
    top->finish();
    primaryComponentOKToEndSim();
  }

  return pass;
}

bool BasicVerilogCounter::clock(Cycle_t cycles){
  if(testBenchPass()){
    return true;
  }

  top->tickClockPeriod("clk");
  return false;
}

#ifndef _Test_HARNESS_H_
#define _Test_HARNESS_H_

#include "testMacros.h"

// -- Verilator SST Headers
#include "verilatorSSTSubcomponent.h"

namespace SST::VerilatorSST {

class TestHarness : public SST::Component {
public:
  /// TestHarness: constuctor
  TestHarness(SST::ComponentId_t id, const SST::Params& params);

  /// TestHarness: destructor
  ~TestHarness();

  /// TestHarness: setup function
  void setup();

  /// TestHarness: finish function
  void finish();

  /// TestHarness: init function
  void init( unsigned int phase );

  /// TestHarness: clock function
  bool clock(SST::Cycle_t currentCycle );

  // -------------------------------------------------------
  // TestHarness Component Registration Data
  // -------------------------------------------------------
  SST_ELI_REGISTER_COMPONENT(
    TestHarness,  // component class
    "verilatorssttests",// component library
    "TestHarness",// component name
    SST_ELI_ELEMENT_VERSION( 1, 0, 0 ),
    "VerilatorSST Test Harness Component",
    COMPONENT_CATEGORY_UNCATEGORIZED
  )

  // -------------------------------------------------------
  // TestHarness Component Parameter Data
  // -------------------------------------------------------
  // clang-format off
  SST_ELI_DOCUMENT_PARAMS(
    {"verbosity",     "Sets the verbosity",       "0"},
    {"clockFreq",   "Clock frequency",          "1GHz"},
    {"numCycles",   "Number of cycles to exec", "1000"},
  )

  // -------------------------------------------------------
  // TestHarness Port Parameter Data
  // -------------------------------------------------------
  SST_ELI_DOCUMENT_PORTS(
  )

  // -------------------------------------------------------
  // TestHarness SubComponent Parameter Data
  // -------------------------------------------------------
  SST_ELI_DOCUMENT_SUBCOMPONENT_SLOTS(
    {"dut", "Verilator Subcomponent Model",   "SST::VerilatorSST::VerilatorSSTBase"},
  )
private:
  SST::Output    *output;                        ///< TestHarness: SST output
  uint32_t numCycles;                           ///< TestHarness: number of cycles to execute
  SST::VerilatorSST::VerilatorSSTBase *dut;     ///< TestHarness: subcomponent dut

  void runTestSuite(SST::Cycle_t currentCycle);

};  // class TestHarness

};  // namespace SST::VerilatorSST

#endif  // _VERILATOR_TEST_DIRECT_H_

// EOF

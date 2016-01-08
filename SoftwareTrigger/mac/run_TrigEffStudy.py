import os, sys

if len(sys.argv) < 3:
    msg  = '\n'
    msg += "Usage: %s $INPUT_CFG_FILE $INPUT_ROOT_FILE(s) $OUTPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

import ROOT as rt
#rt.gSystem.Load("libFEMemulator.so")
from ROOT import fememu
from ROOT import larlite as fmwk
from fememu_pycfg import apply_config

#
# Get configuration
#
cfg_file = sys.argv[1]
config = rt.fememu.FEMBeamTriggerConfig()
if not apply_config(config,cfg_file):
    print '\033[91m[ERROR]\033[00m exiting...'
    sys.exit(1)


# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    if x < 2: continue

    my_proc.add_input_file(sys.argv[x])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file(sys.argv[-1]);

# Attach a template process
mod = fmwk.TrigEffStudy()
mod.setConfig(config)
#mod.setWindowPreTruthPartTime(0.01)
#mod.setWindowPostTruthPartTime(0.1)
mod.setWindowPreTruthPartTime(4.)
mod.setWindowPostTruthPartTime(10.)  
mod.setUseMC(False)
my_proc.add_process(mod)

print
print  "Finished configuring ana_processor. Start event loop!"
print

# Let's run it.
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)


import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from ROOT import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)
# Set input root files
for x in xrange(len(sys.argv)):
    if x == 0: continue
    my_proc.add_input_file(sys.argv[x])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("TrackEnergyPlots_MCTracksContainedFilter_ana_out.root")

# Attach filters

# Select events with 1 muon in final state
mypdgfilter = fmwk.PDGSelection()
mypdgfilter.Select(13,mypdgfilter.kGENERATOR,1)
my_proc.add_process(mypdgfilter)

# Select events with all tracks that are fully contained
myfilter = fmwk.MCTracksContainedFilter()
my_proc.add_process(myfilter)

# Attach plot maker
myplotter = fmwk.TrackEnergyPlots()
my_proc.add_process(myplotter)

print
print  "Finished configuring ana_processor. Start event loop!"
print

# Let's run it.
my_proc.run();

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)


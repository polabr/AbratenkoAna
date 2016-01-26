import sys, os

if len(sys.argv) < 3:
    msg  = '\n'
    msg += "Usage 1: %s $RUN_MODE('cosmic','mu','e') $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from seltool import ertool
from larlite import larlite as fmwk
from ROOT import flashana
from ROOT import gSystem

# Create ERTool analysis
my_ana = ertool.ERAnaFlashMatchValidation()
run_modes = { 'cosmic' : my_ana.kCosmics, 'mu' : my_ana.kSingleMuons, 'e' : my_ana.kSingleElectrons }

if sys.argv[1] not in run_modes.keys():
	print "You need to set the run mode as 'cosmic', 'mu', or 'e' as the first argument of this script."
	quit()

myrunmode = sys.argv[1]
my_ana.SetRunMode(run_modes[myrunmode])

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-2):
    my_proc.add_input_file(sys.argv[x+2])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("flashmashvalidation_anaout.root")

# Create ERTool algorithm
fm_algo = ertool.ERAlgoFlashMatch()
# ERAlgoFlashMatch ignores showers if running on cosmics or single mus
fm_algo.SetIgnoreShowers(myrunmode in ['cosmic', 'mu'])
fm_algo.SetIgnoreCosmics(False)

# Create larlite interfce analysis unit for ERTool
my_anaunit = fmwk.ExampleERSelection()
my_anaunit.setDisableXShift(False)
my_anaunit._mgr._mc_for_ana = True

# Set Producers
# First Argument: True = MC, False = Reco
# Second Argument: producer module label
my_anaunit.SetShowerProducer(True,"mcreco")
my_anaunit.SetTrackProducer(True,"mcreco")
my_anaunit.SetFlashProducer("opflash")


my_anaunit._mgr.AddAlgo(fm_algo)
my_anaunit._mgr.AddAna(my_ana)
my_ana._mode =True # True = Select. False = Fill mode
my_proc.add_process(my_anaunit)

# run!
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

#fm_algo.StoreParams()
sys.exit(0)


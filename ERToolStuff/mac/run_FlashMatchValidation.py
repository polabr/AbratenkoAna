import sys, os

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from seltool import ertool
from larlite import larlite as fmwk
from ROOT import flashana

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("")

# Create ERTool algorithm
fm_algo = ertool.ERAlgoFlashMatch()

# Copying configuration from flash_tagging.py
# TPC Filter Algo
fm_algo.Manager().SetAlgo(flashana.NPtFilter())
# PMT Filter Algo
fm_algo.Manager().SetAlgo(flashana.MaxNPEWindow())
# Match Prohibit Algo
fm_algo.Manager().SetAlgo(flashana.TimeCompatMatch())
# Hypothesis Algo
fm_algo.Manager().SetAlgo(flashana.PhotonLibHypothesis())
# Match Algo
#fm_algo.Manager().SetAlgo( flashana.QLLMatch.GetME() )
fm_algo.Manager().SetAlgo( flashana.QWeightPoint()   )
#fm_algo.Manager().SetAlgo( flashana.CommonAmps()      )
fm_algo.Manager().Configure( "%s/SelectionTool/OpT0Finder/App/mac/flashmatch.fcl" % os.environ['LARLITE_USERDEVDIR'])

# Create ERTool analysis
my_ana = ertool.ERAnaFlashMatchValidation()

# Create larlite interfce analysis unit for ERTool
my_anaunit = fmwk.ExampleERSelection()
#setDisableXShift(True) for cosmics. If cosmic gets shifted outside of cryostat, its QCluster will have 0 size
#from lightpath module
my_anaunit.setDisableXShift(True)

# Set Producers
# First Argument: True = MC, False = Reco
# Second Argument: producer module label
my_anaunit.SetShowerProducer(True,"mcreco")
my_anaunit.SetTrackProducer(True,"mcreco")
my_anaunit.SetFlashProducer("opflash")

# Implement manager
my_anaunit._mgr.AddAlgo(fm_algo)
my_anaunit._mgr.AddAna(my_ana)
my_ana._mode =True # True = Select. False = Fill mode
my_proc.add_process(my_anaunit)

# run!
my_proc.run(0,1)

# done!
print
print "Finished running ana_processor event loop!"
print

#fm_algo.StoreParams()
sys.exit(0)


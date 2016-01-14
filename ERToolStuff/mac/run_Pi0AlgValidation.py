import sys, os

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)


from ROOT import gSystem
from ROOT import larlite as fmwk
from ROOT import ertool


# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("pi0AlgValidation_ana_out.root")

Ecut = 50 # in MeV

pi0ana = ertool.ERAnaPi0AlgValidation()

main_anaunit = fmwk.ExampleERSelection()
main_anaunit._mgr.ClearCfgFile()
main_anaunit._mgr.AddCfgFile(os.environ['LARLITE_USERDEVDIR']+'/SelectionTool/ERTool/dat/ertool_default.cfg')

main_anaunit.SetShowerProducer(True,'mcreco')
main_anaunit.SetTrackProducer(True,'mcreco')

# Create algorithm
my_algo = ertool.ERAlgopi0()
my_algo.setMinMass(40.) #40
my_algo.setMaxMass(220.) #220
my_algo.setMaxIP(10.) #10
my_algo.setMinShrEnergy(10.) #10

main_anaunit._mgr.AddAlgo(my_algo)
main_anaunit._mgr.AddAna(pi0ana)
main_anaunit._mgr._profile_mode = True

main_anaunit.SetMinEDep(Ecut)
main_anaunit._mgr._mc_for_ana = True

my_proc.add_process(main_anaunit)

my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)


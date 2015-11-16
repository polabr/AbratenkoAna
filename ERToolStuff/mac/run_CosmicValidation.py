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

from seltool.primarycosmicDef import GetPrimaryCosmicFinderInstance

# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("cosmicValidation_ana_out.root")

# primary cosmic algoithm 
# this information is loaded from:
# $LARLITE_BASEDIR/python/seltool/primarycosmicDef.py
cosmicprimary_algo = GetPrimaryCosmicFinderInstance()
# cosmicsecondary_algo = ertool.ERAlgoCRSecondary()
# cosmicorphanalgo = ertool.ERAlgoCROrphan()

Ecut = 50 # in MeV

cos_ana = ertool.ERAnaCosmicValidation()
cos_ana.SetECut(Ecut)

main_anaunit = fmwk.ExampleERSelection()
main_anaunit._mgr.ClearCfgFile()
main_anaunit._mgr.AddCfgFile(os.environ['LARLITE_USERDEVDIR']+'/SelectionTool/ERTool/dat/ertool_default.cfg')

main_anaunit.SetShowerProducer(True,'mcreconew')
main_anaunit.SetTrackProducer(True,'mcreconew')
main_anaunit._mgr.AddAlgo(cosmicprimary_algo)
# main_anaunit._mgr.AddAlgo(cosmicsecondary_algo)
# main_anaunit._mgr.AddAlgo(cosmicorphanalgo)

main_anaunit._mgr.AddAna(cos_ana)
main_anaunit._mgr._profile_mode = True

main_anaunit.SetMinEDep(Ecut)
main_anaunit._mgr._mc_for_ana = True

# my_proc.add_process(nuefilter)
my_proc.add_process(main_anaunit)

my_proc.run(0,100)
# my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)


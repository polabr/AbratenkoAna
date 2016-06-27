import sys
from ROOT import gSystem
gSystem.Load("libKalekoAna_DavidAnalysis")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing DavidAnalysis..."

sys.exit(0)


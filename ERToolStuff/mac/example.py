import sys
from ROOT import gSystem
gSystem.Load("libKalekoAna_ERToolStuff")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing ERToolStuff..."

sys.exit(0)


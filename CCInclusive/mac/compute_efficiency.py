import sys

if len(sys.argv) != 2:
    print "Usage: python %s input_file.root" % sys.argv[0]

infile = sys.argv[1]

issignalsample = 'signal' in infile

import pandas as pd
from root_numpy import root2array

df = pd.DataFrame( root2array( infile, 'evttree' ) )

cuts = "flash_in_bgw and is_areco_vtx_in_fidvol and is_atrack_fromvtx and longest_trk_contained and longest_trk_range_longenough"

if issignalsample:
    cuts += ' and is_truth_fiducial and is_numuCC'

nocuts = "is_truth_fiducial and is_numuCC"

num = float(len(df.query(cuts)))

denom = len(df) if not issignalsample else len(df.query(nocuts))

print "Efficiency: %d/%d = %0.2f%%" % (num,denom,100*(num/denom))

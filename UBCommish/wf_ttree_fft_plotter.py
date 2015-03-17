from ROOT import *
import numpy as np
import sys
import nputils as npu

### arguments are :
# file name
# crate number
# slot number
# channel number

filename = sys.argv[1]
mycrate = int(sys.argv[2])
myslot = int(sys.argv[3])
mychannel = int(sys.argv[4])

f = TFile(filename,"READ")

#Find the right entry number in the wftree ttree
tree_entries = wftree.GetEntries()
for tree_idx in xrange(tree_entries):
    dummy = wftree.GetEntry(tree_idx)
    if int(wftree.crate) != mycrate:
        continue
    if int(wftree.slot) != myslot:
        continue
    if int(wftree.femch) != mychannel:
        continue
    break

#Convert the waveform into a numpy array
dummy = wftree.GetEntry(tree_idx)
wf = np.array(wftree.wf)

myx, myy = npu.FFT_on_nparray(wf)

import matplotlib.pyplot as plt
plt.plot(myx[1:], (2.0/float(wf.size))*np.abs(myy[1:wf.size/2]))
plt.grid()
plt.show()

from ROOT import *
import numpy as np
import sys
import nputils as npu

### arguments are :
# file name 
# (~/Data/UBCommish/good_noise_wf_6monthsago_lartf.root or 
#  ~/Data/UBCommish/bad_noise_wf_after_voltage_fuckup.root)
# his name (WF_subRun_1_ev_1_chan_0 or hAvg_Ch_4864_ev_1_subRun_5)

filename = sys.argv[1]
histname = sys.argv[2]

f = TFile(filename,"READ")
h = f.Get(histname)

wf = npu.TH1F_to_nparray(h)

myx, myy = npu.FFT_on_nparray(wf)

import matplotlib.pyplot as plt
plt.plot(myx[1:], (2.0/float(wf.size))*np.abs(myy[1:wf.size/2]))
plt.grid()
plt.ylabel('FFT Value [arb]')
plt.xlabel('Frequency [Hz]')
plt.title('New Bad Noise Channel FFT')
plt.ylim(0.,7.)
plt.show()

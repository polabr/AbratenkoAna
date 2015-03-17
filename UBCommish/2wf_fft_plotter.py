from ROOT import *
import numpy as np
import sys
import nputils as npu
import matplotlib.pyplot as plt

### arguments are :
# run number, (larsoft) channel number

runno = int(sys.argv[1])
chno = int(sys.argv[2])

data_dir = '/Users/davidkaleko/Data/UBCommish/'
filename = data_dir + 'noise_run%0.3d.root'%runno
hnames = [ 'hWF_GOOD_%d'%chno, 'hWF_BAD_%d'%chno ]

f = TFile(filename,"READ")

fig, ax = plt.subplots()

for name in hnames:
    h = f.Get(name)
    wf = npu.TH1F_to_nparray(h)
    myx, myy = npu.FFT_on_nparray(wf)

    ax.plot(myx[1:], (2.0/float(wf.size))*np.abs(myy[1:wf.size/2]),label=name)
    plt.grid()
    plt.ylabel('FFT Value [arb]')
    plt.xlabel('Frequency [Hz]')
    plt.title('FFT Spectrum from Run%0.3d, Ch%d'%(runno,chno))
    plt.ylim(0.,7.)

plt.legend(loc='upper right', shadow=True)
plt.savefig('FFTSpectrum_run%0.3d_ch%d.png'%(runno,chno))

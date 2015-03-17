import os, sys
from ROOT import TH1F
import numpy as np
from scipy.fftpack import fft

def TH1F_to_nparray(hist):

#    if type(hist) is not TH1F:
#        raise AttributeError("Input histogram to TH1F_to_nparray is not type ROOT.TH1F.")

    n = hist.GetNbinsX()
    my_array = np.zeros(n)
    for x in xrange(n):
        my_array[x] = hist.GetBinContent(x)
    
    return my_array

def FFT_on_nparray(nparray):

    if type(nparray) is not np.ndarray:
        raise AttributeError("Input histogram to FFT_on_nparray is not type numpy.ndarray.")
    
    #Number of data points in waveform
    N = nparray.size
    #Spacing between points in waveform (500 ns for TPC waveform)
    T = 1.0/2000000.

    x = np.linspace(0.0,N*T,N)
    y = nparray

    yf = fft(y)
    xf = np.linspace(0.0, 1.0/(2.0*T), N/2)

    return xf, yf

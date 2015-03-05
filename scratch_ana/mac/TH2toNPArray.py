from ROOT import *
import numpy as np

f = TFile("outfile.root","READ")
h = f.Get("hSimChView")
h.SetDirectory(0)
f.Close()

xbins = h.GetNbinsX()
ybins = h.GetNbinsY()

final_array = np.zeros((ybins,xbins))

for x in xrange(xbins):
    for y in xrange(ybins):
        final_array[y,x] = h.GetBinContent(x,y)



# Python script to create graphs of: 
# (1) Truth momentum vs. reconstructed momentum
# (2) Entries vs. delta for each E range
# (3) Delta mean vs. truth E
# (4) Delta std. dev. vs. truth E
#
# Author: Polina Abratenko

import ROOT
from ROOT import TFile, gDirectory, TCanvas, TH1D, TColor, TGraphErrors, TMath, TLatex
from operator import truediv

# Open the file 
f = TFile("testmultiscattermomentum_output.root", "READ")

# See what is inside the file
print "Now looking inside the file:"
f.ls()

# See what variables are stored inside the tree by looking at just one entry
print "Showing one entry of the tree (note the variables):"
f.ana_tree.Show(0)

# Get the total number of entries in the tree
entries = f.ana_tree.GetEntries()
print "This is how many total entries are inside the tree:\n", entries

# Making graph (1):
# With TTree.Draw, first argument is the variable we are plotting, second
# argument is cuts we want to make, third argument is draw styles
print "This is the current graph of MCp vs. MCSp."
trueMomVsMcsRecoColz = TCanvas("trueMomVsMcsRecoColz", "trueMomVsMcsRecoColz", 720, 152, 682, 505)
trueMomVsMcsRecoColz.cd()
# Cut is placed to exclude failed events (-1)
f.ana_tree.Draw("true_mom:mcs_reco_mom", "mcs_reco_mom>0", "COLZ")
input = raw_input("Press enter to continue...")

###-----------------

# Function to create graphs (2), (3), (4):
def getHists(contained_only = False):
    
    # Define the desired energy ranges
    ERanges = [(0.3,0.5), (0.5,0.7), (0.7,0.9), (0.9,1.1), (1.1,1.3), (1.3,1.5), (1.5,1.7), (1.7,1.9), (1.9,2.1)]
    
    # Creating empty histograms
    hist_dictionary = {}
    for myE in ERanges:
        myName = "h_min%s_max%s" % (myE[0], myE[1])
        if contained_only: myName = "cont_h_min%s_max%s" % (myE[0], myE[1])
        myTitle = "Energies from %0.1f to %0.1f;(True p - Reco p) / True p;Entries" % (myE[0],  myE[1])
        if contained_only: myTitle = "Contained Tracks: Energies from %0.1f to %0.1f;(True p - Reco p) / True p;Entries" % (myE[0], myE[1])
        hist_dictionary[myE] = TH1D(myName, myTitle, 100, -4, 4)
        
    # Loop over energy ranges to create graphs of type (2)
    for myE in ERanges:
        myPlot = "(true_mom - mcs_reco_mom) / true_mom >> h_min%0.1f_max%0.1f" % (myE[0], myE[1])
        if contained_only: myPlot = "(true_mom - mcs_reco_mom) / true_mom >> cont_h_min%0.1f_max%0.1f" % (myE[0], myE[1])
        myCut = "mcs_reco_mom > 0 && true_mom > %f && true_mom < %f" % (myE[0], myE[1])
        if contained_only: myCut += " && mu_contained == 1"
        f.ana_tree.Draw(myPlot, myCut)
        
    # Create lists for graphs (3), (4) that will be filled in the next loop
    xpoints = []
    filMeanVals, filSqrtHistEntries = [], []
    filStdVals, filStdErrs = [], []
    
    for myE in ERanges: 
        # Filter out energy ranges with 0 entries
        if hist_dictionary[myE].GetEntries() != 0:
            xpoints.append(myE[0] + ( myE[1] - myE[0] ) / 2.)
            filMeanVals.append(hist_dictionary[myE].GetMean())
            filSqrtHistEntries.append(TMath.sqrt(hist_dictionary[myE].GetEntries()))
            filStdVals.append(hist_dictionary[myE].GetRMS())
            filStdErrs.append(hist_dictionary[myE].GetRMSError())
        
    # Calculate error bars for graph (3): (stdDev / sqrt(entries))
    yerrs = map(truediv, filStdVals, filSqrtHistEntries)

    # Define a constant width (x-error bar) for each marker
    xerrs = [( myE[1] - myE[0] ) / 2. for myE in ERanges]

    # Graph (3) and (4) setup
    meanGraph = TGraphErrors()
    stdGraph = TGraphErrors()

    meanGraphTitle = "Mean vs. Energy;True Muon Energy (GeV);#left[p_{MC}-p_{MCS} / p_{MC}#right]"
    if contained_only: meanGraphTitle = "Contained Tracks: Mean vs. Energy;True Muon Energy (GeV);#left[p_{MC}-p_{MCS} / p_{MC}#right]"
    stdGraphTitle = "Standard Deviation vs. Energy;True Muon Energy (GeV);Fractional Momentum Resolution"
    if contained_only: stdGraphTitle = "Contained Tracks: Standard Deviation vs. Energy;True Muon Energy (GeV);Fractional Momentum Resolution"

    meanGraph.SetTitle(meanGraphTitle)
    stdGraph.SetTitle(stdGraphTitle)

    # Fill in data for graph (3)
    for i in xrange(len(xpoints)):
        meanGraph.SetPoint(i, xpoints[i], filMeanVals[i])
        meanGraph.SetPointError(i, xerrs[i], yerrs[i])
    meanGraph.Draw("ALP")

    # Fill in data for graph (4)
    for i in xrange(len(xpoints)):
        stdGraph.SetPoint(i, xpoints[i], filStdVals[i])
        stdGraph.SetPointError(i, xerrs[i], filStdErrs[i])
    stdGraph.Draw("ALP")

    return hist_dictionary, meanGraph, stdGraph

myDict, meanGraph, stdGraph = getHists()
myDictCont, meanGraphCont, stdGraphCont = getHists(contained_only=True)

print "Now writing the graphs..."
    
fout = TFile("analyzePyOutput.root", "RECREATE")
fout.cd()

for myE, myhist in myDict.iteritems():
    myhist.Write()
meanGraph.Write()
stdGraph.Write()

for myE, myhist in myDictCont.iteritems():
    myhist.Write()
meanGraphCont.Write()
stdGraphCont.Write()

fout.Close()

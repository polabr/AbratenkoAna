import ROOT
from ROOT import TFile, gDirectory, TCanvas, TH1D, TColor, TGraphErrors, TMath
from operator import truediv

# Open the file 
f=TFile("testmultiscattermomentum_output.root","READ")

# See what is inside the file
print "Now looking inside the file:"
f.ls()

# See what variables are stored inside the tree by looking at just one entry
print "Showing one entry of the tree (note the variables):"
f.ana_tree.Show(0)

# Get the total number of entries in the tree.
entries=f.ana_tree.GetEntries()
print "This is how many total entries are inside the tree:\n", entries

# Making a histogram:
# With TTree.Draw, first argument is the variable we are plotting, second
# argument is cuts we want to make, third argument is draw styles
print "This is the current graph of MCp vs. MCSp."
trueMomVsMcsRecoColz=TCanvas("trueMomVsMcsRecoColz","trueMomVsMcsRecoColz",720,152,682,505)
trueMomVsMcsRecoColz.cd()
f.ana_tree.Draw("true_mom:mcs_reco_mom","mcs_reco_mom>0","COLZ")
input=raw_input("Press enter to continue...")

###-----------------

def getMeanHists(contained_only=False):
    
    # Make lists to fill with mean values, std values
    meanVals, stdVals, sqrtHistEntries, stdErrs = [], [], [], []
    
    # Define the desired energy ranges
    ERanges = [(0.3,0.5),(0.5,0.7),(0.7,0.9),(0.9,1.1),(1.1,1.3),(1.3,1.5),(1.5,1.7),(1.7,1.9),(1.9,2.1)]
    
    hist_dictionary = {}
    for myE in ERanges:
        myName = "h_min%s_max%s" % (myE[0],myE[1])
        if contained_only: myName = "cont_h_min%s_max%s" % (myE[0],myE[1])
        myTitle = "Energies from %0.1f to %0.1f;(True p - Reco p) / True p;Entries" % (myE[0], myE[1])
        if contained_only: myTitle = "Contained Tracks: Energies from %0.1f to %0.1f;(True p - Reco p) / True p;Entries" % (myE[0], myE[1])
        hist_dictionary[ myE ] =TH1D(myName,myTitle,100,-4,4)
    
## Create empty histogram
# hist=TH1D("hist","")
    
# Loop over energy values
    for myE in ERanges:
        myPlot = "(true_mom - mcs_reco_mom) / true_mom >> h_min%0.1f_ma\
x%0.1f"%(myE[0],myE[1])
        if contained_only: myPlot = "(true_mom - mcs_reco_mom) / true_mom >> cont_h_min%0.1f_max%0.1f"%(myE[0],myE[1])
        myCut = "mcs_reco_mom > 0 && true_mom > %f && true_mom < %f" % (myE[0], myE[1])
        if contained_only: myCut += ' && mu_contained == 1'
        f.ana_tree.Draw(myPlot,myCut)
        
        meanVals.append(hist_dictionary[myE].GetMean())
#        print "Meanval2:",meanVals
        stdVals.append(hist_dictionary[myE].GetRMS())
        print "Entries:",hist_dictionary[myE].GetEntries()
        sqrtHistEntries.append(TMath.sqrt(hist_dictionary[myE].GetEntries()))
        stdErrs.append(hist_dictionary[myE].GetRMSError())

#    if hist_dictionary[myE].GetEntries()!=0:
    xpoints = []
    ypoints = []
    filteredSqrtHistEntries = []
    filteredStdVals = []
    filteredStdErrs = []
    for myE in ERanges: 
        if hist_dictionary[myE].GetEntries() != 0:
            xpoints.append(myE[0]+(myE[1]-myE[0])/2.)
            ypoints.append(hist_dictionary[myE].GetMean())
            filteredSqrtHistEntries.append(TMath.sqrt(hist_dictionary[myE].GetEntries()))
            filteredStdVals.append(hist_dictionary[myE].GetRMS())
            filteredStdErrs.append(hist_dictionary[myE].GetRMSError())
    print "xpoints",xpoints
    print "xpoints len",len(xpoints)
#    for y in meanVals: 
#        if hist_dictionary[myE].GetEntries() != 0:
#            ypoints.append(y)
    print "ypoints:",ypoints
    print "ypoints len",len(ypoints)
#    xerrs = [0] * len(xpoints) # Makes a list of 0's
#    xerrs = [0.1] * len(xpoints)
    xerrs = [(dummyVar[1]-dummyVar[0])/2. for dummyVar in ERanges]
    print "xerrs:",xerrs
    print "len xerrs:",len(xerrs)
    #    yerrs = [x / TMath.sqrt(entries) for x in stdVals]
    print "filtered Sqrt entries:", filteredSqrtHistEntries
    print "len filtered Sqrt entries:", len(filteredSqrtHistEntries)
    print "filtered std vals",filteredStdVals
    print "len filtered std vals",len(filteredStdVals)
#    for i in sqrtHistEntries:
#        if hist_dictionary[myE].GetEntries() != 0:
#            filteredSqrtHistEntries.append(i)
    yerrs = map(truediv, filteredStdVals, filteredSqrtHistEntries)
#    stdYErrs = []
#    for y in filteredStdVals:        
    meanGraph = TGraphErrors()
    stdGraph = TGraphErrors()
    meanGraphTitle = "Mean vs. Energy;True Muon Energy (GeV);Mean of Fractional Momentum Resolution"
    if contained_only: meanGraphTitle = "Contained Tracks: Mean vs. Energy;True Muon Energy (GeV);Mean"
    stdGraphTitle = "Standard Deviation vs. Energy;True Muon Energy (GeV);Standard Deviation of Fractional Momentum Resolution"
    if contained_only: stdGraphTitle = "Contained Tracks: Standard Deviation vs. Energy;True Muon Energy (GeV);Standard Deviation of Fractional Momentum Resolution"
    meanGraph.SetTitle(meanGraphTitle)
    stdGraph.SetTitle(stdGraphTitle)
    for i in xrange(len(xpoints)):
        meanGraph.SetPoint(i,xpoints[i],ypoints[i])
        meanGraph.SetPointError(i,xerrs[i],yerrs[i])
    meanGraph.Draw("ALP")
    for i in xrange(len(xpoints)):
        stdGraph.SetPoint(i,xpoints[i],filteredStdVals[i])
        stdGraph.SetPointError(i,xerrs[i],filteredStdErrs[i])
    stdGraph.Draw("ALP")

    return meanGraph, hist_dictionary, stdGraph
    
# hist_dictionary[ (0.3, 0.5) ].Draw()
    
# print 'histogram entries are %d' % (hist_dictionary[(0.3,0.5)]).GetEntries()

meanGraph, myDict, stdGraph = getMeanHists()
meanGraphCont, myDictCont, stdGraphCont = getMeanHists(contained_only=True)

print "Now writing the graph..."
    
fout = TFile("myfilename.root","RECREATE")
fout.cd()

for myE, myhist in myDict.iteritems():
    myhist.Write()
# hist_dictionary[ (0.3, 0.5) ].Write()
meanGraph.Write()
stdGraph.Write()

for myE, myhist in myDictCont.iteritems():
    myhist.Write()
meanGraphCont.Write()
stdGraphCont.Write()

fout.Close()

#fout = TFile("myfilename.root","RECREATE")
#fout.cd()
#for myE, myhist in mydict.iteritems():
#    myhist.Write()
#mygraph.Write()
#fout.Close()

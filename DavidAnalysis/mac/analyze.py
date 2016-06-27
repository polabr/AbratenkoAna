import ROOT
from ROOT import TFile, gDirectory, TCanvas, TH1D, TColor
# from ROOT import TFile, gDirectory
# You probably also want to import TH1D,
# unless you're not making any histograms.
# from ROOT import TH1D

# Open the file. 
f=TFile("testmultiscattermomentum_output.root")
# See what is inside the file.
print "Now looking inside the file:"
f.ls()

# Explicitly get the TTree we want to work with. 
# tree=gDirectory.Get("ana_tree")
# entries=tree.GetEntries()

#entry=f.ana_tree.GetEntry(0)
#print entry

# See what variables are stored inside the tree by looking at just one entry. 
print "Showing one entry of the tree (note the variables):"
f.ana_tree.Show(0)

# Get the total number of entries in the tree.
entries=f.ana_tree.GetEntries()
print "This is how many total  entries are inside the tree:\n",entries

# Making a histogram. 
# With TTree.Draw, first argument is the variable we are plotting, second
# argument is cuts we want to make, third argument is draw styles. 
# canvy=TCanvas("canvy","canvy",720,152,682,505)
# canvy.cd()
# tree.Draw("mcs_reco_mom:true_mom","","COLZ")
# f.ana_tree.Draw("true_mom:mcs_reco_mom","mcs_reco_mom<0.1 && mcs_reco_mom>0","COLZ")
# input=raw_input("Press enter to continue...")
print "This is the current graph of MCp vs. MCSp."
f.ana_tree.Draw("true_mom:mcs_reco_mom","","COLZ")
input=raw_input("Press enter to continue...")

# mychain = gDirectory.Get( 'tree1' )
# entries = mychain.GetEntriesFast()

### The Set-up code goes here.

# Setting up the histogram (range 100-150)
histogram1=TH1D("h1","Energies from 0.3-0.5 GeV",100,-4,4)
histogram1.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram1.GetYaxis().SetTitle("Entries")
histogram2=TH1D("h2","Energies from 0.5-0.7 GeV",100,-4,4)
histogram2.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram2.GetYaxis().SetTitle("Entries")
histogram3=TH1D("h3","Energies from 0.7-0.9 GeV",100,-4,4)
histogram3.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram3.GetYaxis().SetTitle("Entries")
histogram4=TH1D("h4","Energies from 0.9-1.1 GeV",100,-4,4)
histogram4.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram4.GetYaxis().SetTitle("Entries")
histogram5=TH1D("h5","Energies from 1.1-1.3 GeV",100,-4,4)
histogram5.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram5.GetYaxis().SetTitle("Entries")
histogram6=TH1D("h6","Energies from 1.3-1.5 GeV",100,-4,4)
histogram6.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram6.GetYaxis().SetTitle("Entries")
histogram7=TH1D("h7","Energies from 1.5-1.7 GeV",100,-4,4)
histogram7.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram7.GetYaxis().SetTitle("Entries")
histogram8=TH1D("h8","Energies from 1.7-1.9 GeV",100,-4,4)
histogram8.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram8.GetYaxis().SetTitle("Entries")
histogram9=TH1D("h9","Energies from 1.9-2.1 GeV",100,-4,4)
histogram9.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram9.GetYaxis().SetTitle("Entries")

# Setting up histograms of range 175-225
histogram11=TH1D("h11","Energies from 0.3-0.5 GeV",100,-4,4)
histogram11.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram11.GetYaxis().SetTitle("Entries")
histogram12=TH1D("h12","Energies from 0.5-0.7 GeV",100,-4,4)
histogram12.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram12.GetYaxis().SetTitle("Entries")
histogram13=TH1D("h13","Energies from 0.7-0.9 GeV",100,-4,4)
histogram13.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram13.GetYaxis().SetTitle("Entries")
histogram14=TH1D("h14","Energies from 0.9-1.1 GeV",100,-4,4)
histogram14.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram14.GetYaxis().SetTitle("Entries")
histogram15=TH1D("h15","Energies from 1.1-1.3 GeV",100,-4,4)
histogram15.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram15.GetYaxis().SetTitle("Entries")
histogram16=TH1D("h16","Energies from 1.3-1.5 GeV",100,-4,4)
histogram16.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram16.GetYaxis().SetTitle("Entries")
histogram17=TH1D("h17","Energies from 1.5-1.7 GeV",100,-4,4)
histogram17.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram17.GetYaxis().SetTitle("Entries")
histogram18=TH1D("h18","Energies from 1.7-1.9 GeV",100,-4,4)
histogram18.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram18.GetYaxis().SetTitle("Entries")
histogram19=TH1D("h19","Energies from 1.9-2.1 GeV",100,-4,4)
histogram19.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram19.GetYaxis().SetTitle("Entries")

# Setting up histograms of range 250-300
histogram21=TH1D("h21","Energies from 0.3-0.5 GeV",100,-4,4)
histogram21.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram21.GetYaxis().SetTitle("Entries")
histogram22=TH1D("h22","Energies from 0.5-0.7 GeV",100,-4,4)
histogram22.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram22.GetYaxis().SetTitle("Entries")
histogram23=TH1D("h23","Energies from 0.7-0.9 GeV",100,-4,4)
histogram23.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram23.GetYaxis().SetTitle("Entries")
histogram24=TH1D("h24","Energies from 0.9-1.1 GeV",100,-4,4)
histogram24.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram24.GetYaxis().SetTitle("Entries")
histogram25=TH1D("h25","Energies from 1.1-1.3 GeV",100,-4,4)
histogram25.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram25.GetYaxis().SetTitle("Entries")
histogram26=TH1D("h26","Energies from 1.3-1.5 GeV",100,-4,4)
histogram26.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram26.GetYaxis().SetTitle("Entries")
histogram27=TH1D("h27","Energies from 1.5-1.7 GeV",100,-4,4)
histogram27.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram27.GetYaxis().SetTitle("Entries")
histogram28=TH1D("h28","Energies from 1.7-1.9 GeV",100,-4,4)
histogram28.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram28.GetYaxis().SetTitle("Entries")
histogram29=TH1D("h29","Energies from 1.9-2.1 GeV",100,-4,4)
histogram29.GetXaxis().SetTitle("(True p - Reco p)/True p")
histogram29.GetYaxis().SetTitle("Entries")

def delta(i):
    return (f.ana_tree.true_mom-f.ana_tree.mcs_reco_mom)/f.ana_tree.true_mom

print "entries:",entries
count=0
print "This is how count starts off:",count

# Energies in GeV
start=0.3
step=0.2
stop=2.1

for i in range(0,entries,1):
    count+=1
    f.ana_tree.GetEntry(i)
    if f.ana_tree.mcs_reco_mom!=-1: # and f.ana_tree.mu_contained==1:
        #while f.ana_tree.true_mom>start and f.ana_tree.true_mom<start+step:
        #    histogram1.Fill(delta(i))
        if f.ana_tree.reco_len>100 and f.ana_tree.reco_len<150:
            #print i,"",delta(i)
            if f.ana_tree.true_mom > start and f.ana_tree.true_mom < start+step:
                histogram1.Fill(delta(i))
            if f.ana_tree.true_mom > start+step and f.ana_tree.true_mom < start+2*step:
                histogram2.Fill(delta(i))
            if f.ana_tree.true_mom > start+2*step and f.ana_tree.true_mom < start+3*step:
                histogram3.Fill(delta(i))
            if f.ana_tree.true_mom > start+3*step and f.ana_tree.true_mom < start+4*step:
                histogram4.Fill(delta(i))
            if f.ana_tree.true_mom > start+4*step and f.ana_tree.true_mom < start+5*step:
                histogram5.Fill(delta(i))
            if f.ana_tree.true_mom > start+5*step and f.ana_tree.true_mom < start+6*step:
                histogram6.Fill(delta(i))
            if f.ana_tree.true_mom > start+6*step and f.ana_tree.true_mom < start+7*step:
                histogram7.Fill(delta(i))
            if f.ana_tree.true_mom > start+7*step and f.ana_tree.true_mom < start+8*step:
                histogram8.Fill(delta(i))
            if f.ana_tree.true_mom > start+8*step and f.ana_tree.true_mom < start+9*step:
                histogram9.Fill(delta(i))
        if f.ana_tree.reco_len>175 and f.ana_tree.reco_len<225:
            if f.ana_tree.true_mom > start and f.ana_tree.true_mom < start+step:
                histogram11.Fill(delta(i))
            if f.ana_tree.true_mom > start+step and f.ana_tree.true_mom < start+2*step:
                histogram12.Fill(delta(i))
            if f.ana_tree.true_mom > start+2*step and f.ana_tree.true_mom < start+3*step:
                histogram13.Fill(delta(i))
            if f.ana_tree.true_mom > start+3*step and f.ana_tree.true_mom < start+4*step:
                histogram14.Fill(delta(i))
            if f.ana_tree.true_mom > start+4*step and f.ana_tree.true_mom < start+5*step:
                histogram15.Fill(delta(i))
            if f.ana_tree.true_mom > start+5*step and f.ana_tree.true_mom < start+6*step:
                histogram16.Fill(delta(i))
            if f.ana_tree.true_mom > start+6*step and f.ana_tree.true_mom < start+7*step:
                histogram17.Fill(delta(i))
            if f.ana_tree.true_mom > start+7*step and f.ana_tree.true_mom < start+8*step:
                histogram18.Fill(delta(i))
            if f.ana_tree.true_mom > start+8*step and f.ana_tree.true_mom < start+9*step:
                histogram19.Fill(delta(i))
        if f.ana_tree.reco_len>250 and f.ana_tree.reco_len<300:
            if f.ana_tree.true_mom > start and f.ana_tree.true_mom < start+step:
                histogram21.Fill(delta(i))
            if f.ana_tree.true_mom > start+step and f.ana_tree.true_mom < start+2*step:
                histogram22.Fill(delta(i))
            if f.ana_tree.true_mom > start+2*step and f.ana_tree.true_mom < start+3*step:
                histogram23.Fill(delta(i))
            if f.ana_tree.true_mom > start+3*step and f.ana_tree.true_mom < start+4*step:
                histogram24.Fill(delta(i))
            if f.ana_tree.true_mom > start+4*step and f.ana_tree.true_mom < start+5*step:
                histogram25.Fill(delta(i))
            if f.ana_tree.true_mom > start+5*step and f.ana_tree.true_mom < start+6*step:
                histogram26.Fill(delta(i))
            if f.ana_tree.true_mom > start+6*step and f.ana_tree.true_mom < start+7*step:
                histogram27.Fill(delta(i))
            if f.ana_tree.true_mom > start+7*step and f.ana_tree.true_mom < start+8*step:
                histogram28.Fill(delta(i))
            if f.ana_tree.true_mom > start+8*step and f.ana_tree.true_mom < start+9*step:
                histogram29.Fill(delta(i))
        
# After the loop above.
# print "This is count after the loop:",count
canvy1=TCanvas("canvy1","canvy1",720,152,682,505)
canvy1.cd()
histogram1.Draw()
mean1=histogram1.GetMean()
rms1=histogram1.GetRMS()
print "This is the mean:",mean1
print "This is the RMS:",rms1
input=raw_input("Press enter to continue...")
canvy2=TCanvas("canvy2","canvy2",720,152,682,505)
canvy2.cd()
histogram2.Draw()
input=raw_input("Press enter to continue...")
canvy3=TCanvas("canvy3","canvy3",720,152,682,505)
canvy3.cd()
histogram3.Draw()
input=raw_input("Press enter to continue...")
canvy4=TCanvas("canvy4","canvy4",720,152,682,505)
canvy4.cd()
histogram4.Draw()
input=raw_input("Press enter to continue...")
canvy5=TCanvas("canvy5","canvy5",720,152,682,505)
canvy5.cd()
histogram5.Draw()
input=raw_input("Press enter to continue...")
canvy6=TCanvas("canvy6","canvy6",720,152,682,505)
canvy6.cd()
histogram6.Draw()
input=raw_input("Press enter to continue...")
canvy7=TCanvas("canvy7","canvy7",720,152,682,505)
canvy7.cd()
histogram7.Draw()
input=raw_input("Press enter to continue...")
canvy8=TCanvas("canvy8","canvy8",720,152,682,505)
canvy8.cd()
histogram8.Draw()
input=raw_input("Press enter to continue...")
canvy9=TCanvas("canvy9","canvy9",720,152,682,505)
canvy9.cd()
histogram9.Draw()
input=raw_input("Press enter to continue...")

meanHist=TH1D("meanHist","Mean Histogram with Standard Deviation Error Bars",9,0.3,2.1)
meanHist.GetXaxis().SetTitle("Energy (GeV)")
meanHist.GetYaxis().SetTitle("Mean")

meanHist.SetBinContent(1,mean1)
meanHist.SetBinContent(2,histogram2.GetMean())
meanHist.SetBinContent(3,histogram3.GetMean())
meanHist.SetBinContent(4,histogram4.GetMean())
meanHist.SetBinContent(5,histogram5.GetMean())
meanHist.SetBinContent(6,histogram6.GetMean())
meanHist.SetBinContent(7,histogram7.GetMean())
meanHist.SetBinContent(8,histogram8.GetMean())
meanHist.SetBinContent(9,histogram9.GetMean())

meanHist.SetBinError(1,histogram1.GetRMS())
meanHist.SetBinError(2,histogram2.GetRMS())
meanHist.SetBinError(3,histogram3.GetRMS())
meanHist.SetBinError(4,histogram4.GetRMS())
meanHist.SetBinError(5,histogram5.GetRMS())
meanHist.SetBinError(6,histogram6.GetRMS())
meanHist.SetBinError(7,histogram7.GetRMS())
meanHist.SetBinError(8,histogram8.GetRMS())
meanHist.SetBinError(9,histogram9.GetRMS())

# Lengths from 175-225
meanHist2=TH1D("meanHist2","Mean Histogram with Standard Deviation Error Bars",9,0.3,2.1)
meanHist2.GetXaxis().SetTitle("Energy (GeV)")
meanHist2.GetYaxis().SetTitle("Mean")

meanHist2.SetBinContent(1,histogram11.GetMean())
meanHist2.SetBinContent(2,histogram12.GetMean())
meanHist2.SetBinContent(3,histogram13.GetMean())
meanHist2.SetBinContent(4,histogram14.GetMean())
meanHist2.SetBinContent(5,histogram15.GetMean())
meanHist2.SetBinContent(6,histogram16.GetMean())
meanHist2.SetBinContent(7,histogram17.GetMean())
meanHist2.SetBinContent(8,histogram18.GetMean())
meanHist2.SetBinContent(9,histogram19.GetMean())

meanHist2.SetBinError(1,histogram11.GetRMS())
meanHist2.SetBinError(2,histogram12.GetRMS())
meanHist2.SetBinError(3,histogram13.GetRMS())
meanHist2.SetBinError(4,histogram14.GetRMS())
meanHist2.SetBinError(5,histogram15.GetRMS())
meanHist2.SetBinError(6,histogram16.GetRMS())
meanHist2.SetBinError(7,histogram17.GetRMS())
meanHist2.SetBinError(8,histogram18.GetRMS())
meanHist2.SetBinError(9,histogram19.GetRMS())

# Lengths from 250-300
meanHist3=TH1D("meanHist3","Mean Histogram with Standard Deviation Error Bars",9,0.3,2.1)
meanHist3.GetXaxis().SetTitle("Energy (GeV)")
meanHist3.GetYaxis().SetTitle("Mean")

meanHist3.SetBinContent(1,histogram21.GetMean())
meanHist3.SetBinContent(2,histogram22.GetMean())
meanHist3.SetBinContent(3,histogram23.GetMean())
meanHist3.SetBinContent(4,histogram24.GetMean())
meanHist3.SetBinContent(5,histogram25.GetMean())
meanHist3.SetBinContent(6,histogram26.GetMean())
meanHist3.SetBinContent(7,histogram27.GetMean())
meanHist3.SetBinContent(8,histogram28.GetMean())
meanHist3.SetBinContent(9,histogram29.GetMean())

meanHist3.SetBinError(1,histogram21.GetRMS())
meanHist3.SetBinError(2,histogram22.GetRMS())
meanHist3.SetBinError(3,histogram23.GetRMS())
meanHist3.SetBinError(4,histogram24.GetRMS())
meanHist3.SetBinError(5,histogram25.GetRMS())
meanHist3.SetBinError(6,histogram26.GetRMS())
meanHist3.SetBinError(7,histogram27.GetRMS())
meanHist3.SetBinError(8,histogram28.GetRMS())
meanHist3.SetBinError(9,histogram29.GetRMS())

# Set color of line
meanHist2.SetLineColor(ROOT.kRed)
meanHist3.SetLineColor(ROOT.kGreen)

canvy10=TCanvas("canvy10","canvy10",720,152,682,505)
canvy10.cd()
meanHist.Draw("E1")
meanHist2.Draw("same E1")
meanHist3.Draw("same E1")
input=raw_input("Press enter to continue...")

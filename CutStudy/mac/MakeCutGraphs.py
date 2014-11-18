import sys
from ROOT import *

if len(sys.argv) != 2:
    print "Usage: python %s output_from_FidVolCutStudy.root" % sys.argv[0]
    quit()


cutvals = [1,2,3,4] + range(5,120,5) #(0, 10, 20, ... 110)

f = TFile(sys.argv[1],"READ")
t = ana_tree

g_econtain_vtxcut = TGraph()
g_econtain_vtxcut.SetTitle("Avg Energy Containment vs. Fid Vol Cut Value;Fiducial Volume Cut Distance [cm];Shower Energy Containment Fraction")
g_econtain_vtxcut.SetMarkerColor(kRed)
g_eff_vtxcut = TGraph()
g_eff_vtxcut.SetTitle("Efficiency to Select Shower;Fiducial Volume Cut Distance [cm];Efficiency")
g_eff_vtxcut.SetMarkerColor(kBlue)

g_econtain_forward_distcut = TGraph()
g_econtain_forward_distcut.SetTitle("Avg Energy Containment vs. Forward Shower Dist Cut Value;Forward Shower Dist Cut Distance [cm];Shower Energy Containment Fraction")
g_econtain_forward_distcut.SetMarkerColor(kRed)
g_eff_forward_distcut = TGraph()
g_eff_forward_distcut.SetTitle("Efficiency to Select Shower;Forward Shower Dist Cut Distance [cm];Efficiency")
g_eff_forward_distcut.SetMarkerColor(kBlue)


tree_entries = t.GetEntriesFast()

for i in xrange(len(cutvals)):

    econtain_avg_vtxcut = 0.    
    passcut_counter_vtxcut = 0
    econtain_avg_forward_distcut = 0.
    passcut_counter_forward_distcut = 0

    for x in xrange(tree_entries):
        y = t.GetEntry(x)

        if t.DistVtxToWall > cutvals[i]:
            econtain_avg_vtxcut = econtain_avg_vtxcut + t.EContainment
            passcut_counter_vtxcut = passcut_counter_vtxcut + 1

        if t.FwdToWallDist > cutvals[i]:
            econtain_avg_forward_distcut = econtain_avg_forward_distcut + t.EContainment
            passcut_counter_forward_distcut = passcut_counter_forward_distcut + 1


    if passcut_counter_vtxcut > 0:
        econtain_avg_vtxcut = float(econtain_avg_vtxcut) / float(passcut_counter_vtxcut)
    else:
        econtain_avg_vtxcut = 0

    if passcut_counter_forward_distcut > 0:
        econtain_avg_forward_distcut = float(econtain_avg_forward_distcut) / float(passcut_counter_forward_distcut)
    else:
        econtain_avg_forward_distcut = 0

    
    efficiency_vtxcut = float(passcut_counter_vtxcut)/float(tree_entries)
    efficiency_forward_distcut = float(passcut_counter_forward_distcut)/float(tree_entries)

    g_econtain_vtxcut.SetPoint(i,cutvals[i],econtain_avg_vtxcut)
    g_eff_vtxcut.SetPoint(i,cutvals[i],efficiency_vtxcut)
    g_econtain_forward_distcut.SetPoint(i,cutvals[i],econtain_avg_forward_distcut)
    g_eff_forward_distcut.SetPoint(i,cutvals[i],efficiency_forward_distcut)


#    print "avg energy coantianment cut %f is %f" % (cut,econtain_avg)
 


c = TCanvas()
c.cd()
g_eff_vtxcut.GetYaxis().SetRangeUser(0,1.1)
g_eff_vtxcut.Draw("AP*")
g_econtain_vtxcut.Draw("P*")

l = TLegend(.5,.5,.6,.6)
l.AddEntry(g_eff_vtxcut,"Efficiency","P")
l.AddEntry(g_econtain_vtxcut,"Energy Containment","P")
l.Draw()

sys.stdin.readline()

g_eff_forward_distcut.GetYaxis().SetRangeUser(0,1.1)
g_eff_forward_distcut.Draw("AP*")
g_econtain_forward_distcut.Draw("P*")

l = TLegend(.5,.5,.6,.6)
l.AddEntry(g_eff_forward_distcut,"Efficiency","P")
l.AddEntry(g_econtain_forward_distcut,"Energy Containment","P")
l.Draw()

c.Update()
c.Modified()
sys.stdin.readline()

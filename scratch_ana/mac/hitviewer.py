import sys
import ROOT
from ROOT import *
from ROOT import larlite as fmwk


if len(sys.argv) == 1:
    print "\n-------You forgot to include a source file!-------\n"
    print "Usage: python hitviewr.py /input/file/with/gaushits.root"
    quit()

if len(sys.argv) != 2:
    print "Usage: python hitviewr.py /input/file/with/gaushits.root"
    quit()


#Get access to the larlite data manager
mgr = fmwk.storage_manager()
mgr.set_io_mode(mgr.kREAD)

#Add input file
mgr.add_in_filename(sys.argv[1])
mgr.open()

#Make a canvas to draw the hits
canvas=TCanvas("HitViewer","HitViewer",600,500)
gStyle.SetOptStat(0)
canvas.SetGridx(1)
canvas.SetGridy(1)
#canvas.SetLogz()

#Make an instance of the HitViewer
algo = fmwk.HitViewer()

processed_events=0

#Loop over events in the file
#You can figure out how to change this to jump to specific events, etc
while mgr.next_event():

    ev_hit = mgr.get_data(fmwk.data.kHit,'gaushit')

    if not ev_hit:
        print "WTF your input file doesn't have hits in it."
        quit()

    plane_todraw = int(2)

    print "Event ID: %d, Plane: %d, Number of events drawn: %d" % (ev_hit.event_id(),plane_todraw, processed_events)

    algo.GenerateHisto(ev_hit,int(plane_todraw))

    canvas.cd()
    myHisto = algo.GetHitHisto()
    if not myHisto:
        print "Uh oh something went wrong... HitViewer returned a non-existant hit histogram. Quitting!"
        quit()

    myHisto.Draw("COLZ")
    canvas.Update()
    
    processed_events += 1

    print "Hit enter to go to the next event!\n"
    sys.stdin.readline()
        

        



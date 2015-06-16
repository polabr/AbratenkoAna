import sys, os
from ROOT import TFile

if len(sys.argv) != 2:
    print 'Usage: python %s batchname'%sys.argv[0]
    quit()

max_files_to_process = 10000

batchname = sys.argv[1]
batchnamepath = '/Users/davidkaleko/Data/larlite/mcc6/larlite_mcinfo/'+batchname+'/'

#files is a list of (good) files (full path) to process
files = []

#build the list
#also save a txt file listing the input files that were used in this run
txtfile = open('%s_mcinfo_inputfiles.txt'%batchname,'w')
for folder in os.listdir(batchnamepath):
    myfile = folder + '/larlite_mcinfo.root'
    #check if a file is a zombie
    f = TFile(batchnamepath+myfile,'READ')
    if not f.IsZombie():
        files.append(batchnamepath+myfile)
        max_files_to_process -= 1
        txtfile.write(batchname+'/'+myfile+'\n')

    f.Close()
    if not max_files_to_process:
        break

#"cull" => "reduce the population of (a wild animal) by selective slaughter"

#This code is to take a list of input root files and return a subset of
#that list which corresponds to good root files.
#For example, if a file is a zombie, is it not returned
#Also, if a reco2d file is a zombie, and its corresponding mcinfo file
#is also in the list, that mcinfo file is not returned, since its presence
#would lead to event alignment errors.
#I check if a file is "corresponding" by getting entry 0 of
#"larlite_id_tree" within the file to look at entry id (run, subrun, eventid)
#and mapping it between the two files


from ROOT import TFile

#Use this function from python run script with "trim_filelist(sys.argv)"
def trim_filelist(*filelist):
    
    #Remove the first element, which is the python script itself
    #mylist is a list of all of the input files
    mylist = [ myfile for myfile in filelist[0] if myfile[-3:] != '.py' ]

    #Dictionary of run/subrun/evt of entry 0 => 
    #list of corresponding filename(s)
    filedict = {}

    #Loop over the input files, build the dictionary
    for myfile in mylist:
        #Make sure the file isn't a zombie
        f = TFile(myfile,'READ')
        if f.IsZombie(): continue
        
        #Make sure the file has larlite_id_tree in it
        if not f.GetListOfKeys().FindObject('larlite_id_tree'): continue

        #Get info from the 0th entry of larlite_id_tree
        dummy = f.larlite_id_tree.GetEntry(0)
        irun = f.larlite_id_tree._run_id
        isub = f.larlite_id_tree._subrun_id
        ievt = f.larlite_id_tree._event_id
        print "file %s has (%d,%d,%d)"%(myfile,irun,isub,ievt)
        
        #If this file isn't in the dictionary yet, add it
        if (irun,isub,ievt) not in filedict.keys():
            filedict[(irun,isub,ievt)] = [myfile]
        #If it is, append it
        else: filedict[(irun,isub,ievt)].append(myfile)

        #Close the file
        f.Close()


    #Now, loop over the dictionary and find the maximum number of files
    #associated with one (run,subrun,evt) combo.
    #Keep only these files. That way, if you hand in 5 reco2d files and
    #5 mcinfo files, and 4 of the reco2d files are valid, you will only
    #return those 4 files and the 4 associated mcinfo files.
    n_files_max = max([len(i) for i in filedict.values()])

    #Build a list of all input files to keep, and return it
    return_list = []
    for values in filedict.values():
        if len(values) < n_files_max: continue
        for filename in values:
            return_list.append(filename)

    return return_list

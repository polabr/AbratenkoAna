from ROOT import *
f = TFile("cosmicValidation_ana_out.root","READ")
t = f.cos_tree
entries = t.GetEntries()
n_true_primary = 0
n_correctly_recod_primary = 0
n_falsely_recod_primary = 0
n_true_secondary = 0
n_correctly_recod_secondary = 0
n_falsely_recod_secondary = 0
n_true_orphan = 0
n_correctly_recod_orphan = 0
n_falsely_recod_orphan = 0

for x in xrange(entries):

	blah = t.GetEntry(x)
	#Ignore all particles that are tracks with length less than 1cm
	# if t._is_track and t._trk_length < 1: continue
	#Ignore all particles with energy deposited < 20 MeV
	# if t._energy < 20.: continue
	# if not t._start_contained_in_TPC: continue
	# if t._is_track and (t._start_contained_in_TPC or t._trackend_contained_in_TPC): continue

	if t._true_primary:
		n_true_primary += 1
		if t._reco_primary:
			n_correctly_recod_primary += 1
	else:
		if t._reco_primary:
			n_falsely_recod_primary += 1

	if t._true_secondary:
		n_true_secondary += 1
		if t._reco_secondary:
			n_correctly_recod_secondary += 1
	else:
		if t._reco_secondary:
			n_falsely_recod_secondary += 1

	if t._true_orphan:# and not t._reco_primary:
		n_true_orphan += 1
		if t._reco_orphan:
			n_correctly_recod_orphan += 1
	else:
		if t._reco_orphan:
			n_falsely_recod_orphan += 1


print
print "n_true_primary particles is ",n_true_primary
print "n_correctly_recod_primary particles is ",n_correctly_recod_primary
print "n_falsely_recod_primary is ",n_falsely_recod_primary
print "primary efficiency is %d/%d = %f"%(n_correctly_recod_primary,n_true_primary,float(n_correctly_recod_primary)/float(n_true_primary))
print 
print "n_true_secondary particles is ",n_true_secondary
print "n_correctly_recod_secondary particles is ",n_correctly_recod_secondary
print "n_falsely_recod_secondary is ",n_falsely_recod_secondary
print "secondary efficiency is %d/%d = %f"%(n_correctly_recod_secondary,n_true_secondary,float(n_correctly_recod_secondary)/float(n_true_secondary))
print
print "n_true_orphan particles is ",n_true_orphan
print "n_correctly_recod_orphan particles is ",n_correctly_recod_orphan
print "n_falsely_recod_orphan is ",n_falsely_recod_orphan
if n_true_orphan:
	print "orphan efficiency is %d/%d = %f"%(n_correctly_recod_orphan,n_true_orphan,float(n_correctly_recod_orphan)/float(n_true_orphan))
else:
	print "orphan efficiency is infinite because there are no true orphans in the denominator."
print

f.Close()
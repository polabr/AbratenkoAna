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

for x in xrange(entries):

	blah = t.GetEntry(x)

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

print
print "n_true_primary particles is ",n_true_primary
print "n_correctly_recod_primary particles is ",n_correctly_recod_primary
print "n_falsely_recod_primary is ",n_falsely_recod_primary
print "primary efficiency is %d/%d = %f"%(n_correctly_recod_primary,n_true_primary,float(n_correctly_recod_primary)/float(n_true_primary))
print 
print "n_true_secondary particles is ",n_true_secondary
print "n_correctly_recod_secondary particles is ",n_correctly_recod_secondary
print "n_falsely_recod_secondary is ",n_falsely_recod_secondary
print "secondary efficiency is %d/(%d-%d) = %f"%(n_correctly_recod_secondary,n_true_secondary,n_falsely_recod_primary,float(n_correctly_recod_secondary)/float(n_true_secondary-n_falsely_recod_primary))
print


f.Close()
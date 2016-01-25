import os

#cfgs = [ '90', '190', '210', '410' ]
cfgs = [ '70', '390' ]
for cfg in cfgs:
#    print "CFG: %s. Sample = Neutrino"%cfg
#    os.system("python run_TrigEffStudy.py smallwindow_cfg_%s.cfg /Users/davidkaleko/Data/larlite/mcc7/pre_mcc7_prodgenie_bnb_nu_uboone/larlit*all* output/nu_reco_smallwindow_thres%s.root"%(cfg,cfg))
    print "CFG: %s. Sample = Electron"%cfg
    os.system("python run_TrigEffStudy.py smallwindow_cfg_%s.cfg /Users/davidkaleko/Data/larlite/mcc7/trigger_study_eminus_all.root output/e_reco_smallwindow_thres%s.root"%(cfg,cfg))
    print "CFG: %s. Sample = Proton"%cfg
    os.system("python run_TrigEffStudy.py smallwindow_cfg_%s.cfg /Users/davidkaleko/Data/larlite/mcc7/trigger_study_proton_all.root output/p_reco_smallwindow_thres%s.root"%(cfg,cfg))

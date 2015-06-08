%matplotlib inline
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from root_numpy import root2array, root2rec, tree2rec, array2root
from ROOT import TFile
import matplotlib.mlab as mlab
from scipy.stats import norm

fname = 'NuEnergyReco_ana_out.root'
df = pd.DataFrame(root2array(fname,'tree'))

def gen_asym_data_for_bin(minE,maxE,n_prot_max=-1):
    #Query the entries in dataframe in this specific neutrino energy bin
    if n_prot_max == -1:
        newdf = df.query('true_nu_E>%f and true_nu_E<%f'%(minE,maxE))
    else:
        newdf = df.query('true_nu_E>%f and true_nu_E<%f and n_protons<=%d'%(minE,maxE,n_prot_max))
   
    data = ((newdf['lep_E'] + newdf['tot_prot_KE'] - newdf['true_nu_E'])/newdf['true_nu_E']).as_matrix()
    #Throw out data that is not in the range -1 to 1
    data = np.array([i for i in data if i <= 1 and i >= -1])
    return data

#Dict: key is n_prot_max where -1 means any number
resolutions = { 1:[], -1:[] }

#Loop over energy bins and compute resolution in each bin. Make a plot.
E_bin_edges = np.array(range(0,3000,200))
fit_range = [-.5,.5]
for n_p in resolutions.keys():
    for x in xrange(len(E_bin_edges)-1):
        data = gen_asym_data_for_bin(E_bin_edges[x],E_bin_edges[x+1],n_prot_max=n_p)
        mu,sigma = norm.fit([i for i in data if i >= fit_range[0] and i <= fit_range[1]])
        resolutions[n_p].append(sigma)

colors = ['c','m','y', 'k','g','b','r']
xvals = E_bin_edges[:-1] + (E_bin_edges[1]-E_bin_edges[0])/2.
plt.figure(figsize=[10,6])
plt.grid(True)

for n_p in resolutions.keys():
    mylabel = '%s protons'%('any' if n_p < 0 else '<= %s'%n_p)
    plt.plot(xvals,resolutions[n_p],'%s'%colors[-1],label=mylabel)
    del colors[-1]

plt.title('Lep + Protons Energy Resolution in 200 MEV Bins of True Nu Energy')
plt.legend()
plt.xlabel('True Neutrino Energy in 200 MEV Bins')
plt.ylabel('Energy Resolution (Lep + Protons)')
plt.ylim([0,.2])

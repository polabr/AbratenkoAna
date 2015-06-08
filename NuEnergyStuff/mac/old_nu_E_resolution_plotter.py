import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from root_numpy import root2array, root2rec, tree2rec, array2root
from ROOT import TFile
import matplotlib.mlab as mlab
from scipy.stats import norm

fname = 'NuEnergyReco_ana_out.root'
df = pd.DataFrame(root2array(fname,'tree')).query('nuance_intxn==1001')

def gen_asym_data_for_bin(minE,maxE,n_prot=-1,n_neut=-1,ccqeflag='CCQE',flag='kaleko'):
    #Query the entries in dataframe in this specific neutrino energy bin
    if n_prot == -1 and n_neut == -1:
        newdf = df.query('true_nu_E>%f and true_nu_E<%f'%(minE,maxE))
    elif n_prot >= 0 and n_neut == -1:
        newdf = df.query('true_nu_E>%f and true_nu_E<%f and n_protons==%d'%(minE,maxE,n_prot))
    elif n_prot == -1 and n_neut >= 0:
        newdf = df.query('true_nu_E>%f and true_nu_E<%f and n_neutrons==%d'%(minE,maxE,n_neut))
    elif flag == "1pnonzeroneutrons":
        newdf = df.query('true_nu_E>%f and true_nu_E<%f and n_protons==1 and n_neutrons>0'%(minE,maxE))
    else:
        newdf = df.query('true_nu_E>%f and true_nu_E<%f and n_protons==%d and n_neutrons==%d'%(minE,maxE,n_prot,n_neut))
        
    #Compute the asymmetry parameter, load to numpy array
    if ccqeflag == 'CCQE': 
        data = ((newdf['E_ccqe'] - newdf['true_nu_E'])/newdf['true_nu_E']).as_matrix()
    else:
        data = ((newdf['lep_E'] + newdf['tot_prot_E'] - newdf['true_nu_E'])/newdf['true_nu_E']).as_matrix()
    #Throw out data that is not in the range -1 to 1
    data = np.array([i for i in data if i <= 1 and i >= -1])
    return data

#Loop over energy bins and compute resolution in each bin. Make a plot.
#Dict: key is (n prot, n neut), -1 means any number, -2 means any non-zero number of neutrons
resolutions = {(1,0):[], (1,-2):[], (-1,-1):[] }
E_bin_edges = np.array(range(0,3000,200))
fit_range = [-.5,.5]
for n_p, n_n in resolutions.keys():
    for x in xrange(len(E_bin_edges)-1):
        if n_n != -2:
            data = gen_asym_data_for_bin(E_bin_edges[x],E_bin_edges[x+1],n_prot=n_p,n_neut=n_n,flag='kaleko',ccqeflag='agwefawf')
        else:
            data = gen_asym_data_for_bin(E_bin_edges[x],E_bin_edges[x+1],n_prot=n_p,n_neut=n_n,flag='1pnonzeroneutrons',ccqeflag='awefawef')

        mu,sigma = norm.fit([i for i in data if i >= fit_range[0] and i <= fit_range[1]])
        resolutions[(n_p,n_n)].append(sigma)

colors = ['b','g','r','c','m','y', 'k']
xvals = E_bin_edges[:-1] + (E_bin_edges[1]-E_bin_edges[0])/2.
plt.figure(figsize=[10,6])
plt.grid(True)

for n_p, n_n in resolutions.keys():
    mylabel = '%s p, %s n'%('any' if n_p < 0 else n_p, 'any' if n_n < 0 else n_n)
    if n_n == -2:
        mylabel = '%s p, %s n'%('any' if n_p < 0 else n_p, '>0')
    plt.plot(xvals,resolutions[(n_p,n_n)],'%s'%colors[-1],label=mylabel)
    del colors[-1]

plt.title('Lep + Protons Energy Resolution in 200 MEV Bins of True Nu Energy')
plt.legend()
plt.xlabel('True Neutrino Energy in 200 MEV Bins')
plt.ylabel('Energy Resolution (Lep + Protons)')
plt.ylim([0,.2])
plt.show()





## Extra function showing how to overlay a gaussian over a histogram in numpy
def draw_gauss(data,mu,sigma):
    #multiply normalized fit by bin_width*total_length_of_data
    plt.figure(figsize=[10,6])
    n,bins,patches = plt.hist(data,50,range=[-1,1])
    y = mlab.normpdf(bins,mu,sigma)*(bins[1]-bins[0])*(data.shape[0])
    plt.plot(bins,y,'r--',linewidth=2)
    plt.title('(E_ccqe - E_nu)/E_nu for Itxn == 1001, E_nu in a specific energy bin')
    plt.grid()

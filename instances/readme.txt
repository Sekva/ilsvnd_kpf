Instances used in:
Giovanni Capobianco, Ciriaco D’Ambrosio, Luigi Pavone, Andrea Raiconi, Gaetano Vitale, Fabio Sebastiano 
A hybrid metaheuristic for the Knapsack Problem with Forfeits
Soft Computing, https://doi.org/10.1007/s00500-021-06331-x
==================================================

Instances are organized in folders depending on their type (O - LK - MF) and subfolders depending on the number of items (500 - 700 - 800 - 1000).

For each instance file, the structure is the following:

Line 1 contains the following values:
nI nP kS
where:
nI = number of items
nP = number of forfeit pairs
kS = knapsack size

Line 2 contains item profits, in order of index 0...nI-1

Line 3 contains item weights, in order of index 0...nI-1

The remaining 2*nP lines contain the following values:

nA_0 fC_0 nI_0
id_0_0 id_0_1
(...)  
nA_i fC_i nI_i
id_i_0 id_i_1
(...)  
nA_(nP-1) fC_(nP-1) nI_(nP-1)
id_(nP-1)_0 id_(nP-1)_1

where:
nA_i = number of items allowed in the solution without having to pay forfeit cost for forfeit pair i (always 1)
fC_i = forfeit cost associated to forfeit pair i
nI_i = cardinality of the list of items contained in the subsequent line (forfeit pair i, always 2)
id_i_0 = index of first item for forfeit pair i (belonging to {0,...,nI-1})
id_i_1 = index of second item for forfeit pair i (belonging to {0,...,nI-1})







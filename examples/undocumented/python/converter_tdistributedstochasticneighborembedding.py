#!/usr/bin/env python
data = '../data/fm_train_real.dat'
parameter_list = [[data]]

def converter_tdistributedstochasticneighborembedding(data_fname, seed=1):
	try:
		from shogun import TDistributedStochasticNeighborEmbedding
		from shogun import Math_init_random
		import shogun as sg

		# reproducible results
		Math_init_random(seed)
		features = sg.features(sg.csv_file(data_fname))

		converter = TDistributedStochasticNeighborEmbedding()
		converter.set_target_dim(2)

		embedding = converter.transform(features)

		return embedding
	except ImportError:
		print('No Eigen3 available')

if __name__=='__main__':
	print('TDistributedStochasticNeighborEmbedding')
	converter_tdistributedstochasticneighborembedding(*parameter_list[0])

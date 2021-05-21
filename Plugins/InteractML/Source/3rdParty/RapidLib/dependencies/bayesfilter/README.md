# INSTALLATION

## Requirements
* Swig
* Python 2.7 + Numpy / Scipy (I think Numpy v1.8+ is required)

## Building
move 'python' directory and type:

	make
	make install
	make clean

To install in a specific location:

	make install INSTALL_DIR=/path/to/install/location/

To specify swig location:

	make install SWIG=/path/to/swig/

# usage
Place the built python library somewhere in your python path. To add personal 
libraries located in '/Path/To/Libs' to the python path, add the following 
lines to your ".bash_profile":

	PYTHONPATH=$PYTHONPATH:/Path/To/Libs
	export PYTHONPATH

Basic usage: 

	import bayesfilter
	
	filt = bayesfilter.BayesianFilter()
	filt.init()
	
	for t in RAW_EMG_MATRIX.shape[0]:
		envelope_at_t = filt.update(RAW_EMG_MATRIX[t,:])

# credits

bayesfilter has been authored by Jules Françoise : jules.francoise@ircam.fr
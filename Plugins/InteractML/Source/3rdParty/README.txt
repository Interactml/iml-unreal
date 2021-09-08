# 3rd party dependencies

## RapidLib

* BSD 3-Clause license
* see LICENSE_RapidLib.txt for full details
* Marketplace declaration:
	RapidLib provides the machine learning algorithm functionality core to the operation of the plugin. The plugin is an integration of RapidLib into Unreal, augmenting it with additional usability features.

Rapidlib References the following, but only the JSON functionality is actually (indirectly) used by InteractML.

* JSON
* BayesFilter - Not needed for InteractML functionality
* LIBSVM - Not needed for InteractML functionality

## JSON

* github: https://github.com/open-source-parsers/jsoncpp (used to be at http://jsoncpp.sourceforge.net/)
* Public Domain (deferring to MIT License where PB not recognised, http://en.wikipedia.org/wiki/MIT_License)
* See LICENSE_JSON.txt for full details
* Marketplace declaration:
	Dependency of RapidLib

## BayesFilter

* On github: https://github.com/ircam-ismm/bayesfilter
* bayesfilter has been authored by Jules Françoise : jules.francoise@ircam.fr
* Copyright (C) 2013-2014 by IRCAM - Centre Pompidou. All Rights Reserved.
* See LICENSE_BayesFilter.txt for full details
* Marketplace declaration:
	Dependency of RapidLib (functionality not used by InteractML plugin)

### LIBSVM

* site: https://www.csie.ntu.edu.tw/~cjlin/libsvm/
* github: https://github.com/cjlin1/libsvm
	* Chih-Chung Chang and Chih-Jen Lin, LIBSVM : a library for support vector machines.
	* ACM Transactions on Intelligent Systems and Technology, 2:27:1--27:27, 2011.
	* Software available at http://www.csie.ntu.edu.tw/~cjlin/libsvm
* Looks like MIT 3-term?
* See LICENSE_LibSVM.txt for full details
* Marketplace declaration:
	Dependency of RapidLib (functionality not used by InteractML plugin)

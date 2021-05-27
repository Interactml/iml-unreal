//----
// InteractML - University Of Arts London
//----

#include "InteractMLContext.h"

//unreal

//module
#include "InteractML.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS


// LOCAL CLASSES & TYPES


// fetch/re-fetch a training set for a specific graph node
//
UInteractMLTrainingSet* UInteractMLContext::GetTrainingSet( UInteractMLContext::TGraphNodeID node_id, FString data_path )
{
	UInteractMLTrainingSet** pentry = TrainingSetLookup.Find(node_id);
	UInteractMLTrainingSet* ptrainingset = pentry?*pentry:nullptr;

	//change of data path?
	bool path_changed = ptrainingset && !ptrainingset->CheckBasePath(data_path);

	//not cached yet, or want different path (assume redirect to another file rather than a rename)
	if(!ptrainingset || path_changed)
	{
		//remove old from cache
		if(ptrainingset)
		{
			TrainingSetCache.Remove(ptrainingset);
		}

		//re-obtain
		ptrainingset = FInteractMLModule::Get().GetTrainingSet( data_path );
		
		//update cache
		TrainingSetCache.Add(ptrainingset);
		TrainingSetLookup.Add(node_id, ptrainingset);
	}

	return ptrainingset;
}

// fetch/re-fetch a model for a specific graph node
UInteractMLModel* UInteractMLContext::GetModel( UInteractMLContext::TGraphNodeID node_id, FString data_path)
{
	UInteractMLModel** pentry = ModelLookup.Find(node_id);
	UInteractMLModel* pmodel = pentry?*pentry:nullptr;
	
	//change of data path?
	bool path_changed = pmodel && !pmodel->CheckBasePath(data_path);
	
	//not cached yet, or want different path (assume redirect to another file rather than a rename)
	if(!pmodel || path_changed)
	{
		//remove old from cache
		if(pmodel)
		{
			ModelCache.Remove(pmodel);
		}
		
		//re-obtain
		pmodel = FInteractMLModule::Get().GetModel( data_path );
		
		//update cache
		ModelCache.Add(pmodel);
		ModelLookup.Add(node_id, pmodel);
	}
	
	return pmodel;
}

// fetch/re-fetch a parameter set for passing around the graph
FInteractMLParameters::Ref UInteractMLContext::GetParameters( UInteractMLContext::TGraphNodeID node_id )
{
	//existing?
	FInteractMLParameters::Ref* pentry = ParametersLookup.Find(node_id);
	if (pentry)
	{
		return *pentry;
	}
	
	//not cached yet, alloc
	FInteractMLParameters::Ref parameters = MakeShareable(new FInteractMLParameters());

	//cache
	ParametersCache.Add(parameters);
	ParametersLookup.Add(node_id, parameters);

	return parameters;
}



// EPILOGUE
#undef LOCTEXT_NAMESPACE


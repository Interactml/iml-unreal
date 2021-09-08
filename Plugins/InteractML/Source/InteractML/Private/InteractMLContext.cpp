//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
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
//
UInteractMLModel* UInteractMLContext::GetModel( UClass* model_type, UInteractMLContext::TGraphNodeID node_id, FString data_path)
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
		pmodel = FInteractMLModule::Get().GetModel( model_type, data_path );
		
		//update cache
		ModelCache.Add(pmodel);
		ModelLookup.Add(node_id, pmodel);
	}
	
	return pmodel;
}

// fetch/re-fetch a parameter set for passing around the graph
//
TSharedPtr<FInteractMLParameterCollection> UInteractMLContext::GetParameters( UInteractMLContext::TGraphNodeID node_id )
{
	//existing?
	TSharedPtr<FInteractMLParameterCollection>* pentry = ParametersLookup.Find(node_id);
	if (pentry)
	{
		return *pentry;
	}
	
	//not cached yet, alloc
	TSharedPtr<FInteractMLParameterCollection> parameters = MakeShareable(new FInteractMLParameterCollection());

	//cache
	ParametersCache.Add(parameters);
	ParametersLookup.Add(node_id, parameters);

	return parameters;
}

// fetch/re-fetch a model state object for a specific node to run a model
//
TSharedPtr<FInteractMLModelState> UInteractMLContext::GetModelState( TGraphNodeID node_id )
{
	//existing?
	TSharedPtr<FInteractMLModelState>* pentry = StateLookup.Find(node_id);
	if (pentry)
	{
		return *pentry;
	}
	
	//not cached yet, alloc
	TSharedPtr<FInteractMLModelState> state = MakeShareable(new FInteractMLModelState());
	
	//cache
	StateLookup.Add(node_id, state);
	
	return state;
}

// notify use of a training set asset
//
void UInteractMLContext::SetTrainingSet( UInteractMLContext::TGraphNodeID node_id, UInteractMLTrainingSet* training_set )
{
	UInteractMLTrainingSet** pentry = TrainingSetLookup.Find(node_id);
	UInteractMLTrainingSet* ptrainingset = pentry?*pentry:nullptr;
	
	//known?
	if (ptrainingset && ptrainingset == training_set)
	{
		return;
	}

	//not cached yet or changed
	if(!ptrainingset)
	{
		//remove old from cache
		if(ptrainingset)
		{
			TrainingSetCache.Remove(ptrainingset);
		}
		
		//register
		FInteractMLModule::Get().SetTrainingSet( training_set );
		
		//update cache
		TrainingSetCache.Add(training_set);
		TrainingSetLookup.Add(node_id, training_set);
	}
}

// notify use of a model asset
//
void UInteractMLContext::SetModel( UInteractMLContext::TGraphNodeID node_id, UInteractMLModel* model )
{
	UInteractMLModel** pentry = ModelLookup.Find(node_id);
	UInteractMLModel* pmodel = pentry?*pentry:nullptr;

	//known?
	if (pmodel && pmodel == model)
	{
		return;
	}
	
	//not cached yet or changed
	if(!pmodel)
	{
		//remove old from cache
		if(pmodel)
		{
			ModelCache.Remove(pmodel);
		}
		
		//register
		FInteractMLModule::Get().SetModel( model );
		
		//update cache
		ModelCache.Add(model);
		ModelLookup.Add(node_id, model);
	}
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE


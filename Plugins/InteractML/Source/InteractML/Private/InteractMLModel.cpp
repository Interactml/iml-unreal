//----
// InteractML - University Of Arts London
//----

#include "InteractMLModel.h"

//unreal

//module

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS

// extension prefix for model data files
FString UInteractMLModel::cExtensionPrefix(TEXT(".model"));

// LOCAL CLASSES & TYPES

// nothing to load, created a new one
//
void UInteractMLModel::Create()
{
	//ensure initialised
	ResetModel();
}

// read and restore stored model state into actual RapidLib model
//
bool UInteractMLModel::LoadJson(const FString& json_string)
{
	//ensure model initialised and clear
	ResetModel();

	//set up model from json
	modelSetFloat* model = GetModelInstance();

	//unfortunately we need to convert FString to std::string for this
	auto ansi_json = StringCast<ANSICHAR>( *json_string );
	std::string std_json = ansi_json.Get();

	//apply
	return model->putJSON( std_json );
}

// save out RapidLib model state
//
bool UInteractMLModel::SaveJson(FString& json_string) const
{
	//extract json
	modelSetFloat* model = GetModelInstance();
	std::string std_json = model->getJSON();

	//unfortunately we need to convert the std::string back to an FString for this
	auto tchar_json = StringCast<TCHAR>( std_json.c_str() );
	json_string = tchar_json.Get();
	
	return true;
}



// run the model against the provided parameters
// returns the label matched
// NOTE: runs synchronously, i.e. blocks until complete
//
int UInteractMLModel::RunModel(FInteractMLParameterCollection* parameters)
{
	return RunModelInstance( parameters );
}

// train the model with the provided training set
// NOTE: runs synchronously, i.e. blocks until complete
//
void UInteractMLModel::TrainModel(UInteractMLTrainingSet* training_set)
{
	TrainModelInstance(training_set);
}

// reset the model to initialised but empty
//
void UInteractMLModel::ResetModel()
{
	ResetModelInstance();
}


// fallback operation of running a model, can be specialised
//
int UInteractMLModel::RunModelInstance(struct FInteractMLParameterCollection* parameters)
{
	return 0;
}

// fallback operation of trainign a model, can be specialised
//
void UInteractMLModel::TrainModelInstance(class UInteractMLTrainingSet* training_set)
{
	//set up model from json
	modelSetFloat* model = GetModelInstance();

	//model->train(	
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE


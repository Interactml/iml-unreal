// Fill out your copyright notice in the Description page of Project Settings.

//module
#include "MachineLearningModel.h"

//std
#include <sstream>
#include <vector>



// Sets default values for this component's properties
UMachineLearningModel::UMachineLearningModel()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}




#pragma region Machine Learning

bool UMachineLearningModel::addTrainingData(TArray<float> input, TArray<float> output) {
    FDataInstance data;
    data.inputs = input;
    data.outputs = output;

    if (m_numInputs < 0) {
        m_numInputs = input.Num();
    }
    else {
        if (m_numInputs != input.Num()) {
            return false;
        }
    }

    if (m_numOutputs < 0) {
        m_numOutputs = output.Num();
    }
    else {
        if (m_numOutputs != output.Num()) {
            return false;
        }
    }

    m_dataset.Add(data);
    return true;
}

bool UMachineLearningModel::SetUpInputsOutputs(FDataInstance example) {
    bool setInputsOutputs = false;
    m_numInputs = example.inputs.Num();
    m_numOutputs = example.outputs.Num();
    if (m_numInputs > 0 && m_numOutputs > 0) {
        setInputsOutputs = true;
    }
    return setInputsOutputs;
}

// helper to convert a data set into a training set from
//
void BuildTrainingSetFromDataSet( TArray<FDataInstance>& dataset, std::vector<trainingExample>& training_set_out )
{
	training_set_out.clear();
	for (int32 i = 0; i < dataset.Num(); i++) 
	{
		FDataInstance& d = dataset[i];
		
		//build example
		trainingExample example;        
		for (int j = 0; j < d.inputs.Num(); j++) 
		{
			example.input.push_back( (double)d.inputs[j] );
		}        
		for (int j = 0; j < d.outputs.Num(); j++) 
		{
			example.output.push_back( (double)d.outputs[j] );
		}
		
		//add to set
		training_set_out.push_back(example);
	}
}

bool UMachineLearningModel::trainRegressor() 
{
	//fill in a training set with current dataset
	std::vector<trainingExample> training_set;
	BuildTrainingSetFromDataSet( m_dataset, training_set );

	//create on demand
    if(m_regressionModel == NULL)
    {
		m_regressionModel = new regression();
    }

	//train	
    bool result = m_regressionModel->train( training_set );   
    return result;
}

bool UMachineLearningModel::trainClassifier() 
{
	//fill in a training set with current dataset
	std::vector<trainingExample> training_set;
	BuildTrainingSetFromDataSet( m_dataset, training_set );
	
	//create on demand
	if(m_classificationModel == NULL)
	{
		m_classificationModel = new classification();
	}
	
	//train	
	bool result = m_classificationModel->train( training_set );   
	return result;
}

/// <summary>
/// trains dynamic time warping
/// </summary>
/// <returns></returns>
bool UMachineLearningModel::trainDTW() {
    return true;
}

TArray<float> UMachineLearningModel::Run(TArray<float> input)
{
	//convert inputs to double
	std::vector<double> inputs;
	for (int i = 0; i < input.Num(); i++) 
	{
		inputs.push_back( (double)input[i] );
	}
	
	//run whichever model is present
	std::vector<double> outputs;
	if (m_regressionModel)
	{
		outputs = m_regressionModel->run(inputs);
	}
	else if (m_classificationModel)
	{
		outputs = m_classificationModel->run(inputs);
	}

	//convert outputs back
	TArray<float> results;
	for (int i = 0; i < (int)outputs.size(); i++) 
	{
		results.Add( (float)outputs[i] );
	}
	return results;
}


#pragma endregion Machine Learning



// Called when the game starts
void UMachineLearningModel::BeginPlay()
{
	Super::BeginPlay();
}



// Called when the game starts
void UMachineLearningModel::BeginDestroy()
{
    Super::BeginDestroy();
}



// Called every frame
void UMachineLearningModel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


// read text from a file on disk into a string
//
FString LoadString( FString file_path, bool& success )
{
    IPlatformFile& file_api = FPlatformFileManager::Get().GetPlatformFile();
    if(file_api.FileExists( *file_path ))
    {
        FString string;
        if(FFileHelper::LoadFileToString( string, *file_path ))
        {
            success = true;
            return string;
        }
    }

    success = false;
    return "";
}

// save a string out to a file on disk
//
void SaveString( FString file_path, FString string, bool& success )
{
    FStringView whole_string( string );
    if(FFileHelper::SaveStringToFile( whole_string, *file_path ))
    {
        success = true;
        return;
    }

    success = false;
    return;
}


// save the dataset as JSON string to a file
//
void UMachineLearningModel::SaveDatasetAsJson( FString file_path, bool& success )
{  
    //convert object to json
    FString json_string;

    //have to manually convert array to json string
    TArray<TSharedPtr<FJsonValue>> data_values;
    for(const FDataInstance& data_inst : m_dataset)
    {
        TSharedPtr<FJsonObject> data_inst_object = FJsonObjectConverter::UStructToJsonObject( data_inst );
        if(data_inst_object.IsValid())
        {
            data_values.Add( MakeShared<FJsonValueObject>( data_inst_object ) );
        }
    }
    TSharedRef<FJsonValueArray> ValuesArrayValue = MakeShared<FJsonValueArray>( data_values );
    TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create( &json_string );
    FJsonSerializer::Serialize( ValuesArrayValue, FString(), JsonWriter );

    //save
    success = false;
    SaveString( file_path, json_string, success );
}

// load the dataset from a JSON file
//
void UMachineLearningModel::LoadDatasetFromJson( FString file_path, bool& success )
{
    //clear existing (even if failed)
    m_dataset.Empty();

    //load
    success = false;
    FString json_string = LoadString( file_path, success );
    if(success)
    {
        //wrapped
        if(FJsonObjectConverter::JsonArrayStringToUStruct( json_string, &m_dataset, 0, 0 ))
        {
            success = true;
        }
    }

    //init in/out counts
    if(m_dataset.Num() > 0)
    {
        SetUpInputsOutputs( m_dataset[0] );
    }
}


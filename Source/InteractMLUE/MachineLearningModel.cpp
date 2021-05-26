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

bool UMachineLearningModel::stopCollecting() {
    if (collecting) {
        collecting = false;
        if (m_datasetSeriesSingle.inputSeries.Num() > 0 && !m_datasetSeriesSingle.label.IsEmpty()) {
            m_datasetSeries.Add(m_datasetSeriesSingle);
            return true;
        }
    }
    return collecting;
}

bool UMachineLearningModel::addTrainingDataSeries(FDataInstanceSeriesMember trainingSeriesMember, FString label) {
    FDataInstanceSeriesMember data;
    data = trainingSeriesMember;

    // if this is the beginning of collecting data
    if (!collecting) {
        collecting = true;
        // clear the previous dtw series 
        m_datasetSeriesSingle.inputSeries.Empty();
        // check if the label is empty 
        if (!label.IsEmpty()) {
            m_datasetSeriesSingle.label = label;
        }
        else {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "no label");
            return false;
        }
             
    }
    if (data.inputData.Num() == m_numInputs) {
        m_datasetSeriesSingle.inputSeries.Add(data);
        return true;
    }
    else {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(m_numInputs));
    }
    return false;
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

FDataInstanceSeriesMember UMachineLearningModel::SetUpSeriesStruct(TArray<float> inputs) {
    m_datasetSeriesSingleMember.inputData = inputs;
    return m_datasetSeriesSingleMember;
}

bool UMachineLearningModel::SetUpInputsOutputsSerie(FDataInstanceSeriesMember example, FString label) {
    bool setInputsOutputs = false;

    m_numInputs = example.inputData.Num();
    if (m_numInputs > 0) {
        setInputsOutputs = true;
    }
    return setInputsOutputs;
}

// helper to convert a data set into a training set from
//
void UMachineLearningModel::BuildTrainingSetFromDataSet( TArray<FDataInstance>& dataset, std::vector<trainingExample>& training_set_out )
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
bool UMachineLearningModel::trainDTW(TArray<FDataInstanceSeries> trainingDataSeries) {
    // check if there is training data
    if (m_datasetSeries.Num() == 0) {

        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "no training data for dtw");
        return false;
    }

    if (m_dtwModel == NULL)
    {
        m_dtwModel = new seriesClassification();
    }
   
    // reset model
    m_dtwModel->reset();

    // create dtw training set
    std::vector<trainingSeries> *trainingSeriesCollection = new std::vector<trainingSeries>();
    trainingSeries *tempTrainingSeries = new trainingSeries();
    std::vector<double> tempFeatureVector;
    
    for(FDataInstanceSeries dtwExample : m_datasetSeries) {
        

        for (FDataInstanceSeriesMember seriesItem : dtwExample.inputSeries) {

            // copy data into plain c arrays
            double* in = new double[seriesItem.inputData.Num()];
            
            // Add external feature to it
            tempFeatureVector.clear();
            for (int i = 0; i < seriesItem.inputData.Num(); i++) {
                tempFeatureVector.push_back(seriesItem.inputData[i]);
            }
            // Push temp vector with external feature to series
            tempTrainingSeries->input.push_back(tempFeatureVector);
        }

        std::string lab = std::string(TCHAR_TO_UTF8(*dtwExample.label));
        tempTrainingSeries->label = lab;

        // add series to collection 
        trainingSeriesCollection->push_back(*tempTrainingSeries);
    }
    // train ghe model 
    bool result = m_dtwModel->train(*trainingSeriesCollection);

    //m_destroyTrainingSetFromDLL(trainingSet);
    return result;
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


bool UMachineLearningModel::PopulatingLogic(TArray<float> input){
    
    TArray<float> inputData = input;

    if (m_dtwModel == NULL)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "model null");
        return false;
    } 

    if (!running)
    {
        running = true;
        return true;
    } 

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "populate");

    if (running) {
        if (m_numInputs == input.Num()) {
            std::vector<double> tempExamp;
            for (int i = 0; i < input.Num(); i++) {
                tempExamp.push_back(input[i]);
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(input[i]));
            }
            m_populatingSet.push_back(tempExamp);
            return true;
        }
    }
    
    return false;

    
}
/// <summary>
/// Stop dtw which runs dtw model on movement just performed
/// </summary>
/// <returns>output of the model or empty if did not work</returns>
FString UMachineLearningModel::StopPopulatingDTW(){
    // if dtwmodel is not null the populating set is not empty and the system was previously running
    if (m_dtwModel != NULL && !m_populatingSet.empty() && running) {
        // set running to false so it doesn't run again
        running = false;
        //create training set
        trainingSeries *series = new trainingSeries();
        for (int i = 0; i < m_populatingSet.size(); i++) {
            series->input.push_back(m_populatingSet[i]);
        }
        // code causing crash 
        std::string predOutput = m_dtwModel->run(series->input);
        delete series;
        m_populatingSet.clear();
        FString output = predOutput.c_str();
        return output;
    }
    m_populatingSet.clear();
    return "empty";
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


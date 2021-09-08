//----
// InteractML - Interactive Machine Learning Plugin
// Copyright (c) 2021 Phoenix Perry and Rebecca Fiebrink
// Using the MIT License. https://github.com/Interactml
//----

#include "InteractMLBlueprintLibrary.h"

//unreal
#include "GameFramework/Actor.h"

//module
#include "InteractML.h"
#include "InteractMLContext.h"
#include "Models/InteractMLClassificationModel.h"
#include "Models/InteractMLRegressionModel.h"
#include "Models/InteractMLDynamicTimeWarpModel.h"
#include "InteractMLModelState.h"
#include "InteractMLHelpers.h"
#include "InteractMLLabel.h"

// PROLOGUE
#define LOCTEXT_NAMESPACE "InteractML"

// CONSTANTS & MACROS


// LOCAL CLASSES & TYPES


//////////////////////////////////// HELPERS ////////////////////////////////////

// ML operations going on in a blueprint need somewhere to store state both as control passes between nodes
// and between evaluation frames. This provides access to some such place, by adding a UInteractMLContext object
// to the actor on demand and returning an existing one if already added.  This is a transient object so won't be
// persisted along with the actor.
//
static UInteractMLContext* GetMLContext(AActor* Actor)
{
	//look for existing
	UInteractMLContext* context = Cast<UInteractMLContext>( Actor->GetComponentByClass(UInteractMLContext::StaticClass()) );

	//create/add on demand
	if (!context)
	{
		context = NewObject<UInteractMLContext>( Actor, NAME_None, RF_Transient | RF_DuplicateTransient );
		Actor->AddOwnedComponent( context );
		context->RegisterComponent();
	}

	return context;
}


//////////////////////////////// BLUEPRINT LIBRARY /////////////////////////////////


// Parameter Collection custom node: call to obtain a parameter object to accumulate into
//
FInteractMLParameters UInteractMLBlueprintLibrary::GetParameters(AActor* Actor, UInteractMLContext::TGraphNodeID NodeID)
{
	//check
	if(!Actor)
	{
		UE_LOG( LogInteractML, Error, TEXT( "No actor provided to obtain a parameter collection from." ) );
		return FInteractMLParameters();
	}

	//find the context we cache our state in
	UInteractMLContext* Context = GetMLContext( Actor );

	//get a parameter collection for this node to use
	TSharedPtr<FInteractMLParameterCollection> parameters = Context->GetParameters( NodeID );

	//prep for (re)accumulation
	parameters->Reset();

	return FInteractMLParameters( parameters );
}

// Parameter Collection custom node: call to add a parameter
//
void UInteractMLBlueprintLibrary::AddIntegerParameter(FInteractMLParameters Parameters, int Value)
{
	if(Parameters.Ptr.IsValid())
	{
		Parameters.Ptr->Add( Value );
	}
}
void UInteractMLBlueprintLibrary::AddFloatParameter( FInteractMLParameters Parameters, float Value )
{
	if(Parameters.Ptr.IsValid())
	{
		Parameters.Ptr->Add( Value );
	}
}
void UInteractMLBlueprintLibrary::AddBooleanParameter( FInteractMLParameters Parameters, bool Value )
{
	if(Parameters.Ptr.IsValid())
	{
		Parameters.Ptr->Add( Value );
	}
}
void UInteractMLBlueprintLibrary::AddVector2Parameter( FInteractMLParameters Parameters, FVector2D Value )
{
	if(Parameters.Ptr.IsValid())
	{
		Parameters.Ptr->Add( Value );
	}
}
void UInteractMLBlueprintLibrary::AddVector3Parameter( FInteractMLParameters Parameters, FVector Value )
{
	if(Parameters.Ptr.IsValid())
	{
		Parameters.Ptr->Add( Value );
	}
}
void UInteractMLBlueprintLibrary::AddQuaternionParameter( FInteractMLParameters Parameters, FQuat Value )
{
	if(Parameters.Ptr.IsValid())
	{
		Parameters.Ptr->Add( Value );
	}
}
void UInteractMLBlueprintLibrary::AddColourParameter( FInteractMLParameters Parameters, FLinearColor Value )
{
	if(Parameters.Ptr.IsValid())
	{
		Parameters.Ptr->Add( Value );
	}
}

///////////////////// TRAINING SET //////////////////////

// Training set access
// obtain the training set from the context object for this actor
// NOTE: training sets are shared objects (same path/same object), cached at the plugin level
// NOTE: we still disambiguate so we can track which nodes/how many are using these objects
//
UInteractMLTrainingSet* UInteractMLBlueprintLibrary::GetTrainingSet(AActor* Actor, FString DataPath, FString NodeID, bool& HasData)
{
	//find the context we cache our state in
	UInteractMLContext* Context = GetMLContext( Actor );
	check(Context);
	
	//get a parameter collection for this node to use
	UInteractMLContext::TGraphNodeID id = NodeID;
	UInteractMLTrainingSet* training_set = Context->GetTrainingSet( id, DataPath );
	check(training_set);

	//any data?
	HasData = training_set->HasExamples();
	
	return training_set;
}

// training set recording : simple label
// SINGLE: when recording requested, takes snapshot of parameter data and associates it with the given label and stores it in the training set
// SERIES: when recording is active, accumulates snapshots of parameter data and associates them with the given label and stores it in the training set
// when finished returns true briefly to signify that new training set data is ready
// NOTE: state about recording can be held in the training set object even though it's technically shared because it doesn't make
//       too much sense to be recording using two training nodes at the same time.
bool UInteractMLBlueprintLibrary::RecordExampleSimple(
	AActor* Actor,
	UInteractMLTrainingSet* TrainingSet, 
	FInteractMLParameters Parameters,
	bool WantSeries,
	float Label,
	bool Record,
	bool DeleteLast,
	bool DeleteLabel,
	bool DeleteAll,
	FString NodeID)
{
	if(!TrainingSet)
	{
		UE_LOG( LogInteractML, Warning, TEXT( "Recording node used with no training set (actor %s)" ), *AActor::GetDebugName( Actor ) );
		return false;
	}
	FInteractMLParameterCollection* parameters = Parameters.Ptr.Get();

	//recording
	bool is_finished = false;
	bool ok = true;
	bool initial_record = false;
	if(TrainingSet->RecordingAction.Triggered( Record, NodeID ))
	{
		if (Record)
		{
			//start recording
			ok = TrainingSet->BeginRecording(Label);
			if (ok)
			{
				//record single snapshot upon start
				initial_record = true;
			}
		}
		else
		{
			//stop recording
			ok = TrainingSet->EndRecording();

			//need to make sure context/module are aware of model use/changes during PIE
			UInteractMLContext* Context = GetMLContext( Actor );
			check( Context );
			Context->SetTrainingSet(NodeID, TrainingSet);
			
			//success?
			is_finished = ok; //briefly return true upon successful end trigger
		}
	}
	
	//single/continuous recording
	if (ok)
	{
		if (initial_record || (WantSeries && Record))
		{
			//perform a sample of the parameters
			ok = TrainingSet->RecordParameters(parameters);
		}
	}

	//reset handling
	if (TrainingSet->DeletingLastAction.Triggered( DeleteLast, NodeID ))
	{
		if (DeleteLast)
		{
			//just activated
			TrainingSet->DeleteLastExample();
		}
	}
	if (TrainingSet->DeletingLabelAction.Triggered( DeleteLabel, NodeID ))
	{
		if (DeleteLabel)
		{
			//just activated
			TrainingSet->DeleteLabelExamples( Label );
		}
	}
	if (TrainingSet->DeletingAllAction.Triggered( DeleteAll, NodeID ))
	{
		if (DeleteAll)
		{
			//just activated
			TrainingSet->DeleteAllExamples();
		}
	}
	
	//finished recording
	return is_finished;
}

// training set recording : composite label
// SINGLE: when recording requested, takes snapshot of parameter data and associates it with the given label and stores it in the training set
// SERIES: when recording is active, accumulates snapshots of parameter data and associates them with the given label and stores it in the training set
// when finished returns true briefly to signify that new training set data is ready
// NOTE: state about recording can be held in the training set object even though it's technically shared because it doesn't make
//       too much sense to be recording using two training nodes at the same time.
bool UInteractMLBlueprintLibrary::RecordExampleComposite(
	AActor* Actor,
	UInteractMLTrainingSet* TrainingSet,
	FInteractMLParameters Parameters,
	bool WantSeries,
	const UInteractMLLabel* LabelType,
	const FGenericStruct& LabelData, 	//<-- placeholder for the generic parameter mapped in the thunk function below
	bool Record,
	bool DeleteLast,
	bool DeleteLabel,
	bool DeleteAll,
	FString NodeID)
{
	//placeholder for generic structure binding, never actually called
	//see Generic_RecordExampleComposite below
	check(0);
	return false;
}

// generic implementation of above fn
//
bool UInteractMLBlueprintLibrary::Generic_RecordExampleComposite(
	AActor* Actor,
	UInteractMLTrainingSet* TrainingSet,
	FInteractMLParameters Parameters,
	bool WantSeries,
	const UInteractMLLabel* LabelType,
	const void* LabelData,	//<-- the generic parameter
	bool Record,
	bool DeleteLast,
	bool DeleteLabel,
	bool DeleteAll,
	FString NodeID)
{
	//NOTE: Code should be identical to RecordExampleSimple above, but passing
	//NOTE:  LabelType/LabelData instead of Label (float) to RecordingAction.Triggered
	//NOTE: Other than that they should be kept in sync.

	if(!TrainingSet)
	{
		UE_LOG( LogInteractML, Warning, TEXT( "Recording node used with no training set (actor %s)" ), *AActor::GetDebugName( Actor ) );
		return false;
	}
	FInteractMLParameterCollection* parameters = Parameters.Ptr.Get();
	
	//recording
	bool is_finished = false;
	bool ok = true;
	bool initial_record = false;
	if(TrainingSet->RecordingAction.Triggered( Record, NodeID )) //<-- This line is different to RecordExampleSimple
	{
		if (Record)
		{
			//start recording
			ok = TrainingSet->BeginRecording(LabelType, LabelData);
			if (ok)
			{
				//record single snapshot upon start
				initial_record = true;
			}
		}
		else
		{
			//stop recording
			ok = TrainingSet->EndRecording();
			
			//need to make sure context/module are aware of model use/changes during PIE
			UInteractMLContext* Context = GetMLContext( Actor );
			check( Context );
			Context->SetTrainingSet(NodeID, TrainingSet);
			
			//success?
			is_finished = ok; //briefly return true upon successful end trigger
		}
	}
	
	//single/continuous recording
	if (ok)
	{
		if (initial_record || (WantSeries && Record))
		{
			//perform a sample of the parameters
			ok = TrainingSet->RecordParameters(parameters);
		}
	}
	
	//reset handling
	if (TrainingSet->DeletingLastAction.Triggered( DeleteLast, NodeID ))
	{
		if (DeleteLast)
		{
			//just activated
			TrainingSet->DeleteLastExample();
		}
	}
	if (TrainingSet->DeletingLabelAction.Triggered( DeleteLabel, NodeID ))
	{
		if (DeleteLabel)
		{
			//just activated
			TrainingSet->DeleteLabelExamples( LabelType, LabelData );
		}
	}
	if (TrainingSet->DeletingAllAction.Triggered( DeleteAll, NodeID ))
	{
		if (DeleteAll)
		{
			//just activated
			TrainingSet->DeleteAllExamples();
		}
	}
	
	//finished recording
	return is_finished;
}


///////////////////// MODEL //////////////////////

// model access
//
UInteractMLModel* UInteractMLBlueprintLibrary::GetModel(AActor* Actor, FString DataPath, EInteractMLModelType ModelType, FString NodeID, bool& IsTrained)
{
	IsTrained = false;

	//find the context we cache our state in
	UInteractMLContext* Context = GetMLContext( Actor );
	check( Context );

	UClass* model_class = nullptr;
	switch (ModelType)
	{
		case EInteractMLModelType::Classification:
			model_class = UInteractMLClassificationModel::StaticClass();
			break;
		case EInteractMLModelType::Regression:
			model_class = UInteractMLRegressionModel::StaticClass();
			break;
		case EInteractMLModelType::DynamicTimewarp:
			model_class = UInteractMLDynamicTimeWarpModel::StaticClass();
			break;
		default:
			UE_LOG(LogInteractML, Error, TEXT("Unspecified model type requested. Ensure you have set the Model type you require on the Model node."));
			return nullptr;
	}

	//get a parameter collection for this node to use
	UInteractMLContext::TGraphNodeID id = NodeID;
	UInteractMLModel* model = Context->GetModel( model_class, id, DataPath );
	check( model );

	IsTrained = model->IsTrained();

	return model;
}

// model running : simple label, blocking
//
float UInteractMLBlueprintLibrary::RunModelSimple( 
	AActor* Actor, 
	UInteractMLModel* Model, 
	FInteractMLParameters Parameters, 
	bool Run, 
	FString NodeID)
{
	//possible?
	if (!Model)
	{
		return 0;
	}

	//need context and state store to run with
	UInteractMLContext* Context = GetMLContext( Actor );
	check( Context );
	TSharedPtr<FInteractMLModelState> model_state = Context->GetModelState( NodeID );
	check( model_state.IsValid() );

	//series/single operation
	bool success = true;
	if(Model->IsSeries())
	{		
		//-------------- SERIES: accumulate whilst active and run once complete ---------------
	
		//check for transition
		if (model_state->RunAction.Triggered(Run, NodeID))
		{
			//change in run state
			if (Run)
			{
				//just started a run
				//reset stored series
				model_state->ParameterSeries.Clear();
			}
			else
			{
				//just stopped a run
				//TODO: Do we want to include the last parameter set sent as the run is stopped in the test set?
				success = Model->RunModel( &model_state->ParameterSeries, model_state->CurrentResult );
			}
		}
		else if(Run)
		{
			//still running
			//record next parameter set
			model_state->ParameterSeries.Add( Parameters.Ptr.Get() );
		}

	}
	else
	{
		//--------------- SINGLE: just repeatedly run against a single parameter set ---------------
		if (Run)
		{
			success = Model->RunModel( Parameters.Ptr.Get(), model_state->CurrentResult );
		}
	}
	
	//report current/last available result
	if (success)
	{
		if (model_state->CurrentResult.Num() == 1)
		{
			return model_state->CurrentResult[0];
		}
	}
	return 0.0f;
}

// model running : simple label, async
//
float UInteractMLBlueprintLibrary::RunModelSimpleAsync( 
	AActor* Actor, 
	UInteractMLModel* Model, 
	FInteractMLParameters Parameters, 
	bool Run, 
	FString NodeID,
	bool& Running,
	bool& Completed)
{
	if (!Model)
	{
		//no model, nothing to do
		Running = false;
		Completed = false;
		return 0;
	}
	
	//need context and state store to run with
	UInteractMLContext* Context = GetMLContext( Actor );
	check( Context );
	TSharedPtr<FInteractMLModelState> model_state = Context->GetModelState( NodeID );
	check( model_state.IsValid() );

	//monitor for completion (before state checks which may want to retrigger this frame)
	bool just_completed = model_state->CheckCompleted();	//(only check once as this resets on query)
	//run complete? - new output will be available from the model state
		
	//series/single operation
	if(Model->IsSeries())
	{		
		//-------------- SERIES: accumulate whilst active and run once complete ---------------
		
		//check for transition
		if (model_state->RunAction.Triggered(Run, NodeID))
		{
			//change in run state
			if (Run)
			{
				//just started a run
				//reset stored series
				model_state->ParameterSeries.Clear();
			}
			else
			{
				//just stopped a run
				//TODO: Do we want to include the last parameter set sent as the run is stopped in the test set?
				FInteractMLTask::Ptr run_task = Model->RunModelAsync( &model_state->ParameterSeries );
				model_state->StartRunning(run_task);
			}
		}
		
		//accumulate
		if(Run)
		{
			//still running
			//record next parameter set
			model_state->ParameterSeries.Add( Parameters.Ptr.Get() );
		}
	}
	else
	{
		//--------------- SINGLE: just repeatedly run (retrigger) against a single parameter set ---------------
		if (Run && !model_state->IsRunning())
		{
			FInteractMLTask::Ptr run_task = Model->RunModelAsync( Parameters.Ptr.Get() );
			model_state->StartRunning(run_task);
		}
	}

	//report status
	Running = model_state->IsRunning();
	Completed = just_completed;

	//report new/current/last available result
	if (model_state->CurrentResult.Num() == 1)
	{
		return model_state->CurrentResult[0];
	}
	return 0.0f;
}

// model running : composite label, blocking
//
void UInteractMLBlueprintLibrary::RunModelComposite( 
	AActor* Actor, 
	UInteractMLModel* Model, 
	FInteractMLParameters Parameters, 
	bool Run, 
	FString NodeID,
	const UInteractMLLabel* LabelType,
	FGenericStruct& LabelData) 	//<-- placeholder for the generic output parameter mapped in the thunk function below
{
	//placeholder for generic structure binding, never actually called
	//see Generic_RunModelComposite below
	check(0);
}

// generic implementation of above fn
//
void UInteractMLBlueprintLibrary::Generic_RunModelComposite(
	AActor* Actor, 
	UInteractMLModel* Model, 
	FInteractMLParameters Parameters, 
	bool Run, 
	FString NodeID,
	const UInteractMLLabel* LabelType,
	void* LabelData)	//<-- the generic parameter
{
	//NOTE: Code should be identical to RunModelSimple above, but passing
	//NOTE:  LabelType/LabelData instead of returning Label (float)
	//NOTE: Other than that they should be kept in sync.

	if (!Model)
	{
		return;
	}
	
	//need context and state store to run with
	UInteractMLContext* Context = GetMLContext( Actor );
	check( Context );
	TSharedPtr<FInteractMLModelState> model_state = Context->GetModelState( NodeID );
	check( model_state.IsValid() );
	
	//series/single operation
	bool success = true;
	if(Model->IsSeries())
	{		
		//-------------- SERIES: accumulate whilst active and run once complete ---------------
		
		//check for transition
		if (model_state->RunAction.Triggered(Run, NodeID))
		{
			//change in run state
			if (Run)
			{
				//just started a run
				//reset stored series
				model_state->ParameterSeries.Clear();
			}
			else
			{
				//just stopped a run
				//TODO: Do we want to include the last parameter set sent as the run is stopped in the test set?
				success = Model->RunModel( &model_state->ParameterSeries, model_state->CurrentResult );
			}
		}
		else if(Run)
		{
			//still running
			//record next parameter set
			model_state->ParameterSeries.Add( Parameters.Ptr.Get() );
		}
		
	}
	else
	{
		//--------------- SINGLE: just repeatedly run against a single parameter set ---------------
		if (Run)
		{
			success = Model->RunModel( Parameters.Ptr.Get(), model_state->CurrentResult );
		}
	}

	//report current/last available result (if available)
	if (success && model_state->CurrentResult.Num() > 0)
	{
		const FInteractMLLabelCache& label_cache = Model->GetLabelCache();

		//map results back to output label structure
		if (Model->IsDiscrete())
		{
			//just one, use labels as-is, just index the one needed
			if(ensure( model_state->CurrentResult.Num() == 1 )) //should only be single model output value
			{
				//find label to apply
				int label_index = (int)model_state->CurrentResult[0];
				TArray<float> specific_label;
				if(label_cache.GetLabel( label_index, specific_label ))
				{
					//rebuild struct from this specific label
					LabelType->RecreateData( specific_label, LabelData, label_cache );
				}
			}
		}
		else
		{
			//interpolated multi-value output, they map to each structure member directly
			LabelType->RecreateData( model_state->CurrentResult, LabelData, label_cache );
		}
	}
}

// model running : composite label, asynchronous
//
void UInteractMLBlueprintLibrary::RunModelCompositeAsync( 
	AActor* Actor, 
	UInteractMLModel* Model, 
	FInteractMLParameters Parameters, 
	bool Run, 
	FString NodeID,
	const UInteractMLLabel* LabelType,
	FGenericStruct& LabelData, 	//<-- placeholder for the generic output parameter mapped in the thunk function below
	bool& Running, 
	bool& Completed )
{
	//placeholder for generic structure binding, never actually called
	//see Generic_RunModelCompositeAsync below
	check(0);
}

// generic implementation of above fn
//
void UInteractMLBlueprintLibrary::Generic_RunModelCompositeAsync(
	AActor* Actor, 
	UInteractMLModel* Model, 
	FInteractMLParameters Parameters, 
	bool Run, 
	FString NodeID,
	const UInteractMLLabel* LabelType,
	void* LabelData,	//<-- the generic parameter
	bool& Running, 
	bool& Completed )
{
	//NOTE: Code should be identical to RunModelSimple above, but passing
	//NOTE:  LabelType/LabelData instead of returning Label (float)
	//NOTE: Other than that they should be kept in sync.
	
	if (!Model)
	{		
		//no model, nothing to do
		Running = false;
		Completed = false;
		return;
	}
	
	//need context and state store to run with
	UInteractMLContext* Context = GetMLContext( Actor );
	check( Context );
	TSharedPtr<FInteractMLModelState> model_state = Context->GetModelState( NodeID );
	check( model_state.IsValid() );
	
	//monitor for completion (before state checks which may want to retrigger this frame)
	bool just_completed = model_state->CheckCompleted();	//(only check once as this resets on query)
	//run complete? - new output will be available from the model state

	//series/single operation
	if(Model->IsSeries())
	{		
		//-------------- SERIES: accumulate whilst active and run once complete ---------------
		
		//check for transition
		if (model_state->RunAction.Triggered(Run, NodeID))
		{
			//change in run state
			if (Run)
			{
				//just started a run
				//reset stored series
				model_state->ParameterSeries.Clear();
			}
			else
			{
				//just stopped a run
				//TODO: Do we want to include the last parameter set sent as the run is stopped in the test set?
				FInteractMLTask::Ptr run_task = Model->RunModelAsync( &model_state->ParameterSeries );
				model_state->StartRunning(run_task);
			}
		}
		
		//accumulate
		if(Run)
		{
			//still running
			//record next parameter set
			model_state->ParameterSeries.Add( Parameters.Ptr.Get() );
		}	
	}
	else
	{
		//--------------- SINGLE: just repeatedly run against a single parameter set ---------------
		if (Run && !model_state->IsRunning())
		{
			FInteractMLTask::Ptr run_task = Model->RunModelAsync( Parameters.Ptr.Get() );
			model_state->StartRunning(run_task);
		}
	}
	
	//report status
	Running = model_state->IsRunning();
	Completed = just_completed;
	
	//report new/current/last available result
	if (model_state->CurrentResult.Num() > 0)
	{
		const FInteractMLLabelCache& label_cache = Model->GetLabelCache();
		
		//map results back to output label structure
		if (Model->IsDiscrete())
		{
			//just one, use labels as-is, just index the one needed
			if(ensure( model_state->CurrentResult.Num() == 1 )) //should only be single model output value
			{
				//find label to apply
				int label_index = (int)model_state->CurrentResult[0];
				TArray<float> specific_label;
				if(label_cache.GetLabel( label_index, specific_label ))
				{
					//rebuild struct from this specific label
					LabelType->RecreateData( specific_label, LabelData, label_cache );
				}
			}
		}
		else
		{
			//interpolated multi-value output, they map to each structure member directly
			LabelType->RecreateData( model_state->CurrentResult, LabelData, label_cache );
		}
	}
}

// model training : blocking
//
bool UInteractMLBlueprintLibrary::TrainModel( AActor* Actor, UInteractMLModel* Model, UInteractMLTrainingSet* TrainingSet, bool Train, bool Reset, FString NodeID )
{
	if(Model)
	{
		bool changed = false;

		//training state changed?
		if(Model->TrainingRequest.Triggered(Train, NodeID))
		{
			//activated/deactivated?
			if (Train)
			{
				//trigger training when Train bool transitions to true
				Model->TrainModel(TrainingSet);
				changed = true;
			}
		}
		
		//reset state changed?
		if(Model->ResetRequest.Triggered(Reset,NodeID))
		{
			//activated/deactivated?
			if (Reset)
			{
				//trigger reset when Reset transitions to true
				Model->ResetModel();

				//explicitly count this as new data
				Model->MarkUnsavedData();
				changed = true;
			}
		}

		//need to make sure context/module are aware of model use/changes during PIE
		if (changed)
		{	
			UInteractMLContext* Context = GetMLContext( Actor );
			check( Context );
			Context->SetModel(NodeID, Model);
		}

		return Model->IsTrained();
	}

	return false;
}

// model training : asynchronous
//
bool UInteractMLBlueprintLibrary::TrainModelAsync( AActor* Actor, UInteractMLModel* Model, UInteractMLTrainingSet* TrainingSet, bool Train, bool Reset, FString NodeID, bool& Training, bool& Completed )
{
	if (!Model)
	{
		//no model, nothing to do	
		Training = false;
		Completed = false;
		return false;
	}

	bool notify_context = false;
	
	//training state changed?
	if(Model->TrainingRequest.Triggered(Train, NodeID))
	{
		//activated/deactivated?
		if (Train)
		{
			//trigger start of training when Train bool transitions to true
			Model->TrainModelAsync( TrainingSet );
			notify_context = true;
		}
	}
	
	//reset state changed?
	if(Model->ResetRequest.Triggered(Reset,NodeID))
	{
		//activated/deactivated?
		if (Reset)
		{
			//trigger reset when Reset transitions to true
			Model->ResetModel();
			
			//explicitly count this as new data
			Model->MarkUnsavedData();
			notify_context = true;
		}
	}
	
	//need to make sure context/module are aware of model use/changes during PIE
	if (notify_context)
	{	
		UInteractMLContext* Context = GetMLContext( Actor );
		check( Context );
		Context->SetModel(NodeID, Model);
	}

	//current status
	Training = Model->IsTraining();
	Completed = Model->CheckJustCompleted();
	return Model->IsTrained();
}


///////////////////// UTILITY //////////////////////

//persistence
bool UInteractMLBlueprintLibrary::Save()
{
	FInteractMLModule::Get().Save();
	return true;
}

//property access, by name
float UInteractMLBlueprintLibrary::GetFloatProperty( UObject* Target, FName Name )
{
	float value = 0;
	if(Target)
	{
		FProperty* prop = Target->GetClass()->FindPropertyByName( Name );
		if(prop)
		{
			FFloatProperty* fprop = CastField<FFloatProperty>( prop );
			if(fprop)
			{
				void* ptr = prop->ContainerPtrToValuePtr<float>( Target );
				value = (float)fprop->GetFloatingPointPropertyValue( ptr );
			}
		}
	}
	return value;
}


///////////////////// DEBUG/DIAGS //////////////////////

// dump parameter collection output
//
void UInteractMLBlueprintLibrary::LogParameterCollection(FInteractMLParameters Parameters)
{
	FInteractMLParameterCollection* params = Parameters.Ptr.Get();
	if (params)
	{
		FString txt;
		for (int i = 0; i < params->Values.Num(); i++)
		{
			if (i > 0)
			{
				txt.Append(TEXT(", "));
			}
			float val = params->Values[i];
			txt.Appendf( TEXT("%.00f"), val );
		}
		UE_LOG(LogInteractML, Display, TEXT("Parameters (%i): %s"), params->Values.Num(), *txt);
	}
}


// EPILOGUE
#undef LOCTEXT_NAMESPACE

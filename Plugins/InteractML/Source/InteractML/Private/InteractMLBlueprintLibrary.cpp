//----
// InteractML - University Of Arts London
//----

#include "InteractMLBlueprintLibrary.h"

//unreal

//module
#include "InteractML.h"
#include "InteractMLContext.h"
#include "Models/InteractMLClassificationModel.h"
#include "Models/InteractMLRegressionModel.h"

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
// NOTE: we still disambiguate so we can train which nodes/how many are using these objects
//
UInteractMLTrainingSet* UInteractMLBlueprintLibrary::GetTrainingSet(AActor* Actor, FString DataPath, FString NodeID)
{
	//find the context we cache our state in
	UInteractMLContext* Context = GetMLContext( Actor );
	check(Context);
	
	//get a parameter collection for this node to use
	UInteractMLContext::TGraphNodeID id = NodeID;
	UInteractMLTrainingSet* training_set = Context->GetTrainingSet( id, DataPath );
	check(training_set);
	
	return training_set;
}

// training set recording
// when recording requested, takes snapshot of parameter data and associates it with the given label and stores it in the training set
// when finished returns true briefly to signify that new training set data is ready
// NOTE: just single snapshots at the moment
// NOTE: state about recording can be held in the training set object even though it's technically shared because it doesn't make
//       too much sense to be recording using two training nodes at the same time.
//
bool UInteractMLBlueprintLibrary::RecordExample(
	UInteractMLTrainingSet* TrainingSet, 
	FInteractMLParameters Parameters, 
	float Label,
	bool Record,
	bool Reset,
	int Mode,
	FString NodeID)
{
	check(TrainingSet);
	FInteractMLParameterCollection* parameters = Parameters.Ptr.Get();

	//recording
	bool is_finished = false;
	if(TrainingSet->RecordingAction.Triggered( Record, NodeID ))
	{
		if (Record)
		{
			//start recording
			bool ok = TrainingSet->BeginRecording(Label);
			if (ok)
			{
				//record single snapshot upon start
				ok = TrainingSet->RecordParameters(parameters);
			}
		}
		else
		{
			//stop recording
			bool ok = TrainingSet->EndRecording();

			//success?
			is_finished = ok; //briefly return true upon successful trigger
		}
	}

	//reset handling
	if (TrainingSet->ResettingAction.Triggered( Reset, NodeID ))
	{
		if (Reset)
		{
			//just activated
			TrainingSet->ResetTrainingSet();
		}
	}

	//finished recording
	return is_finished;
}

///////////////////// MODEL //////////////////////

// model access
//
UInteractMLModel* UInteractMLBlueprintLibrary::GetModel_Classification(AActor* Actor, FString DataPath, FString NodeID)
{
	//find the context we cache our state in
	UInteractMLContext* Context = GetMLContext( Actor );
	check( Context );

	//get a parameter collection for this node to use
	UInteractMLContext::TGraphNodeID id = NodeID;
	UInteractMLModel* model = Context->GetModel( UInteractMLClassificationModel::StaticClass(), id, DataPath );
	check( model );

	return model;
}
UInteractMLModel* UInteractMLBlueprintLibrary::GetModel_Regression(AActor* Actor, FString DataPath, FString NodeID)
{
	//find the context we cache our state in
	UInteractMLContext* Context = GetMLContext( Actor );
	check( Context );
	
	//get a parameter collection for this node to use
	UInteractMLContext::TGraphNodeID id = NodeID;
	UInteractMLModel* model = Context->GetModel( UInteractMLRegressionModel::StaticClass(), id, DataPath );
	check( model );
	
	return model;
}

// model running
//
float UInteractMLBlueprintLibrary::RunModel(UInteractMLModel* Model, FInteractMLParameters Parameters, bool Run, FString NodeID)
{
	if(Model)
	{
		if(Run)
		{
			return Model->RunModel( Parameters.Ptr.Get() );
		}
	}
	return 0;
}

// model training
//
bool UInteractMLBlueprintLibrary::TrainModel( UInteractMLModel* Model, UInteractMLTrainingSet* TrainingSet, bool Train, bool Reset, FString NodeID )
{
	if(Model)
	{
		//training state changed?
		if(Model->TrainingAction.Triggered(Train, NodeID))
		{
			//activated/deactivated?
			if (Train)
			{
				//trigger training when Train bool transitions to true
				Model->TrainModel(TrainingSet);
			}
		}
		
		//reset state changed?
		if(Model->ResetAction.Triggered(Reset,NodeID))
		{
			//activated/deactivated?
			if (Reset)
			{
				//trigger reset when Reset transitions to true
				Model->ResetModel();

				//explicitly count this as new data
				Model->MarkUnsavedData();
			}
		}

		return Model->IsTrained();
	}

	return false;
}



///////////////////// UTILITY //////////////////////

//persistence
bool UInteractMLBlueprintLibrary::Save()
{
	FInteractMLModule::Get().Save();
	return true;
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


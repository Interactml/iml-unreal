//----
// InteractML - University Of Arts London
//----

#include "InteractMLBlueprintLibrary.h"

//unreal

//module
#include "InteractML.h"
#include "InteractMLContext.h"
#include "Models/InteractMLClassificationModel.h"

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
	//find the context we cache our state in
	UInteractMLContext* Context = GetMLContext( Actor );

	//get a parameter collection for this node to use
	TSharedPtr<FInteractMLParameterCollection> parameters = Context->GetParameters( NodeID );
	
	//prep for (re)accumulation
	parameters->Reset();

	return FInteractMLParameters(parameters);
}

// Parameter Collection custom node: call to add a parameter
//
void UInteractMLBlueprintLibrary::AddIntegerParameter(FInteractMLParameters Parameters, int Value)
{
	Parameters.Ptr->Add(Value);
}
void UInteractMLBlueprintLibrary::AddFloatParameter( FInteractMLParameters Parameters, float Value )
{
	Parameters.Ptr->Add(Value);
}
void UInteractMLBlueprintLibrary::AddBooleanParameter( FInteractMLParameters Parameters, bool Value )
{
	Parameters.Ptr->Add(Value);
}
void UInteractMLBlueprintLibrary::AddVector2Parameter( FInteractMLParameters Parameters, FVector2D Value )
{
	Parameters.Ptr->Add(Value);
}
void UInteractMLBlueprintLibrary::AddVector3Parameter( FInteractMLParameters Parameters, FVector Value )
{
	Parameters.Ptr->Add(Value);
}
void UInteractMLBlueprintLibrary::AddQuaternionParameter( FInteractMLParameters Parameters, FQuat Value )
{
	Parameters.Ptr->Add(Value);
}
void UInteractMLBlueprintLibrary::AddColourParameter( FInteractMLParameters Parameters, FLinearColor Value )
{
	Parameters.Ptr->Add(Value);
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
	int Label, 
	bool Record,
	bool Reset,
	int Mode,
	FString NodeID)
{
	check(TrainingSet);
	FInteractMLParameterCollection* parameters = Parameters.Ptr.Get();

	//current state
	bool is_recording = TrainingSet->IsRecording();
	bool want_recording = Record;

	//recording
	bool is_finished = false;
	if(want_recording!=is_recording) //change of recording request
	{
		if (want_recording)
		{
			//start recording
			bool ok = TrainingSet->BeginRecording(Label, NodeID);
			if (ok)
			{
				//record single snapshot upon start
				ok = TrainingSet->RecordParameters(parameters,NodeID);
			}
		}
		else
		{
			//stop recording
			bool ok = TrainingSet->EndRecording(NodeID);

			//success?
			is_finished = ok; //briefly return true upon successful trigger
		}
	}

	//reset handling
	bool is_resetting = TrainingSet->IsResetting();
	bool want_reset = Reset;
	if (want_reset!=is_resetting) //change of reset state
	{
		if (want_reset)
		{
			TrainingSet->BeginReset(NodeID);
		}
		else
		{
			TrainingSet->EndReset(NodeID);
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

// model running
//
int UInteractMLBlueprintLibrary::RunModel(UInteractMLModel* Model, FInteractMLParameters Parameters, bool Run, FString NodeID)
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
void UInteractMLBlueprintLibrary::TrainModel( UInteractMLModel* Model, UInteractMLTrainingSet* TrainingSet, bool Train, bool Reset, FString NodeID )
{
	if(Model)
	{
		if(Train)
		{
			Model->TrainModel( TrainingSet );
		}
		else if(Reset)
		{
			Model->ResetModel();
		}
	}
}



///////////////////// UTILITY //////////////////////

//persistence
bool UInteractMLBlueprintLibrary::Save()
{
	//TODO: save
	check(false);
	return false;
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


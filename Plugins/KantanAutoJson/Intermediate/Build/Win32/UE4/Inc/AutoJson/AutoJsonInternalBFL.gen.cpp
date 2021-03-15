// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AutoJson/PluginPrivate/AutoJsonInternalBFL.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAutoJsonInternalBFL() {}
// Cross Module References
	AUTOJSON_API UScriptStruct* Z_Construct_UScriptStruct_FAJStubStruct();
	UPackage* Z_Construct_UPackage__Script_AutoJson();
	AUTOJSON_API UClass* Z_Construct_UClass_UKantanAutoJsonInternalBFL_NoRegister();
	AUTOJSON_API UClass* Z_Construct_UClass_UKantanAutoJsonInternalBFL();
	ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
	COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
	AUTOJSON_API UClass* Z_Construct_UClass_UAutoJsonSerializer_NoRegister();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
// End Cross Module References
class UScriptStruct* FAJStubStruct::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern AUTOJSON_API uint32 Get_Z_Construct_UScriptStruct_FAJStubStruct_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FAJStubStruct, Z_Construct_UPackage__Script_AutoJson(), TEXT("AJStubStruct"), sizeof(FAJStubStruct), Get_Z_Construct_UScriptStruct_FAJStubStruct_Hash());
	}
	return Singleton;
}
template<> AUTOJSON_API UScriptStruct* StaticStruct<FAJStubStruct>()
{
	return FAJStubStruct::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FAJStubStruct(FAJStubStruct::StaticStruct, TEXT("/Script/AutoJson"), TEXT("AJStubStruct"), false, nullptr, nullptr);
static struct FScriptStruct_AutoJson_StaticRegisterNativesFAJStubStruct
{
	FScriptStruct_AutoJson_StaticRegisterNativesFAJStubStruct()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("AJStubStruct")),new UScriptStruct::TCppStructOps<FAJStubStruct>);
	}
} ScriptStruct_AutoJson_StaticRegisterNativesFAJStubStruct;
	struct Z_Construct_UScriptStruct_FAJStubStruct_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAJStubStruct_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FAJStubStruct_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FAJStubStruct>();
	}
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FAJStubStruct_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_AutoJson,
		nullptr,
		&NewStructOps,
		"AJStubStruct",
		sizeof(FAJStubStruct),
		alignof(FAJStubStruct),
		nullptr,
		0,
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FAJStubStruct_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAJStubStruct_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FAJStubStruct()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FAJStubStruct_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_AutoJson();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("AJStubStruct"), sizeof(FAJStubStruct), Get_Z_Construct_UScriptStruct_FAJStubStruct_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FAJStubStruct_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FAJStubStruct_Hash() { return 3581179008U; }
	DEFINE_FUNCTION(UKantanAutoJsonInternalBFL::execReadJsonStringFromFile)
	{
		P_GET_PROPERTY_REF(FStrProperty,Z_Param_Out_Json);
		P_GET_PROPERTY(FStrProperty,Z_Param_Filename);
		P_GET_UBOOL_REF(Z_Param_Out_bSuccess);
		P_FINISH;
		P_NATIVE_BEGIN;
		UKantanAutoJsonInternalBFL::ReadJsonStringFromFile(Z_Param_Out_Json,Z_Param_Filename,Z_Param_Out_bSuccess);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UKantanAutoJsonInternalBFL::execWriteJsonStringToFile)
	{
		P_GET_PROPERTY(FStrProperty,Z_Param_Json);
		P_GET_PROPERTY(FStrProperty,Z_Param_Filename);
		P_GET_UBOOL_REF(Z_Param_Out_bSuccess);
		P_FINISH;
		P_NATIVE_BEGIN;
		UKantanAutoJsonInternalBFL::WriteJsonStringToFile(Z_Param_Json,Z_Param_Filename,Z_Param_Out_bSuccess);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UKantanAutoJsonInternalBFL::execDeserializeObject_SerializerClass)
	{
		P_GET_OBJECT(UObject,Z_Param_InVar);
		P_GET_PROPERTY(FStrProperty,Z_Param_Json);
		P_GET_OBJECT(UClass,Z_Param_Serializer);
		P_GET_OBJECT_REF(UObject,Z_Param_Out_OutVar);
		P_GET_UBOOL_REF(Z_Param_Out_bSuccess);
		P_FINISH;
		P_NATIVE_BEGIN;
		UKantanAutoJsonInternalBFL::DeserializeObject_SerializerClass(Z_Param_InVar,Z_Param_Json,Z_Param_Serializer,Z_Param_Out_OutVar,Z_Param_Out_bSuccess);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UKantanAutoJsonInternalBFL::execSerializeObject_SerializerClass)
	{
		P_GET_OBJECT(UObject,Z_Param_InVar);
		P_GET_OBJECT(UClass,Z_Param_Serializer);
		P_GET_PROPERTY_REF(FStrProperty,Z_Param_Out_Json);
		P_GET_UBOOL_REF(Z_Param_Out_bSuccess);
		P_FINISH;
		P_NATIVE_BEGIN;
		UKantanAutoJsonInternalBFL::SerializeObject_SerializerClass(Z_Param_InVar,Z_Param_Serializer,Z_Param_Out_Json,Z_Param_Out_bSuccess);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UKantanAutoJsonInternalBFL::execDeserializeObject)
	{
		P_GET_OBJECT(UObject,Z_Param_InVar);
		P_GET_PROPERTY(FStrProperty,Z_Param_Json);
		P_GET_OBJECT(UAutoJsonSerializer,Z_Param_Serializer);
		P_GET_OBJECT_REF(UObject,Z_Param_Out_OutVar);
		P_GET_UBOOL_REF(Z_Param_Out_bSuccess);
		P_FINISH;
		P_NATIVE_BEGIN;
		UKantanAutoJsonInternalBFL::DeserializeObject(Z_Param_InVar,Z_Param_Json,Z_Param_Serializer,Z_Param_Out_OutVar,Z_Param_Out_bSuccess);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UKantanAutoJsonInternalBFL::execSerializeObject)
	{
		P_GET_OBJECT(UObject,Z_Param_InVar);
		P_GET_OBJECT(UAutoJsonSerializer,Z_Param_Serializer);
		P_GET_PROPERTY_REF(FStrProperty,Z_Param_Out_Json);
		P_GET_UBOOL_REF(Z_Param_Out_bSuccess);
		P_FINISH;
		P_NATIVE_BEGIN;
		UKantanAutoJsonInternalBFL::SerializeObject(Z_Param_InVar,Z_Param_Serializer,Z_Param_Out_Json,Z_Param_Out_bSuccess);
		P_NATIVE_END;
	}
	void UKantanAutoJsonInternalBFL::StaticRegisterNativesUKantanAutoJsonInternalBFL()
	{
		UClass* Class = UKantanAutoJsonInternalBFL::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "DeserializeArrayProperty", &UKantanAutoJsonInternalBFL::execDeserializeArrayProperty },
			{ "DeserializeMapProperty", &UKantanAutoJsonInternalBFL::execDeserializeMapProperty },
			{ "DeserializeObject", &UKantanAutoJsonInternalBFL::execDeserializeObject },
			{ "DeserializeObject_SerializerClass", &UKantanAutoJsonInternalBFL::execDeserializeObject_SerializerClass },
			{ "DeserializeSetProperty", &UKantanAutoJsonInternalBFL::execDeserializeSetProperty },
			{ "DeserializeStruct", &UKantanAutoJsonInternalBFL::execDeserializeStruct },
			{ "ReadJsonStringFromFile", &UKantanAutoJsonInternalBFL::execReadJsonStringFromFile },
			{ "SerializeArrayProperty", &UKantanAutoJsonInternalBFL::execSerializeArrayProperty },
			{ "SerializeMapProperty", &UKantanAutoJsonInternalBFL::execSerializeMapProperty },
			{ "SerializeObject", &UKantanAutoJsonInternalBFL::execSerializeObject },
			{ "SerializeObject_SerializerClass", &UKantanAutoJsonInternalBFL::execSerializeObject_SerializerClass },
			{ "SerializeSetProperty", &UKantanAutoJsonInternalBFL::execSerializeSetProperty },
			{ "SerializeStruct", &UKantanAutoJsonInternalBFL::execSerializeStruct },
			{ "WriteJsonStringToFile", &UKantanAutoJsonInternalBFL::execWriteJsonStringToFile },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics
	{
		struct KantanAutoJsonInternalBFL_eventDeserializeArrayProperty_Parms
		{
			FString Json;
			TSubclassOf<UAutoJsonSerializer>  Serializer;
			bool bSuccess;
			TArray<int32> InVar;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_Serializer;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_InVar_Inner;
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_InVar;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_Json_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeArrayProperty_Parms, Json), METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_Json_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_Json_MetaData)) };
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_Serializer = { "Serializer", nullptr, (EPropertyFlags)0x0014000000000080, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeArrayProperty_Parms, Serializer), Z_Construct_UClass_UAutoJsonSerializer_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventDeserializeArrayProperty_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventDeserializeArrayProperty_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_InVar_Inner = { "InVar", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_InVar = { "InVar", nullptr, (EPropertyFlags)0x0010000008000180, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeArrayProperty_Parms, InVar), EArrayPropertyFlags::None, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_Serializer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_bSuccess,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_InVar_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::NewProp_InVar,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::Function_MetaDataParams[] = {
		{ "ArrayParm", "InVar" },
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "AutoJson" },
		{ "CustomThunk", "true" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "DeserializeArrayProperty", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventDeserializeArrayProperty_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics
	{
		struct KantanAutoJsonInternalBFL_eventDeserializeMapProperty_Parms
		{
			FString Json;
			TSubclassOf<UAutoJsonSerializer>  Serializer;
			bool bSuccess;
			TMap<int32,int32> InVar;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_Serializer;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_InVar_ValueProp;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_InVar_Key_KeyProp;
		static const UE4CodeGen_Private::FMapPropertyParams NewProp_InVar;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_Json_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeMapProperty_Parms, Json), METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_Json_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_Json_MetaData)) };
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_Serializer = { "Serializer", nullptr, (EPropertyFlags)0x0014000000000080, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeMapProperty_Parms, Serializer), Z_Construct_UClass_UAutoJsonSerializer_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventDeserializeMapProperty_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventDeserializeMapProperty_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_InVar_ValueProp = { "InVar", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_InVar_Key_KeyProp = { "InVar_Key", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_InVar = { "InVar", nullptr, (EPropertyFlags)0x0010000008000180, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeMapProperty_Parms, InVar), EMapPropertyFlags::None, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_Serializer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_bSuccess,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_InVar_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_InVar_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::NewProp_InVar,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::Function_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "AutoJson" },
		{ "CustomThunk", "true" },
		{ "MapParam", "InVar" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "DeserializeMapProperty", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventDeserializeMapProperty_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics
	{
		struct KantanAutoJsonInternalBFL_eventDeserializeObject_Parms
		{
			UObject* InVar;
			FString Json;
			UAutoJsonSerializer* Serializer;
			UObject* OutVar;
			bool bSuccess;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_InVar;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Serializer;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_OutVar;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_InVar = { "InVar", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeObject_Parms, InVar), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_Json_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeObject_Parms, Json), METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_Json_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_Json_MetaData)) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_Serializer = { "Serializer", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeObject_Parms, Serializer), Z_Construct_UClass_UAutoJsonSerializer_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_OutVar = { "OutVar", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeObject_Parms, OutVar), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventDeserializeObject_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventDeserializeObject_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_InVar,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_Serializer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_OutVar,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::NewProp_bSuccess,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::Function_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "AutoJson" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "DeserializeObject", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventDeserializeObject_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics
	{
		struct KantanAutoJsonInternalBFL_eventDeserializeObject_SerializerClass_Parms
		{
			UObject* InVar;
			FString Json;
			TSubclassOf<UAutoJsonSerializer>  Serializer;
			UObject* OutVar;
			bool bSuccess;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_InVar;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_Serializer;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_OutVar;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_InVar = { "InVar", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeObject_SerializerClass_Parms, InVar), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_Json_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeObject_SerializerClass_Parms, Json), METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_Json_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_Json_MetaData)) };
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_Serializer = { "Serializer", nullptr, (EPropertyFlags)0x0014000000000080, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeObject_SerializerClass_Parms, Serializer), Z_Construct_UClass_UAutoJsonSerializer_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_OutVar = { "OutVar", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeObject_SerializerClass_Parms, OutVar), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventDeserializeObject_SerializerClass_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventDeserializeObject_SerializerClass_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_InVar,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_Serializer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_OutVar,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::NewProp_bSuccess,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::Function_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "AutoJson" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "DeserializeObject_SerializerClass", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventDeserializeObject_SerializerClass_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics
	{
		struct KantanAutoJsonInternalBFL_eventDeserializeSetProperty_Parms
		{
			FString Json;
			TSubclassOf<UAutoJsonSerializer>  Serializer;
			bool bSuccess;
			TSet<int32> InVar;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_Serializer;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_InVar_ElementProp;
		static const UE4CodeGen_Private::FSetPropertyParams NewProp_InVar;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_Json_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeSetProperty_Parms, Json), METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_Json_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_Json_MetaData)) };
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_Serializer = { "Serializer", nullptr, (EPropertyFlags)0x0014000000000080, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeSetProperty_Parms, Serializer), Z_Construct_UClass_UAutoJsonSerializer_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventDeserializeSetProperty_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventDeserializeSetProperty_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_InVar_ElementProp = { "InVar", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FSetPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_InVar = { "InVar", nullptr, (EPropertyFlags)0x0010000008000180, UE4CodeGen_Private::EPropertyGenFlags::Set, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeSetProperty_Parms, InVar), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_Serializer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_bSuccess,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_InVar_ElementProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::NewProp_InVar,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::Function_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "AutoJson" },
		{ "CustomThunk", "true" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
		{ "SetParam", "InVar" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "DeserializeSetProperty", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventDeserializeSetProperty_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics
	{
		struct KantanAutoJsonInternalBFL_eventDeserializeStruct_Parms
		{
			FString Json;
			TSubclassOf<UAutoJsonSerializer>  Serializer;
			bool bSuccess;
			FAJStubStruct InVar;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_Serializer;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_InVar;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::NewProp_Json_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeStruct_Parms, Json), METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::NewProp_Json_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::NewProp_Json_MetaData)) };
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::NewProp_Serializer = { "Serializer", nullptr, (EPropertyFlags)0x0014000000000080, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeStruct_Parms, Serializer), Z_Construct_UClass_UAutoJsonSerializer_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventDeserializeStruct_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventDeserializeStruct_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::NewProp_InVar = { "InVar", nullptr, (EPropertyFlags)0x0010000008000180, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventDeserializeStruct_Parms, InVar), Z_Construct_UScriptStruct_FAJStubStruct, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::NewProp_Serializer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::NewProp_bSuccess,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::NewProp_InVar,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::Function_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "AutoJson" },
		{ "CustomStructureParam", "InVar, OutVar" },
		{ "CustomThunk", "true" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "DeserializeStruct", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventDeserializeStruct_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics
	{
		struct KantanAutoJsonInternalBFL_eventReadJsonStringFromFile_Parms
		{
			FString Json;
			FString Filename;
			bool bSuccess;
		};
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Filename_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Filename;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventReadJsonStringFromFile_Parms, Json), METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::NewProp_Filename_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::NewProp_Filename = { "Filename", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventReadJsonStringFromFile_Parms, Filename), METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::NewProp_Filename_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::NewProp_Filename_MetaData)) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventReadJsonStringFromFile_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventReadJsonStringFromFile_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::NewProp_Filename,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::NewProp_bSuccess,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::Function_MetaDataParams[] = {
		{ "Category", "AutoJson" },
		{ "Comment", "/**\n\x09* Utility function for reading a JSON string from file.\n\x09*\n\x09* @param Json The read JSON text.\n\x09* @param Filename File path to read from.\n\x09* @param bSuccess I/O success/failure flag.\n\x09*/" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
		{ "ToolTip", "Utility function for reading a JSON string from file.\n\n@param Json The read JSON text.\n@param Filename File path to read from.\n@param bSuccess I/O success/failure flag." },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "ReadJsonStringFromFile", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventReadJsonStringFromFile_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics
	{
		struct KantanAutoJsonInternalBFL_eventSerializeArrayProperty_Parms
		{
			FString Json;
			TSubclassOf<UAutoJsonSerializer>  Serializer;
			bool bSuccess;
			TArray<int32> InVar;
		};
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_Serializer;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_InVar_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_InVar_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_InVar;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeArrayProperty_Parms, Json), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_Serializer = { "Serializer", nullptr, (EPropertyFlags)0x0014000000000080, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeArrayProperty_Parms, Serializer), Z_Construct_UClass_UAutoJsonSerializer_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventSerializeArrayProperty_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventSerializeArrayProperty_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_InVar_Inner = { "InVar", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_InVar_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_InVar = { "InVar", nullptr, (EPropertyFlags)0x0010000008000182, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeArrayProperty_Parms, InVar), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_InVar_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_InVar_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_Serializer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_bSuccess,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_InVar_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::NewProp_InVar,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::Function_MetaDataParams[] = {
		{ "ArrayParm", "InVar" },
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "AutoJson" },
		{ "CustomThunk", "true" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "SerializeArrayProperty", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventSerializeArrayProperty_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics
	{
		struct KantanAutoJsonInternalBFL_eventSerializeMapProperty_Parms
		{
			FString Json;
			TSubclassOf<UAutoJsonSerializer>  Serializer;
			bool bSuccess;
			TMap<int32,int32> InVar;
		};
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_Serializer;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_InVar_ValueProp;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_InVar_Key_KeyProp;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_InVar_MetaData[];
#endif
		static const UE4CodeGen_Private::FMapPropertyParams NewProp_InVar;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeMapProperty_Parms, Json), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_Serializer = { "Serializer", nullptr, (EPropertyFlags)0x0014000000000080, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeMapProperty_Parms, Serializer), Z_Construct_UClass_UAutoJsonSerializer_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventSerializeMapProperty_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventSerializeMapProperty_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_InVar_ValueProp = { "InVar", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_InVar_Key_KeyProp = { "InVar_Key", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_InVar_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_InVar = { "InVar", nullptr, (EPropertyFlags)0x0010000008000182, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeMapProperty_Parms, InVar), EMapPropertyFlags::None, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_InVar_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_InVar_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_Serializer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_bSuccess,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_InVar_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_InVar_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::NewProp_InVar,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::Function_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "AutoJson" },
		{ "CustomThunk", "true" },
		{ "MapParam", "InVar" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "SerializeMapProperty", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventSerializeMapProperty_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics
	{
		struct KantanAutoJsonInternalBFL_eventSerializeObject_Parms
		{
			const UObject* InVar;
			UAutoJsonSerializer* Serializer;
			FString Json;
			bool bSuccess;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_InVar_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_InVar;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Serializer;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::NewProp_InVar_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::NewProp_InVar = { "InVar", nullptr, (EPropertyFlags)0x0010000000000082, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeObject_Parms, InVar), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::NewProp_InVar_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::NewProp_InVar_MetaData)) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::NewProp_Serializer = { "Serializer", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeObject_Parms, Serializer), Z_Construct_UClass_UAutoJsonSerializer_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeObject_Parms, Json), METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventSerializeObject_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventSerializeObject_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::NewProp_InVar,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::NewProp_Serializer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::NewProp_bSuccess,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::Function_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "AutoJson" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "SerializeObject", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventSerializeObject_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics
	{
		struct KantanAutoJsonInternalBFL_eventSerializeObject_SerializerClass_Parms
		{
			const UObject* InVar;
			TSubclassOf<UAutoJsonSerializer>  Serializer;
			FString Json;
			bool bSuccess;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_InVar_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_InVar;
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_Serializer;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::NewProp_InVar_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::NewProp_InVar = { "InVar", nullptr, (EPropertyFlags)0x0010000000000082, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeObject_SerializerClass_Parms, InVar), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::NewProp_InVar_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::NewProp_InVar_MetaData)) };
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::NewProp_Serializer = { "Serializer", nullptr, (EPropertyFlags)0x0014000000000080, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeObject_SerializerClass_Parms, Serializer), Z_Construct_UClass_UAutoJsonSerializer_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeObject_SerializerClass_Parms, Json), METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventSerializeObject_SerializerClass_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventSerializeObject_SerializerClass_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::NewProp_InVar,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::NewProp_Serializer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::NewProp_bSuccess,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::Function_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "AutoJson" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "SerializeObject_SerializerClass", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventSerializeObject_SerializerClass_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics
	{
		struct KantanAutoJsonInternalBFL_eventSerializeSetProperty_Parms
		{
			FString Json;
			TSubclassOf<UAutoJsonSerializer>  Serializer;
			bool bSuccess;
			TSet<int32> InVar;
		};
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_Serializer;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_InVar_ElementProp;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_InVar_MetaData[];
#endif
		static const UE4CodeGen_Private::FSetPropertyParams NewProp_InVar;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeSetProperty_Parms, Json), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_Serializer = { "Serializer", nullptr, (EPropertyFlags)0x0014000000000080, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeSetProperty_Parms, Serializer), Z_Construct_UClass_UAutoJsonSerializer_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventSerializeSetProperty_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventSerializeSetProperty_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_InVar_ElementProp = { "InVar", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_InVar_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FSetPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_InVar = { "InVar", nullptr, (EPropertyFlags)0x0010000008000182, UE4CodeGen_Private::EPropertyGenFlags::Set, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeSetProperty_Parms, InVar), METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_InVar_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_InVar_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_Serializer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_bSuccess,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_InVar_ElementProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::NewProp_InVar,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::Function_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "AutoJson" },
		{ "CustomThunk", "true" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
		{ "SetParam", "InVar" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "SerializeSetProperty", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventSerializeSetProperty_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics
	{
		struct KantanAutoJsonInternalBFL_eventSerializeStruct_Parms
		{
			FString Json;
			TSubclassOf<UAutoJsonSerializer>  Serializer;
			bool bSuccess;
			FAJStubStruct InVar;
		};
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_Serializer;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_InVar_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_InVar;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeStruct_Parms, Json), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::NewProp_Serializer = { "Serializer", nullptr, (EPropertyFlags)0x0014000000000080, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeStruct_Parms, Serializer), Z_Construct_UClass_UAutoJsonSerializer_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventSerializeStruct_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventSerializeStruct_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::NewProp_InVar_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::NewProp_InVar = { "InVar", nullptr, (EPropertyFlags)0x0010000008000182, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventSerializeStruct_Parms, InVar), Z_Construct_UScriptStruct_FAJStubStruct, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::NewProp_InVar_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::NewProp_InVar_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::NewProp_Serializer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::NewProp_bSuccess,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::NewProp_InVar,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::Function_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "AutoJson" },
		{ "CustomStructureParam", "InVar" },
		{ "CustomThunk", "true" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "SerializeStruct", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventSerializeStruct_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics
	{
		struct KantanAutoJsonInternalBFL_eventWriteJsonStringToFile_Parms
		{
			FString Json;
			FString Filename;
			bool bSuccess;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Json;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Filename_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Filename;
		static void NewProp_bSuccess_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_Json_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventWriteJsonStringToFile_Parms, Json), METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_Json_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_Json_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_Filename_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_Filename = { "Filename", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(KantanAutoJsonInternalBFL_eventWriteJsonStringToFile_Parms, Filename), METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_Filename_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_Filename_MetaData)) };
	void Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_bSuccess_SetBit(void* Obj)
	{
		((KantanAutoJsonInternalBFL_eventWriteJsonStringToFile_Parms*)Obj)->bSuccess = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(KantanAutoJsonInternalBFL_eventWriteJsonStringToFile_Parms), &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_Json,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_Filename,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::NewProp_bSuccess,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::Function_MetaDataParams[] = {
		{ "Category", "AutoJson" },
		{ "Comment", "/**\n\x09 * Utility function for writing a JSON string to file.\n\x09 * \n\x09 * @param Json The JSON text to write to file.\n\x09 * @param Filename File path to write to.\n\x09 * @param bSuccess I/O success/failure flag.\n\x09 */" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
		{ "ToolTip", "Utility function for writing a JSON string to file.\n\n@param Json The JSON text to write to file.\n@param Filename File path to write to.\n@param bSuccess I/O success/failure flag." },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UKantanAutoJsonInternalBFL, nullptr, "WriteJsonStringToFile", nullptr, nullptr, sizeof(KantanAutoJsonInternalBFL_eventWriteJsonStringToFile_Parms), Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UKantanAutoJsonInternalBFL_NoRegister()
	{
		return UKantanAutoJsonInternalBFL::StaticClass();
	}
	struct Z_Construct_UClass_UKantanAutoJsonInternalBFL_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UKantanAutoJsonInternalBFL_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
		(UObject* (*)())Z_Construct_UPackage__Script_AutoJson,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UKantanAutoJsonInternalBFL_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeArrayProperty, "DeserializeArrayProperty" }, // 1812277757
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeMapProperty, "DeserializeMapProperty" }, // 4035604637
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject, "DeserializeObject" }, // 2283747713
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeObject_SerializerClass, "DeserializeObject_SerializerClass" }, // 3391892950
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeSetProperty, "DeserializeSetProperty" }, // 1299903734
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_DeserializeStruct, "DeserializeStruct" }, // 3430543486
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_ReadJsonStringFromFile, "ReadJsonStringFromFile" }, // 1632414768
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeArrayProperty, "SerializeArrayProperty" }, // 1261128816
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeMapProperty, "SerializeMapProperty" }, // 2356961391
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject, "SerializeObject" }, // 2689328726
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeObject_SerializerClass, "SerializeObject_SerializerClass" }, // 2489237437
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeSetProperty, "SerializeSetProperty" }, // 3283846990
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_SerializeStruct, "SerializeStruct" }, // 1263476379
		{ &Z_Construct_UFunction_UKantanAutoJsonInternalBFL_WriteJsonStringToFile, "WriteJsonStringToFile" }, // 2218916564
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UKantanAutoJsonInternalBFL_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * Class used by the custom nodes to invoke native code functionality.\n * Exported purely for use by the AutoJsonCustomNodes module.\n */" },
		{ "IncludePath", "PluginPrivate/AutoJsonInternalBFL.h" },
		{ "ModuleRelativePath", "PluginPrivate/AutoJsonInternalBFL.h" },
		{ "ToolTip", "Class used by the custom nodes to invoke native code functionality.\nExported purely for use by the AutoJsonCustomNodes module." },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UKantanAutoJsonInternalBFL_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UKantanAutoJsonInternalBFL>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UKantanAutoJsonInternalBFL_Statics::ClassParams = {
		&UKantanAutoJsonInternalBFL::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UKantanAutoJsonInternalBFL_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UKantanAutoJsonInternalBFL_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UKantanAutoJsonInternalBFL()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UKantanAutoJsonInternalBFL_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UKantanAutoJsonInternalBFL, 2497761799);
	template<> AUTOJSON_API UClass* StaticClass<UKantanAutoJsonInternalBFL>()
	{
		return UKantanAutoJsonInternalBFL::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UKantanAutoJsonInternalBFL(Z_Construct_UClass_UKantanAutoJsonInternalBFL, &UKantanAutoJsonInternalBFL::StaticClass, TEXT("/Script/AutoJson"), TEXT("UKantanAutoJsonInternalBFL"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UKantanAutoJsonInternalBFL);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif

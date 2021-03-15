// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AutoJsonCustomNodes/Private/BPNode_AutoJsonSerialize.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBPNode_AutoJsonSerialize() {}
// Cross Module References
	AUTOJSONCUSTOMNODES_API UClass* Z_Construct_UClass_UBPNode_AutoJsonSerialize_NoRegister();
	AUTOJSONCUSTOMNODES_API UClass* Z_Construct_UClass_UBPNode_AutoJsonSerialize();
	AUTOJSONCUSTOMNODES_API UClass* Z_Construct_UClass_UBPNode_AutoJsonBase();
	UPackage* Z_Construct_UPackage__Script_AutoJsonCustomNodes();
// End Cross Module References
	void UBPNode_AutoJsonSerialize::StaticRegisterNativesUBPNode_AutoJsonSerialize()
	{
	}
	UClass* Z_Construct_UClass_UBPNode_AutoJsonSerialize_NoRegister()
	{
		return UBPNode_AutoJsonSerialize::StaticClass();
	}
	struct Z_Construct_UClass_UBPNode_AutoJsonSerialize_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UBPNode_AutoJsonSerialize_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UBPNode_AutoJsonBase,
		(UObject* (*)())Z_Construct_UPackage__Script_AutoJsonCustomNodes,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UBPNode_AutoJsonSerialize_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "BPNode_AutoJsonSerialize.h" },
		{ "ModuleRelativePath", "Private/BPNode_AutoJsonSerialize.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UBPNode_AutoJsonSerialize_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBPNode_AutoJsonSerialize>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UBPNode_AutoJsonSerialize_Statics::ClassParams = {
		&UBPNode_AutoJsonSerialize::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UBPNode_AutoJsonSerialize_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UBPNode_AutoJsonSerialize_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UBPNode_AutoJsonSerialize()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UBPNode_AutoJsonSerialize_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UBPNode_AutoJsonSerialize, 1142192601);
	template<> AUTOJSONCUSTOMNODES_API UClass* StaticClass<UBPNode_AutoJsonSerialize>()
	{
		return UBPNode_AutoJsonSerialize::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UBPNode_AutoJsonSerialize(Z_Construct_UClass_UBPNode_AutoJsonSerialize, &UBPNode_AutoJsonSerialize::StaticClass, TEXT("/Script/AutoJsonCustomNodes"), TEXT("UBPNode_AutoJsonSerialize"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UBPNode_AutoJsonSerialize);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif

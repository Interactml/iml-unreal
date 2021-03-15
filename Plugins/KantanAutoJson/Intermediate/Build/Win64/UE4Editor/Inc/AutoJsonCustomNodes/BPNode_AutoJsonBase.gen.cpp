// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AutoJsonCustomNodes/Private/BPNode_AutoJsonBase.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBPNode_AutoJsonBase() {}
// Cross Module References
	AUTOJSONCUSTOMNODES_API UClass* Z_Construct_UClass_UBPNode_AutoJsonBase_NoRegister();
	AUTOJSONCUSTOMNODES_API UClass* Z_Construct_UClass_UBPNode_AutoJsonBase();
	BLUEPRINTGRAPH_API UClass* Z_Construct_UClass_UK2Node();
	UPackage* Z_Construct_UPackage__Script_AutoJsonCustomNodes();
	COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
	AUTOJSON_API UClass* Z_Construct_UClass_UAutoJsonSerializer_NoRegister();
// End Cross Module References
	void UBPNode_AutoJsonBase::StaticRegisterNativesUBPNode_AutoJsonBase()
	{
	}
	UClass* Z_Construct_UClass_UBPNode_AutoJsonBase_NoRegister()
	{
		return UBPNode_AutoJsonBase::StaticClass();
	}
	struct Z_Construct_UClass_UBPNode_AutoJsonBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Serializer_MetaData[];
#endif
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_Serializer;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UK2Node,
		(UObject* (*)())Z_Construct_UPackage__Script_AutoJsonCustomNodes,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "BPNode_AutoJsonBase.h" },
		{ "ModuleRelativePath", "Private/BPNode_AutoJsonBase.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::NewProp_Serializer_MetaData[] = {
		{ "AllowAbstract", "false" },
		{ "Category", "Configuration" },
		{ "Comment", "// @TODO: Ideally we have an instanced object of this type, with properties editable in the node details.\n// Not sure how we can have this embedded into the compiled node though (AddComponent nodes rely on dedicated property on the blueprint).\n// If doable, probably involves at least overriding CreateNodeHandler.\n// Hacky alternative may be to create a hidden pin with default value for each property iterated on this serializer class.\n" },
		{ "ModuleRelativePath", "Private/BPNode_AutoJsonBase.h" },
		{ "ToolTip", "@TODO: Ideally we have an instanced object of this type, with properties editable in the node details.\nNot sure how we can have this embedded into the compiled node though (AddComponent nodes rely on dedicated property on the blueprint).\nIf doable, probably involves at least overriding CreateNodeHandler.\nHacky alternative may be to create a hidden pin with default value for each property iterated on this serializer class." },
	};
#endif
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::NewProp_Serializer = { "Serializer", nullptr, (EPropertyFlags)0x0024080002000001, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UBPNode_AutoJsonBase, Serializer), Z_Construct_UClass_UAutoJsonSerializer_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::NewProp_Serializer_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::NewProp_Serializer_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::NewProp_Serializer,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBPNode_AutoJsonBase>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::ClassParams = {
		&UBPNode_AutoJsonBase::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::PropPointers),
		0,
		0x000000A1u,
		METADATA_PARAMS(Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UBPNode_AutoJsonBase()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UBPNode_AutoJsonBase_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UBPNode_AutoJsonBase, 3091729874);
	template<> AUTOJSONCUSTOMNODES_API UClass* StaticClass<UBPNode_AutoJsonBase>()
	{
		return UBPNode_AutoJsonBase::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UBPNode_AutoJsonBase(Z_Construct_UClass_UBPNode_AutoJsonBase, &UBPNode_AutoJsonBase::StaticClass, TEXT("/Script/AutoJsonCustomNodes"), TEXT("UBPNode_AutoJsonBase"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UBPNode_AutoJsonBase);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif

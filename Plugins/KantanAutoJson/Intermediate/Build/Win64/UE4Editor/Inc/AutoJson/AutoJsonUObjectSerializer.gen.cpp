// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AutoJson/Public/UObjectSerializers/AutoJsonUObjectSerializer.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAutoJsonUObjectSerializer() {}
// Cross Module References
	AUTOJSON_API UClass* Z_Construct_UClass_UAutoJsonSerializer_NoRegister();
	AUTOJSON_API UClass* Z_Construct_UClass_UAutoJsonSerializer();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_AutoJson();
// End Cross Module References
	void UAutoJsonSerializer::StaticRegisterNativesUAutoJsonSerializer()
	{
	}
	UClass* Z_Construct_UClass_UAutoJsonSerializer_NoRegister()
	{
		return UAutoJsonSerializer::StaticClass();
	}
	struct Z_Construct_UClass_UAutoJsonSerializer_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UAutoJsonSerializer_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_AutoJson,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UAutoJsonSerializer_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/*\n * Base UObject version of an auto-serializer, for editor/blueprint exposure.\n */" },
		{ "DontUseGenericSpawnObject", "" },
		{ "IncludePath", "UObjectSerializers/AutoJsonUObjectSerializer.h" },
		{ "ModuleRelativePath", "Public/UObjectSerializers/AutoJsonUObjectSerializer.h" },
		{ "ToolTip", "* Base UObject version of an auto-serializer, for editor/blueprint exposure." },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UAutoJsonSerializer_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UAutoJsonSerializer>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UAutoJsonSerializer_Statics::ClassParams = {
		&UAutoJsonSerializer::StaticClass,
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
		0x041010A1u,
		METADATA_PARAMS(Z_Construct_UClass_UAutoJsonSerializer_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UAutoJsonSerializer_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UAutoJsonSerializer()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UAutoJsonSerializer_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UAutoJsonSerializer, 1975203207);
	template<> AUTOJSON_API UClass* StaticClass<UAutoJsonSerializer>()
	{
		return UAutoJsonSerializer::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UAutoJsonSerializer(Z_Construct_UClass_UAutoJsonSerializer, &UAutoJsonSerializer::StaticClass, TEXT("/Script/AutoJson"), TEXT("UAutoJsonSerializer"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UAutoJsonSerializer);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif

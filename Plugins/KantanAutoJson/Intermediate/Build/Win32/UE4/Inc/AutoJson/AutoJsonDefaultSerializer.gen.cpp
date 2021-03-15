// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AutoJson/Public/UObjectSerializers/AutoJsonDefaultSerializer.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAutoJsonDefaultSerializer() {}
// Cross Module References
	AUTOJSON_API UClass* Z_Construct_UClass_UAutoJsonDefaultSerializer_NoRegister();
	AUTOJSON_API UClass* Z_Construct_UClass_UAutoJsonDefaultSerializer();
	AUTOJSON_API UClass* Z_Construct_UClass_UAutoJsonSerializer();
	UPackage* Z_Construct_UPackage__Script_AutoJson();
// End Cross Module References
	void UAutoJsonDefaultSerializer::StaticRegisterNativesUAutoJsonDefaultSerializer()
	{
	}
	UClass* Z_Construct_UClass_UAutoJsonDefaultSerializer_NoRegister()
	{
		return UAutoJsonDefaultSerializer::StaticClass();
	}
	struct Z_Construct_UClass_UAutoJsonDefaultSerializer_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UAutoJsonDefaultSerializer_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UAutoJsonSerializer,
		(UObject* (*)())Z_Construct_UPackage__Script_AutoJson,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UAutoJsonDefaultSerializer_Statics::Class_MetaDataParams[] = {
		{ "DisplayName", "Default Serializer" },
		{ "IncludePath", "UObjectSerializers/AutoJsonDefaultSerializer.h" },
		{ "ModuleRelativePath", "Public/UObjectSerializers/AutoJsonDefaultSerializer.h" },
		{ "ShortToolTip", "Standard serialization" },
		{ "ToolTip", "The default serializer serializes all compatible properties that are not marked as transient." },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UAutoJsonDefaultSerializer_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UAutoJsonDefaultSerializer>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UAutoJsonDefaultSerializer_Statics::ClassParams = {
		&UAutoJsonDefaultSerializer::StaticClass,
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
		0x001010A0u,
		METADATA_PARAMS(Z_Construct_UClass_UAutoJsonDefaultSerializer_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UAutoJsonDefaultSerializer_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UAutoJsonDefaultSerializer()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UAutoJsonDefaultSerializer_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UAutoJsonDefaultSerializer, 3844679397);
	template<> AUTOJSON_API UClass* StaticClass<UAutoJsonDefaultSerializer>()
	{
		return UAutoJsonDefaultSerializer::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UAutoJsonDefaultSerializer(Z_Construct_UClass_UAutoJsonDefaultSerializer, &UAutoJsonDefaultSerializer::StaticClass, TEXT("/Script/AutoJson"), TEXT("UAutoJsonDefaultSerializer"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UAutoJsonDefaultSerializer);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif

// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AutoJson/Public/UObjectSerializers/AutoJsonSaveGameSerializer.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAutoJsonSaveGameSerializer() {}
// Cross Module References
	AUTOJSON_API UClass* Z_Construct_UClass_UAutoJsonSaveGameSerializer_NoRegister();
	AUTOJSON_API UClass* Z_Construct_UClass_UAutoJsonSaveGameSerializer();
	AUTOJSON_API UClass* Z_Construct_UClass_UAutoJsonDefaultSerializer();
	UPackage* Z_Construct_UPackage__Script_AutoJson();
// End Cross Module References
	void UAutoJsonSaveGameSerializer::StaticRegisterNativesUAutoJsonSaveGameSerializer()
	{
	}
	UClass* Z_Construct_UClass_UAutoJsonSaveGameSerializer_NoRegister()
	{
		return UAutoJsonSaveGameSerializer::StaticClass();
	}
	struct Z_Construct_UClass_UAutoJsonSaveGameSerializer_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UAutoJsonSaveGameSerializer_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UAutoJsonDefaultSerializer,
		(UObject* (*)())Z_Construct_UPackage__Script_AutoJson,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UAutoJsonSaveGameSerializer_Statics::Class_MetaDataParams[] = {
		{ "DisplayName", "SaveGame Serializer" },
		{ "IncludePath", "UObjectSerializers/AutoJsonSaveGameSerializer.h" },
		{ "ModuleRelativePath", "Public/UObjectSerializers/AutoJsonSaveGameSerializer.h" },
		{ "ShortToolTip", "SaveGame-only serialization" },
		{ "ToolTip", "The SaveGame serializer, despite the name, is not really intended to be used for game save data. It makes use of the fact that the SaveGame property flag is exposed to blueprints, and serializes only properties that have this flag. Use this if you want to restrict serialization to specific marked properties." },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UAutoJsonSaveGameSerializer_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UAutoJsonSaveGameSerializer>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UAutoJsonSaveGameSerializer_Statics::ClassParams = {
		&UAutoJsonSaveGameSerializer::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UAutoJsonSaveGameSerializer_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UAutoJsonSaveGameSerializer_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UAutoJsonSaveGameSerializer()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UAutoJsonSaveGameSerializer_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UAutoJsonSaveGameSerializer, 3886165516);
	template<> AUTOJSON_API UClass* StaticClass<UAutoJsonSaveGameSerializer>()
	{
		return UAutoJsonSaveGameSerializer::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UAutoJsonSaveGameSerializer(Z_Construct_UClass_UAutoJsonSaveGameSerializer, &UAutoJsonSaveGameSerializer::StaticClass, TEXT("/Script/AutoJson"), TEXT("UAutoJsonSaveGameSerializer"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UAutoJsonSaveGameSerializer);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif

// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaRestPluginPrivatePCH.h"

typedef TJsonWriterFactory< TCHAR, TCondensedJsonPrintPolicy<TCHAR> > FCondensedJsonStringWriterFactory;
typedef TJsonWriter< TCHAR, TCondensedJsonPrintPolicy<TCHAR> > FCondensedJsonStringWriter;

UVaRestJsonObject::UVaRestJsonObject(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	Reset();
}

UVaRestJsonObject* UVaRestJsonObject::ConstructJsonObject(UObject* WorldContextObject)
{
	return NewObject<UVaRestJsonObject>();
}

void UVaRestJsonObject::Reset()
{
	if (JsonObj.IsValid())
	{
		JsonObj.Reset();
	}

	JsonObj = MakeShareable(new FJsonObject());
}

TSharedPtr<FJsonObject>& UVaRestJsonObject::GetRootObject()
{
	return JsonObj;
}

void UVaRestJsonObject::SetRootObject(TSharedPtr<FJsonObject>& JsonObject)
{
	JsonObj = JsonObject;
}


//////////////////////////////////////////////////////////////////////////
// Serialization

FString UVaRestJsonObject::EncodeJson() const
{
	if (!JsonObj.IsValid())
	{
		return TEXT("");
	}

	FString OutputString;
	TSharedRef< FCondensedJsonStringWriter > Writer = FCondensedJsonStringWriterFactory::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);

	return OutputString;
}

FString UVaRestJsonObject::EncodeJsonToSingleString() const
{
	FString OutputString = EncodeJson();

	// Remove line terminators
	OutputString.Replace(LINE_TERMINATOR, TEXT(""));
	
	// Remove tabs
	OutputString.Replace(LINE_TERMINATOR, TEXT("\t"));

	return OutputString;
}

bool UVaRestJsonObject::DecodeJson(const FString& JsonString)
{
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(*JsonString);
	if (FJsonSerializer::Deserialize(Reader, JsonObj) && JsonObj.IsValid())
	{
		return true;
	}

	// If we've failed to deserialize the string, we should clear our internal data
	Reset();

	UE_LOG(LogVaRest, Error, TEXT("Json decoding failed for: %s"), *JsonString);

	return false;
}


//////////////////////////////////////////////////////////////////////////
// FJsonObject API

TArray<FString> UVaRestJsonObject::GetFieldNames()
{
	TArray<FString> Result;
	
	if (!JsonObj.IsValid())
	{
		return Result;
	}
	
	JsonObj->Values.GetKeys(Result);
	
	return Result;
}

bool UVaRestJsonObject::HasField(const FString& FieldName) const
{
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return false;
	}

	return JsonObj->HasField(FieldName);
}

void UVaRestJsonObject::RemoveField(const FString& FieldName)
{
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return;
	}

	JsonObj->RemoveField(FieldName);
}

UVaRestJsonValue* UVaRestJsonObject::GetField(const FString& FieldName) const
{
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return nullptr;
	}

	TSharedPtr<FJsonValue> NewVal = JsonObj->TryGetField(FieldName);
	if (NewVal.IsValid())
	{
		UVaRestJsonValue* NewValue = NewObject<UVaRestJsonValue>();
		NewValue->SetRootValue(NewVal);

		return NewValue;
	}
	
	return nullptr;
}

void UVaRestJsonObject::SetField(const FString& FieldName, UVaRestJsonValue* JsonValue)
{
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return;
	}

	JsonObj->SetField(FieldName, JsonValue->GetRootValue());
}


//////////////////////////////////////////////////////////////////////////
// FJsonObject API Helpers (easy to use with simple Json objects)

float UVaRestJsonObject::GetNumberField(const FString& FieldName) const
{
	if (!JsonObj.IsValid() || !JsonObj->HasTypedField<EJson::Number>(FieldName))
	{
		UE_LOG(LogVaRest, Warning, TEXT("No field with name %s of type Number"), *FieldName);
		return 0.0f;
	}

	return JsonObj->GetNumberField(FieldName);
}

void UVaRestJsonObject::SetNumberField(const FString& FieldName, float Number)
{
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return;
	}

	JsonObj->SetNumberField(FieldName, Number);
}

FString UVaRestJsonObject::GetStringField(const FString& FieldName) const
{
	if (!JsonObj.IsValid() || !JsonObj->HasTypedField<EJson::String>(FieldName))
	{
		UE_LOG(LogVaRest, Warning, TEXT("No field with name %s of type String"), *FieldName);
		return TEXT("");
	}
		
	return JsonObj->GetStringField(FieldName);
}

void UVaRestJsonObject::SetStringField(const FString& FieldName, const FString& StringValue)
{
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return;
	}

	JsonObj->SetStringField(FieldName, StringValue);
}

bool UVaRestJsonObject::GetBoolField(const FString& FieldName) const
{
	if (!JsonObj.IsValid() || !JsonObj->HasTypedField<EJson::Boolean>(FieldName))
	{
		UE_LOG(LogVaRest, Warning, TEXT("No field with name %s of type Boolean"), *FieldName);
		return false;
	}

	return JsonObj->GetBoolField(FieldName);
}

void UVaRestJsonObject::SetBoolField(const FString& FieldName, bool InValue)
{
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return;
	}

	JsonObj->SetBoolField(FieldName, InValue);
}

TArray<UVaRestJsonValue*> UVaRestJsonObject::GetArrayField(const FString& FieldName)
{
	if (!JsonObj->HasTypedField<EJson::Array>(FieldName))
	{
		UE_LOG(LogVaRest, Warning, TEXT("No field with name %s of type Array"), *FieldName);
	}

	TArray<UVaRestJsonValue*> OutArray;
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return OutArray;
	}

	TArray< TSharedPtr<FJsonValue> > ValArray = JsonObj->GetArrayField(FieldName);
	for (auto Value : ValArray)
	{
		UVaRestJsonValue* NewValue = NewObject<UVaRestJsonValue>();
		NewValue->SetRootValue(Value);

		OutArray.Add(NewValue);
	}

	return OutArray;
}

void UVaRestJsonObject::SetArrayField(const FString& FieldName, const TArray<UVaRestJsonValue*>& InArray)
{
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return;
	}

	TArray< TSharedPtr<FJsonValue> > ValArray;

	// Process input array and COPY original values
	for (auto InVal : InArray)
	{
		TSharedPtr<FJsonValue> JsonVal = InVal->GetRootValue();

		switch (InVal->GetType())
		{
		case EVaJson::None:
			break;

		case EVaJson::Null:
			ValArray.Add(MakeShareable(new FJsonValueNull()));
			break;

		case EVaJson::String:
			ValArray.Add(MakeShareable(new FJsonValueString(JsonVal->AsString())));
			break;

		case EVaJson::Number:
			ValArray.Add(MakeShareable(new FJsonValueNumber(JsonVal->AsNumber())));
			break;

		case EVaJson::Boolean:
			ValArray.Add(MakeShareable(new FJsonValueBoolean(JsonVal->AsBool())));
			break;

		case EVaJson::Array:
			ValArray.Add(MakeShareable(new FJsonValueArray(JsonVal->AsArray())));
			break;

		case EVaJson::Object:
			ValArray.Add(MakeShareable(new FJsonValueObject(JsonVal->AsObject())));
			break;

		default:
			break;
		}
	}

	JsonObj->SetArrayField(FieldName, ValArray);
}

void UVaRestJsonObject::MergeJsonObject(UVaRestJsonObject* InJsonObject, bool Overwrite)
{
	TArray<FString> Keys = InJsonObject->GetFieldNames();
	
	for (auto Key : Keys)
	{
		if (Overwrite == false && HasField(Key))
		{
			continue;
		}
		
		SetField(Key, InJsonObject->GetField(Key));
	}
}

UVaRestJsonObject* UVaRestJsonObject::GetObjectField(const FString& FieldName) const
{
	if (!JsonObj.IsValid() || !JsonObj->HasTypedField<EJson::Object>(FieldName))
	{
		UE_LOG(LogVaRest, Warning, TEXT("No field with name %s of type Object"), *FieldName);
		return nullptr;
	}

	TSharedPtr<FJsonObject> JsonObjField = JsonObj->GetObjectField(FieldName);

	UVaRestJsonObject* OutRestJsonObj = NewObject<UVaRestJsonObject>();
	OutRestJsonObj->SetRootObject(JsonObjField);

	return OutRestJsonObj;
}

void UVaRestJsonObject::SetObjectField(const FString& FieldName, UVaRestJsonObject* JsonObject)
{
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return;
	}

	JsonObj->SetObjectField(FieldName, JsonObject->GetRootObject());
}


//////////////////////////////////////////////////////////////////////////
// Array fields helpers (uniform arrays)

TArray<float> UVaRestJsonObject::GetNumberArrayField(const FString& FieldName)
{
	if (!JsonObj->HasTypedField<EJson::Array>(FieldName))
	{
		UE_LOG(LogVaRest, Warning, TEXT("No field with name %s of type Array"), *FieldName);
	}

	TArray<float> NumberArray;
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return NumberArray;
	}

	TArray<TSharedPtr<FJsonValue> > JsonArrayValues = JsonObj->GetArrayField(FieldName);
	for (TArray<TSharedPtr<FJsonValue> >::TConstIterator It(JsonArrayValues); It; ++It)
	{
		auto Value = (*It).Get();
		if (Value->Type != EJson::Number)
		{
			UE_LOG(LogVaRest, Error, TEXT("Not Number element in array with field name %s"), *FieldName);
		}
		
		NumberArray.Add((*It)->AsNumber());
	}

	return NumberArray;
}

void UVaRestJsonObject::SetNumberArrayField(const FString& FieldName, const TArray<float>& NumberArray)
{
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return;
	}

	TArray< TSharedPtr<FJsonValue> > EntriesArray;

	for (auto Number : NumberArray)
	{
		EntriesArray.Add(MakeShareable(new FJsonValueNumber(Number)));
	}

	JsonObj->SetArrayField(FieldName, EntriesArray);
}

TArray<FString> UVaRestJsonObject::GetStringArrayField(const FString& FieldName)
{
	if (!JsonObj->HasTypedField<EJson::Array>(FieldName))
	{
		UE_LOG(LogVaRest, Warning, TEXT("No field with name %s of type Array"), *FieldName);
	}

	TArray<FString> StringArray;
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return StringArray;
	}

	TArray<TSharedPtr<FJsonValue> > JsonArrayValues = JsonObj->GetArrayField(FieldName);
	for (TArray<TSharedPtr<FJsonValue> >::TConstIterator It(JsonArrayValues); It; ++It)
	{
		auto Value = (*It).Get();
		if (Value->Type != EJson::String)
		{
			UE_LOG(LogVaRest, Error, TEXT("Not String element in array with field name %s"), *FieldName);
		}

		StringArray.Add((*It)->AsString());
	}

	return StringArray;
}

void UVaRestJsonObject::SetStringArrayField(const FString& FieldName, const TArray<FString>& StringArray)
{
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return;
	}

	TArray< TSharedPtr<FJsonValue> > EntriesArray;
	for (auto String : StringArray)
	{
		EntriesArray.Add(MakeShareable(new FJsonValueString(String)));
	}

	JsonObj->SetArrayField(FieldName, EntriesArray);
}

TArray<bool> UVaRestJsonObject::GetBoolArrayField(const FString& FieldName)
{
	if (!JsonObj->HasTypedField<EJson::Array>(FieldName))
	{
		UE_LOG(LogVaRest, Warning, TEXT("No field with name %s of type Array"), *FieldName);
	}

	TArray<bool> BoolArray;
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return BoolArray;
	}

	TArray<TSharedPtr<FJsonValue> > JsonArrayValues = JsonObj->GetArrayField(FieldName);
	for (TArray<TSharedPtr<FJsonValue> >::TConstIterator It(JsonArrayValues); It; ++It)
	{
		auto Value = (*It).Get();
		if (Value->Type != EJson::Boolean)
		{
			UE_LOG(LogVaRest, Error, TEXT("Not Boolean element in array with field name %s"), *FieldName);
		}

		BoolArray.Add((*It)->AsBool());
	}

	return BoolArray;
}

void UVaRestJsonObject::SetBoolArrayField(const FString& FieldName, const TArray<bool>& BoolArray)
{
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return;
	}

	TArray< TSharedPtr<FJsonValue> > EntriesArray;
	for (auto Boolean : BoolArray)
	{
		EntriesArray.Add(MakeShareable(new FJsonValueBoolean(Boolean)));
	}

	JsonObj->SetArrayField(FieldName, EntriesArray);
}

TArray<UVaRestJsonObject*> UVaRestJsonObject::GetObjectArrayField(const FString& FieldName)
{
	if (!JsonObj->HasTypedField<EJson::Array>(FieldName))
	{
		UE_LOG(LogVaRest, Warning, TEXT("No field with name %s of type Array"), *FieldName);
	}

	TArray<UVaRestJsonObject*> OutArray;
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return OutArray;
	}

	TArray< TSharedPtr<FJsonValue> > ValArray = JsonObj->GetArrayField(FieldName);
	for (auto Value : ValArray)
	{
		if (Value->Type != EJson::Object)
		{
			UE_LOG(LogVaRest, Error, TEXT("Not Object element in array with field name %s"), *FieldName);
		}

		TSharedPtr<FJsonObject> NewObj = Value->AsObject();

		UVaRestJsonObject* NewJson = NewObject<UVaRestJsonObject>();
		NewJson->SetRootObject(NewObj);

		OutArray.Add(NewJson);
	}

	return OutArray;
}

void UVaRestJsonObject::SetObjectArrayField(const FString& FieldName, const TArray<UVaRestJsonObject*>& ObjectArray)
{
	if (!JsonObj.IsValid() || FieldName.IsEmpty())
	{
		return;
	}

	TArray< TSharedPtr<FJsonValue> > EntriesArray;
	for (auto Value : ObjectArray)
	{
		EntriesArray.Add(MakeShareable(new FJsonValueObject(Value->GetRootObject())));
	}

	JsonObj->SetArrayField(FieldName, EntriesArray);
}

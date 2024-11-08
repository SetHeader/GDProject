#pragma once

#include "CoreMinimal.h"

struct FReflectTool
{
	/**
	 * 通过类和属性名来获取属性类型
	 * @param Class 类
	 * @param PropertyName 属性名
	 * @param Fuzzy 是否要模糊匹配
	 * @return 属性类型的指针，没找到返回nullptr
	 */
	static FProperty* GetClassProperty(const UClass* Class, const FString& PropertyName, const bool Fuzzy = false)
	{
		if (!Fuzzy)
		{
			return Class->FindPropertyByName(FName(PropertyName));
		} 
		
		for (TFieldIterator<FProperty> P(Class); P; ++P)
		{
			FProperty* Property = *P;
			if (Property && Property->GetName().Contains(PropertyName))
			{
				return Property;	
			}
		}

		return nullptr;
	}

	/**
	 * 通过反射类来获取属性值
	 * @tparam Ret 属性值类型 
	 * @param Obj 具体对象
	 * @param PropertyName 属性名 
	 * @return 属性值的指针，找不到返回nullptr
	 */
	template<class Ret>
	static const Ret* GetClassPropertyValue(const UObject* Obj, const FString& PropertyName)
	{
		if (const FProperty* const Property = GetClassProperty(Obj->GetClass(), PropertyName))
		{
			return Property->ContainerPtrToValuePtr<Ret>(Obj);
		}
		return nullptr;
	}

	/**
	 * 获取类的所有属性定义
	 * @param Class 类
	 * @param OutResult 输出的属性定义数组 
	 */
	static void GetClassPropertys(const UClass* Class, TArray<FProperty*>& OutResult)
	{
		for (TFieldIterator<FProperty> P(Class); P; ++P)
		{
			OutResult.Add(*P);
		}
	}

	static void GetClassPropertysByFilter(const UClass* Class, const TFunction<bool(const FProperty*)>& Filter, TArray<FProperty*>& OutResult)
	{
		for (TFieldIterator<FProperty> P(Class); P; ++P)
		{
			FProperty* Property = *P;
			if (Property && Filter(Property))
			{
				OutResult.Add(*P);
			}
		}
	}

	static void GetClassPropertysByPropType(const UClass* Class, const FString& Type, TArray<FProperty*>& OutResult)
	{
		for (TFieldIterator<FProperty> P(Class); P; ++P)
		{
			FProperty* Property = *P;
			if (Property && Property->GetCPPType() == Type)
			{
				OutResult.Add(*P);
			}
		}
	}

	

	/**
	 * 遍历枚举的所有元素
	 * @tparam T 承继自UEnum
	 * @param InvokeFun 回调函数，遍历每个枚举元素时都会调用，会传参 枚举下标 和 枚举名字
	 */
	template<class T>
	static void TraverseEnum(TFunction<void(int32, FName)> InvokeFun)
	{
		const UEnum* EnumClass = StaticEnum<T>();
		for(int32 Index = 0; Index < EnumClass->NumEnums(); ++Index)
		{
			FName EnumName = EnumClass->GetNameByIndex(Index);
			InvokeFun(Index, EnumName);
		}
	}
	
	/**
	 * 获取结构的属性
	 * @tparam T 
	 * @param PropertyName 
	 * @return 
	 */
	template<class T>
	static const FProperty* GetStructProperty(const FString& PropertyName, const bool Fuzzy = false)
	{
		if (!Fuzzy)
		{
			return StaticStruct<T>()->FindPropertyByName(FName(PropertyName));
		}
		
		for (TFieldIterator<FProperty> P(StaticStruct<T>()); P; ++P)
		{
			FProperty* Property = *P;
			if (Property && Property->GetName().Contains(PropertyName))
			{
				return Property;
			}
		}

		return nullptr;
	}
	
	template<class Ret>
	static Ret* GetPropertyValue(const FProperty* Property, void* ContainPtr)
	{
		if (Property)
		{
			return Property->ContainerPtrToValuePtr<Ret>(ContainPtr);
		}
		return nullptr;
	}

	/**
	 * 获取函数
	 * @param Class 
	 * @param FuncName 
	 * @param Fuzzy 
	 * @return 
	 */
	static UFunction* GetFunction(const UClass* Class, const FString& FuncName, const bool Fuzzy = false)
	{
		if (!Fuzzy)
		{
			return Class->FindFunctionByName(FName(FuncName));
		}

		for (TFieldIterator<UFunction> P(Class); P; ++P)
		{
			UFunction* Func = *P;
			if (Func && Func->GetName().Contains(FuncName))
			{
				return Func;
			}
		}
		
		return nullptr;
	}

	static void InvokeFunc(UObject* Obj, const FString& FuncName, void* Params = nullptr)
	{
		check(Obj);
		UFunction* Func = GetFunction(Obj->GetClass(), FuncName);
		if (Func)
		{
			Obj->ProcessEvent(Func, Params);
		}
	}
};


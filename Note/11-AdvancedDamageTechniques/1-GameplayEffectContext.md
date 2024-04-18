# The Gameplay Effect Context

## MakeEffectContext

`MakeEffectContext` 是虚幻引擎（Unreal Engine）中用于创建游戏效果上下文（Gameplay Effect Context）的函数。游戏效果上下文是在游戏效果应用到游戏对象时使用的上下文，它包含了与应用游戏效果相关的各种信息，如应用者、目标、捕捉的属性值等。

`MakeEffectContext` 函数通常在游戏效果的执行计算（Execution Calculations）中使用，用于创建一个游戏效果上下文对象。这个上下文对象可以传递给游戏效果的执行计算函数，以便在执行计算过程中使用。

例如，以下是一个使用 `MakeEffectContext` 创建游戏效果上下文的例子：

```cpp
// 创建游戏效果上下文
FGameplayEffectContext* EffectContext = MakeEffectContext();

// 使用游戏效果上下文进行计算
float NewHealth = EffectContext->GetCaptureAttributeValue(FAbilityAttributeCapture, EAttributeCaptureMethod::CurrentValue, UHealthAttributeComponent::StaticClass());

// 应用游戏效果
UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
AbilitySystemComponent->ApplyGameplayEffectToTarget(HealthGameplayEffect, EffectContext, TargetActor);
```

在这个例子中，我们首先使用 `MakeEffectContext` 创建了一个游戏效果上下文对象。然后，我们在游戏效果的执行计算过程中使用这个上下文对象来获取属性值。最后，我们将游戏效果应用到目标对象上，同时传递了游戏效果上下文对象，以便在应用游戏效果时使用。



我们之前的技能，已经可以计算出格挡，计算出暴击。现在，我们需要根据是否格挡，是否暴击，在界面上有所表现。这也就是说，我们需要拿到这个只出现在计算过程中的效果。而目前的困境就是，所有的这些效果是否生效，结果只在Execute里才能知道

所以，为了将这些计算过程从Execute中带出来，我们创建一个GameplayEffectContext子类，在子类中新增这些成员变量，通过效果上下文，传递给 `void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)`，因为在Data数据里，有上下文信息，这里也是一次攻击结算的最后调用（Post阶段），然后我们在这里拿到是否暴击，是否格挡，就能在界面做出相对应的响应。

> `FGameplayEffectModCallbackData`下的 `FGameplayEffectSpec`成员，这里面有`FGameplayEffectContextHandle EffectContext; `上下文信息，所有效果相关的信息都能通过它来传递。

```c++
#pragma once
#include "GameplayEffectTypes.h"

class UAuraAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }

	// 获取脚本结构
	virtual UScriptStruct* GetScriptStruct() const override;

	// 网络序列化。这里决定了这个结构体在网络中如何序列化
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;		// 是否格挡

	UPROPERTY()
	bool bIsCriticalHit = false;	// 是否暴击
	
};

```



## NetSerialize

```c++
bool FGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	uint8 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
	}

	Ar.SerializeBits(&RepBits, 7);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}	
	
	bOutSuccess = true;
	return true;
}
```

这是FGameplayEffectContext基类中的NetSerialize实现。

`FGameplayEffectContext::NetSerialize` 是虚幻引擎（Unreal Engine）中一个用于网络序列化的函数，它在游戏效果上下文（Gameplay Effect Context）类中定义。这个函数的主要作用是在网络传输中序列化和反序列化游戏效果上下文的数据。

在游戏效果上下文中，`NetSerialize` 函数通常用于在网络传输中序列化和反序列化游戏效果上下文的数据。这个函数接收一个 `FArchive` 对象作为参数，这个对象负责读取和写入数据。此外，还需要一个 `UPackageMap` 对象，用于映射游戏对象和网络包之间的关系。最后，一个布尔值 `bOutSuccess` 用于指示是否成功完成了序列化和反序列化。

在函数内部，`NetSerialize` 会遍历游戏效果上下文中的各个成员变量，并根据需要进行序列化和反序列化。具体来说，它会检查每个成员变量是否有 `net` 标记，如果有，则表示这个成员变量需要在网络传输中进行序列化和反序列化。

在序列化过程中，`NetSerialize` 会将游戏效果上下文的数据写入到 `FArchive` 对象中。在反序列化过程中，它会从 `FArchive` 对象中读取数据，并将这些数据赋值给游戏效果上下文的成员变量。

需要注意的是，`NetSerialize` 函数通常在游戏效果上下文的网络传输过程中使用，而不是在游戏逻辑中使用。在游戏逻辑中，通常使用其他函数来获取和设置游戏效果上下文的数据。

## UPackageMap

`UPackageMap` 是虚幻引擎（Unreal Engine）中一个用于管理游戏对象和网络包之间关系的类。在游戏网络传输过程中，`UPackageMap` 负责将游戏对象转换为网络包，以及将网络包转换回游戏对象。

在游戏网络传输过程中，`UPackageMap` 主要用于以下几个方面：

1. **游戏对象到网络包的转换**：在发送游戏对象时，`UPackageMap` 将游戏对象转换为网络包，以便在网络中传输。这个过程通常涉及将游戏对象的属性值转换为网络格式，并将游戏对象的引用转换为网络包的索引。

2. **网络包到游戏对象的转换**：在接收网络包时，`UPackageMap` 将网络包转换为游戏对象，以便在游戏中使用。这个过程通常涉及将网络包中的属性值转换为游戏对象的属性值，并将网络包中的引用转换为游戏对象的引用。

3. **网络包的管理**：`UPackageMap` 还负责管理网络包的生命周期，包括分配和释放网络包的内存，以及更新网络包的状态。

在游戏网络传输过程中，`UPackageMap` 通常与 `FArchive` 对象一起使用。`FArchive` 对象负责读取和写入数据，而 `UPackageMap` 则负责将这些数据转换为网络包或游戏对象。

需要注意的是，`UPackageMap` 是一个比较底层的类，通常由虚幻引擎的网络系统自动调用。在游戏逻辑中，通常不需要直接使用 `UPackageMap`。

## FArchive

`FArchive` 是虚幻引擎（Unreal Engine）中一个用于读取和写入数据的类。在虚幻引擎中，`FArchive` 通常用于网络传输、文件存储、内存操作等场景。

`FArchive` 类的主要功能包括：

1. **数据读取和写入**：`FArchive` 提供了一系列的读取和写入函数，用于读取和写入各种类型的数据，如整数、浮点数、字符串、向量等。

2. **数据压缩和解压缩**：`FArchive` 还支持数据的压缩和解压缩，以减少网络传输的大小。

3. **数据流控制**：`FArchive` 可以控制数据的读取和写入，包括跳过某些数据、读取指定长度的数据等。

4. **数据验证**：`FArchive` 还可以验证读取和写入的数据，以确保数据的完整性和正确性。

在游戏网络传输过程中，`FArchive` 通常与 `UPackageMap` 类一起使用。`UPackageMap` 负责将游戏对象转换为网络包，而 `FArchive` 则负责读取和写入这些网络包中的数据。

需要注意的是，`FArchive` 类通常在游戏网络传输过程中使用，而在游戏逻辑中，通常使用其他函数来获取和设置游戏对象的属性值。

![image-20240418121617090](.\image-20240418121617090.png)

FArchive 是虚幻引擎（Unreal Engine）中一个用于读取和写入数据的类。在游戏网络传输过程中，FArchive 通常与 `UPackageMap` 类一起使用，用于将游戏对象转换为网络包，以及将网络包转换回游戏对象。

在游戏网络传输过程中，FArchive 通常使用移位运算符（<<）来读取和写入数据。具体来说，FArchive 使用 << 运算符来将数据从一个对象中读取出来，或者将数据写入到一个对象中。

在读取数据时，FArchive 使用 << 运算符来将数据从一个对象中读取出来。例如，如果一个对象有一个整数属性值，FArchive 可以使用 << 运算符来读取这个属性值。在这个过程中，FArchive 会将数据从对象中读取出来，并将数据写入到一个缓冲区中。

在写入数据时，FArchive 使用 << 运算符来将数据写入到一个对象中。例如，如果一个对象有一个整数属性值，FArchive 可以使用 << 运算符来将这个属性值写入到对象中。在这个过程中，FArchive 会将数据从缓冲区中读取出来，并将数据写入到对象中。

需要注意的是，FArchive 使用 << 运算符的方式取决于上下文。在保存数据时，FArchive 通常使用 << 运算符来将数据从对象中读取出来，并将数据写入到一个缓冲区中。在加载数据时，FArchive 通常使用 << 运算符来将数据从缓冲区中读取出来，并将数据写入到对象中。



##  Implementing Net Serialize

对于基类中的FGameplayEffectContext::NetSerialize()序列化函数，因为我们新增了两个变量，所以原本的7位序列化就不够用了，所以需要我们在子类中重写覆盖NetSerialize，这里我将其扩展为32位

```c++
	UPROPERTY()
	bool bIsBlockedHit = false;		// 是否格挡

	UPROPERTY()
	bool bIsCriticalHit = false;	// 是否暴击
```

```c++
bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bIsBlockedHit)	// 是否格挡
		{
			RepBits |= 1 << 7;
		}
		if (bIsCriticalHit)	// 是否暴击
		{
			RepBits |= 1 << 8;
		}
	}

	Ar.SerializeBits(&RepBits, 9);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}

	if (RepBits & (1 << 7))
	{
		Ar << bIsBlockedHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bIsCriticalHit;
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;
	return true;
}
```



# Struct Ops Type Traits

![image-20240418163014380](.\image-20240418163014380.png)

TStructOpsTypeTraitsBase2源码如下

<img src=".\image-20240418163501407.png" alt="image-20240418163501407" style="zoom:200%;" />

```c++
/** type traits to cover the custom aspects of a script struct **/
template <class CPPSTRUCT>
struct TStructOpsTypeTraitsBase2
{
	enum
	{
		WithZeroConstructor            = false,                         // struct can be constructed as a valid object by filling its memory footprint with zeroes.
		WithNoInitConstructor          = false,                         // struct has a constructor which takes an EForceInit parameter which will force the constructor to perform initialization, where the default constructor performs 'uninitialization'.
		WithNoDestructor               = false,                         // struct will not have its destructor called when it is destroyed.
		WithCopy                       = !TIsPODType<CPPSTRUCT>::Value, // struct can be copied via its copy assignment operator.
		WithIdenticalViaEquality       = false,                         // struct can be compared via its operator==.  This should be mutually exclusive with WithIdentical.
		WithIdentical                  = false,                         // struct can be compared via an Identical(const T* Other, uint32 PortFlags) function.  This should be mutually exclusive with WithIdenticalViaEquality.
		WithExportTextItem             = false,                         // struct has an ExportTextItem function used to serialize its state into a string.
		WithImportTextItem             = false,                         // struct has an ImportTextItem function used to deserialize a string into an object of that class.
		WithAddStructReferencedObjects = false,                         // struct has an AddStructReferencedObjects function which allows it to add references to the garbage collector.
		WithSerializer                 = false,                         // struct has a Serialize function for serializing its state to an FArchive.
		WithStructuredSerializer       = false,                         // struct has a Serialize function for serializing its state to an FStructuredArchive.
		WithPostSerialize              = false,                         // struct has a PostSerialize function which is called after it is serialized
		WithNetSerializer              = false,                         // struct has a NetSerialize function for serializing its state to an FArchive used for network replication.
		WithNetDeltaSerializer         = false,                         // struct has a NetDeltaSerialize function for serializing differences in state from a previous NetSerialize operation.
		WithSerializeFromMismatchedTag = false,                         // struct has a SerializeFromMismatchedTag function for converting from other property tags.
		WithStructuredSerializeFromMismatchedTag = false,               // struct has an FStructuredArchive-based SerializeFromMismatchedTag function for converting from other property tags.
		WithPostScriptConstruct        = false,                         // struct has a PostScriptConstruct function which is called after it is constructed in blueprints
		WithNetSharedSerialization     = false,                         // struct has a NetSerialize function that does not require the package map to serialize its state.
		WithGetPreloadDependencies     = false,                         // struct has a GetPreloadDependencies function to return all objects that will be Preload()ed when the struct is serialized at load time.
		WithPureVirtual                = false,                         // struct has PURE_VIRTUAL functions and cannot be constructed when CHECK_PUREVIRTUALS is true
		WithFindInnerPropertyInstance  = false,							// struct has a FindInnerPropertyInstance function that can provide an FProperty and data pointer when given a property FName
		WithCanEditChange			   = false,							// struct has an editor-only CanEditChange function that can conditionally make child properties read-only in the details panel (same idea as UObject::CanEditChange)
	};

	static constexpr EPropertyObjectReferenceType WithSerializerObjectReferences = EPropertyObjectReferenceType::Conservative; // struct's Serialize method(s) may serialize object references of these types - default Conservative means unknown and object reference collector archives should serialize this struct 
};
```



1. `WithZeroConstructor`: 结构体可以通过填充其内存足迹为零来构造为一个有效的对象。
2. `WithNoInitConstructor`: 结构体具有一个构造器，该构造器采用一个EForceInit参数，该参数将强制构造器执行初始化，而不是默认的构造器执行“未初始化”。
3. `WithNoDestructor`: 结构体将没有析构函数，当它被销毁时。
4. `WithCopy`: 结构体可以通过其赋值运算符进行复制。这应该与WithIdenticalViaEquality互斥。
5. `WithIdenticalViaEquality`: 结构体可以通过其相等运算符进行比较。这应该与WithIdentical互斥。
6. `WithIdentical`: 结构体可以通过一个Identical(const T * Other, uint32 PortFlags)函数进行比较。这应该与WithIdenticalViaEquality互斥。
7. `WithExportTextItem`: 结构体具有一个ExportTextItem函数用于将状态序列化为字符串。
8. `WithAddStructReferencedObjects`: 结构体具有AddStructReferencedObjects函数，允许它将引用添加到垃圾收集器。
9. `WithSerializer`: 结构体具有一个Serialize函数用于将状态序列化到一个FArchive。
10. `WithStructuredSerializer`: 结构体具有一个Serialize函数用于将状态序列化到一个FStructuredArchive。
11. `WithPostSerialize`: 结构体具有一个PostSerialize函数，在其被序列化后被调用。
12. `WithNetSerializer`: 结构体具有一个NetSerialize函数，用于将状态序列化到一个FArchive用于网络复制。
13. `WithNetDeltaSerializer`: 结构体具有一个NetDeltaSerialize函数，用于序列化状态的差异。
14. `WithSerializerFromPrevious`: 结构体具有一个SerializeFromPrevious函数，用于从以前的序列化状态转换。
15. `WithStructuredSerializerFromPrevious`: 结构体具有一个StructuredSerializeFromPrevious函数，用于从以前的序列化状态转换。
16. `WithPreLoadDependencies`: 结构体具有一个GetPreLoadDependencies函数，返回所有将在加载时预加载的包。
17. `WithPureVirtual`: 结构体的所有纯虚拟函数不能被调用。
18. `WithFindInnerPropertyInstance`: 结构体具有一个FindInnerPropertyInstance函数，可以根据给定的属性名称提供属性实例和数据指针。
19. `WithCanEditChange`: 结构体具有一个CanEditChange函数，可以在编辑器中有条件地使子属性只读。
20. `WithSerializerObjectReferences`: 结构体的Serialize方法可能序列化这些类型的对象引用 - 默认的Conservative意味着未知的对象引用收集器档案应该序列化这个结构体。

TStructOpsTypeTraitsBase2模板类用于描述结构体的特性，这些特性会影响结构体如何被序列化和使用。



所以，我们还需要在FAuraGameplayEffectContext下文中补充完整操作类型注册

```c++

// 如果我们需要自定义GameplayEffectContext, 那么我们需要在这里注册GameplayEffectContext的操作类型
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};
```



```c++
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UAuraAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }


	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	// 网络序列化。这里决定了这个结构体在网络中如何序列化
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;		// 是否格挡

	UPROPERTY()
	bool bIsCriticalHit = false;	// 是否暴击
	
};

// 如果我们需要自定义GameplayEffectContext, 那么我们需要在这里注册GameplayEffectContext的操作类型
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};

```


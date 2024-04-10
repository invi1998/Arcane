![image-20240410132750952](.\image-20240410132750952.png)

构建一个优雅强大的系统来管理和获取资产里的Attributes。

1. 创建Secondary Attribute Gameplay Tags（更好地在C++中处理标签）：
   - 目的：改进对Gameplay Tags的管理方式，使其更适合在C++中使用。
   - 做法：创建一个更高效、更易于编程的Gameplay Tags系统，可能包括自定义数据结构或API。

2. 创建UAttributeInfo Data Asset：
   - 目的：存储每个属性的信息，如名称、描述、图标等。
   - 做法：创建一个数据资产类，用于存储每个属性的元数据。每个属性对应一个UAttributeInfo实例。

3. 创建FAuraAttributeInfo结构：
   - 目的：定义一个C++结构体，用于在C++代码中表示属性信息。
   - 做法：创建一个名为FAuraAttributeInfo的结构体，包含属性名称、描述、图标等字段。

4. 填充每个属性的数据资产：
   - 目的：为每个属性提供详细的元数据。
   - 做法：为每个属性创建一个UAttributeInfo数据资产实例，并填充其元数据字段。

5. 创建UAttributeMenuWidgetController：
   - 目的：提供一个控制器类，用于管理属性菜单的显示和交互。
   - 做法：创建一个UAttributeMenuWidgetController类，负责处理属性菜单的逻辑，如加载属性信息、显示属性列表、处理用户操作等。

构建思路

> 首先，当属性发生变化时，我们需要一个属性菜单的Widget控制器，用于接收来自能力系统的广播。
>
> 接下来，当我们接收到一个属性时，我们需要一种方法来确定与该属性关联的正确的Gameplay Tag。我们需要将这两个关联起来，我们还希望有一种方法可以在C++中引用这些Gameplay Tags。
>
> 我们现在看到了一种实现这一目标的方法。我们可以使用Request Gameplay Tag，它接受一个FName参数，我们在其中指定我们想要的标签。这样，我们基本上是在向Gameplay Tag Manager请求一个标签结构，给定一个名称，如果Gameplay Tag Manager拥有这个标签，那么Request Gameplay Tag将返回它给我们。
>
> 不过，这种方法存在一些问题，我不喜欢在FName参数中输入字符串名称来指定Gameplay Tag。我希望有一个更好的方法来处理这些标签。我想要一个类，它可以存储全局的、我们可以访问的Tag变量。这就是我们需要研究的东西。
>
> 接下来，我们需要一个数据资产，它能够接收Gameplay Tags，并返回我们需要制作的我们的Aura Attribute Info结构。
>
> 这样一来，一旦我们知道某个属性的标签，我们的数据资产就能提供我们需要发送给我们的Widgets的所有数据。这将是我们的最后一步，一旦我们有了这个结构，那就是将它广播给我们的Widgets，它们可以检查自己的Gameplay Tags是否与发送的结构中的Gameplay Tags匹配，然后它们可以更新自己。
>
> 这意味着我们属性菜单有一些下一步的工作。
>
> 首先，我们需要创建我们的二级属性Gameplay Tags，并且我们希望在C++中处理这些标签的方式更好，而不是到处调用request gameplay tag。
>
> 我们需要在C++的一边有一个集中式的Gameplay Tag变量的来源。
>
> 我们还需要一个数据资产，它能够存储这些结构，并允许我们根据Gameplay Tag进行查找，以找到我们需要的属性信息，这样我们就可以将它广播给我们的Widgets。
>
> 我们还需要创建一个结构体，我们将把所有的信息都放在这个结构体中，这样我们就可以把它发送给我们的Widgets。
>
> 我们还需要创建这个结构体。我们将创建一个Aura Attribute Info结构体，它包含了我们想要发送给Widgets的所有信息。
>
> 当然，我们需要创建我们的属性菜单的Widget控制器。这个Widget控制器将把所有的事情联系在一起，因为它将监听来自能力系统的广播，并能够从数据资产中检索到属性信息的结构体，然后将它广播给我们的Widgets，它们可以检查自己的Gameplay Tags是否与发送的结构中的Gameplay Tags匹配，然后它们可以更新自己。



## FAuraGameplayTags

C++中创建一个原生单例，用于管理游戏中所有的GameplayTags

```c++
// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 *
 * 原生单例，用于管理游戏中的所有GameplayTags
 */
struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; };

	static void InitializeNativeGameplayTags();

protected:

private:
	static FAuraGameplayTags GameplayTags;
};
```

cPP

```c++
FAuraGameplayTags FAuraGameplayTags::GameplayTags;	// 静态变量初始化

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// 添加原生标签，属性->次要属性->护甲，描述为减少受到的伤害，提高格挡几率
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage Taken, improves Block Chance"));
	
}

```

现在，我们只是为属性 Attributes.Secondary.Armor 添加了一个本地游戏标签，现在我们需要在某个时刻调用这个函数，这也就是接下来我们的需要创建自己的资源管理器的作用。

## UAuraAssetManager

我们基于UAssetManager创建一个新的资产管理器，该类也是一个单例例，然后重写StartInitialLoading函数，这个函数会在引擎启动的时候调用，所以我们可以在这里去初始化上面的FAuraGameplayTags（GameplayTags资产）

```c++

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UAuraAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;	// 在这里加载资源，这个函数会在引擎启动时调用，所以这个函数时调用初始化本地Tags最好的地方
	
};
```

cPP

```c++

#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	checkf(GEngine, TEXT("UAuraAssetManager::Get() called with GEngine == nullptr.  This function should only be called after Engine has been initialized."));
	UAuraAssetManager* AuraAssetManagerSingleton = Cast<UAuraAssetManager>(GEngine->AssetManager);	// 将GEngine->AssetManager转换为UAuraAssetManager类型
	return *AuraAssetManagerSingleton;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();	// 初始化游戏标签

	// 现在要做的最后一步，就是将这个AssetManager注册到GEngine中，设置为我们的项目的资产管理器
}

```

现在要做的最后一步，就是将这个AssetManager注册到GEngine中，设置为我们的项目的资产管理器。

进入项目配置文件（defaultEngine.ini），将我们的资产管理器设置为游戏的资产管理器，记住，这里设置命字的时候，需要去掉类名例的F

![image-20240410164611014](.\image-20240410164611014.png)

编译启动，可以看到，我们新增的这个属性tag，就称为了一个Native（原生属性）

![image-20240410165015251](.\image-20240410165015251.png)



## Attribute Info Data Asset-- FAuraAttributeInfo

现在，我们已经创建了自己的GamplayTag，并在C++中处理这些Tag，现在，当我们使用功能我们的UAuraAssetManager单例或者FAuraGameplayTags创建本地（Native）标签时，我们需要一个信息数据作为资产属性，所以这里我们还要创建一个 UAttributeInfo数据资产，我们可以以结构体形式存储信息的资产。

```c++
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

// 这里创建一个结构体，用来存储给定属性的相关信息，一但属性发生变化，我们就可以将这个结构体传递给UI，让UI更新
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	// GamelayTag
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AttributeTag;

	// 属性名
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AttributeName = FText();

	// 属性中文名
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AttributeChineseName = FText();

	// 属性描述
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AttributeDescription = FText();

	// 属性值
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.0f;

	// 属性Icon
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* AttributeIcon = nullptr;
};


/**
 * 
 */
UCLASS()
class ARCANE_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	// 传入一个GameplayTag，返回一个FAuraAttributeInfo，bLogNotFound 如果为true，当找不到属性时会打印错误信息，记录日志
	FAuraAttributeInfo GetAttributeInfo(const FGameplayTag AttributeTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)	// 只读
	TArray<FAuraAttributeInfo> AttributeInformation;	// 属性信息


};

```

GetAttributeInfo要做的也很简单，就是遍历属性信息列表，然后进行匹配返回

```c++
#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::GetAttributeInfo(const FGameplayTag AttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& AttributeInfo : AttributeInformation)
	{
		// 这里判断Tag是否相等，如果不想用等于，可以使用AttributeInfo.AttributeTag.MatchesTagExact(AttributeTag)来判段也是可以的
		if (AttributeInfo.AttributeTag == AttributeTag)		// 全匹配
		{
			return AttributeInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("AttributeInfo for %s not found in %s"), *AttributeTag.ToString(), *GetName());
	}

	return FAuraAttributeInfo();
}

```

从实现上就可以看到，这个类要做的事情，基本就是将我们所有的属性信息进行归总，然后定义一个属性结构体，方便我们后续代理绑定和UI响应所需的数据内容

然后编译启动UE，我们基于这个DateAsset创建一个蓝图


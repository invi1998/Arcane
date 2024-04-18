# UE5 的内存管理和垃圾回收

## 什么是垃圾回收？

在计算机科学中，**垃圾回收**（英语：Garbage Collection，缩写为GC）是指一种自动的内存管理机制。当某个程序占用的一部分内存空间不再被这个程序访问时，这个程序会借助**垃圾回收**算法向操作系统归还这部分内存空间。垃圾回收器可以减轻程序员的负担，也减少程序中的错误。目前许多语言如[Smalltalk](https://link.zhihu.com/?target=https%3A//zh.wikipedia.org/wiki/Smalltalk)、[Java](https://link.zhihu.com/?target=https%3A//zh.wikipedia.org/wiki/Java)、[C#](https://link.zhihu.com/?target=https%3A//zh.wikipedia.org/wiki/C_Sharp)、[Go](https://link.zhihu.com/?target=https%3A//zh.wikipedia.org/wiki/Go)和[D语言](https://link.zhihu.com/?target=https%3A//zh.wikipedia.org/wiki/D%E8%AF%AD%E8%A8%80)都支持垃圾回收器。

较低级别的语言（如 C 和 C++）不提供现成的垃圾回收器。这意味着你必须手动跟踪正在使用的内存，并在不再希望使用的时候释放。这可能容易出错，程序员也更难管理，因此虚幻引擎创建了自己的垃圾回收系统。

## 垃圾回收在ue中的工作原理？

- 一个UObject派生的对象被实例化时，会被注册到虚幻引擎的GC系统中。GC系统在自动运行时，查找不再使用的对象并将其删除。
- GC系统有一个对象的“根集”，这些对象永久处于活动状态。
- GC系统使用反射来查看对象的属性并跟踪对其他对象的引用，然后跟踪这些对象的属性等。

工作过程：TODO

## 哪些内容会被垃圾回收？

继承自UObject的类的成员变量指针，在它之前添加 UPROPERTY（...） 宏时，虚幻构建工具就会生成所需的代码，保证该类的对象与垃圾回收系统正常工作。

只有UPROPERTY() 宏修饰的派生自 “UObject”的类会被GC。

```text
示例：
UPROPERTY()
UObject* HardUObjectReference;
```



### TWeakObjectPtr/FWeakObjectPtr的使用

如果只想在类中引用UObject对象，而不影响引用对象的GC，可以使用 TWeakObjectPtr/FWeakObjectPtr，它不会使对象保持活动状态，但在对象被销毁后，当您调用这两种数据类型的 IsValid 方法时，它会自动开始返回 false。

```cpp
UCLASS(Abstract, Config = Game)
class COMMONGAME_API UCommonGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	/** This is the primary player*/
	TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;
};

struct FLoadedGameplayTagToProcessData
{
	FGameplayTag Tag;
	TWeakObjectPtr<UObject> WeakOwner;

	FLoadedGameplayTagToProcessData() {}
	FLoadedGameplayTagToProcessData(const FGameplayTag& InTag, const TWeakObjectPtr<UObject>& InWeakOwner)
                                       : Tag(InTag), WeakOwner(InWeakOwner) {}
};
```

## 在ue里创建对象

### 创建c++对象

```cpp
class A
{
  int m;
};
//case 1: 单指针
A* a = new A;
delete a;

//case 2: 数组
A* arr = new A[3];
delete[] arr;
```

### 创建在继承于 Actor的类的实例

```cpp
APawn* ALyraGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Never save the default player pawns into a map.
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			return SpawnedPawn;
		}
	}

	return nullptr;
}
```

### 创建UObject对象

```cpp
UCommonUserInfo* UCommonUserSubsystem::CreateLocalUserInfo(int32 LocalPlayerIndex)
{
	UCommonUserInfo* NewUser = nullptr;
	if (ensure(!LocalUserInfos.Contains(LocalPlayerIndex)))
	{
		NewUser = NewObject<UCommonUserInfo>(this);
		NewUser->LocalPlayerIndex = LocalPlayerIndex;
		NewUser->InitializationState = ECommonUserInitializationState::Unknown;

		// Always create game and default cache
		NewUser->CachedDataMap.Add(ECommonUserOnlineContext::Game, UCommonUserInfo::FCachedData());
		NewUser->CachedDataMap.Add(ECommonUserOnlineContext::Default, UCommonUserInfo::FCachedData());

		// Add platform if needed
		if (HasSeparatePlatformContext())
		{
			NewUser->CachedDataMap.Add(ECommonUserOnlineContext::Platform, UCommonUserInfo::FCachedData());
		}

		LocalUserInfos.Add(LocalPlayerIndex, NewUser);
	}
	return NewUser;
}
```

## UObject成员变量的常见使用情况

### 硬引用

下面这些方式，防止被引用的UObject对象被垃圾回收

1、在蓝图类的实例中保存对 UObject 的

2、通过原始C++指针保存对 UObject 的引用，将类声明为成员字段并使用 UPROPERTY（） 进行修饰：

```cpp
// If this points to an object at runtime, there will be a hard reference between 
// an instance of this class and that object.
UPROPERTY()
UObject* HardUObjectReference;
```

3、在容器中使用原始C++指针，该容器是用 UPROPERTY 修饰的成员字段：

```cpp
UPROPERTY()
TArray<UObject*> ListOfObjectsWithHardReferences;
```

4、将对象/实例添加到根集，确保此 UObject 和所有其他具有硬引用的对象/实例不会被垃圾回收：

```cpp
void UMyObject::UMyFunction()
{
 UMyClass* MyObject = NewObject<UMyClass>(this);
  MyObject->AddToRoot();
  // At this point, MyObject will not be garbage collected until it's removed from 
  // the Root Set with RemoveFromRoot().
}
```

### 弱引用

下面这些方式不会阻止UObject被垃圾回收

1、在函数参数中传递原始C++指针，或在函数体中使用它：

```cpp
void UMyObject::UMyFunction()
{
 UMyClass* MyObject = NewObject<UMyClass>(this);
  // At this point, the object pointed to by MyObject can be garbage-collected 
  // practically as soon as UMyFunction() finishes executing.
}
```

2、使用原始C++指针作为类和结构中的成员

```cpp
UCLASS(Abstract, Config = Game)
class COMMONGAME_API UCommonGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	UCommonSession_SearchResult* RequestedSession;
};
```

3、使用弱 UObject 指针（如 TWeakObjectPtr 或 TSoftObjectPointer ）作为成员

```cpp
// This one can only point to objects in memory.
UPROPERTY()
TWeakObjectPtr<UMyClass> WeakUObjectReference; 

// This one can point to objects in memory or unloaded assets, and it's useful in 
// lazy-loading.
UPROPERTY()
TSoftObjectPointer<UMyClass> SoftUObjectReference;
```

## 原始指针与TObjectPtr的使用

细心的你一定会发现，在UE5里，有如下两种写法：

```cpp
//Case 1: 使用TObjectPtr
UPROPERTY()
TObjectPtr<UCommonSession_SearchResult> RequestedSession;

//Case 2: 裸指针
UPROPERTY()
UUserWidget* CurrentWidget;
```

从官方文档可以获悉：[TObjectPtr | Unreal Engine Documentation](https://link.zhihu.com/?target=https%3A//docs.unrealengine.com/5.0/en-US/API/Runtime/CoreUObject/UObject/TObjectPtr/)

[TObjectPtr](https://link.zhihu.com/?target=https%3A//docs.unrealengine.com/5.0/en-US/API/Runtime/CoreUObject/UObject/TObjectPtr/)是一种指向[UObject](https://link.zhihu.com/?target=https%3A//docs.unrealengine.com/5.0/en-US/API/Runtime/CoreUObject/UObject/UObject)的指针类型，其作用是替换原始指针的成员变量。

- 它的大小相当于64位指针，并支持编辑器构建中的访问跟踪和可选的惰性加载行为。
- 它存储被引用对象的地址，或者(在编辑器构建中)对象句柄表中的一个索引，该索引描述了尚未加载的被引用对象。
- 它的序列化方式与UObject的原始指针相同。
- 解析后，它在垃圾收集中的参与与指向UObject的原始指针相同。

这对于自动替换原始指针非常有用，可以支持高级的cook-time依赖项跟踪和编辑时延迟加载用例。 UnrealObjectPtrTool 工具可以自动替换原生指针。



我们来看下UHT在gen.cpp里生成的内容：

```cpp
//Case 1: 使用TObjectPtr
const UECodeGen_Private::FObjectPtrPropertyParams 
Z_Construct_UClass_UCommonGameInstance_Statics::NewProp_RequestedSession = {
 "RequestedSession", nullptr, (EPropertyFlags)0x0044000000000000, 
UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr,//多出来的参数
 RF_Public|RF_Transient|RF_MarkAsNative, 1, 
nullptr, nullptr, STRUCT_OFFSET(UCommonGameInstance, RequestedSession),
 Z_Construct_UClass_UCommonSession_SearchResult_NoRegister,
 METADATA_PARAMS(Z_Construct_UClass_UCommonGameInstance_Statics::NewProp_RequestedSession_MetaData,
 UE_ARRAY_COUNT(Z_Construct_UClass_UCommonGameInstance_Statics::NewProp_RequestedSession_MetaData)) };

//Case 2: 裸指针
const UECodeGen_Private::FObjectPropertyParams 
Z_Construct_UClass_AHowTo_UMGGameModeBase_Statics::NewProp_CurrentWidget = {
 "CurrentWidget", nullptr, (EPropertyFlags)0x0020080000080008,
 UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1,
 nullptr, nullptr, STRUCT_OFFSET(AHowTo_UMGGameModeBase, CurrentWidget), 
Z_Construct_UClass_UUserWidget_NoRegister, 
METADATA_PARAMS(Z_Construct_UClass_AHowTo_UMGGameModeBase_Statics::NewProp_CurrentWidget_MetaData,
UE_ARRAY_COUNT(Z_Construct_UClass_AHowTo_UMGGameModeBase_Statics::NewProp_CurrentWidget_MetaData)) };
```

这两个只有第四个参数不一样，TObjectPtr多了 UECodeGen_Private::EPropertyGenFlags::ObjectPtr

我们再到UHT源码里，看下两者生成的逻辑差异。

在CodeGenerator.cpp里可以看到UECodeGen_Private::EPropertyGenFlags::Object 对应的两个记录：

```c++
//Case 1: 使用TObjectPtr
switch (PropertyBase.GetUHTPropertyType())
case EUHTPropertyType::ObjectPtrReference://ObjectReference = CPT_ObjectReference,
{
UECodeGen_Private::EPropertyGenFlags::ObjectPtr
}
//Case 2: 裸指针
case EUHTPropertyType::ObjectReference://ObjectPtrReference = CPT_ObjectPtrReference,
{
}
```

那PropertyBase又是怎么来的，代码逻辑：

```c++
FToken VarType;
GetIdentifier(VarType, true)

EPropertyType PropertyType = CPT_ObjectReference;//默认值
const bool bIsObjectPtrTemplate = VarType.IsValue(TEXT("TObjectPtr"), ESearchCase::CaseSensitive);
const bool bIsWeakPtrTemplate = VarType.IsValue(TEXT("TWeakObjectPtr"), ESearchCase::CaseSensitive);
if (bIsObjectPtrTemplate)
{
	PropertyType = CPT_ObjectPtrReference;
}
else if (bIsWeakPtrTemplate)
{
	PropertyType = CPT_WeakObjectReference;
}
else if (bIsSoftObjectPtrTemplate)
{
	PropertyType = CPT_SoftObjectReference;
}
```

这里我们知道了，如果token的值不是其他模板类型，那就会使用默认值。

## 如果不是UClass，但希望对UObject产生引用

可以继承FGCObject，手动添加引用



---



**反射系统**

我理解的反射系统：运行时可得到对象的类型信息（包括：属性，方法）、动态创建类对象等

UE采用的是将对象的类，属性，方法用关键字（UCLASS，UPROPERTY，UFUNCTION）标记；编译之前启动UHT去扫描这些关键字，生成/刷新该对象的 UClass "伴随对象" ，UClass 保存了反射相关的信息，描述了各个属性之间的内存布局，通过UObject对象实例就可以将相应的属性的值取出来，进行读/写操作。关于UObject、UClass 可参考我的专栏：[虚幻引擎 UObject](https://www.zhihu.com/column/c_1734236701558595584)

### **垃圾回收的功能**

如果让你来实现这个垃圾回收系统，你觉得垃圾回收系统应该具备哪些功能呢？

- 实现自定义的 NewObject 方法( 无需调用对应的 DeleteObject )
- 自动回收没有被引用的垃圾对象
- 不影响其他系统的正常功能

**垃圾回收伪实现**

垃圾回收的算法有很多， 标记-清除算法 、 复制算法 、标记-整理算法 、分代收集算法等。我们就用标记清除算法来实现。标记-清除算法，看名字就知道有两个阶段，标记和清除：

- 标记：遍历所有对象，根据某种规则，标记其是否需要清除
- 清除：遍历所有对象，清除标记了的对象，回收内存

因此可知，要实现标记清除垃圾回收，在标记阶段我们需要做到以下两点：

- 能拿到所有对象
- 确定对象清除的规则

在自定义的 NewObject 方法内，把生成的对象指针放入全局数组 GUObjectArray ，这样我就能拿到所有对象了

想象一个画面：空场景内站着一个英雄。这个情况下，垃圾回收系统是不是应该围绕着英雄来判断？英雄用到的对象就保留，没用到的对象就清除。此时这个英雄就是 **“根对象”**。因此，标记清除的规则就是，根对象用到的对象保留，其他对象清除。那根对象怎么确定？就得我们”手动”标记（AddToRoot）

以下就是垃圾回收的伪实现：

- 启动垃圾回收，加锁（ 保持所有对象的引用关系不变 ）
- 设置所有对象为”不可达”标记（根对象、特殊对象 除外）
- 遍历根对象列表，根对象引用到的对象去除”不可达”标记
- 收集所有仍然标记为”不可达”的对象，全部删除

### **垃圾回收 UE4 实现**

UE4的垃圾回收过程，正是照着上文的伪实现一步步执行的，不过细节更丰富，效率也更高

### **GC 启动**

手动调用：UWorld::ForceGarbageCollection( bool bFullPurge)，它会在World.tick 的下一帧强行进行垃圾回收

自动调用：系统会根据默认的设置（可重新配置）一定的间隔时间或者条件下，自动调用垃圾回收

### **GC 锁**

```cpp
void CollectGarbage(EObjectFlags KeepFlags, bool bPerformFullPurge)
{
    AcquireGCLock(); //获取GC锁
    CollectGarbageInternal(KeepFlags, bPerformFullPurge); //垃圾回收
    ReleaseGCLock(); //释放GC锁
}
```

GC锁的主要用处就是为了暂停其他线程以免UObject对象的引用关系在GC过程中发生变化。主要步骤：

- 发送信号，表示我想获取GC锁，GCWantsToRunCounter 自增(原子操作)
- GC 线程 Sleep，查看 AsyncCounter 是否等于 0 判断其他线程是否有阻塞GC的操作还在执行，不等于 0 就继续等待
- AsyncCounter = 0，通过另一个变量 GCCounter 递增(原子操作)，来标识正在执行GC，其他所有线程将被阻塞
- 执行内存屏障
- 将 GCWantsToRunCounter 设为 0，开始真正的 GC 操作
- GC 操作完毕， GCCounter 自减释放 GC 锁

*内存屏障的主要意思就是，在这个屏障之前的所有读和写的操作，一定会在这个屏障后面的读和写的操作之前执行。为了防范多线程读写操作时序问题导致的逻辑 bug，详细内容自行 Google*

### **获取所有对象**

上文伪实现，NewObject的时候把生成的对象的指针放入一个全局数组，实际上UE4确实也是这么做的。UObject对象继承自UObjectBase，它的构造方法如下（代码有删减改动）：

```cpp
//UObjectBase.cpp
//NewUObject方法调用后,UObject对象初始化
UObjectBase::UObjectBase(UClass* InClass..等参数)
{
   AddObject(InName, InInternalFlags); 
}

void UObjectBase::AddObject(FName InName, 
EInternalObjectFlags InSetInternalFlags)
{ 
   //加入GUObjectArray,为Object分配InternalIndex
   GUObjectArray.AllocateUObjectIndex(Object);
}
```

GUObjectArray 虽然命名是 Array 结尾，实际上它是个容器体。虽然它里面一堆花里胡哨的东西，实际上只要理解它是为了多线程的时候分块扫描其存储的内容而设计就行。UObject对象不直接存入容器的，而是被组装成 **FUObjectItem** 结构：

```cpp
//UObjectArray.h
//对象存储的结构体,GC操作的就是这个对象
struct FUObjectItem
{
   class UObjectBase* Object; //对象
   int32 Flags;               //EInternalObjectFlags标识
   int32 ClusterRootIndex;    //当前所属簇索引
   int32 SerialNumber;        //对象序列码(WeakObjectPtr实现用到它)
}
```

这个结构体对于理解GC很重要。成员 Object 是 NewObject 生成的对象的指针，EInternalObjectFlags 是啥呢？就是用来做标记的枚举类型。结构如下：

```cpp
//ObjectMacros.h
enum class EInternalObjectFlags : int32
{
   None = 0,
   ReachableInCluster = 1 << 23, ///< 簇中可达
   ClusterRoot = 1 << 24, //cluster root 不会被GC回收,簇根节点
   Native = 1 << 25, // C++类对象
   Async = 1 << 26, //异步对象
   AsyncLoading = 1 << 27, //异步加载中
   Unreachable = 1 << 28, // 不可达对象，会被GC删除
   PendingKill = 1 << 29, // 等待析构，会被GC删除
   RootSet = 1 << 30, // 根节点

    //拥有这三种标记之一的对象在GC检测时会被跳过
   GarbageCollectionKeepFlags = Native | Async | AsyncLoading,
};
```

### **标记不可达**

以下代码就是获取所有对象，并根据标签 标记部分对象为不可达；可达对象都放入 **ObjectsToSerialize** 数组内 ( 代码有删减，部分变量名用auto替换，是为了减少长度)

```cpp
//GarbageCollection.cpp
   void PerformReachabilityAnalysis(EObjectFlags KeepFlags......等参数)
   {
      //从系统提供的数组池中获取数组（为了支持多线程）
      auto ArrayStruct = FGCArrayPool::Get().GetArrayStructFromPool();
      auto ObjectsToSerialize = ArrayStruct->ObjectsToSerialize;

      // 继承了FGCObject的非Uobject对象，放入ObjectsToSerialize
      ObjectsToSerialize.Add(FGCObject::GGCObjectReferencer);

      //将对象标记为不可达,并且将根节点以及不可删除对象放入ObjectsToSerialize
      MarkObjectsAsUnreachable(ObjectsToSerialize, KeepFlags);
      
      //分析ObjectsToSerialize数组内的对象,它能达到的对象,去掉不可达标签
      PerformReachabilityAnalysisOnObjectsInternal(ArrayStruct)
   }
```

代码都有注释，很好理解；ObjectsToSerialize.Add(FGCObject::GGCObjectReferencer); 这一行，很关键，这也是为什么你看很多文章，都说非UObject 对象，继承 FGCObject 后，也可以将它引用的对象加入垃圾回收

```cpp
class FMyStruct: public FGCObject
{
    UObject* NoGCObj;

    void AddReferencedObjects(FReferenceCollector& Collector) override
    {
	Collector.AddReferencedObject(NoGCObj);
    }
}
```

这部分的代码分析，可见我的上一篇文章：[[原创\]UE —UObject类智能指针](https://zhuanlan.zhihu.com/p/392864278)

### **引用关系分析**

MarkObjectsAsUnreachable() 方法消耗不大，因为是多线程操作，且这些 FUObjectItem 结构体对象是内存块连续的数据，对CPU很友好。PerformReachabilityAnalysisOnObjectsInternal(ArrayStruct) 才是最复杂，最耗时的操作

UE 是怎么获取一个对象引用了哪些其他对象呢？

UE 中每个 UObject 对象都有一个与之对应的 UClass 类对象 ， 这个 UClass 保存了对应 UObject 的所有属性（标记UPROPERTY）和方法（标记UFUNCTION）等相关信息，描述了各个属性之间的内存布局和关系；**通过UObject对象的实例化地址就可以将相应的属性遍历出来，进行读写**

Obj 对象所引用的其他 UObject 对象，伪代码如下：

```cpp
//遍历Obj对象的所有属性,如果属性的类型是UObjectle类型,就取消不可达标签
for(TFieldIterator<FProperty> PropertyIter(Obj->GetClass()); 
    PropertyIter; ++PropertyIter)
{
    const FProperty* PropertyIns = *PropertyIter;
    //该属性是否是UObject,是否是TArray<UObject>等
    //然后依次取消 "不可达"标记
    //然后递归遍历该属性对象所引用的对象
}
```

这么实现，确实可以 ；但是 UE 并没有这么做，为什么呢？因为属性内其实大部分都是非 UObject 类型（比如：int32, bool 等） ，全部遍历效率太低。因此在生成 UObject 对应 UClass 对象的时候，就构造了一个新的概念，将所引用的其他 UObject 对象用一个很巧妙的**整数**，存入 ReferenceTokenStream 变量的 Tokens 数组内

### **ReferenceTokenStream**

先假设有如下对象以及引用关系

```cpp
USTRUCT()
struct FHeroStruct
{
   GENERATED_BODY()
   UPROPERTY() ACharacter* Hero;
};

UCLASS()
class UChelsea : public UObject
{
   GENERATED_BODY()
public:
   UPROPERTY() int LingID = 0;
   AActor* ChelseaActor;
   static void AddReferencedObjects(
               UObject* InThis, 
               FReferenceCollector& Collector)
   {
       UChelsea* This = CastChecked<UChelsea>(InThis);
       Collector.AddReferencedObject(This->ChelseaActor);
   }
};

UCLASS()
class ULiverpool : public UChelsea
{
   GENERATED_BODY()
public:
   UPROPERTY() TArray<FHeroStruct> HeroArray;
   UPROPERTY() AActor* LiverpoolActor;
};
```

UE会根据反射的信息，将对象的引用信息保存在对应 UClass 内的 ReferenceTokenStream 内；ReferenceTokenStream 结构内只有2个数组，起作用的是Tokens (TokenDebugInfo 是调试信息)，Tokens 内的数字的结构如下：

```cpp
struct FGCReferenceInfo
{
    union
    {
        struct
        {
            uint32 ReturnCount  : 8;
            uint32 Type         : 5;
            uint32 Offset       : 19;
        };
        //ReturnCount + Type  + Offset
        //00000000    + 00000 + 0000000000000000000
        uint32 Value; 
    };
};

//执行下列代码并断点查看结果：
ULiverpool* Liverpool = NewObject<ULiverpool>(this);
```

这个里面包含当前对象 Obj 引用到的所有标记了 UPROPERTY 的属性（包括它的父类），那这个数字到底是啥意思呢 ？其实它是三个数字的合并。前8位， 引用对象的嵌套深度 ；中间5位，引用对象的类型( EGCRefenceType )；最后则是当前引用的对象的变量在 Obj 对象内的内存偏移值（根据这个偏移值，可获取这个引用的对象）

ULiverpool 的 ReferenceTokenStream 内容如下：

![img](https://pic4.zhimg.com/80/v2-1524fc8d2dc7983f8d32987d8d189ceb_720w.webp)

所有的token

我们将几个关键的Token内容解析一下，结果如下：

![img](https://pic2.zhimg.com/80/v2-e32a7f9b6915bc8e267839f59dcbfa65_720w.webp)

解析token的值

根据 Tokens 内的值，我们可以获取它引用的属性的类型。比如： GCRT_ArrayObject 、 GCRT_ArrayStruct、 GCRT_Object 等，根据它们的类型做不同的操作。PerformReachabilityAnalysisOnObjectsInternal 代码内最后调用的分析代码是 ProcessObjectArray （以下代码有删减）：

```cpp
void ProcessObjectArray(FGCArrayStruct& InObjectsToSerializeStruct......等参数)
{
    while (CurrentIndex < ObjectsToSerialize.Num())
    {
        CurrentObject = ObjectsToSerialize[CurrentIndex++];
        // 获取当前对象的ReferenceTokenStream
        auto* TokenStream = &CurrentObject->GetClass()->ReferenceTokenStream;
        uint32 ReferenceTokenStreamIndex = 0;
        //当前对象的起始地址
        uint8* StackEntryData = (uint8*)CurrentObject;
        while (true)
        {
	    //注意:源码这里不是直接++,但是为了方便理解,这里直接用
	    ReferenceTokenStreamIndex++;
	    FGCReferenceInfo ReferenceInfo = TokenStream->AccessReferenceInfo(ReferenceTokenStreamIndex);
            switch(ReferenceInfo.Type)
            {
                case GCRT_Object:
                case GCRT_Class:
                {
                    // 引用对象的地址：起始地址 + Offset
                    UObject**   ObjectPtr = (UObject**)(StackEntryData + ReferenceInfo.Offset);
                    UObject*&   Object = *ObjectPtr;
             ReferenceProcessor.HandleTokenStreamObjectReference(Object...);
                }
                break;
                case GCRT_ArrayObject:
                {
                    TArray<UObject*>& ObjectArray = *((TArray<UObject*>*)(StackEntryData + ReferenceInfo.Offset));
                    for (int32 ObjectIndex = 0, ObjectNum = ObjectArray.Num(); ObjectIndex < ObjectNum; ++ObjectIndex)
                    {
                        ReferenceProcessor.HandleTokenStreamObjectReference(Object...);
                    }
                }
                break;
            }
        }
    }
}
```

HandleTokenStreamObjectReference 会调用 HandleObjectReference，它会去除它的”不可达”标记，并将它加入NewObjectsToSerialize，开辟新的 task 线程去处理，而不是在当前线程递归

### **清理**

整个过程结束以后，遍历 GObjectArray内的数组仍然被标记为不可达的对象放入 GUnreachableObjects ，随后就是执行清除

```cpp
//标记RF_BeginDestroyed
bool UnhashUnreachableObjects(bool bUseTimeLimit, float TimeLimit)
{
   while (GUnrechableObjectIndex < GUnreachableObjects.Num())
   {
      ObjectItem = GUnreachableObjects[GUnrechableObjectIndex++];
      UObject* Object = static_cast<UObject*>(ObjectItem->Object);
      Object->ConditionalBeginDestroy();//调用BeginDestroy();
      ++GUnrechableObjectIndex;
   }
   return bTimeLimitReached;
}

//标记RF_FinishDestroyed
bool IncrementalDestroyGarbage(bool bUseTimeLimit, float TimeLimit)
{
 	GObjCurrentPurgeObjectIndex = 0;
   while (GObjCurrentPurgeObjectIndex < GUnreachableObjects.Num())
   {
      FUObjectItem* ObjectItem = GUnreachableObjects[GObjCurrentPurgeObjectIndex];
      UObject* Object = static_cast<UObject*>(ObjectItem->Object);
      Object->ConditionalFinishDestroy();//调用FinishDestroy();
      ++GObjCurrentPurgeObjectIndex;
   }
   //真正调用析构函数释放内存的地方
   TickDestroyGameThreadObjects(bUseTimeLimit, TimeLimit, StartTime);
   return bCompleted;
}
```

在GC Sweep 阶段主要执行两个逻辑

- UnhashUnreachableObjects：调用不可达对象的ConditionalBeginDestroy()方法，最终会调用 BeginDestroy()
- IncrementalDestroyGarbage：调用不可达对象的ConditionalFinishDestroy()方法，最终会调用 FinishDestroy()

为什么要分这两个步骤呢？

BeginDestroy() 像是通知UObject对象，告知这个对象即将被销毁，销毁之前需要做什么事情这是最后的机会；只有确定清理完毕，FinishDestroy() 才能被调用。即便是FinishDestroy 方法，其内部其实也没有真正销毁对象，因为没有调用对象的析构函数。真正调用析构方法的地方是在 TickDestroyGameThreadObjects 内，它并不是在IncrementalDestroyGarbage 方法内直接调用，会根据GC的设置在下一帧或者规则的时间内执行

```cpp
// TickDestroyGameThreadObjects 对垃圾对象最后的处理 
FUObjectItem* ObjectItem = GUnreachableObjects[ObjCurrentPurgeObjectIndexOnGameThread];       
if (ObjectItem)
{
   //将GUObject内的对象设置为nullptr
   GUnreachableObjects[ObjCurrentPurgeObjectIndexOnGameThread] = nullptr;
   UObject* Object = (UObject*)ObjectItem->Object;
   //调用析构函数
   Object->~UObject();
   //释放内存
   GUObjectAllocator.FreeUObject(Object);
}
```

因此，GC以后可能会出现的情况是：isVaild(Obj) 仍为 true, 实际上这个对象已经被标记为 PendingKill，只是还未置空。因此可使用 isVaildLowLevel(Obj) 来判断更准确

```cpp
FORCEINLINE bool IsValid(const UObject *Test)
{
   return Test && !Test->IsPendingKill(); 
}

FORCEINLINE bool IsPendingKill() const
{
    //InternalIndex 指向的数组位置可能已经被其他对象占据
   return GUObjectArray.IndexToObject(InternalIndex)->IsPendingKill();
}
```

整个分析过程中，我省略了GC锁操作，多线程分析引用等，是为了更专注分析垃圾回收的流程；其实省略的最大一个块就是：Cluster。为什么需要Cluster ? 因为在游戏过程中很多对象的生命周期一致，是命运共同体。比如：粒子内的一堆东西其实就可以当成一个"对象"来处理，能加快分析速度。

### 总结

至此，当前GC算是真正结束，最后我们来梳理总流程

```cpp
void CollectGarbageInternal(EObjectFlags KeepFlags, bool bPerformFullPurge)
{
   FCoreUObjectDelegates::GetPreGarbageCollectDelegate().Broadcast();
   {
      FGCScopeLock GCLock;
      //可达性分析
      {
         PerformReachabilityAnalysis(...);
      }
      //解散簇
      if (GUObjectClusters.ClustersNeedDissolving())
      {
         GUObjectClusters.DissolveClusters();
      }      
      //收集不可达对象释放
      {
         GatherUnreachableObjects(bForceSingleThreadedGC);
         if (bPerformFullPurge || !GIncrementalBeginDestroyEnabled)
         {
            UnhashUnreachableObjects(false);//BeginDestory
         }
      }
      //全量回收
      //执行FinishDestory 必须IsReadyForFinishDestroy(异步对象清理完毕) 
      IncrementalPurgeGarbage(false);     
   }
   FCoreUObjectDelegates::GetPostGarbageCollect().Broadcast();
}
```

UE4的 GC 是一种追踪式、非实时、精确式、增量回收的内存管理机制来管理 UObject对象的内存；C++ Native对象需要我们自己把控（可使用智能指针减少内存泄漏风险）

# GC问答

**为什么UE不采用引用计数的垃圾回收方式？**

因为UE允许使用 UObject 的裸指针到处传递和赋值，而指针的传递和赋值底层没有办法捕捉这个操作，故引用计数法无法实施

**对象A引用了对象A本身，GC的时候会造成死循环吗？**

不会；对象A引用了A本身，但是在 GUObjectArray 内只有一个对象。因此在分析引用阶段，A身上没有不可达标记，会直接跳过而不会放入 NewObjectsToSerialize 从而造成死循环

**了解了GC的流程，你觉得应该怎么优化GC？**

1. 打开簇，将Character，Weapon等生命周期一致的 Actor 对象勾选 Cluster
2. 最好的优化还是减少UObject对象数量（包括：少用蓝图宏，Level内的Actor数量控制）
3. 优化GC调用时机，原则上能不调用就不调用，可在关键点调用
4. 采用对象池，不要频繁清理和生成大的对象
5. 优化源代码，将可达性分析这块看看能不能改成无锁的方式，加快速度

**UObject对象如果不想被GC回收，可以怎么做？**

1. UObject对象被 非UObject对象引用，有两种办法

- 让非UObject对象继承FGCObject，重写 AddReferencedObjects 方法
- 用 TStrongObjectPtr 强引用包装
- UObject对象被 UObject 对象引用，那么就有以下几种方式，我们用Actor来举例

```cpp
class ALingzeCharacter : public ACharacter
{
public:
   AActor* TestActor;
   UPROPERTY() AActor* TestActorWithProperty;//方式1
   AActor* TestActorWithAddReference;//方式2
   AActor* TestActorWithSetRoot;//方式3
   AActor* TestActorWithSpawnAPI;//方式4
   AActor* TestActorWithStrongPtr;//方式5
   
   TStrongObjectPtr<AActor> StrongTestPtr;
   static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);
   virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
   void OnGC();
   void BeginPlay();
};

void ALingzeCharacter::BeginPlay()
{
   //裸奔
   TestActor = NewObject<AActor>(this, TestActorClass, "TestActor");
   //UPROPERTY标记
   TestActorWithProperty = NewObject<AActor>(this, TestActorClass, "TestActorWithProperty"); 
   //重载AddReferencedObjects方法
   TestActorWithAddReference = NewObject<AActor>(this, TestActorClass, "TestActorWithAddReference"); 
   //设置为根节点
   TestActorWithSetRoot = NewObject<AActor>(this, TestActorClass, "TestActorWithSetRoot"); 
   TestActorWithSetRoot->AddToRoot();
   //World.SpawnActor生成(原理是放入Level的Actors数组内)
   TestActorWithSpawnAPI = GetWorld()->SpawnActor(TestActorClass);
  //强引用
   TestActorWithStrongPtr =  NewObject<AActor>(this, TestActorClass, "TestActorWithStrongPtr"); 
   StrongTestPtr = TStrongObjectPtr<AActor>(TestActorWithStrongPtr);
}

void ALingzeCharacter::AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector)
{
   ALingzeCharacter* This = CastChecked<ALingzeCharacter>(InThis);
   Collector.AddReferencedObject(This->TestActorWithAddReference);
}

void ALingzeCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
   //一定要加,否则会崩溃
   TestActorWithSetRoot->RemoveFromRoot();
   //清空强引用
   StrongTestPtr.Reset();
}

void ALingzeCharacter::OnGC()
{
   CollectGarbage(RF_NoFlags,true);
   //打印变量
   //LogTemp: TestActor IsValid: true; IsValidLowLevel false
   //LogTemp: TestActorWithProperty IsValid: true; IsValidLowLevel true
   //LogTemp: TestActorWithAddReference IsValid: true; IsValidLowLevel: true
   //LogTemp: TestActorWithSetRoot IsValid: true; IsValidLowLevel: true
   //LogTemp:TestActorWithSpawnAPI IsValid: true; IsValidLowLevel: true
   //LogTemp:TestActorWithStrongPtr IsValid: true; IsValidLowLevel: true
}
```

TestActor 用IsValid判断，GC后仍然有可能为true，这也验证了上文

**GC什么时候给对象标记 Pendingkill，又什么时候移除 PendingKill？**

GC不会给对象标记 PendingKill，PendingKill 是我们手动销毁对象时给它标记的。目的是告诉GC，这个被标记了PendingKill的对象就别瞎特么分析了，直接不可达就完事儿（如果是簇根直接解散该簇）

**不确定对象是否被GC了但是又想安全使用，有什么办法？**

可以用 TWeakObjectPtr 包装一下，一般多用于回调或者隔帧操作对象（比如：定时器回调）；原理可参考：[UObject智能指针](https://zhuanlan.zhihu.com/p/392864278)

**标记阶段那么耗时，那能分帧标记降低峰值吗？**

不可以。分帧的话，就会导致引用关系的改变，从而导致漏掉垃圾 (下一帧就手动删除了某个Actor) 以及错误的标记为不可达 (下一帧就执行了AddRoot )，不过倒是可以分帧销毁已经标记了不可达的对象

**TArray<TArrat<int32>>这样的双重数组定义合法吗？**

要分情况看待。如果没有标记UPROPERTY，该类型合法；如果标记了UPROPERTY 那么编译不通过，垃圾回收的 ReferenceTokenStream 内没有与之对应的类型，其实就是反射本身不支持网状结构（别问，问就是太复杂）

如果想要实现这种效果，可以用Struct包装TArray，最终类型是TArray<MyStruct>
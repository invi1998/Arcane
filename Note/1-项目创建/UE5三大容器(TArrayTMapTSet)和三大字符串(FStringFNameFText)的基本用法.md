# UE5三大容器(TArray/TMap/TSet)和三大字符串(FString/FName/FText)的基本用法

## 三大容器类(TArray/TMap/TSet)

### 1. TArray

TArray是UE中最常用的容器类，其基本原理比较接近std::vector，但std::vector无论是效率还是便捷性上都不适合在UE中直接使用，可能是出于类似的考虑UE大佬实现了自己一套容器类。

可以做个简单粗暴的小实验比较下两者在添加，遍历和删除元素的效率，大体代码如下：

```cpp
void CmpTArrayVector()
{
    int Count = 1000000;
    TArray<int> TestTArray;
    {
        PROFILE(TEXT("TArray add profile"));
        for (int i = 0; i < Count; i++)
        {
            TestTArray.Add(i);
        }
        UE_LOG(LogTemp, Warning, TEXT("TestTArray size is %d"), TestTArray.GetAllocatedSize());
        TestTArray.Shrink();
        UE_LOG(LogTemp, Warning, TEXT("TestTArray size after shrink is %d"), TestTArray.GetAllocatedSize());
    }
    {
        PROFILE(TEXT("TArray index profile"));
        int sum = 0;
        for (int i = 0; i < Count; i++)
        {
            sum += TestTArray.GetData()[i];
        }
    }
    {
        PROFILE(TEXT("TArray remove profile"));
        for (int i = 0; i < Count / 10; i++)
        {
            int Index = FMath::Rand() % TestTArray.Num();
            TestTArray.RemoveAtSwap(Index);
        }
    }

    std::vector<int> TestVector;
    {
        PROFILE(TEXT("std::vector add profile"));
        for (int i = 0; i < Count; i++)
        {
            TestVector.push_back(i);
        }
        UE_LOG(LogTemp, Warning, TEXT("std::vector size is %d"), TestVector.capacity() * sizeof(int));
        TestVector.shrink_to_fit();
        UE_LOG(LogTemp, Warning, TEXT("std::vector size after shrink is %d"), TestVector.capacity() * sizeof(int));
    }
    {
        PROFILE(TEXT("std::vector index profile"));
        int sum = 0;
        for (int i = 0; i < Count; i++)
        {
            sum += TestVector[i];
        }
    }
    {
        PROFILE(TEXT("std::vector remove profile"));
        for (int i = 0; i < Count / 10; i++)
        {
            int Index = FMath::Rand() % TestVector.size();
            TestVector.erase(TestVector.begin() + Index);
        }
    }
}
```

结果打印如下：

```csharp
LogTemp: Warning: TestTArray size is 5227328
LogTemp: Warning: TestTArray size after shrink is 4000000
LogTemp: Warning: TArray add profile cost time: 0.005936
LogTemp: Warning: TArray index profile cost time: 0.001216
LogTemp: Warning: TArray remove profile cost time: 0.002195
LogTemp: Warning: std::vector size is 4199476
LogTemp: Warning: std::vector size after shrink is 4000000
LogTemp: Warning: std::vector add profile cost time: 0.009769
LogTemp: Warning: std::vector index profile cost time: 0.001445
LogTemp: Warning: std::vector remove profile cost time: 33.533298(这一项由于内在处理差距较大故并不公平，仅做参考)
```

> ps: PROFILE是本人写的一个小测试执行时间的函数，由于出了作用域才打印所以打印结果晚于直接写的UE_LOG。

可以明显的看到添加元素，索引遍历和删除元素上TArray的效率高于std::vector，特别是删除元素上，std::vector由于每删除一个元素都要做大量的元素移动，而TArray可以用swap的方式直接和最后一个元素交换位置，免去了大量元素移动的开销。

**TArray的常用接口**

**此处只列举下常用接口，更详细的接口官网文档已经给的很详细了，[Array Containers in Unreal Engine | 虚幻引擎5.1文档](https://link.zhihu.com/?target=https%3A//docs.unrealengine.com/5.1/zh-CN/array-containers-in-unreal-engine/)**

1）创建、初始化和填充

```cpp
TArray<int32> IntArray;
IntArray.Init(10, 5); // IntArray == [10,10,10,10,10]
IntArray.Add(1);
IntArray.Emplace(2); // IntArray == [1,2]
int32 Temp[] = { 10, 20 };
IntArray.Append(Temp); // 填充数组
IntArray += IntArray; // 直接用+
```

2）遍历，有三种方式：

```cpp
//StrArr == ["Hello","Brave","World","of","Tomorrow","!"];
// 方式一
FString JoinedStr;
for (auto& Str :StrArr)
{
    JoinedStr += Str + TEXT(" ");
}
// 方式二
for (int32 Index = 0; Index != StrArr.Num(); ++Index)
{
    JoinedStr += StrArr[Index] + TEXT(" ");
}
// 方式三， 函数CreateIterator和CreateConstIterator可分别用于元素的读写和只读访问
for (auto It = StrArr.CreateConstIterator(); It; ++It)
{ 
    JoinedStr += *It + TEXT(" ");
}
// JoinedStr == "Hello Brave World of Tomorrow !"
```

3）排序，也有三种方式

```cpp
StrArr.Sort(); // 快排，不稳定
StrArr.HeapSort(); // 堆排序，不稳定
StrArr.StableSort(); // 归并排序，稳定

// 同时，可以直接传入lambda设置比较方式
StrArr.HeapSort([](const FString& A,const FString& B) {
    return A.Len() < B.Len();
});
```

4）查询

```cpp
int32 Count = StrArr.Num(); //获取数量
FString* StrPtr = StrArr.GetData(); //获取数组内存指针
FString Elem1 = StrArr[1]; // 下标索引

FString ElemEnd  = StrArr.Last(1); // 获取倒数第2个元素
FString ElemTop  = StrArr.Top(); // 获取最后的元素，等于StrArr.Last()，StrArr.Last(0)

bool bValidM1 = StrArr.IsValidIndex(-1); //检查下标
bool bHello   = StrArr.Contains(TEXT("Hello")); // 判断包含

// 谓词判断，判断数组中是否有满足谓词的元素
bool bLen5 = StrArr.ContainsByPredicate([](const FString& Str){
    return Str.Len() == 5;
});

// 查找元素并获取索引，找不到则返回INDEX_NONE 
int32 Index; 
int32 IndexLast;
StrArr.Find(TEXT("Hello"), Index); // Index == 3
StrArr.FindLast(TEXT("Hello"), IndexLast); // 倒序查找
Index = StrArr.Find(TEXT("Hello"));
IndexLast = StrArr.FindLast(TEXT("Hello"));
Index = StrArr.IndexOfByKey(TEXT("Hello")); // 适用于存在==运算符的元素类型
// 谓词查找
Index = StrArr.IndexOfByPredicate([](const FString& Str){
    return Str.Contains(TEXT("r")); //找到了Brave，返回2
});
// 获取元素指针，找不到则返回nullptr
auto* OfPtr  = StrArr.FindByKey(TEXT("of")));
// 同样支持谓词查找
auto* Len5Ptr = StrArr.FindByPredicate([](const FString& Str){
    return Str.Len() == 5;
});
```

5）删除

普通删除

```cpp
StrArr.Remove(TEXT("Hello")); // 删除所有等于入参的元素
StrArr.RemoveSingle(TEXT("Hello")); // 删除第一个等于入参的元素
StrArr.RemoveAt(1); // 删除下标1的元素
// 谓词匹配删除
StrArr.RemoveAll([](const FString& Str) {
    return Str.Len() == 5;
});
StrArr.Empty(); // 删除所有元素
```

通过swap实现高效删除

```cpp
StrArr.RemoveSwap(TEXT("Hello")); //删除所有等于入参的元素
StrArr.RemoveAtSwap(1); // 删除下标1的元素

// 谓词匹配删除
StrArr.RemoveAllSwap([](const FString& Str) {
    return Str.Len() == 5;
});
```

6）其他

```cpp
bool bComparison1 = ValArr1 == ValArr2; // 运算符判断数组相同与否
ValArr1 = MoveTemp(ValArr2); // 支持移动语义
ValArr1.Heapify(); // 将数组转为堆
```

### 2. TMap

TMap主要用于替代std::map系列，存储基于键值对的数据，这里同样进行一个简单粗暴的效率比较，大体代码如下：

```cpp
void FTestContainers::CmpTMapStdMap()
{
    int Count = 1000000;
    TMap<int, int> TestTMap;
    {
        PROFILE(TEXT("TMap add profile"));
        for (int i = 0; i < Count; i++)
        {
            TestTMap.Add(i, i);
        }
    }
    {
        PROFILE(TEXT("TMap index profile"));
        int sum = 0;
        for (const auto& KV : TestTMap)
        {
            sum += KV.Value;
        }
    }
    {
        PROFILE(TEXT("TMap remove profile"));
        for (int i = 0; i < Count / 10; i++)
        {
            TestTMap.Remove(i);
        }
    }

    std::map<int, int> TestStdMap;
    {
        PROFILE(TEXT("std::map add profile"));
        for (int i = 0; i < Count; i++)
        {
            TestStdMap.emplace(i, i);
        }
    }
    {
        PROFILE(TEXT("std::map index profile"));
        int sum = 0;
        for (const auto& [key, value] : TestStdMap)
        {
            sum += value;
        }
    }
    {
        PROFILE(TEXT("std::map remove profile"));
        for (int i = 0; i < Count / 10; i++)
        {
            TestStdMap.erase(i);
        }
    }
}
```

结果打印如下：

```csharp
TMap add profile cost time: 0.092569
TMap index profile cost time: 0.010703
TMap remove profile cost time: 0.003637
std::map add profile cost time: 0.197412
std::map index profile cost time: 0.022094
std::map remove profile cost time: 0.021095
```

可以看出TMap效率依旧优于std::map。

**TMap的常用接口**

**此处只列举下常用接口，更详细的接口官网文档同样给的很详细了，[Map Containers in Unreal Engine | 虚幻引擎5.1文档](https://link.zhihu.com/?target=https%3A//docs.unrealengine.com/5.1/zh-CN/map-containers-in-unreal-engine/)**

1）创建、初始化和填充

```cpp
TMap<int32, FString> FruitMap;
FruitMap.Add(5, TEXT("Banana"));
FruitMap.Emplace(3, TEXT("Orange"));
TMap<int32, FString> FruitMap2;
FruitMap.Append(FruitMap2); // 合并Map
```

2）遍历，有两种方式

```cpp
// 方式一
for (auto& Elem : FruitMap)
{
	FPlatformMisc::LocalPrint(
		*FString::Printf(
			TEXT("(%d, \"%s\")\n"),
			Elem.Key,
			*Elem.Value
		)
	);
}
// 方式二，函数CreateIterator和CreateConstIterator可分别用于元素的读写和只读访问
for (auto It = FruitMap.CreateConstIterator(); It; ++It)
{
	FPlatformMisc::LocalPrint(
		*FString::Printf(
			TEXT("(%d, \"%s\")\n"),
			It.Key(),   // same as It->Key
			*It.Value() // same as *It->Value
		)
	);
}
```

3）查询

```cpp
int32 Count = FruitMap.Num();
bool bHas7 = FruitMap.Contains(7); // 查询是否包含Key
FString Val7 = FruitMap[7]; // 获取Key对应的Value，此处一定要提前判断包含以免越界
FString* Ptr7 = FruitMap.Find(7); // 查询Key对应的Value，找到则返回value指针，找不到则返回nullptr
FString& Ref7 = FruitMap.FindOrAdd(7); // 查询Key对应的Value，找到则返回value引用，找不到则添加该Key
FString Val7 = FruitMap.FindRef(7); // 查询Key对应的Value，找到则返回value值，找不到则返回空值，如""
const int32* KeyMangoPtr   = FruitMap.FindKey(TEXT("Mango")); // 查询Vaule对应的Key
```

批量获取Key和Value

```cpp
TArray<int32>   FruitKeys;
TArray<FString> FruitValues;
FruitKeys.Add(999); // 无效添加
// GenerateKeyArray和GenerateValueArray会在填充前清空所传递的数组，因此产生的元素数量始终等于映射中的元素数量
FruitMap.GenerateKeyArray (FruitKeys);
FruitMap.GenerateValueArray(FruitValues);
```

4）删除

```cpp
FruitMap.Remove(8); // 移除Key==8的元素
FString Removed7 = FruitMap.FindAndRemoveChecked(7); // 移除Key==7的元素，没有则会触发断言
bool bFound2 = FruitMap.RemoveAndCopyValue(2, Removed7); // 移除并记录对应Value

FruitMap.Empty();  // 清空所有元素
FruitMap.Reset();   // 清空所有元素
```

5）排序，可以按Key或Value进行排序

```cpp
FruitMap.KeySort([](int32 A, int32 B) {
    return A > B; // sort keys in reverse
});

FruitMap.ValueSort([](const FString& A,const FString& B) {
    return A.Len() < B.Len(); // sort strings by length
});
```

6）其他

```cpp
// 支持深拷贝
TMap<int32, FString> NewMap = FruitMap;
// 支持移动语义
FruitMap = MoveTemp(NewMap);
```

### 3. TSet

TSet用于替代std::set，同样可以实现类似代码比较下效率

```csharp
TSet add profile cost time: 0.079112
TSet index profile cost time: 0.011911
TSet remove profile cost time: 0.003172
std::set add profile cost time: 0.192238
std::set index profile cost time: 0.007655
std::set remove profile cost time: 0.019182
```

结果也毫不意外，TSet效率远高于std::set。

**TSet的常用接口**

**此处只列举下常用接口，更详细的接口官网文档同样给的很详细了，[虚幻引擎中的Set容器 | 虚幻引擎5.1文档 (unrealengine.com)](https://link.zhihu.com/?target=https%3A//docs.unrealengine.com/5.%3C/b%3E1/zh-CN/set-containers-in-unreal-engine/)**

1）创建、初始化和填充

```cpp
TSet<FString> FruitSet;
FruitSet.Add(TEXT("Banana"));
FruitSet.Emplace(TEXT("Orange"));
TSet<FString> FruitSet2;
FruitSet.Append(FruitSet2); // 合并
```

2）遍历，有两种方式

```cpp
// 方式一
for (auto& Elem : FruitSet)
{
	FPlatformMisc::LocalPrint( *FString::Printf(TEXT(" \"%s\"\n"), *Elem) );
}
// 方式二，函数CreateIterator和CreateConstIterator可分别用于元素的读写和只读访问
for (auto It = FruitSet.CreateConstIterator(); It; ++It) {}
```

3）查询

```cpp
int32 Count = FruitSet.Num();
bool bHasBanana = FruitSet.Contains(TEXT("Banana"));
FString* PtrBanana = FruitSet.Find(TEXT("Banana")); // 查询并返回，如果不存在在返回nullptr
TArray<FString> FruitArray = FruitSet.Array(); // TSet转TArray
```

使用`FSetElementId`结构体可查找集合中某个键的索引，就可使用该索引与`运算符[]`查找元素

```cpp
FSetElementId BananaIndex = FruitSet.Index(TEXT("Banana"));
// BananaIndex is a value between 0 and (FruitSet.Num() - 1)
FPlatformMisc::LocalPrint(*FString::Printf(TEXT(" \"%s\"\n"), *FruitSet[BananaIndex]));
```

4）删除

```cpp
FruitSet.Remove(0); // 移除下标0的元素
int32 RemovedAmountPineapple = FruitSet.Remove(TEXT("Pineapple")); // 返回删除的元素下标，如果元素不存在则返回0

FruitSet.Empty();  // 清空所有元素
FruitSet.Reset();   // 清空所有元素
```

5）排序，需要定义比较函数

```cpp
FruitSet.Sort([](const FString& A, const FString& B) {
    return A.Len() < B.Len(); // sort strings by length, shortest to longest
});
```

6）其他

```cpp
// 支持深拷贝
TSet<FString> NewSet = FruitSet;
```

------

## 三大字符串类(FString/FName/FText)

### 1. FName

FName 通过一个轻型系统使用字符串。在数据表中也只存储一次，**不区分大小写，**作为索引组合存储在唯一字符串和实例编号的表格中，无法被操作。FNames 的存储系统和静态特性决定了通过键进行 FNames 的查找和访问速度较快。

其创建方式如下，

```cpp
FName TestName = FName(TEXT("xxx"));
```

FName转FString和FText，

```cpp
FString TestString = TestName.ToString();
FText TestText = FText::FromName(TestName);
```

FString转FName，FText转FName不存在，因为FName 不区分大小写，可以FText -> FString -> FName强行转，

```text
FName TestName = FName(*TestString);
```

### 2. FText

FText主要用于支持文本本地化，包含与本地化数据关联的字符串。本地化文本通常由三个部分组成（命名空间、密钥、字符串），其创建如下：

```cpp
// 定义要与LOCTEXT一起使用的命名空间
// 这只在单个文件中有效，并且必须在文件结束前取消定义
#define LOCTEXT_NAMESPACE "MyNamespace"
// 创建文本文字值
constFTextHelloWorld= NSLOCTEXT("MyOtherNamespace","HelloWorld","Hello World!")
constFTextGoodbyeWorld= LOCTEXT("GoodbyeWorld","Goodbye World!")
// 在文件结束前取消定义命名空间
#undef LOCTEXT_NAMESPACE
```

可从FString和FName创建

```cpp
FText TestText1 = FText::FromString(TestString);
FText TestText2 = FText::FromName(TestName);
```

可通过ToString()转到FString

### 3. FString

与FName和FText不同，FString可以与搜索、修改并且与其他字符串比较。不过，这些操作会导致FString的开销比不可变字符串类更大。这是因为FString对象保存自己的字符数组，而FName和FText对象保存共享字符数组的指针，并且可以完全根据索引值建立相等性。

常用方法

```cpp
// 创建
FString TestString = FString(TEXT("xxx"));
FString TestString2 = FString::Printf(TEXT("%d"), AnyInt);

// 转FString
TestString = FString::SanitizeFloat(FloatVariable);
TestString = FString::FromInt(IntVariable);
TestString = VectorVariable.ToString();
TestString = Vector2DVariable.ToString();
TestString = RotatorVariable.ToString();
TestString = LinearColorVariable.ToString();

// 转其他
bool bTest = TestString.ToBool();
int  IntTest = FCString::Atoi(*TestString);
float FloatTest = FCString::Atof(*TestString);
```

其他也**包含Equals、Contains、Find、Reverse、Replace等方法**，方法十分丰富，直接源码中查找即可（\Engine\Source\Runtime\Core\Public\Containers\UnrealString.h）
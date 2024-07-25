# UE5 C++ Interface



### 概述

- 接口类有助于确保一组（可能）不相关的类实现一组通用函数

### 接口声明

- 声明接口类与声明普通的虚幻类相似，但仍有两个主要区别

  - 首先，接口类使用`UINTERFACE`宏而不是`UCLASS`宏，且直接从`UInterface`而不是`UObject`继承
  - 其次，`UINTERFACE`类不是实际的接口；它是一个空白类，它的存在只是为了向虚幻引擎反射系统确保可见性。将由其他类继承的实际接口必须具有相同的类名，但是开头字母"U"必须改为"I"。
  - 如果想让蓝图实现此接口，则需要`Blueprintable`说明符

  ```cpp
  #pragma once
  
      #include "ReactToTriggerInterface.generated.h"
  
      UINTERFACE(MinimalAPI, Blueprintable)
      class UReactToTriggerInterface : public UInterface
      {
          GENERATED_BODY()
      };
  
      class IReactToTriggerInterface
      {
          GENERATED_BODY()
  
      public:
          /** 在此处添加接口函数声明 */
      };
  ```

  - 前缀为U的类不需要构造函数或任何其他函数，而前缀为I的类将包含所有接口函数，且此类实际上将被你的其他类继承

### 接口说明符

- `BlueprintType` 将该类公开为可用于蓝图中的变量的类型

- ```
  DependsOn=(ClassName1, ClassName2, ...)"
  ```

  - 所有列出的类都将在该类之前编译。ClassName必须在同一个（或上一个）包中指定一个类。多个依赖性类可以使用以逗号分隔的单个"DependsOn"行来指定，也可以使用单个"DependsOn"行为每个类指定。当一个类使用在另一个类中声明的结构体或枚举时，这一点非常重要，因为编译器只知道它已经编译的类中的内容。

- ```
  "MinimalAPI"
  ```

  - 仅导致该类的类型信息被导出以供其他模块使用。你可以向该类转换，但不能调用该类的函数（内联方法除外）。对于不需要其所有函数在其他模块中均可供访问的类，通过不导出这些类的所有内容，这可以缩短编译时间

### c++中实现接口

- 需从"前缀为I（I-prefixed）"的接口类继承（除了你正在使用的任何基于"UObject"的类）即可

  ```cpp
   #include "CoreMinimal.h"
      #include "GameFramework/Actor.h"
      #include "ReactToTriggerInterface.h"
      #include "Trap.generated.h"
  
      UCLASS(Blueprintable, Category="MyGame")
      class ATrap : public AActor, public IReactToTriggerInterface
      {
          GENERATED_BODY()
  
      public:
          /** Add interface function overrides here. */
      }
  ```

### 声明接口函数

- 仅C++的接口函数

  - 不带`UFUNCTION`说明的虚函数，以便在实现接口的类中覆盖它们, 但这些C++接口函数对**蓝图不可见**

  ```csharp
  public:
      virtual bool ReactToTrigger();
  ```

  - 当在一个`Actor`类中实现接口后，可以创建并实现一个针对该类的覆盖

  ```csharp
   public:
      virtual bool ReactToTrigger() override;
  ```

- 蓝图可调用接口函数

  - 要创建蓝图可调用的接口函数，必须在带`BlueprintCallable`说明符的函数声明中提供一个`UFUNCTION`宏
  - 还必须使用`BlueprintImplementableEvent`或`BlueprintNativeEvent`说明
  - 不能为虚函数

  ```csharp
   public:
      /**只能在蓝图中实现的React To Trigger版本。*/
      UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category=Trigger Reaction)
      bool ReactToTrigger();
  
  public:
      /**可以在C++或蓝图中实现的React To Trigger版本。*/
      UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Trigger Reaction)
      bool ReactToTrigger();
  ```

### 确定类是否实现了接口

- 为了与实现接口的C++和蓝图类兼容，可以使用以下任意函数

  ```rust
  // 如果OriginalObject实现了    UReactToTriggerInterface，则bisimplemated将为true
  bool bIsImplemented = OriginalObject->GetClass()->ImplementsInterface(UReactToTriggerInterface::StaticClass()); 
  
  // 如果OriginalObject实现了UReactToTrigger，bIsImplemented将为true
  bIsImplemented = OriginalObject->Implements<UReactToTriggerInterface>(); 
  
  // 如果OriginalObject实现了UReactToTriggerInterface，则ReactingObject将为非空
  IReactToTriggerInterface* ReactingObject = Cast<IReactToTriggerInterface>(OriginalObject); 
  ```

### 类型转换

- 虚幻引擎的转换系统支持从一个接口转换到另一个接口，或者在适当的情况下，从一个接口转换到一个虚幻类型

  ```swift
   // 如果接口被实现，则ReactingObject将为非空。
   IReactToTriggerInterface* ReactingObject = Cast<IReactToTriggerInterface>(OriginalObject); 
   
   // 如果ReactingObject为非空而且还实现了ISomeOtherInterface，则DifferentInterface将为非空。
   ISomeOtherInterface* DifferentInterface = Cast<ISomeOtherInterface>(ReactingObject); 
  
   // 如果ReactingObject为非空且OriginalObject为AActor或AActor派生的类，则Actor将为非空
   AActor* Actor = Cast<AActor>(ReactingObject); 
  ```


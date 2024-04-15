## Abstract

对于基础的C++基类，我们希望后续的角色类都基于该类创建蓝图，所以，某种意义上来说，我们并不希望该类被拖入关卡中，所以，要实现这种愿景，我们可以在类宏中添加抽象修饰符

```C++
UCLASS(Abstract)	// 添加Abstract关键字，表示这个类是一个抽象类
class ARCANE_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
```

在虚幻引擎5（UE5）的蓝图系统或C++编程中，`Abstract` 关键字并非UE5特有，而是来源于面向对象编程（OOP）的概念。在C++中，`Abstract` 关键字用于定义抽象类（Abstract Class），也被称为接口类或者纯虚基类。在UE5中，无论是蓝图还是C++代码，都可以创建抽象类。

在C++中，抽象类的特点如下：

1. 抽象类不能被实例化，也就是说，不能直接创建抽象类的对象。
2. 抽象类中至少包含一个纯虚函数（Virtual Function），纯虚函数在声明时前面带有`= 0`，表示没有提供实现。
3. 抽象类的目的通常是为了被其他类继承，作为基类，强迫派生类提供纯虚函数的实现。

在UE5中，抽象类通常用于定义一种规范或协议，要求所有继承它的类必须实现某些方法。例如，在游戏开发中，可以创建一个抽象的游戏角色基类，规定所有游戏角色都需要实现移动、攻击等基本功能，但是具体怎么实现这些功能由派生的特定角色类决定。

在蓝图中，虽然没有直接的`Abstract`关键字，但蓝图类可以通过创建一个没有实现的函数蓝图节点（Blueprint Function）来模拟抽象类的行为。在C++和蓝图混合编程时，C++中的抽象类可以被蓝图类继承，此时蓝图类也需要实现C++抽象类中所有的纯虚函数。



## TObjectPtr

在虚幻引擎（Unreal Engine，包括UE5）中，`TObjectPtr` 是一个智能指针类型，主要用于管理和操作Unreal Engine中的 UObject 类及其派生类。`TObjectPtr` 提供了对 UObject 的引用计数式内存管理，确保对象在不再被引用时自动释放，从而简化了资源管理和内存泄漏的问题。

在C++代码中，`TObjectPtr<UClass>` 或 `TObjectPtr<AActor>` 等声明了一个可以自动管理 UObject 生命周期的智能指针。例如：

```cpp
TObjectPtr<AActor> MyActor;

// 创建一个Actor实例并赋值给智能指针
MyActor = ConstructObject<AActor>(AActor::StaticClass(), GetTransientPackage(), TEXT("MyActorName"));

// 使用完后，无需手动释放，TObjectPtr会根据引用计数自动处理对象的销毁
```

`TObjectPtr` 还具备 nullptr 检查、自动转换为 UObject* 的特性，并且支持隐式转换为bool值，方便在条件语句中使用。同时，它还提供了许多便捷的操作符重载方法，如解引用 (`*`)、箭头 (`->`) 和赋值 (`=`) 等。

`TObjectPtr` 类型自身并不直接提供延迟加载机制，但它经常用于管理那些可能支持延迟加载的对象，尤其是在虚幻引擎（Unreal Engine）中。

在虚幻引擎中，`UObject` 类及其派生类（如`AActor`、`UClass`等）的加载和卸载是由虚幻引擎的资源管理器来控制的，其中包括了延迟加载机制。当使用 `TObjectPtr` 管理这些对象时，如果对象尚未加载，`TObjectPtr` 的引用计数机制会确保在对象实际需要时（例如通过解引用 `*` 或成员访问 `->` 时）触发对象的加载。

举个例子，当你通过 `TObjectPtr<AActor>` 持有一个蓝图或C++派生的Actor的路径或类名，而不是实际的Actor实例时，首次访问这个Actor的属性或方法时，如果Actor尚未加载，引擎会尝试自动加载这个Actor。

简而言之，虽然 `TObjectPtr` 不是专门设计为延迟加载工具，但在处理虚幻引擎资源时，它会配合引擎的底层资源管理系统，间接参与到延迟加载的过程中。

因此，标准用法是更倾向于在成员变量中使用TObjectPtr对象指针



## TObjectPtr优点

`TObjectPtr` 是虚幻引擎（Unreal Engine）中的一个智能指针类型，主要用于管理和引用`UObject`派生类实例。在UE中，所有游戏对象都是`UObject`的子类，如`AActor`、`UClass`、`UMaterial`等。`TObjectPtr` 通过引用计数（Reference Counting）机制实现了自动内存管理，确保当一个对象不再被任何地方引用时，能够及时释放内存，从而避免内存泄露。

使用`TObjectPtr` 的主要原因和好处包括：

1. **自动内存管理**：
   - 当对象的引用计数变为零时，`TObjectPtr` 会自动销毁所引用的对象，大大降低了手动管理对象生命周期的复杂度和出错风险。

2. **安全性**：
   - `TObjectPtr` 内部会检查其所引用的对象是否存在，如果对象在外部已经被销毁，`TObjectPtr` 会自动变为无效状态，防止对已释放对象的非法访问。

3. **兼容性**：
   - `UObject` 系统在虚幻引擎中有着特殊的加载和卸载机制，`TObjectPtr` 与这个系统紧密结合，确保在对象跨包加载或者资源卸载时，能够正确处理对象引用。

4. **性能优化**：
   - `TObjectPtr` 支持弱引用（Weak Pointer），当不需要维持对象的生存期时，可以使用弱引用避免增加对象的引用计数，有利于降低内存压力和优化性能。

5. **简洁的API**：
   - `TObjectPtr` 提供了与原始指针相似的语法和操作方式，如解引用（dereferencing）、赋值（assignment）等，同时增加了额外的安全性和便利性。

在虚幻引擎的C++类设计中，推荐使用`TObjectPtr`作为成员属性的主要原因在于它可以简化资源管理和避免内存泄漏问题，使代码更加健壮和易于维护。同时，对于跨越不同执行上下文（如游戏模式、关卡流送等）的对象引用，`TObjectPtr` 能够很好地处理潜在的生命周期问题。



## 和C++智能指针的区别

`TObjectPtr` 是虚幻引擎（Unreal Engine）中的智能指针类型，而C++标准库中包含了几种智能指针，如`std::unique_ptr`、`std::shared_ptr`和`std::weak_ptr`。虽然它们都是为了自动化内存管理，但`TObjectPtr` 和 C++标准智能指针有一些显著的异同点：

**共同点**：
- **自动内存管理**：`TObjectPtr` 和 C++标准智能指针都能够自动管理对象的生命周期，当不再有引用指向对象时，会自动释放内存，避免内存泄漏。

**不同点**：
- **特化设计**：`TObjectPtr` 是专门为虚幻引擎中的`UObject`类设计的，它理解虚幻引擎特有的对象加载和卸载机制，适合处理跨包加载、对象序列化等复杂情况。

- **引用计数机制**：`TObjectPtr` 内部使用了虚幻引擎的引用计数系统，而C++标准库中的智能指针机制各异，如`std::unique_ptr`是独占所有权，`std::shared_ptr`是共享所有权（基于引用计数），`std::weak_ptr`是弱引用。

- **对象生命周期管理**：
   - `TObjectPtr` 与虚幻引擎的垃圾回收机制紧密集成，确保即使在游戏对象跨关卡、跨流加载等情况也能正确管理对象的生命周期。
   - C++标准智能指针独立于任何特定的游戏引擎或框架，适用于一般的C++程序内存管理。

- **API和特性**：
   - `TObjectPtr` 提供的功能和服务直接针对虚幻引擎对象的特性，如支持弱引用、跨包加载等特定功能。
   - C++标准智能指针提供了更通用的内存管理解决方案，适用于更广泛的C++编程场景，但不一定直接支持特定游戏引擎的高级特性。

- **互操作性**：
   - `TObjectPtr` 只能在虚幻引擎项目中使用，与C++标准库智能指针没有直接的互操作性。
   - C++标准智能指针可以在任何符合C++标准的环境中使用，具有更好的通用性和移植性。

总结来说，`TObjectPtr` 是为虚幻引擎定制的智能指针类型，它的设计目标是解决虚幻引擎中`UObject`类及其派生类的内存管理问题，而C++标准智能指针提供了适用于通用C++编程的内存管理工具。虽然它们都属于智能指针范畴，但适用范围和实现机制各有侧重。



# Actor对象和AvatarActor对象

在虚幻引擎5（UE5）中，Actor对象和AvatarActor对象都是继承自Actor类的对象，但它们在功能和用途上有一定的差异。

Actor对象：
Actor对象是UE5中所有可交互游戏对象的基础类。它定义了基本的属性和行为，如位置、旋转、缩放等。Actor对象可以包含组件，这些组件可以提供额外的功能，如物理模拟、碰撞检测、动画播放等。Actor对象可以表示游戏世界中的各种实体，如场景中的静态物体、可移动的物体、触发器、光源等。

AvatarActor对象：
AvatarActor对象是专门用于表示玩家角色或AI控制的非玩家角色（NPC）的Actor子类。它通常包含一个AvatarAsset组件，用于加载和播放角色的动画序列。AvatarActor对象还包含了与玩家输入相关的逻辑，如移动、跳跃、攻击等。AvatarActor对象通常具有更复杂的动画系统和行为树，以支持更丰富的角色动画和智能行为。

总结来说，Actor对象是UE5中所有可交互游戏对象的通用基础类，而AvatarActor对象则是专门用于表示玩家角色或AI控制的非玩家角色的Actor子类。AvatarActor对象通常具有更复杂的动画系统和行为树，以支持更丰富的角色动画和智能行为。



# BlueprintNativeEvent

在虚幻引擎（Unreal Engine）中，BlueprintNativeEvent 是一种特殊的函数声明，用于指示蓝图类中的函数可以被 C++ 类继承。这种函数声明通常用于将蓝图类的功能扩展到 C++ 类中，以便在 C++ 类中实现更多的功能。

具体来说，BlueprintNativeEvent 函数声明通常包含以下几个部分：

1. 函数签名：包括函数名、参数列表和返回类型等信息。

2. BlueprintNativeEvent 关键字：用于指示该函数是一个蓝图原生事件，可以被 C++ 类继承。

3. 属性：用于指定该函数是否可以被重写，以及是否需要实现默认的 C++ 实现。

例如，以下是一个简单的 BlueprintNativeEvent 函数声明的例子：

```cpp
UFUNCTION(BlueprintNativeEvent)
void NotifyPlayerDamaged(float DamageAmount);
```

在这个例子中，函数名为 NotifyPlayerDamaged，参数为 float DamageAmount。关键字 BlueprintNativeEvent 表示这是一个蓝图原生事件，可以被 C++ 类继承。属性部分可以指定该函数是否可以被重写，以及是否需要实现默认的 C++ 实现。

在 C++ 类中，可以继承这个蓝图原生事件，并实现自己的 C++ 版本的 NotifyPlayerDamaged 函数。这样，当蓝图类中的 NotifyPlayerDamaged 函数被调用时，C++ 类中的实现也会被调用，从而实现了蓝图类和 C++ 类之间的功能扩展。

我们知道，在UE里，如果一个函数被指定为在蓝图中进行实现，那么这个函数是无法成为虚函数的，

```c++
UFUNCTION(BlueprintImplementableEvent)
```

但是，如果我们在接口类中，需要提供一个可被子类重写的函数，但是又希望该哈数是一个蓝图实现函数，这种情况，我们就应该使用BlueprintNativeEvent声明。


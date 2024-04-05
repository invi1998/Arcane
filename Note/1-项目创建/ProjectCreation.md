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
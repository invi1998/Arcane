# Motion Warping

在虚幻引擎5（UE5）中，Motion Warping 是一种技术，用于实时调整和优化动画序列，以适应游戏中的各种情况和变化。Motion Warping 主要应用于两个方面：动画混合空间（Animation Blend Spaces）和动画重定向（Animation Retargeting）。

1. 动画混合空间：动画混合空间允许在运行时动态混合动画，以适应不同的运动状态和方向。例如，当角色从跑步变为跳跃时，可以使用动画混合空间来平滑过渡这两个动作。通过这种方式，可以创建出更加自然和流畅的角色运动。

2. 动画重定向：动画重定向允许将预设的动画序列重新映射到新的目标骨架上，以适应不同角色的体型和骨骼结构。这使得可以在不重新制作整个动画序列的情况下，将动画应用到不同角色上。例如，可以将为人类角色制作的动画序列重新映射到四足动物角色上，以适应它们不同的身体结构。

Motion Warping 技术在 UE5 中得到了进一步增强，提供了更多的灵活性和控制力，使得开发者能够更好地调整和优化动画，以适应游戏中的各种需求。



现在，我们的角色已经能够指向性的发射火球，还有一点小瑕疵是，当前角色在发射火球的时候，不会改变自身朝向，这将是接下来需要解决的。

要在蓝图中添加Motion Warping，需要先添加插件

![image-20240413181433236](.\image-20240413181433236.png)

然后去角色蓝图添加 Motion Warping，（记得先去想要调整的动画资产里先开启允许根运动，比如这里我希望施法动作能释放的时候朝向怪物，所以我开启的就是这个动画）

![image-20240413181830005](.\image-20240413181830005.png)

然后去到我们的蒙太奇蓝图，添加Motion设置，如下

![image-20240413183030487](.\image-20240413183030487.png)

（为什么我们要关闭Warp Translation，因为我们想要的效果是让动画旋转我们的根骨骼来面向我们的目标，而不是通过扭曲来实现）

那，现在对于动画来说，还需要解决一个问题，就是这个Target是什么，这是我们必须要设置的。因为这事火球术的动画，所以自然，这个Target将有 GA_FireBolt提供。

回到角色蓝图，添加一个自定义事件，事件传入一个Vector向量参数，target方向，该事件主要做的就是让我们的MotionWarping朝向目标方向。蓝图如下

![image-20240413183843367](.\image-20240413183843367.png)

然后回到GA_FireBolt蓝图中，在播放蒙太奇动画节点之前，插入这段获取并朝向目标的蓝图，这样我们就能看到角色在施法的时候，正确的朝向了施法目标了。

![image-20240413184457269](.\image-20240413184457269.png)

# 更灵活的接口

这种方式其实写死了只能角色使用，但是如果我想要让怪物施法也有这个朝向功能，是不是又得来该蓝图，所以，更灵活的选择是使用接口

>记住：如果是需要蓝图实现的函数（BlueprintImplementableEvent），我们是不能把它定义为虚函数的，所以，不要想着CombatInterface是一个接口类，就想当然得像吧它里面的函数都写成虚函数。
>
>当然，如果非要实现那种在子类有差异化实现的虚接口，那就不要定义为蓝图实现，去子类代码中通过代码实现也是一样的

记得将接口类型设置为蓝图类型，因为我们要在蓝图中使用它，我们需要将Actor在蓝图中Cast到CombatInterface对象，所以需要保证我们的UCombatInterface是一个蓝图类型

```c++
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)	// 将接口类型设置为蓝图类型
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCANE_API ICombatInterface
{
	GENERATED_BODY()
	
public:
	virtual int32 GetPlayerLevel() const;

	virtual FVector GetCombatSocketLocation() const;	// 获取战斗插槽位置

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)		// 蓝图实现，同时蓝图可调用
	void UpdateFacingTarget(const FVector& Target);	//更新面向目标
};

```

然后，回到蓝图中，替换掉之前的自定义事件

![image-20240413190653183](.\image-20240413190653183.png)

然后回到GA_FireBolt蓝图中，修改节点

![image-20240413191511798](.\image-20240413191511798.png)

这样，主要继承了CombatInterface接口的角色，就都能使用这个FaceTarget功能了





# 自定义碰撞通道

现在，还有一个问题就是我们的弹道会和比如场景中的药水碰撞，和这些静态物体碰撞是我们不希望看见的，所以这里新增一个碰撞通道

![image-20240413213603583](.\image-20240413213603583.png)

项目头文件（Arcane.h）

```c++
#include "CoreMinimal.h"

#define CUSTOM_DEPTH_STENCIL_RED 250	// 红色自定义深度缓冲值

#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1	// 子弹碰撞频道

```


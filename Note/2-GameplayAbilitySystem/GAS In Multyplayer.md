# GAS in Multyplayer

在多人游戏中，UE5的Gameplay Ability System（GAS）扮演着至关重要的角色，确保各个玩家的角色技能在网络环境下的同步和一致性。以下是在多人游戏中使用GAS的关键要点：

1. **网络同步与预测**: GAS设计时考虑到了网络同步的需求，能够支持服务器权威模式下的技能施放和效果应用。服务器会验证并执行所有的Ability激活请求，然后将结果广播给客户端。为了减少延迟感，客户端通常会对玩家的输入进行预测，即在服务器确认前先行模拟技能的施放和效果，随后再通过服务器回滚或修正不一致的行为。

2. **Gameplay Effects Replication**:
   - 当一个Ability在网络环境中触发时，相关联的Gameplay Effects会被复制到所有客户端。GAS通过Replication机制确保这些效果在网络上的准确传播，包括属性增益、减益、持续伤害等状态的变化。

3. **RepNotify Callbacks**:
   - 使用RepNotify功能，服务器可以通知客户端特定属性或状态的变化，这样客户端可以根据服务器的决定来实时更新本地表现，例如角色的生命值变动、技能冷却状态等。

4. **Ability授权与撤销**:
   - 在多人游戏中，服务器需要控制每个玩家何时获得或失去使用特定技能的能力。GAS提供了机制来追踪和管理每个玩家Controller拥有的Abilities，确保不会出现非法或未授权的Ability使用。

5. **技能冷却同步**:
   - 技能冷却时间在多人游戏中同样需要精确同步。服务器计算和管理Ability冷却，客户端则依据服务器的指示更新用户界面及阻止非法的提前再次施放。

6. **反作弊保护**:
   - GAS可以通过严格的服务器验证来防止作弊行为，比如确保玩家不能随意增加资源（如能量点数）、绕过冷却时间或非法施放未经授予的技能。

7. **游戏逻辑一致性**:
   - GAS支持编写清晰的游戏逻辑，无论是简单的一次性技能施放还是复杂的状态机转换，都能在网络环境下保持一致的执行顺序和结果。

总的来说，UE5的Gameplay Ability System为多人游戏提供了强大的基础架构，使得开发者能够在创建丰富多样的角色技能的同时，确保这些技能在网络环境中的稳定性和公平性。通过合理配置和优化GAS，开发者能够构建出既有趣又具备良好网络性能的多人游戏体验。



## 这与传统的UE里的RPC有什么异同吗

UE5的Gameplay Ability System（GAS）与传统虚幻引擎（UE）中的远程过程调用（RPC）机制有着不同的用途和应用场景，但它们在网络同步和通信方面有一定的协同工作。

**传统UE中的RPC**：

- RPC是一种跨网络通信的技术，允许服务器与客户端之间互相调用函数，实现数据的同步和指令的传输。
- RPC分为Server RPC和Client RPC，分别用于服务器对客户端和客户端对服务器的直接函数调用。
- 在多人游戏中，RPC常用于处理非即时游戏状态的同步，例如通知客户端更新UI，或者客户端请求服务器执行特定操作等。

**UE5的GAS**：
- GAS专注于游戏内的技能、状态和效果的管理，这些往往是直接影响游戏实时动态和平衡性的核心元素。
- GAS在网络同步方面采取的是服务器权威模型，即技能的施放、效果的触发和结算都在服务器上进行，并通过网络同步给客户端。
- GAS中，技能的执行和效果的复制（Replication）是系统级别的，更关注的是实时游戏逻辑的正确执行，包括冷却时间、消耗资源、触发特效、改变角色属性等。

**异同点**：
- 异同点在于，RPC主要用于通用的消息传递和调用，而GAS则专用于游戏内角色技能系统的设计与同步。
- 在多人游戏中，RPC和GAS可以相互补充。例如，使用RPC进行非游戏状态相关操作的同步，而GAS则确保所有与角色能力相关的动作在网络上的准确执行和同步。
- GAS实际上也是建立在UE的网络同步机制基础上的，它内部可能也会使用到RPC或者其他网络通信技术来确保服务器和客户端之间数据的一致性。

综上所述，GAS是针对游戏内角色能力这一特定领域设计的高度抽象化的框架，它在很大程度上封装和优化了网络通信和同步逻辑，减轻了开发者在实现多人游戏中技能同步方面的负担。而传统的RPC则是更底层的网络通信工具，适用于更广泛的应用场景。



## Multiplayer

![image-20240405234809753](.\image-20240405234809753.png)

在UE5中，一些游戏属性和类确实有不同的存在位置，下面列举了一些常见的游戏组件，并说明它们在网络环境下的存在形式：

1. **GameMode**：
   - GameMode类在单人游戏中存在于服务器和客户端。在多人游戏中，服务器端拥有唯一的GameMode实例，客户端上也有一个GameMode实例，但客户端的GameMode更多的是为了同步服务器状态和控制本地玩家行为。
   - 在服务器端，GameMode负责游戏规则的制定、回合开始与结束、玩家重生等全局游戏逻辑。
   - 客户端上的GameMode主要用于同步服务器的设置，并驱动本地游戏规则的表现。

2. **GameState**：
   - GameState类也同时存在于服务器和客户端，但真正的游戏状态数据（如分数、游戏阶段等）是由服务器维护的单一权威实例。
   - 服务器上的GameState记录并更新游戏的全局状态，然后通过网络复制（Replication）将状态同步给所有客户端。
   - 客户端上的GameState接收服务器同步来的状态，并据此更新本地游戏世界的呈现。

3. **PlayerController**：
   - PlayerController通常在每个玩家的客户端都有实例，而且服务器上也为每个玩家创建一个PlayerController实例。
   - 服务器端的PlayerController负责处理玩家输入和执行服务器端的AI逻辑（如果适用）。
   - 客户端的PlayerController负责本地玩家的输入处理、摄像机控制和本地预测。

4. **Pawn/ACharacter** 和 **PlayerState**：
   - Pawn和ACharacter（角色）在网络游戏中同样在服务器和客户端都存在。
   - 服务器端的Pawn/Character实例负责处理游戏逻辑，如移动、交互等，并通过网络复制同步到客户端。
   - 客户端的Pawn/Character实例负责本地预测和渲染。
   - PlayerState保存每个玩家持久化的数据，比如得分、等级等，同样在服务器和客户端都有实例，但数据变更由服务器决定并同步给客户端。

在不同网络架构下：

- **监听服务器（Listen Server）**：
  - 在监听服务器模式下，同一个进程既充当服务器又充当一个客户端，所有游戏组件（GameMode、GameState、PlayerController、Pawn/Character和PlayerState）在本地都存在两个版本，一个是作为服务器的权威版本，另一个是作为客户端的本地副本。

- **专用服务器（Dedicated Server）**：
  - 在专用服务器模式下，服务器上只存在服务器侧的游戏组件实例，负责维护游戏的逻辑和状态，而客户端只存在客户端侧的游戏组件实例，负责本地渲染和输入处理。专用服务器模式下，服务器和客户端的责任划分更为明确，服务器专注于游戏逻辑，客户端专注于用户体验。  

总结来说，大多数游戏组件在UE5的多人游戏中都会在服务器和客户端两边都存在，但实际的数据和逻辑处理根据组件的性质和网络架构的不同，会在服务器或客户端进行相应的操作和同步。



## Replication是双向的吗

在虚幻引擎（Unreal Engine，包括UE5）中，**Replication（复制）不是双向的，而是单向的，且是从服务器到客户端的**。服务器上的游戏逻辑和状态变化会被复制到所有连接的客户端，确保所有玩家看到的世界状态一致。这是因为服务器是游戏逻辑的权威来源，以防止客户端篡改游戏状态导致的不公平现象。

客户端不能直接将变化复制回服务器，但客户端可以通过以下方式向服务器发送信息和请求：

1. **远程过程调用（RPCs）**：
   - 客户端可以发起客户端到服务器（Client-to-Server）的RPC调用，请求服务器执行某个函数并将结果返回给客户端。这是一种从客户端向服务器发送信息和请求服务器执行动作的方式。

2. **输入转发**：
   - 客户端可以将玩家的输入信息发送到服务器，服务器接收到输入后，基于这些输入更新游戏状态，并将更新后的状态复制回所有客户端。

3. **Actor组件的网络功能**：
   - 客户端可以触发特定的网络事件，如网络通知（RepNotify）或网络代理（NetProxy）函数，这些函数在服务器上执行并产生相应的游戏逻辑变化。

总之，虽然Replication本身是单向的，但客户端可以通过各种网络通信机制间接地影响服务器的状态，而服务器对这些请求的处理结果会通过Replication同步到所有客户端。



## 设置复制模式 AbilitySystemComponent->SetReplicationMode



Minimal, Mixed 和 Full 这三种模式是针对 GameplayEffect（游戏效果）的 replication settings，而不是 `AbilitySystemComponent` 的 `SetReplicationMode` 方法。

在 Unreal Engine 5 的 Gameplay Ability System 中，GameplayEffect 的 replication settings 包括：

1. **Minimal Replication**：
   - 仅在初始应用 GameplayEffect 时，基本信息（例如，效果的ID和持续时间）会被复制到客户端。后续的效果更新（如随时间变化的数值）不会被复制。

2. **Mixed Replication**：
   - 初始应用时的基本信息会被复制，同时一些关键的数值更新（如造成伤害或治疗量）也会被复制到客户端。此模式提供了比 Minimal 更多的同步信息，但不是所有数值变化都会复制。

3. **Full Replication**：
   - 所有 GameplayEffect 的相关信息都会被复制到客户端，包括初始应用时的信息以及所有后续的数值更新。这是最完整的同步模式，保证了客户端和服务器之间的完全一致。

在 Unreal 编程中，这些设置通常在创建 GameplayEffectSpec 时指定，例如：

```cpp
// 创建 GameplayEffectSpec
FGameplayEffectSpecHandle SpecHandle = FGameplayEffectSpec::CreateAndSetDuration(GetDefault<UGameplayEffect>(), Duration, EffectContext);

// 设置 replication 设置
SpecHandle.Data.SetReplicationMode(EGameplayEffectReplicationMode::Full); // 示例，设置为全复制模式

// 应用 GameplayEffect
AbilitySystemComponent->ApplyGameplayEffectToTarget(SourceActor.Get(), TargetActor.Get(), SpecHandle, 1.0f);
```
![image-20240406003451466](.\image-20240406003451466.png)

这张图片显示的是不同类型的复制模式（Replication Mode），以及它们各自的应用场景。根据表格内容，我们可以看到以下三种复制模式及其描述：

1. Full Replication Mode（全复制模式）：
   - 使用场景：单人游戏（Single Player）
   - 描述：所有游戏效果都会被复制到所有客户端。

2. Mixed Replication Mode（混合复制模式）：
   - 使用场景：多人游戏，玩家控制（Multiplayer, Player-Controlled）
   - 描述：游戏效果只被复制到拥有者客户端。游戏提示和游戏标签被复制到所有客户端。

3. Minimal Replication Mode（最小化复制模式）：
   - 使用场景：多人游戏，AI控制（Multiplayer, AI-Controlled）
   - 描述：游戏效果不被复制。游戏提示和游戏标签被复制到所有客户端。

这些复制模式在游戏开发中用于管理游戏状态和数据在网络中的传输方式。在单人游戏中，所有数据都需要在本地机器上处理和呈现；而在多人游戏中，需要区分哪些数据需要在所有玩家之间共享，哪些数据只需要在特定玩家或AI之间共享。通过选择合适的复制模式，开发者可以优化网络性能并确保游戏体验的一致性。



## Init Ability Actor Info

![image-20240406005131969](.\image-20240406005131969.png)

在多人游戏中，AbilitySystemComponent的初始化是一个关键步骤，因为它涉及到游戏玩法和角色能力的正确设置。图中展示了AbilitySystemComponent的初始化过程，并强调了在不同角色类型（如敌人角色和玩家角色）之间的差异。

在初始化AbilitySystemComponent时，需要注意以下几点：

1. Owner Actor and Avatar Actor:
   - 对于敌人角色，AbilitySystemComponent是直接挂载在其角色上的，因此Owner Actor和Avatar Actor是同一个实体。
   - 对于玩家角色，AbilitySystemComponent通常挂载在PlayerState上，因此Owner Actor是PlayerState，而Avatar Actor是实际的游戏角色。

2. Init Ability Actor Info:
   - 图中展示了UAbilitySystemComponent::InitAbilityActorInfo()函数的调用，这个函数用于设置AbilitySystemComponent的Owner Actor和Avatar Actor属性。
   - 对于敌人角色，调用InitAbilityActorInfo()时传入其自身作为Owner Actor和Avatar Actor参数。
   - 对于玩家角色，调用InitAbilityActorInfo()时传入PlayerState作为Owner Actor，以及实际的游戏角色作为Avatar Actor。

3. Ability System Component:
   - AbilitySystemComponent是Ability System框架的核心组件，它负责管理角色的能力、技能和Buff等。
   - 在初始化AbilitySystemComponent时，需要确保它与正确的Owner Actor和Avatar Actor关联，以便正确地应用和管理角色的能力。

4. Multiplayer Considerations:
   - 在多人游戏中，需要确保AbilitySystemComponent的初始化在所有玩家之间同步。
   - 这可能需要使用网络编程技术来确保每个玩家的角色都能正确地初始化AbilitySystemComponent，并与其他玩家的角色保持同步。

综上所述，初始化AbilitySystemComponent时，需要考虑Owner Actor和Avatar Actor的区别，以及在多人游戏环境下的同步问题。正确地初始化AbilitySystemComponent对于确保游戏玩法的一致性和角色能力的正确管理至关重要。

![image-20240406010538856](.\image-20240406010538856.png)

### 那我们应该在什么时机去调用函数呢？

对于Player-Controlled Character，初始化AbilitySystemComponent应该在角色被控制后进行。在这个过程中，Controller（通常是PlayerController）会将自己设置为Pawn（通常是PlayerCharacter）的拥有者。在这一时刻，AbilitySystemComponent已经存在于Pawn上，因此可以在服务器和客户端同时进行初始化。

- 在服务器上，对于Player-Controlled Character，AbilitySystemComponent的初始化确实应该在PossessionBy函数中进行。当PlayerController对Pawn进行控制时，可以通过调用UAbilitySystemComponent::InitAbilityActorInfo()函数来初始化AbilitySystemComponent，同时将Owner Actor设置为PlayerController，将Avatar Actor设置为PlayerCharacter。

​	PossessionBy()函数是在一个Controller（通常是PlayerController或AIScriptedProxy）控制一个Pawn（通常是PlayerCharacter或	AICharacter）时被调用的。这个函数在Controller和Pawn之间建立了一个关联关系，使得Controller可以控制Pawn的行为和动作。

​	这点对于Character和PlayerState来说都是一样的，在服务端。

- 在客户端，可以通过调用UAbilitySystemComponent::AcknowledgePossession()函数来确认AbilitySystemComponent的所有权，并将Avatar Actor设置为PlayerCharacter。同时，客户端还需要调用UAbilitySystemComponent::OnRep_AvatarActor()函数来处理Avatar Actor的更新。而且，因为对于玩家控制的角色，ASC是挂载在PlayerState上的，所以我们不仅要等Character生成完成，还需要等PlayerState生成完成，所以，在PlayerState中，我们需要在收到服务端的OnRep_PlayerState rpc后才能进行ASC的初始化调用。

对于AI-Controlled Character，初始化AbilitySystemComponent也应在角色被控制后进行。在这个过程中，Controller（通常是AIScriptedProxy）会将自己设置为Pawn（通常是AICharacter）的拥有者。在这一时刻，AbilitySystemComponent已经存在于Pawn上，因此可以在服务器和客户端同时进行初始化。

- 在客户端，可以通过调用UAbilitySystemComponent::BeginPlay()函数来确认AbilitySystemComponent的所有权，并将Avatar Actor设置为AICharacter。同时，客户端还需要调用UAbilitySystemComponent::OnRep_AvatarActor()函数来处理Avatar Actor的更新。



![image-20240406020332916](.\image-20240406020332916.png)

在Unreal Engine中，Mixed Replication Mode是一种特殊的网络复制模式，它允许在网络中同时存在多个拥有者。在这种模式下，每个Actor都有一个Owner Actor，而Owner Actor的拥有者可能是Controller（如PlayerController或AIController），也可能是PlayerState。

在Mixed Replication Mode下，对于Player-Controlled Characters，Owner Actor通常是PlayerState，因为PlayerState的Owner自动设置为Controller。对于AI-Controlled Characters，Owner Actor通常是AIController，因为AIController的Owner自动设置为Controller。

然而，如果Owner Actor不是PlayerState，但在使用Mixed Replication Mode时，你需要手动设置Owner Actor的Owner为其Controller。这可以通过调用SetOwner()函数来实现。例如，如果你的Owner Actor是一个非PlayerState的Actor，但在使用Mixed Replication Mode时，你需要将其Owner设置为Controller。



## Attributes

![image-20240406094829066](.\image-20240406094829066.png)

尽管可以直接通过代码来改变Attributes的值，但更推荐使用Gameplay Effect来改变Attributes有以下几个原因：

1. **预测性**：使用Gameplay Effect可以让客户端无需等待服务器的许可就能立即改变属性值。这意味着客户端可以立即看到属性值的变化，而服务器只需稍后被告知这些变化。这种机制称为预测，它提高了游戏的响应速度和流畅性。

2. **可逆性**：服务器可以撤销无效的属性更改。如果客户端的某个属性更改被服务器拒绝，服务器可以回滚该更改，以保持数据的一致性和正确性。

3. **统一性**：使用Gameplay Effect可以提供一种统一的方式来管理属性更改，无论这些更改是来自客户端的预测还是来自服务器的命令。这样可以简化代码逻辑，提高代码的可维护性和可扩展性。

4. **灵活性**：Gameplay Effect还可以包含持续时间、衰减曲线等额外信息，这使得属性更改更加灵活和可控。

综上所述，尽管直接修改属性值在技术上是可行的，但使用Gameplay Effect提供了更好的性能、可逆性、统一性和灵活性，因此是更推荐的做法。
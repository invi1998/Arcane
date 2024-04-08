# Gameplay Tags

![image-20240408101825256](.\image-20240408101825256.png)

在虚幻引擎 5 (UE5) 的 Gameplay Ability System (GAS) 中，Gameplay Tags 是一种标签系统，用于标识和组织游戏中的各种资源和功能。这些标签可以用于跟踪游戏状态、触发事件、控制游戏行为等。下面是对 GameplaTags 的详细分析：

1. 数据结构类型：
   - `FGameplayTag`：表示一个具体的标签，由字符串组成，如 "Abilities.Fire"。
   - `FGameplayTagContainer`：表示一组标签，可以包含多个 `FGameplayTag`。
   - `IGameplayTagAssetInterface`：一个接口，允许资产（如蓝图）拥有和操作 `GameplayTag`。

2. 成员函数：
   - `GetOwnedGameplayTags()`：获取资产拥有的所有 `GameplayTag`。
   - `HasMatchingGameplayTag()`：检查资产是否具有与给定 `GameplayTag` 匹配的标签。
   - `HasAnyMatchingGameplayTags()`：检查资产是否具有与给定 `GameplayTag` 集合中的任何一个匹配的标签。
   - `HasAllMatchingGameplayTags()`：检查资产是否具有与给定 `GameplayTag` 集合中的所有标签匹配的标签。
   - `HasAlMatchingGameplayTags()`：检查资产是否具有与给定 `GameplayTag` 集合中的所有标签匹配的标签。

3. 容器和管理方式：
   - `GameplayTagContainer`：用于存储和管理一组 `GameplayTag`。
   - `GameplayTagManager`：全局管理 `GameplayTag` 的注册、分配和查询。

在虚幻引擎 5 中，Gameplay Tags 可以通过以下方式使用：
- 在蓝图中，可以使用 `HasMatchingGameplayTag()` 函数来检查一个对象是否具有特定的标签，或者使用 `GetOwnedGameplayTags()` 函数来获取对象的所有标签。
- 在游戏逻辑中，可以使用 `GameplayTagContainer` 来存储和管理一组标签，然后使用 `HasMatchingGameplayTag()` 函数来检查容器中的标签是否与另一个标签匹配。
- 在事件响应系统中，可以使用 `GameplayTag` 作为条件来触发事件，例如当一个角色获得某个技能标签时触发相应的事件。

总之，Gameplay Tags 是一个强大的工具，可以帮助开发者组织和管理游戏中的各种资源和功能。通过使用 `GameplayTag`、`GameplayTagContainer` 和 `GameplayTagManager`，开发者可以轻松地实现复杂的游戏逻辑和行为。



![image-20240408102532172](.\image-20240408102532172.png)

图中展示的是虚幻引擎 5 (UE5) 中的 Gameplay Tags 系统的工作原理和用途。Gameplay Tags 是一种标签系统，用于标识和组织游戏中的各种资源和功能。以下是如何使用 Gameplay Tags，它们能做什么，以及需要注意的事项：

工作原理：
- Gameplay Tags 可以用于跟踪游戏状态、触发事件、控制游戏行为等。
- 图中展示了 Gameplay Tags 的使用场景，包括输入（如按键）、能力（如技能）、属性、伤害类型、增益/减益、消息和数据等。
- Gameplay Tags 可以用于任何你想要的方面，比如在游戏中添加新的功能或元素。

能做的事情：
- Gameplay Tags 可以用于标识和组织游戏中的各种资源和功能。
- 它们可以用于跟踪游戏状态，例如玩家的血量、能量值等。
- 它们也可以用于触发事件，例如当玩家使用某个技能时，可以触发一个事件来播放特效或播放音效。
- 它们还可以用于控制游戏行为，例如当玩家受到某个减益效果时，可以降低其移动速度或攻击力。

注意事项：
- Gameplay Tags 应该设计得简洁明了，以便于理解和使用。
- 应该避免使用过于复杂的标签系统，因为这可能会增加游戏的复杂性并使玩家感到困惑。
- 应该确保 Gameplay Tags 的一致性，例如使用相同的标签来标识相同的功能或资源。
- 应该注意 Gameplay Tags 的命名，因为它们应该能够准确地描述它们所代表的功能或资源。

总的来说，Gameplay Tags 是一个非常有用的工具，可以用于标识和组织游戏中的各种资源和功能。通过正确地使用 Gameplay Tags，可以提高游戏的可玩性和可扩展性。
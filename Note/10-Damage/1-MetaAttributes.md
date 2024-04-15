# Meta Attributes

Meta Attributes并不直接代表游戏角色的具体数值属性，如生命值、攻击力等，而是对这些属性进行管理和操作的额外元数据信息。

**Meta Attributes**：
Meta Attributes主要用于描述 Gameplay Attributes的行为特性或者管理规则，它们不存储具体的游戏数据，而是定义了属性如何运作、如何影响其他系统，以及它们与其他游戏机制之间的交互方式。例如，Meta Attributes可以包括以下类型的设置：

1. **Attribute Aggregation Type**：决定多个来源赋予同一属性值时如何聚合，比如是否累加还是取最大值、最小值等。
2. **Attribute Lifetime Policy**：指定属性值如何持久化或何时重置，如每回合结束时清零，或者一直累积直到手动重置。
3. **Display Name & Tooltip**：为属性提供用户界面显示的名称和提示信息。
4. **Clamp Min/Max Values**：限制属性实际数值的上下界。
5. **Scaling Factors**：属性值可能受到某种比例因子的影响，如基于等级或其他属性的变化而变化。

**普通属性（Gameplay Attributes）**：
相比之下，普通属性（Gameplay Attributes）是直接反映游戏角色状态的数据指标，例如：
- 生命值（Health）
- 魔法值（Mana）
- 攻击力（Damage）
- 移动速度（Movement Speed）
- 护甲（Armor）
- 冰冻时间（Freeze Duration）

这些属性通常会被实时更新，并且可以直接影响到游戏玩法和角色性能。它们的值可以在游戏过程中被增减、查询并应用到各种游戏逻辑中。

总的来说，Meta Attributes是关于普通属性如何管理和使用的附加规则集合，而普通属性则是角色状态的具体数值表现。两者结合使得GAS能够灵活且高效地处理各种复杂的角色状态和能力效果。

![image-20240415101515657](.\image-20240415101515657.png)

普通的游戏属性在多人游戏中会需要网络同步，进行网络复制，而元数据不需要进行网络同步，它仅仅作为一种占位符，被允许参与到计算中，用于计算其他GameplayEffect或者属性。
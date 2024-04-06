

# GamePlay Effects

![image-20240406230521099](.\image-20240406230521099.png)

在Unreal Engine 5 (UE5)的Ability System插件中，Gameplay Effects是一种强大的工具，用于描述和应用游戏中的各种效果，如增益、减益、执行操作等。Gameplay Effects允许开发者创建复杂的规则系统，用于处理游戏中的各种属性变化和状态更新。

Gameplay Effects主要由以下几个部分组成：

1. **Gameplay Effect**: 这是一个数据结构，用于描述一个游戏效果。它不包含任何执行逻辑，只包含描述效果的数据，如属性修改器、执行操作等。Gameplay Effect是不可继承的，即不能通过子类化来扩展它。

2. **Modifier**: Modifier是一个用于修改属性的组件，它定义了如何修改属性以及修改的程度。Modifier可以是简单的数值增加或减少，也可以是复杂的计算，如乘法、除法等。Modifier还支持使用其他属性作为输入进行计算。

3. **Execution**: Execution是一个用于执行特定操作的组件，它可以是简单的动作，如设置一个属性值，也可以是复杂的操作，如调用一个函数或执行一个脚本。

4. **Magnitude Calculation Type**: Magnitude Calculation Type定义了如何计算Gameplay Effect的强度或影响程度。它可以是固定值、基于属性的计算、自定义计算等。

5. **Gameplay Tags**: Gameplay Tags是一组标签，用于标识和分类Gameplay Effects。这些标签可以用于过滤和匹配Gameplay Effects，以便在游戏中应用特定的效果。

6. **Gameplay Effect Execution Calculation**: Gameplay Effect Execution Calculation定义了如何计算和应用Gameplay Effect的执行操作。它可以是简单的函数调用，也可以是复杂的计算过程。

通过组合这些组件，开发者可以创建出各种各样的Gameplay Effects，用于在游戏中实现各种效果。这些效果可以应用于不同的属性，如生命值、攻击力等，也可以应用于不同的游戏对象，如角色、敌人等。由于Gameplay Effects是数据驱动的，它们可以在运行时动态地创建、修改和应用，从而为游戏提供高度的灵活性和可扩展性。

![image-20240406231307982](.\image-20240406231307982.png)

几个关键特性：

- “Duration Policy”（持续时间策略）：包括三种类型，“Instant”（瞬间）、“Has Duration”（有持续时间）和“Infinite”（无限持续）。
- “Stacking”（堆叠）：表示游戏效果是否可以叠加，即多个相同效果是否可以同时作用于同一个目标。
- “Add Gameplay Tags”（添加游戏标签）：表示游戏效果可以附加一些标签，用于标记和分类游戏效果。
- “Grant Abilities”（授予能力）：表示游戏效果可以赋予目标某些特殊的能力或技能。
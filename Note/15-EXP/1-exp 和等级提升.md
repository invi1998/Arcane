

# Exp

![image-20240429104532997](.\image-20240429104532997.png)



## Awarding EXP

![image-20240429131049899](.\image-20240429131049899.png)

图中展示了击杀怪物获取经验的过程。整个流程由多个步骤组成，包括监听游戏事件、奖励经验、处理攻击和死亡事件等。

具体来说，流程如下：

1. 游戏事件监听：通过GA_监听事件（ListenForEvents）节点监听游戏事件，当游戏事件触发时，执行下一步。
2. 经验奖励：当游戏事件触发时，通过Gameplay Event Event节点执行奖励经验的操作。这可以通过设置Gameplay Event Event节点的参数来实现。
3. 攻击处理：通过Attack节点处理攻击事件，当玩家对怪物造成伤害时，执行下一步。
4. 死亡处理：通过Death节点处理怪物死亡事件，当怪物死亡时，执行下一步。
5. 经验扣除：通过Gameplay Event Event节点执行经验扣除的操作。这可以通过设置Gameplay Event Event节点的参数来实现。

通过这种方式，GAS可以实现游戏中的经验奖励

![image-20240429131007461](.\image-20240429131007461.png)



1. **XP Reward on Enemies (and a way to get it)**：在敌人身上设置经验值奖励，并提供获取这些经验值的方法。
2. **Incoming XP Meta Attribute**：创建一个用于接收经验值的元属性。
3. **Passive Gameplay Ability, GA_ListenForEvents (and grant it)**：添加一个被动游戏能力，称为GA_ListenForEvents，并赋予玩家此能力。
4. **Gameplay Effect to apply in response to Event**：为响应事件而应用的游戏效果。
5. **Award XP from Attribute Set when damage is fatal**：当伤害是致命的时，从属性集中奖励经验值。
6. **Handle Incoming XP in Attribute Set and increase XP on Player State**：处理来自属性集的传入经验值，并增加玩家状态中的经验值。


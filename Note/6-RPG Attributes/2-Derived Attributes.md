# Derived Attributes (当前游戏项目里的派生属性)

![image-20240409123124782](.\image-20240409123124782.png)

这张图片展示了游戏项目中关于属性的设计图。图中分为两个部分，左边是“Primary Attributes”（主要属性），右边是“Secondary Attributes”（次要属性）。主要属性包括力量（Strength）、智力（Intelligence）、耐力（Resilience）和活力（Vigor），而次要属性则与这些主要属性相关联，提供了更多的游戏机制和角色特性。

具体来说，主要属性影响了次要属性的效果。例如，力量（Strength）增加了物理伤害，智力（Intelligence）增加了魔法伤害。耐力（Resilience）则影响了护甲（Armor）和护甲穿透（Armor Penetration），活力（Vigor）影响了最大生命值（Max Health）和生命恢复（Health Regeneration）。

次要属性中，装甲（Armor）可以减少所受的伤害，并提高格挡几率（Block Chance）。格挡几率（Block Chance）则是抵挡一半伤害的几率。护甲穿透（Armor Penetration）可以无视敌人的一部分护甲，从而提高暴击几率（Critical Hit Chance）。暴击几率（Critical Hit Chance）决定了玩家攻击时触发双倍伤害和暴击加成的几率。暴击伤害（Critical Hit Damage）是在暴击时额外增加的伤害。此外，活力（Vigor）还影响了暴击抵抗（Critical Hit Resistance）和最大生命值（Max Health）。

从整体来看，这张图片展示了游戏中的属性系统如何运作，以及不同属性如何相互影响，从而提供多样化的角色发展路径和游戏体验。

以下是图中列出的主要属性及其对应的次要属性：

1. 力量（Strength）
	* 抵抗力（Resilience）：增加物理伤害
	* 耐力（Resilience）：增加韧性，减少受到的伤害
	* 活力（Vigor）：增加最大生命值（Max Health）

2. 智力（Intelligence）
	* 抵抗力（Resilience）：增加魔法伤害
	* 活力（Vigor）：增加法力值恢复（Mana Regeneration）
	* 活力（Vigor）：增加最大法力值（Max Mana）

3. 耐力（Resilience）
	* 抵抗力（Resilience）：增加韧性，减少受到的伤害
	* 耐力（Resilience）：增加护甲（Armor）
	* 耐力（Resilience）：增加护甲穿透（Armor Penetration）

4. 活力（Vigor）
	* 活力（Vigor）：增加最大生命值（Max Health）
	* 活力（Vigor）：增加生命恢复（Health Regeneration）
	* 活力（Vigor）：增加最大法力值（Max Mana）

5. 护甲（Armor）
	* 抵抗力（Resilience）：减少受到的伤害
	* 抵抗力（Resilience）：增加格挡几率（Block Chance）

6. 格挡几率（Block Chance）
	* 护甲（Armor）：增加格挡几率（Block Chance）

7. 护甲穿透（Armor Penetration）
	* 抵抗力（Resilience）：无视敌人的一部分护甲，提高暴击几率（Critical Hit Chance）

8. 暴击几率（Critical Hit Chance）
	* 护甲穿透（Armor Penetration）：提高暴击几率（Critical Hit Chance）

9. 暴击伤害（Critical Hit Damage）
	* 护甲穿透（Armor Penetration）：在暴击时增加额外的伤害

10. 暴击抵抗（Critical Hit Resistance）
	* 活力（Vigor）：减少受到的暴击伤害

11. 最大生命值（Max Health）
	* 活力（Vigor）：增加最大生命值（Max Health）

12. 生命恢复（Health Regeneration）
	* 活力（Vigor）：增加生命恢复（Health Regeneration）

13. 最大法力值（Max Mana）
	* 活力（Vigor）：增加最大法力值（Max Mana）

14. 法力恢复（Mana Regeneration）
	* 活力（Vigor）：增加法力恢复（Mana Regeneration）

这些属性关系说明了游戏中的角色可以通过提升主要属性来解锁和增强次要属性，从而实现角色的成长和发展。
# 在编辑器中创建Gameplay Tags

在项目设置下的编辑和项目设置中，有一个游戏玩法标签部分，在这里创建游戏玩法标签或将它们添加到我们的项目中。

![image-20240408103350404](.\image-20240408103350404.png)

点击管理Gameplay标签，添加Tag，添加的时候，最好就先有一个清晰的层次结构，比如，这里我们添加一个蓝量（Mana）的标签，我希望把他放到 Attribute/Vital/这个组下，我们就可以这样写。然后我们可以给每个Tag添加注释，选择一个配置文件（这里我直接用系统默认的Tag源就行）

![image-20240408104414782](.\image-20240408104414782.png)

这样我们就成功在Editor中添加了一个Tag。



# 使用数据表格添加Tag

![image-20240408105016918](.\image-20240408105016918.png)

我们在蓝图文件夹下，创建一个数据表格，用该表格来创建Tags，创建该表格的时候，我们需要选择行结构，选择GameplayTagRow

![image-20240408105232600](.\image-20240408105232600.png)

然后就可以给我们想要的属性添加Tags了

![image-20240408105631968](.\image-20240408105631968.png)

配置好后，我们将这个表格应用到GameplayTag中，回到项目设置，找到标签列表，并添加我们刚刚的表格即可

![image-20240408110241146](.\image-20240408110241146.png)



# 将GamplayTag应用到AbilitySystem中

![image-20240408111107705](.\image-20240408111107705.png)

在虚幻引擎 5 (UE5) 的 Gameplay Effects 系统中，Gameplay Effect 配置中的 Components 包含了许多不同的组件，每个组件都有特定的功能。以下是一些常见的组件及其功能：

1. Abilities Gameplay Effect Component：用于与能力相关的效果。例如，当一个能力被激活时，可以应用或移除某些效果。

2. Additional Effects Gameplay Effect Component：用于添加额外的效果。例如，当一个效果被应用时，可以同时应用其他效果。

3. Asset Tags Gameplay Effect Component：用于与资产标签相关的效果。例如，当一个资产标签被应用到一个目标上时，可以应用或移除某些效果。

4. Block Ability Tags Gameplay Effect Component：用于阻止与指定标签相关的所有能力。例如，当一个效果被应用时，可以阻止与指定标签相关的能力。

5. Chance to Apply Gameplay Effect Component：用于设置应用效果的概率。例如，当一个效果被应用时，可以设置应用该效果的概率。

6. Custom Can Apply Gameplay Effect Component：用于自定义是否可以应用效果的条件。例如，当一个效果被应用时，可以根据自定义条件判断是否可以应用该效果。

7. Gameplay Effect UIData Text Only：用于显示效果的用户界面数据。例如，当一个效果被应用时，可以显示该效果的文本描述。

8. Immunity Gameplay Effect Component：用于免疫指定效果。例如，当一个效果被应用时，可以免疫与指定标签相关的效果。

9. Modifier Gameplay Effect Component：用于修改属性值。例如，当一个效果被应用时，可以增加或减少目标的属性值。

10. Remove Other Gameplay Effect Component：用于移除其他效果。例如，当一个效果被应用时，可以移除与指定标签相关的效果。

11. Target Tag Requirements Gameplay Effect Component：用于设置目标必须具有的标签要求。例如，当一个效果被应用时，可以设置目标必须具有的标签。

12. Target Tags Gameplay Effect Component：用于设置目标的标签。例如，当一个效果被应用时，可以设置目标的标签。

以上只是一部分常见的组件，实际上还有许多其他组件可用。每个组件都有特定的功能和用法，可以根据需要选择合适的组件来构建效果。

![image-20240408112452342](.\image-20240408112452342.png)
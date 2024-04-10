![image-20240410132750952](.\image-20240410132750952.png)

构建一个优雅强大的系统来管理和获取资产里的Attributes。

1. 创建Secondary Attribute Gameplay Tags（更好地在C++中处理标签）：
   - 目的：改进对Gameplay Tags的管理方式，使其更适合在C++中使用。
   - 做法：创建一个更高效、更易于编程的Gameplay Tags系统，可能包括自定义数据结构或API。

2. 创建UAttributeInfo Data Asset：
   - 目的：存储每个属性的信息，如名称、描述、图标等。
   - 做法：创建一个数据资产类，用于存储每个属性的元数据。每个属性对应一个UAttributeInfo实例。

3. 创建FAuraAttributeInfo结构：
   - 目的：定义一个C++结构体，用于在C++代码中表示属性信息。
   - 做法：创建一个名为FAuraAttributeInfo的结构体，包含属性名称、描述、图标等字段。

4. 填充每个属性的数据资产：
   - 目的：为每个属性提供详细的元数据。
   - 做法：为每个属性创建一个UAttributeInfo数据资产实例，并填充其元数据字段。

5. 创建UAttributeMenuWidgetController：
   - 目的：提供一个控制器类，用于管理属性菜单的显示和交互。
   - 做法：创建一个UAttributeMenuWidgetController类，负责处理属性菜单的逻辑，如加载属性信息、显示属性列表、处理用户操作等。

构建思路

> 首先，当属性发生变化时，我们需要一个属性菜单的Widget控制器，用于接收来自能力系统的广播。
>
> 接下来，当我们接收到一个属性时，我们需要一种方法来确定与该属性关联的正确的Gameplay Tag。我们需要将这两个关联起来，我们还希望有一种方法可以在C++中引用这些Gameplay Tags。
>
> 我们现在看到了一种实现这一目标的方法。我们可以使用Request Gameplay Tag，它接受一个FName参数，我们在其中指定我们想要的标签。这样，我们基本上是在向Gameplay Tag Manager请求一个标签结构，给定一个名称，如果Gameplay Tag Manager拥有这个标签，那么Request Gameplay Tag将返回它给我们。
>
> 不过，这种方法存在一些问题，我不喜欢在FName参数中输入字符串名称来指定Gameplay Tag。我希望有一个更好的方法来处理这些标签。我想要一个类，它可以存储全局的、我们可以访问的Tag变量。这就是我们需要研究的东西。
>
> 接下来，我们需要一个数据资产，它能够接收Gameplay Tags，并返回我们需要制作的我们的Aura Attribute Info结构。
>
> 这样一来，一旦我们知道某个属性的标签，我们的数据资产就能提供我们需要发送给我们的Widgets的所有数据。这将是我们的最后一步，一旦我们有了这个结构，那就是将它广播给我们的Widgets，它们可以检查自己的Gameplay Tags是否与发送的结构中的Gameplay Tags匹配，然后它们可以更新自己。
>
> 这意味着我们属性菜单有一些下一步的工作。
>
> 首先，我们需要创建我们的二级属性Gameplay Tags，并且我们希望在C++中处理这些标签的方式更好，而不是到处调用request gameplay tag。
>
> 我们需要在C++的一边有一个集中式的Gameplay Tag变量的来源。
>
> 我们还需要一个数据资产，它能够存储这些结构，并允许我们根据Gameplay Tag进行查找，以找到我们需要的属性信息，这样我们就可以将它广播给我们的Widgets。
>
> 我们还需要创建一个结构体，我们将把所有的信息都放在这个结构体中，这样我们就可以把它发送给我们的Widgets。
>
> 我们还需要创建这个结构体。我们将创建一个Aura Attribute Info结构体，它包含了我们想要发送给Widgets的所有信息。
>
> 当然，我们需要创建我们的属性菜单的Widget控制器。这个Widget控制器将把所有的事情联系在一起，因为它将监听来自能力系统的广播，并能够从数据资产中检索到属性信息的结构体，然后将它广播给我们的Widgets，它们可以检查自己的Gameplay Tags是否与发送的结构中的Gameplay Tags匹配，然后它们可以更新自己。
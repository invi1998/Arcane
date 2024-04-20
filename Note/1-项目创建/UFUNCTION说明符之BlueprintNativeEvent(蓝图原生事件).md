# UFUNCTION说明符之"BlueprintNativeEvent"(蓝图原生事件)

1.c++中定义事件，c++和蓝图中都可以实现（c++必须实现）。

2.执行优先级：

- 如果蓝图不实现，会执行c++函数实现。
- 如果蓝图和c++都实现，蓝图会覆盖c++实现从而只执行蓝图实现。

3.*BlueprintNativeEvent* 需要配合*BlueprintCallable* 一起使用,否则蓝图中不可调用。

4.c++函数实现方法：

函数声明：

```cpp
UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
   void NativeEvent(int32 number);
   void NativeEvent_Implementation(int32 number);
```

[参考](https://link.zhihu.com/?target=https%3A//www.bilibili.com/read/cv9583709%3Fspm_id_from%3D333.999.0.0)可以不写 *NativeEvent_Implementation* 的函数声明，直接写该函数定义。

也可将c++实现声明为虚函数：

```cpp
virtual void NativeEvent_Implementation(int32 number);
```

这样做可以实现定义另一个新的c++类继承自该类，override该c++实现方法。

函数实现：

```cpp
void AMyActor::NativeEvent_Implementation(int32 number)
{
   GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("NativeEvent C++ Call  number:%d"), number));
}
```

5.有返回值的在蓝图以函数形式存在，无返回值的在蓝图中以事件存在。

![img](https://pic2.zhimg.com/80/v2-6dce941b49d4f7d00b44fbaf4ceb9f29_720w.webp)

蓝图中可以直接从Function的Override下找到点击去进行蓝图实现

点击实现后该列表不再显示该事件：

![img](https://pic4.zhimg.com/80/v2-f2c4b09decaf53294a0de5cc03d7db9b_720w.webp)

无返回值：

```text
UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
   void NativeEvent(int32 number);
```

![img](https://pic1.zhimg.com/80/v2-11e9fdb179a1fc45e4b5bb34f50c7564_720w.webp)

以事件形式存在

有返回值：

```text
UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
   int32 NativeEvent_RetVal(int32 number);
```

![img](https://pic3.zhimg.com/80/v2-7447a1bf89e9b83865361a37cb4133e6_720w.webp)

以函数形式存在

6.蓝图和c++都实现，蓝图会覆盖c++实现从而只执行蓝图实现，但是可以通过事件鼠标右击：

![img](https://pic2.zhimg.com/80/v2-d6ae382b6b3228c908ddbd48586cab91_720w.webp)

![img](https://pic2.zhimg.com/80/v2-ac9d8db45b605054df1259db1e0df509_720w.webp)

实现调用c++实现的同时进行蓝图实现。

------

测试：

![img](https://pic2.zhimg.com/80/v2-b9e2e2a4622478ee5c26622ce5e99d8d_720w.webp)

拖入关卡一个继承自该c++类的Actor

- 蓝图未实现，仅c++实现：

![img](https://pic1.zhimg.com/80/v2-e0d0df9a39d97c18b5aa028e1b6f9a84_720w.webp)

输出结果，c++方法调用

- 蓝图实现，c++也实现：

![img](https://pic4.zhimg.com/80/v2-7596a1d7830e6e6ff9c2a87a71d36563_720w.webp)

蓝图实现方法

![img](https://pic3.zhimg.com/80/v2-d5734e2696bdb1d97b508ef16e1f366a_720w.webp)

输出结果，蓝图方法调用，覆盖了c++实现

- 同时调用c++和蓝图实现：

![img](https://pic3.zhimg.com/80/v2-374db220e9e06b9d410821a2b0abe802_720w.webp)

蓝图实现内含c++实现调用

![img](https://pic3.zhimg.com/80/v2-6917484615d99f7ffec1fb69960ddf32_720w.webp)

先输出c++调用，再输出蓝图调用
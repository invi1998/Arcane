# FMemoryWriter

`FMemoryWriter` 是 Unreal Engine 中用于处理内存流的一个类。它主要负责将数据写入到一个内存缓冲区中，可以用于序列化对象或数据结构到内存中。`FMemoryWriter` 与 `FMemoryReader` 配对使用，后者用于从内存缓冲区中读取数据。

### 使用 `FMemoryWriter`

`FMemoryWriter` 主要有以下几个用途：
1. 序列化对象到内存缓冲区。
2. 将数据写入到内存缓冲区中。
3. 用于网络传输前的数据打包。

### 创建 `FMemoryWriter`

要创建一个 `FMemoryWriter`，首先需要创建一个 `TArray<uint8>` 用于存储数据。然后，使用这个数组来构造 `FMemoryWriter` 对象。

```cpp
TArray<uint8> Buffer;
FMemoryWriter MemoryWriter(Buffer);
```

或者，如果你想指定缓冲区的大小，可以这样做：

```cpp
const int32 BufferSize = 1024; // 1KB
TArray<uint8> Buffer(BufferSize);
FMemoryWriter MemoryWriter(Buffer);
```

### 写入数据

`FMemoryWriter` 提供了一系列的方法来写入不同类型的变量。例如，写入整数、浮点数或字符串等。

```cpp
int32 MyInt = 42;
float MyFloat = 3.14f;
FString MyString = "Hello, World!";

MemoryWriter << MyInt;
MemoryWriter << MyFloat;
MemoryWriter << MyString;
```

### 序列化对象

如果你需要序列化一个 UObject 的实例（例如，一个 UActorComponent 或 AActor），你可以使用 `SerializeObject` 方法。

```cpp
UObject* SomeObject = /* ... */;
MemoryWriter.SerializeObject(SomeObject);
```

### 使用 `FStructuredArchive`

Unreal Engine 4 引入了 `FStructuredArchive` 以提高序列化的可读性和可维护性。`FMemoryWriter` 可以与 `FStructuredArchive` 配合使用。

```cpp
FStructuredArchive::FRecord Record = MemoryWriter.GetArchive().MakeRecord();
Record << MyInt;
Record << MyFloat;
Record << MyString;
```

### 示例

下面是一个完整的示例，展示如何使用 `FMemoryWriter` 来序列化一些数据：

```cpp
#include "CoreMinimal.h"

void SerializeExample()
{
    TArray<uint8> Buffer;
    FMemoryWriter MemoryWriter(Buffer);

    int32 MyInt = 42;
    float MyFloat = 3.14f;
    FString MyString = "Hello, World!";

    MemoryWriter << MyInt;
    MemoryWriter << MyFloat;
    MemoryWriter << MyString;

    // 输出序列化后的缓冲区大小
    UE_LOG(LogTemp, Warning, TEXT("Serialized buffer size: %d bytes"), Buffer.Num());

    // 如果需要，可以在此处将 Buffer 保存到文件或通过网络发送
}

int main(int argc, char* argv[])
{
    SerializeExample();
    return 0;
}
```

### 总结

`FMemoryWriter` 是一个非常有用的工具，可以帮助你在 Unreal Engine 中处理数据的序列化和反序列化。它非常适合于在网络上传输数据或在内存中临时存储数据。通过结合使用 `FMemoryWriter` 和 `FMemoryReader`，你可以轻松地实现数据的序列化和反序列化过程。
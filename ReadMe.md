# 协程

### 1、函数调用栈

来自：[协程篇（一）-- 函数调用栈 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/462968883)

一、x64下的常用寄存器

​	寄存器就是CPU用来储存计算过程中的一些指令以及数据信息，由于CPU读写寄存器特别快，所以寄存器的存在，能加快CPU的计算速度。

​	x64架构下一共有16个寄存器，64位系统的寄存器以'r'开头，32位系统的寄存器以'e'开头，64位系统可以向下兼容32位系统，所以也可以使用'e'开头的寄存器，但是其只使用其低32位，剩余的32位会自动补0。

重要的寄存器如下：

- rax:用于存放函数返回值、中间计算结果
- rsp、rbp:存放当前函数栈的栈顶、栈底的地址
- **rdi、rsi**、rdx、rcx、r8、r9:依次存放第一到第六个参数
- rip:存放下一条指令地址

二、函数调用过程

例子：

```C++
int add(int num1,int num2)
{
    return num1+num2;
}

int main()
{
    int a = 1;
    int b = 2;
    int c = add(a,b);
    return 0;
}
```

通过gcc生成汇编文件:

```shell
gcc -s sum.cpp -o sum.s
```

有用汇编信息如下：

```asm
    112d:	55                   	push   %rbp
    112e:	48 89 e5             	mov    %rsp,%rbp
    1131:	89 7d fc             	mov    %edi,-0x4(%rbp)
    1134:	89 75 f8             	mov    %esi,-0x8(%rbp)
    1137:	8b 55 fc             	mov    -0x4(%rbp),%edx
    113a:	8b 45 f8             	mov    -0x8(%rbp),%eax
    113d:	01 d0                	add    %edx,%eax
    113f:	5d                   	pop    %rbp
    1140:	c3                   	ret    
    1141:	f3 0f 1e fa          	endbr64 
    1145:	55                   	push   %rbp
    1146:	48 89 e5             	mov    %rsp,%rbp
    1149:	48 83 ec 10          	sub    $0x10,%rsp
    114d:	c7 45 f4 01 00 00 00 	movl   $0x1,-0xc(%rbp)
    1154:	c7 45 f8 02 00 00 00 	movl   $0x2,-0x8(%rbp)
    115b:	8b 55 f8             	mov    -0x8(%rbp),%edx
    115e:	8b 45 f4             	mov    -0xc(%rbp),%eax
    1161:	89 d6                	mov    %edx,%esi
    1163:	89 c7                	mov    %eax,%edi
    1165:	e8 bf ff ff ff       	call   1129 <__cxa_finalize@plt+0xf9>
    116a:	89 45 fc             	mov    %eax,-0x4(%rbp)
    116d:	b8 00 00 00 00       	mov    $0x0,%eax
    1172:	c9                   	leave  
    1173:	c3                   	ret    

```

![img](https://pic3.zhimg.com/v2-388dbe61fc52190d6bcb72b38e3f90a2_r.jpg)
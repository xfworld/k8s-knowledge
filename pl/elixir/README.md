>2018-08-12, 散乱的笔记

## 一、模式匹配：

1. 等号左边的叫 pattern，右边的叫 term。模式匹配会用右边的 term 去尝试匹配左边的 pattern
1. 单个 variable 能匹配任何元素。
1. 若匹配成功，pattern 内的所有 variable 都会被 bound 到 term 中的匹配值。（用 pin 符号固定的除外）
1. 如果匹配失败，会抛出 error.

函数传参中的模式匹配（可用于实现 arity 相同，传参不同的方法重载）
同方法名的函数，会按源码中的声明顺序，从上至下进行匹配

## 二、 解惑:(个人理解，可能有误)

1. bound(绑定) 和 assign(赋值):
    - 主流语言中的赋值，会改变全局所有对应变量的值。而函数式的优点是不可变性，无副作用，很"纯"，所有的变量都可直接用对应的值替换而不会出现问题。
1. 为何函数式语言不会有for循环?
    - 循环需要依赖可变量，而函数式中不存在可变量，因此无法实现。而只能用递归。
1. 那elixir中也有 a=1 这样的语法，如果能多加一个 goto 语句，不就能实现了么?
    - 这个 a=1 是绑定，不是赋值。即使再写个 a=2，也只是名字的重新绑定而已。绑定只影响后面的名字a，而对之前使用过的a没有任何影响。**elixir中的名字，全部都可以用对应的值替换，而不会影响程序结果，这就是函数式的特性。**(递归的话，这种替换会展开整个递归方法) 任何有状态变化的代码，都使用不了这种替换。
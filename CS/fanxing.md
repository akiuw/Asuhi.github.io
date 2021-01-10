# 泛型

# 泛型

> 占位符,编译时多态

##基本写法
```
//main:  Calc<int> c1 = new Calc<int>();

    public class Calc<T>
    {
        private T a;
        private T b;

        public T Add(T a, T b)
        {
            return a;
        }
    }
```

###方法泛型
```
        public T SetA<T>(T a)
        {
            return a;
        }
```

###泛型抽象接口
```
    public interface IRun<TP>
    {

    }
```

##泛型限制

1. where T:类名   ----表示传进来的类必须继承与这个类
2. where T:接口 ----表示该类必须实现这个接口
3. where T:new() ---- 这个类必须有无参构造函数‘
4. where T:new(int) ----必须有int类型的构造函数
4. where T:struct ----表示值类型
5. where T:class ----表示引用类型

```
        public T SetA<T>(T a)
            where T : class  // 表示必须是一个引用类型，如string ，如果传入一个值类型如int就会报错
        {
            return a;
        }
```

##泛型的继承

1. 泛型可以继承泛型类型
2. 泛型可以继承泛型实例化后的类型
3. 类型可以继承实例化后的泛型
4. 非泛型类型不能继承泛型类型

```
class A<U,V>
class B<U.V>:A(U,V)
class F<U,V>:A(string,int)
class D:A(string,int)
//class E:A(U,V) <--- 错误
```




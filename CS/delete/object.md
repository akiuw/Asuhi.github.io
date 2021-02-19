##类的访问关系


* 所有的类都继承于Object类

```
    class Program{
        static void main(){
            Type typ = typeof(Vehicle);
            Console.WriteLine(typ.ToString());
            Console.WriteLine(typ.BaseType.ToString());
        }
    }
    
    class Vehicle{

    }
```

> LearCSharp.Vehicle
> System.Object

#### * is a （is关键字 : [obj] is [基类] 返回 true /false）：

#### **一个子类的实例就是父类的实例 反过来不成立**

```
    class Program{
        static void Main(string[] args){
            Vehicle veh = new Car();
        }
    }
    class Vehicle{
    }
    class Car : Vehicle{
    }
```

#### 封闭类 （seald）

> 使用 `seald` 修饰的类不能有派生类 (不能被继承)

#### 访问级别

>  `internal` 修饰的类的子类不能用 `pubilc`  反过来可以

创建一个新的程序集MyLib 并且引用它

![](image/程序集.png)

![](image/引用程序集.png)

情况1：

```
namespace MyLib
{
    internal class Vehicle{
        public int Owner { get; set; }
    }
}
```

示例代码中的Owner字段是不能被Programma看见的

如果写成·`public class Vehicle` 就可以访问了

情况2：

```
    public class Vehicle{
        internal int Owner { get; set; }
    }
```

Onwer还是不能访问

情况3：
如果Onwer是private 级别
那么继承Vehicle的子类也不能访问 （和C++类似）

外部调用
MyLib：
```
    public class Vehicle{
        private int _rpm;
        public void Accelerate(){
            _rpm += 1000;
        }
        public int Speed { get { return _rpm / 100; } } //只读字段Speed
    }
    public class Car : Vehicle{
    }
```
Program：
```
    class Program{
        static void Main(string[] args){
            Car car = new Car();
            car.Accelerate();
            car.Accelerate();
            Console.WriteLine(car.Speed); ;
        }
    }
```
这里可以查看到Speed的变化

###关于 protected ：

protected  是跨程序集的 在Program里面是可以派生到的
internal 和 protected 是可以组合的


##重写

子类要加上override 基类要加virtual
不加的话叫做子类对父类隐藏

```
    class Vehicle{
        public virtual void Run(){
            Console.WriteLine("I am vehicle");
        }
    }
    class Car: Vehicle{
        public override void Run(){
            Console.WriteLine("I am car");
        }
    }
```

注意 ： set 和 get也可以被重写

```
    class Vehicle{
        private int _speed;
        public virtual int Speed{
            set { _speed = value; }
            get { return _speed; }
        }

        public virtual void Run(){
            Console.WriteLine("I am vehicle");
        }
    }
    class Car: Vehicle{
        private int _rpm;
        public override int Speed {
            get { return _rpm / 100; } 
        }
        public override void Run(){
            Console.WriteLine("I am car");
        }
    }
```





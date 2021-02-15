## 隐试转化




1. 大范围装小范围
2. 不同类型之间的转换

```
//long int short sbyte 从大到小
  long l = 1;
  int i = 0;
  l = i; //隐试转换
  i = l; //错误
  
// ulong > uint > ushort > byte
// decimal 不参与 double > float
decomal de = 1.1m;
double d = 1.1;
float f = 1.1f;
//decomal  不能用隐试转换 存储double 和float

```


大类型之间的转换



```
//无符号和有符号

byte b = 1;
ushort u = 1;
uint ui = 1;
ulong ul = 1;

sbyte sb = 1;
short s = 1;
int i = 1;
long l = 1;

b = sb; // 报错 由于sbyte 有负数 不能转到 无符号

i = b;  // 可以 无符号的数可以转到有符号的

sb = b; //报错 ， 虽说可以转到有符号，但盒子的范围应该超过这个无符号数 例如： ushort -> int 或 uint -> long
//如果范围不够或相同就会报错

double d = 1.1;
float f = 1.1f;
d = f; //同理

f = i; 
f = l;
f = s;
f = sb;
//浮点数可以装整型

i = f; // 报错 不能将浮点转化到整数

//bool类型不能转化到任意类型
```



## ref和out



1. ref传入的变量必须初始化
2. out传入的变量必须在内部被赋值



## get和set


```
class Person{
  private string name;
  private bool sex;
  public string Name{
    get{
      //可以在返回前配置一些逻辑规则
      return name;
    }
    set{
      //value 关键字用于表示外部传入带值，只在set有用
      //这里value 类型为string和属性一致
      name = value;
    }
  }
  //只读属性
  public bool Sex{
    get{
      return sex;
    }
  }
  
  //自动属性 可以有访问修饰符
  public float Money{ get; private set;}
}

static void main(){
  Person p = new Person();
  p.Name = "123";
}
```





## 索引器



> 让对象像数组一样访问
>
> 索引器里面可以写逻辑
>
> 访问修饰符 返回值 this[参数]{get和set 语句} 
>
> 索引器时可以重载的 



```
class Person{
  private string name;
  private int age;
  private Person[] friends;
  
  //索引器
  public Person this[int index]{
    get{
      //可以添加逻辑判断friends是否为空，
      //也可以判断是否超出范围
      if(friends == null || friends.Length - 1 < index){
        return null;
      }
      return friends[index];
    }
    set{
      if(value == null){
        friends = new Person[]{value};
      }else if(friends.Length - 1 < index){
        //自己添加逻辑，越界时把最后一个朋友替换掉
        friends[friends.Length - 1] = value;
      }
      friends[index] = value;
    }
    
    //重载索引器
    private int[,] array;//二维数组
    public Person this[int i, int j]{
      get{
        return array[i,j];
      }
      set{
        array[i,j] = value;
      }
    }
    
    public string this[string str]{
      get{
        switch(str){
          case "name":
            return this.name;
          case "age":
            return this.age.ToString();
        }
        return "";
      }
    }
  }
  
  static void main(){
    Person p = new Person();
    p[0] = new Person();
    p[0,0] = 10;//报错，二维数组没用初始化
  }
}
```



## 拓展方法



> 为现有的**非静态**类型添加新方法
>
> 在任意一个**静态类**里面添加：访问修饰符 static 返回值 函数名(this (ref)拓展类名 参数名, 参数类型 参数名...) 
>
> ref可选



```
    static class Tools{
        //为int拓展了一个成员方法
        //value 代表使用这个方法的实例化后的对象
        //如果拓展方法和原方法签名相同，系统会调用原方法

        public static void Print(this int value)
        {
            Console.WriteLine(value);
        }
        //这里只能写ref 如果不写 value是拷贝传递
        public static void Plus(this ref int value, int v)
        {
            value += v;
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            int i = 10;
            i.Print(); // 10 value就是i
            i.Plus(12);
            i.Print(); //22 
        }
    
    }   
```



## 操作符重载



不可重载的操作符

1. 逻辑与`&&` 逻辑或`||`
2. 索引符`[]`
3. 强制转换`()`
4. 反问成员变了的 `.`  三目运算符 `?:` 赋值 `=`
5. 不能使用ref 和out

对于前后++

如果是class前后++

效果都是一样没区别 结果是前++

```
  class Point
  {
    public int x;
    public int y;

    public Point(int x, int y)
    {
      this.x = x;
      this.y = y;
    }
    public Point(Point p)
    {
      this.x = p.x;
      this.y = p.y;
    }
    //(p1 + p2)
    public static Point operator +(Point p1, Point p2)
    {
      Point p = new Point(0, 0);
      p.x = p1.x + p2.x;
      p.y = p2.y + p2.y;
      return p;
    }

    //只能(p1 + value) 不能(value + p1)
    public static Point operator +(Point p1, int value)
    {
      Point p = new Point(0, 0);
      p.x = p1.x + value;
      p.y = p1.y + value;
      return p;
    }

    public static Point operator ++(Point p)
    {
      p.x += 1;
      p.y += 1;
      return p;
    }
  }
  class Program
    {
        static void Main(string[] args)
        {
            int i = 1;
            int a = i++;
            int b = ++i;
            Console.WriteLine("{0},{1}", a, b); //1,1

            Point p = new Point(1,1);

            Point p1 = new Point(p++);

            Console.WriteLine("{0},{1}",p1.x, p1.y);//2,2

            Point p2 = new Point(++p);
            Console.WriteLine("{0},{1}", p2.x, p2.y);//3,3
        }
    }
```



对于结构体 opertor++就有区别了

后++先使用变量的值在进行++操作

前++先进行++操作在使用变量的值

```
  struct Point
  {
        public int x;
        public int y;
    public Point(int x, int y)
    {
            this.x = x;
            this.y = y;
    }

        public Point(Point p)
        {
            this.x = p.x;
            this.y = p.y;
        }
        static public Point operator ++(Point p)
    {
            p.x += 1;
            p.y += 1;
            return p;
    }
  }

class Program
    {
        static void Main(string[] args)
        {
            int i = 1;
            int a = i++;
            int b = ++i;
            Console.WriteLine("{0},{1}", a, b);//1,1
            Point p = new Point(1,1);
            Point p1 = new Point(p++);
            Console.WriteLine("{0},{1}",p1.x, p1.y);//1,1
            Point p2 = new Point(++p);
            Console.WriteLine("{0},{1}", p2.x, p2.y);//3,3
        }
    }
```



## 分部类



>  把一个类分成几部分申明，可以写到不同到脚本文件里面
>
>  也可以把方法分开生命和实现



```C#
partial class Student{
  public bool sex;
  public string name;
  
  partial void Speak(string str);
}
partial class Student{
  public int number;
  
  public partial void Speak(){
    //实现逻辑
  }
  //public bool sex; //报错
}

//这里其实只申明了一个类，Student 拥有三个成员变量


```



## 里氏替换原则 is / as



>  任何父类出现的地方可以用子类替代
>
>  1. is 返回一个对象是否为指定类对象 返回bool值
>  2. as 返回一个指定类型的对象，成功返回对象，失败返回null

```
class GameObject{}

class Player:GameObject{
  public void PlayerAtk(){
    Console.WriteLine("攻击敌人");
  }
}

static void main(){
  GameObject player = new Player();// 基类 引用子类对象
  if(player is Player){ //is 判断对象是否为子类对象
    Console.WriteLine(true);
  }else{
    Console.WriteLine(false);
  }
  Player p = player as Player;
  if(p == null){
    Console.WriteLine("p 不是 Player");
  }else{
    Console.WriteLine("p 是 Player");
  }
  
  //is 和as配合使用
  
  if(player is Player){
    Player p = player as Player;
    p.PlayerAtk();// 调用方法
  }
}

```






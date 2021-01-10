#集合

##List<>

属性
* `Count`表示元素个数
* `Add()`添加一个元素
* `AddRange()`添加一个数组集合
* `Clear()`清空集合
* `RemoveAt()`删除指定索引处的元素，注意：后面元素索引值会发生变化
* 遍历：推荐`foreach`,只能读不能改
* `operator[]()` 索引，和数组一样从0开始

```
        static void Main(string[] args)
        {
            List<string> l1 = new List<string>();
            l1.Add("abc");
            l1.Add("def");
            l1.Add("ghi");
            Console.WriteLine(l1.Count);//3

            Console.WriteLine(l1[1]);//def

            foreach(var it in l1)
            {
                Console.WriteLine(it);
            }
            
            l1.AddRange(new string[] { "aaa", "bbb", "ccc" });
            Console.WriteLine(l1.Count);//6
            
            Console.ReadLine();
        }
```


##Dictionary<Key,Value>

* key，value表示键值对，key不能重复
* `operator[](key)`,通过key找到值

```
        static void Main(string[] args)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();

            dict.Add("a", "abc");
            dict.Add("b", "def");
            dict.Add("c", "ghi");

            Console.WriteLine(dict["a"]);//abc

            foreach(var it in dict)
            {
                Console.WriteLine("key: " + it.Key + "  value: " + it.Value);
            }

            Console.ReadLine();
        }
```

#Chrono

摘抄自https://www.cnblogs.com/jinxiang1224/p/8468162.html

> chrono库主要包含了三种类型：duration, time_point 和 clock。

## Duration（时间间隔）

chrono库中用一个duration模板类，用来表示一段时间间隔，可以表示几秒钟、几分钟或者几个小时的时间间隔。

第一个是数值第二个是ratio类型
```
template<typename _Rep, typename _Period = ratio<1>>
struct duration
{
typedef _Rep   rep;
...
private:
    rep  __r;  //内部维护的计数个数成员
...
};
```

关于ratio类型
```
template<intmax_t _Num, intmax_t _Den = 1>
struct ratio;
```
第一个是分子第二个是分母，单位是秒
如 ratio<1,1000> 为1ms
     ratio<1> 为1s

duration为前面的乘后面的
库中已经定义的类型:
```
/// nanoseconds
typedef duration<int64_t, nano>     nanoseconds;
/// microseconds
typedef duration<int64_t, micro>    microseconds;
/// milliseconds
typedef duration<int64_t, milli>    milliseconds;
/// seconds
typedef duration<int64_t>       seconds;
/// minutes
typedef duration<int, ratio< 60>>   minutes;
/// hours
typedef duration<int, ratio<3600>>  hours;
```
例如：
```
std::this_thread::sleep_for(std::chrono::seconds(10));
//10 * ratio(1,1); 也就是10s
```

###count
duration对象内部的rep类型周期个数--tick数

###静态方法
min、max和zero三个静态成员函数
cout << chrono::seconds::max().count() << endl;
//输出结果为：9223372036854775807

###运算符
支持所有的算术运算符不同单位之间也可以自动匹配
```
chrono::minutes t1(5);
chrono::seconds t2(30);
chrono::seconds t3 = t1 - t2;
```

##Time point（时间点）

通过一个相对epoch的时间间隔duration来实现
epoch就是1970-1-1时刻
这个类可以与标准库ctime结合起来显示时间，ctime内部的time_t类型就是代表这个秒数。
```
template<typename _Clock, typename _Dur = typename _Clock::duration>
struct time_point
{
typedef _Clock                      clock;
typedef _Dur                        duration;
typedef typename duration::rep              rep;
typedef typename duration::period           period;
private:
    duration __d; //维护的内部duration成员
...
};
```

输出当前时间戳
```
chrono::time_point<system_clock> tp = chrono::system_clock::now();
cout << tp.time_since_epoch().count() << endl;
```

###运算
本质上是通过内部维护的duration成员进行的

表示当前系统时钟，共有三种：
system_clock：从系统获取时钟
steady_clock：不能被修改的时钟
high_resolution_clock：高精度时钟，实际上是system_clock或者steady_clock的别名,最小精度是纳秒


##system_clock

system_clock 提供三个静态的函数，可以用于time_point提供了与C API的时间交互的良好定义。因此，可以很容易与time_t类型打交道。接口函数如下：

```
//get current time
static time_point now() noexcept;

//time_point conver to time_t
static time_t to_time_t (const time_point& tp) noexcept;

//convert time_t to time_oint
std::chrono::system_clock::from_time_t
static time_point from_time_t (time_t t) noexcept;
```



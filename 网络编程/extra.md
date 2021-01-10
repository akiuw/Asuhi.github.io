#网络编程常见问题整理

##协议

摘抄自 https://blog.csdn.net/libaineu2004/article/details/78803296

###TCP头部结构

![](image/TCP-Header-01.jpg)

4个重要的部分：

1. Sequence Number是包的序号，用来解决网络包乱序（reordering）问题。
2. Acknowledgement Number就是ACK——用于确认收到，用来解决不丢包的问题。
3. Window又叫Advertised-Window，也就是著名的滑动窗口（Sliding Window），用于解决流控的。
4. TCP Flag ，也就是包的类型，主要是用于操控TCP的状态机的。


##三次握手

####为什么要握手？

* 对于建链接的3次握手，主要是要初始化Sequence Number 的初始值。通信的双方要互相通知对方自己的初始化的Sequence Number
这个号要作为以后的数据通信的序号，以保证应用层接收到的数据不会因为网络上的传输的问题而乱序

####为什么要三次？

* 试想一下，如果server端接到了clien发的SYN后回了SYN-ACK后client掉线了，server端没有收到client回来的ACK，那么，这个连接处于一个中间状态，即没成功，也没失败。
三次的话server会等待63秒

* 关于SYN Flood攻击。一些恶意的人就为此制造了SYN Flood攻击——给服务器发了一个SYN后，就下线了，于是服务器需要默认等63s才会断开连接，这样，攻击者就可以把服务器的syn连接的队列耗尽，让正常的连接请求不能处理。

####握手的一些须知

1. 三次握手，如果前两次有某一次失败，会重新从第一次开始，重来三次。
2. 三次握手，如果最后一次失败，服务器并不会重传ack报文，而是直接发送RTS报文段，进入CLOSED状态。这样做的目的是为了防止SYN洪泛攻击。
3. 发起连接时如果发生TCP SYN丢包，那么系统默认的重试间隔是3s，这期间不会返回错误码。


##四次挥手

####为什么要有TIME_WAIT？

1. TIME_WAIT确保有足够的时间让对端收到了ACK，如果被动关闭的那方没有收到Ack，就会触发被动端重发Fin，一来一去正好2个MSL
2. 有足够的时间让这个连接不会跟后面的连接混在一起（你要知道，有些自做主张的路由器会缓存IP数据包，如果连接被重用了，那么这些延迟收到的包就有可能会跟新连接混在一起）。

####大并发短连接导致TIME_WAIT过多

**TIME_WAIT表示的是你主动断连接**

tcp_tw_reuse和tcp_tw_recycle，这两个参数默认值都是被关闭的，后者recyle比前者resue更为激进，resue要温柔一些。

使用tcp_tw_reuse和tcp_tw_recycle来解决TIME_WAIT的问题是非常非常危险的，因为这两个参数违反了TCP协议

##重传机制

###超时重传机制

###超时重传机制

不回ack，死等3，当发送方发现收不到3的ack超时后，会重传3。一旦接收方收到3后，会ack回4
意味着3和4都收到了。

问题：
因为要死等3，所以会导致4和5即便已经收到了，而发送方也完全不知道发生了什么事，因为没有收到Ack，所以，发送方可能会悲观地认为也丢了，所以有可能也会导致4和5的重传。

方案：两种
* 仅重传timeout的包。也就是第3份数据。
* 重传timeout后所有的数据，也就是第3，4，5这三份数据。

###快速重传机制

包没有连续到达，就ack最后那个可能被丢了的包，如果发送方连续收到3次相同的ack，就重传
Fast Retransmit的好处是不用等timeout了再重传。

![](image/FASTIncast021.png)

如果发送方发出了1，2，3，4，5份数据，第一份先到送了，于是就ack回2，结果2因为某些原因没收到，3到达了，于是还是ack回2，后面的4和5都到了，但是还是ack回2，因为2还是没有收到，于是发送端收到了三个ack=2的确认，知道了2还没有到，于是就马上重转2。然后，接收端收到了2，此时因为3，4，5都收到了，于是ack回6

###SACK 方法
这种方式需要在TCP头里加一个SACK的东西，ACK还是Fast Retransmit的ACK，SACK则是汇报收到的数据碎版

###Duplicate SACK – 重复收到数据的问题


##套接字选项

摘抄自：https://blog.csdn.net/libaineu2004/article/details/78850227

套接字选项设置
```
int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
```

###SO_SNDTIMEO和SO_RCVTIMEO

阻塞模式的超时时间。如果不设置会造成长等待卡死。

###TCP_NODELAY

关闭Nagle算法，在一些交互性强的场景，比如游戏。需要关闭
> Nagle算法：允许网络中最多只能有一个小分组被发送，而待发送的其它小分组会被重新分组成一个”较大的”小分组，等收到上一个小分组的应答后再发送

###SO_REUSEADDR和SO_REUSEPORT

当服务器进程关闭时，想立刻再复用原来的ip和端口需要等待2MSL的时间。

为了方便重启服务器或调试，会设置这两个选项，REUSE就是复用的意思，让进程立刻可以复用地址和端口

###SO_LINGER

户端异常关闭,或网络断开。client无响应,server收不到ACK,在一定时间(/proc/sys/net/ipv4/tcp_keepalive_intvl 75 即75秒)后重发keepalive packet, 并且重发一定次数(/proc/sys/net/ipv4/tcp_keepalive_probes 9 即9次);
客户端曾经崩溃,但已经重启.server收到的探测响应是一个复位,server端终止连接。

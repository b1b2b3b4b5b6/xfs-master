## xfs语音播报http post服务器

### 配置

设备默认连接的wifi为

SSID：MESH_ROUTER

PASSWD: 12345678

wifi配置需要在固件中修改，以实际情况为准



用户需要获得设备的ip才能http post设备，可以通过设备串口查看，或从网关查看



### 功能

- 开放http post接口，用来接收语音播报请求和设置心跳服务器
- 主动向指定服务器进行post请求，作为心跳包



### 播报语音接口

每次请求接口都会打断前一次的语音播报（如果上次的语音播报未结束）

文本格式见控制指令pdf

请求地址：http://deviceIP:80/speak

方式：post

```json
{
	"Text":"welcom hon",
}
```

> - `Text` 播报文本，编码格式在config.h中配置

返回

```json
{
    "ErrCode": 0
}
```

> `ErrCode` 错误代码
>
> - `0` 无错误
> - `1` 数据接收失败





### 修改心跳服务器接口

正确请求后，设备会立刻重启，下次启动将使用新的心跳服务器

请求地址：http://deviceIP:80/change

方式：post

```json
{
	"Server":"http://lumoschen.cn:1717/ech0"
}
```

> `Server` 心跳服务器地址

返回

```json
{
    "ErrCode": 0
}
```

> `ErrCode` 错误代码
>
> - `0` 无错误
> - `1` 数据接收失败
> - `2` 参数错误



### 设备心跳请求

请求地址：待配置

心跳间隔：默认5s

方式：post

```json
{
	"Typ":"HB",
    "IP":"192.168.1.44",
    "Mac":"12:34:34:35:34:34",
    "Version":"0.0.1"
}
```

> - `Typ`  指令类型，“HB”代表这是心跳包
> - `IP` 自身IP地址
> - `Mac` esp32 mac地址
> - `Version` 设备版本信息

不关心返回


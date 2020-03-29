## xfs语音播报http post服务器

### 依赖组件

需要在components中clone基本库ccom和epcom

https://git.dev.tencent.com/cesc_mu/ccom.git

https://git.dev.tencent.com/cesc_mu/epcom.git





### 配置

main/include/config.h

包括wifi，http路径，硬件配置，默认配置



### 播报语音接口

每次请求接口都会打断前一次的语音播报（如果上次的语音播报未结束）

请求地址：http://localip:80/speak

方式：post

```json
{
	"Text":"测试语音空格 A B C D",
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



### 获取设备信息接口

获取基本的设备信息

请求地址：http://localip:80/info

方式：post

```json
{
}
```

返回

```json
{
    "Version": "0.0.2",
    "ErrCode": 0,
    "Server": "http://lumoschen.cn:1717/ech"
}
```

> `Version` 设备版本号
>
> `Server` 心跳服务器地址
>
> `ErrCode` 错误代码
>
> - `0` 无错误
> - `1` 数据接收失败



### 修改心跳服务器接口

正确请求后，设备会立刻重启，下次启动将使用新的心跳服务器

请求地址：http://localip:80/change

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





### 心跳请求

请求地址：待配置

心跳间隔：待配置

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
>
> 



### 开机自动OTA

在main/include/config.h中配置默认REMOTE_BIN_PATH，即版本bin文件的http下载地址，每次设备重启后都会自动判断云端bin的版本号是否与现有的不一致，如果不一致，则更新版本并重启



#### 手动ota

手动发送ota bin文件地址以实现ota，ota成功后立刻重启，http无返回或直接断开连接

请求地址：http://localip:80/ota

方式：post

```json
{
    "Path":"http://lumoschen.cn:3000/xfs.bin"
}
```

> `Path` 新版本bin文件http下载地址

返回：

```json
{
    "ErrCode": 3,
    "ErrMessage": "ota fail, check ota path and version"   
}
```

> `ErrCode` 错误代码
>
> - `0` 无错误，ota成功http无返回或直接断开连接
> - `1` 数据接收失败
> - `2` 参数错误
> - `3` ota失败
>
> `ErrMessage` 错误信息


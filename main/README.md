## xfs语音播报http post服务器

### 配置

main/include/config.h



### 播报语音接口

每次请求接口都会打断前一次的语音播报（如果上次的语音播报未结束）

请求地址：http://localip:80/speak

方式：post

```json
{
	"Text":"测试语音空格 A B C D",
	"Vlume":1
}
```

> - `Text` 播报文本，编码格式在config.h中配置
> - `Vlume` 播报音量，待用



### 心跳请求

请求地址：待配置

心跳间隔：待配置

方式：post

```json
{
	"Typ":"HB",
    "IP":"192.168.1.44",
    "Mac":"12:34:34:35:34:34"
}
```

> - `Typ`  指令类型，“HB”代表这是心跳包
> -  `IP` 自身IP地址
> - `Mac` esp32 mac地址



### OTA

在main/include/config.h中配置REMOTE_BIN_PATH，即版本bin文件的http下载地址，每次设备重启后都会自动判断云端bin的版本号是否与现有的不一致，如果不一致，则更新版本并重启

#### 远程重启接口

手动重启以实现ota，请求后设备立刻重启

请求地址：http://localip:80/reboot

方式：post

```json
{
}
```
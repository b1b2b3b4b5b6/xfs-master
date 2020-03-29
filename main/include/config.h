
#define SSID "MESH_ROUTER"
#define PASSWD "12345678"

#define REMOTE_BIN_PATH "http://lumoschen.cn:3000/xfs.bin" //开启自动ota bin文件地址

#define SERVER_PORT 80 //本地 http 服务器端口
#define SPEAK_PATH "/speak" //语音播报接口路径
#define OTA_PATH "/ota" //重启接口路径
#define INFO_PATH "/info" //设备信息接口
#define CHANGE_PATH "/change"//修改心跳服务器地址

#define HB_SERVER "http://lumoschen.cn:1717/echo" //心跳服务器地址
#define HB_TIME_S 5 //心跳时间间隔

#define XFS_TX 32 //连接xfs的rx  
#define XFS_RX 33 //连接xfs的tx
#define XFS_BUSY 25 //xfs busy 引脚
#define XFS_BAND_RATE 115200 //xfs波特率



#define HB_SERVER_KEY "hb_key" //心跳服务器 nvs 键，不需要修改
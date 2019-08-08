# pili-camera-sdk

`pili-camera-sdk` 是一个适用于 IPCamera 的 RTMP 直播推流 SDK，封装了 RTMP 推流协议细节，提供了 FLV 封包的支持，开发者只需要利用 SDK 写入 H.264 视频帧 和 AAC/G.711 音频帧即可完成数据的发送，小巧、简洁。

## 1. 功能特性

- [x] 支持简单 RTMP 握手
- [x] 支持 H.264 帧
- [x] 支持 AAC/G.711 音频帧
- [x] 支持直接写入 H264 NAL 数据
- [x] 低内存占用

## 2. 模块说明

- hmac-sha1, urlsafe-b64：用于签名，生成推流 URL
- pili-librtmp：来自 [pili-librtmp](https://github.com/pili-engineering/pili-librtmp) 源码，也可以使用 rtmpdump 源码，不过接口需要稍做修改

## 3. API 用法

```c
// 头文件 pili_camera.h 包含了所有必须的依赖
#include "pili_macro.h"
#include "pili_type.h"
#include "push.h"
#include "flv.h"
```

```c
// 创建推流上下文 stream context
pili_stream_context_p ctx = pili_create_stream_context();
// 初始化推流上下文
// @param ctx 创建的推流上下文对象
// @param user_data 用户的自定义数据
// @param stream_state_cb 用于在流状态变更后获取状态的回调函数
pili_init_stream_context(pili_stream_context_p ctx, 
                         void *user_data,
                         pili_stream_state_cb stream_state_cb); 

// 流状态回调方法的定义
typedef void (*pili_stream_state_cb)(void *user_data, uint8_t state, pili_error_p error);
void stream_state_cb(uint8_t state, pili_error_t error) 
{
    // 其中 state 在 pili_type.h 中有定义，error 包含 code 和 msg 供排查错误出处
}
```

```c
// 开启推流
// @param context 创建的推流上下文对象
// @param push_url 推流地址，如 "rtmp://YOUR_PUSH_URL"
// @param enable_quic 是否激活 quic 推流 1 or 0
// @param quic_port 仅当 enable_quic 为 1 时，有效，获取方式请参考第 5 节
// @return success:0, failed:-1
int pili_stream_push_open(pili_stream_context_p context, 
                          const char *push_url, 
                          int enable_quic, 
                          short quic_port);
```

```c
// 封装 FLV Tag，定义在 flv.h
flv_tag_p tag = flv_create_tag();
int flv_init_tag(flv_tag_p tag,
                 uint8_t   tag_type,
                 uint32_t  data_size,
                 uint32_t  timestamp,
                 uint32_t  stream_id,
                 void      *data);
```

```c
// 发送封装后的 flv_tag 数据
pili_write_flv_tag(ctx, tag);
```

```c
// 对于 H264 编码的视频数据，SDK 提供了直接写入的方法，FLV Tag 的封装在 SDK 内部完成，开发者无需关心封装琐事。

// 写入关键帧，当某个 nalu 在关键帧中不包含时，将其的 length 填写为 -1 即可
pili_h264_key_frame_t key_frame;
key_frame.sps = sps;
key_frame.pps = pps;
key_frame.sei = sei;
key_frame.idr = idr;
pili_write_h264_key_frame(ctx, key_frame, ts);

// 写入非关键帧
pili_h264_nalu_t nalu;
nalu.data = data;
nalu.length = data_length;
pili_write_h264_slice(ctx, nalu, ts)
```

```c
// 结束推流并释放上下文
pili_stream_push_close(ctx);
pili_stream_context_release(ctx);
```

```c
// 查看 pili camera sdk 版本
pili_version();
```

## 4. 封包详解

可参考 Adobe 的 [Video File Format Specification Version 10](http://www.adobe.com/content/dam/Adobe/en/devnet/flv/pdfs/video_file_format_spec_v10.pdf)

## 5. 获取 Quic 推流端口列表

### 5.1 接口介绍

在开始 QUIC 推流前，通过此接口获取七牛直播云服务器上开放的端口列表，客户端从中随机选择一个，通过 `serverPort` 传入 `pili_stream_push_open` 用于创建 Quic 推流链接。

### 5.2 请求示例

`publishdomain` ：七牛推流域名

`Host`：pili-zeus.qiniuapi.com

- **Request**

~~~
> GET /v2/puicrtmp?publishdomain=<domain>
> Host: pili-zeus.qiniuapi.com
~~~

- **Response**

~~~
200 OK
{
    "puicPort": [
        6935,
        6936
    ],
    "ttl": 60
}
~~~

`puicPort`：为 int 数组，长度为 2，表示`Quic`协议使用的端口列表，可使用端口范围为[`<portStart>`,`<portEnd>`)，**左闭右开**；

`ttl`：端口有效期，单位为 秒，过期后需重新请求；当请求失败时，可沿用原来的端口列表，或者使用 6935（不推荐）。

- **完整的请求记录信息**

~~~
curl http://pili-zeus.qiniuapi.com/v2/puicrtmp?publishdomain=pili-publish.jztest11.test.cloudvdn.com -v
*   Trying 180.97.72.166...
* TCP_NODELAY set
* Connected to pili-zeus.qiniuapi.com (180.97.72.166) port 80 (#0)
> GET /v2/puicrtmp?publishdomain=pili-publish.jztest11.test.cloudvdn.com HTTP/1.1
> Host: pili-zeus.qiniuapi.com
> User-Agent: curl/7.53.1
> Accept: */*
>
< HTTP/1.1 200 OK
< Server: openresty
< Date: Thu, 12 Apr 2018 09:22:44 GMT
< Content-Type: application/json
< Content-Length: 34
< Connection: keep-alive
< Vary: Accept-Encoding
< X-Log: PILI:2;PILI-ZEUSD:3
< X-Reqid: tXoAALhDKRvipSQV
< X-Via: vdn-jssz-tel-1-5
<
{"puicPort":[6935,6936],"ttl":600}* Connection #0 to host pili-zeus.qiniuapi.com left intact
~~~

## 6. 里程碑

- 实现基本视频推流
  - h264 flv 封包
  - h264 文件读取模拟实时流
  - 缓存队列基本实现
  - rtmp 推流
- 加入音频
  - aac flv 封包
  - 音视频同步
  - g711 flv 封包
  - g711 -> aac 转码 (optional)
- 缓存队列优化
  - 丢包策略：视频关键帧相关丢包、音频或视频帧优先丢包等

## 7. 版本历史

- 0.3.0
    - 添加 `pili_error_t`
    - 更新流状态回调方法，增加 error 参数
    - 更新 `pili_write_packet` 方法为 `pili_write_flv_tag`
- 0.2.0
   - 更新接口
   - 添加 FLV 结构
   - 添加 h264 nalu 数据直接推送
   - 更新为简单握手
- 0.1.0
	- 实现 FLV 文件直推

## 8. 说明
项目由 Pili SDK 团队维护，有任何问题，可以提 PR 或者发送邮件到 pili-coresdk@qiniu.com


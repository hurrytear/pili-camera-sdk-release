//
//  flv_muxer.h
//  pili-camera-sdk
//
//  Created on 15/3/31.
//  Copyright (c) Pili Engineering, Qiniu Inc. All rights reserved.
//

#ifndef __PILI_CAMERA_SDK__FLV_MUXER__
#define __PILI_CAMERA_SDK__FLV_MUXER__

#include <stdio.h>
#include "rtmp.h"
#include "flv.h"

PILI_CAMERA_SDK_EXPORT int pili_flv_tag_mux(flv_tag_p flv_tag, PILI_RTMP *rtmp, PILI_RTMPPacket *pkt);

#endif /* defined(__PILI_CAMERA_SDK__FLV_MUXER__) */

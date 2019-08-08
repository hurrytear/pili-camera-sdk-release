//
//  push.h
//  camera-sdk
//
//  Created on 15/1/26.
//  Copyright (c) Pili Engineering, Qiniu Inc. All rights reserved.
//

#ifndef camera_sdk_push_h
#define camera_sdk_push_h

#include "pili_type.h"
#include "comm_def.h"

/**
 * Parse a push live url and connect
 * @param context struct of pili_stream_context_p
 * @param push_url push url
 * @param enable_quic whether use rtmp over quci protocol? 1 or  0
 * @param quic_port if enable_quic is 1, quic_port must be valid
 * @return 0:success; -1:failed
 **/
PILI_CAMERA_SDK_EXPORT int pili_stream_push_open(pili_stream_context_p context, const char *push_url, int enable_quic, short quic_port);

/**
 * Close push live connection
 * @param context struct of pili_stream_context_p
 * @return 0:success; -1:failed
 **/
PILI_CAMERA_SDK_EXPORT int pili_stream_push_close(pili_stream_context_p context);

/**
 * Close push live connection
 * @param context struct of pili_stream_context_p
 * @param flv_tag flv packet
 * @return 0:success; -1:failed
 **/
PILI_CAMERA_SDK_EXPORT int pili_write_flv_tag(pili_stream_context_p context, flv_tag_p flv_tag);

/**
 * Write key frame
 * @param context struct of pili_stream_context_p
 * @param key_frame struct of pili_h264_key_frame_t
 * @param ts timestamp, unit:millisecond
 * @return 0:success; -1:failed
 **/
PILI_CAMERA_SDK_EXPORT int pili_write_h264_key_frame(pili_stream_context_p context, pili_h264_key_frame_t key_frame, uint32_t ts);

/**
 * Write non-key frame
 * @param context struct of pili_stream_context_p
 * @param slice_nalu struct of pili_h264_nalu_t
 * @param ts timestamp, unit:millisecond
 * @return 0:success; -1:failed
 **/
PILI_CAMERA_SDK_EXPORT pili_write_h264_slice(pili_stream_context_p context, pili_h264_nalu_t slice_nalu, uint32_t ts);

/**
 * Write audio frame
 * @param context struct of pili_stream_context_p
 * @param params struct of pili_audio_params_t
 * @param frame struct of pili_audio_frame_t
 * @param ts timestamp, unit:millisecond
 * @return 0:success; -1:failed
 **/
PILI_CAMERA_SDK_EXPORT pili_write_audio_frame(pili_stream_context_p context, pili_audio_params_t params, pili_audio_frame_t frame, uint32_t ts);


#endif

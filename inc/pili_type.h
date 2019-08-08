//
//  pili_type.h
//  pili-camera-sdk
//
//  Created on 15/3/31.
//  Copyright (c) Pili Engineering, Qiniu Inc. All rights reserved.
//

#ifndef __PILI_CAMERA_SDK__TYPE__
#define __PILI_CAMERA_SDK__TYPE__

#include "pili_macro.h"
#include "flv.h"
#include "rtmp.h"

PILI_TYPE(struct pili_stream_context, pili_stream_context_t);
PILI_TYPE_POINTER(struct pili_stream_context, pili_stream_context_p);

PILI_TYPE(struct pili_h264_nalu, pili_h264_nalu_t);
PILI_TYPE_POINTER(struct pili_h264_nalu, pili_h264_nalu_p);

PILI_TYPE(struct pili_h264_key_frame, pili_h264_key_frame_t);
PILI_TYPE_POINTER(struct pili_h264_key_frame, pili_h264_key_frame_p);

PILI_TYPE(struct pili_audio_frame, pili_audio_frame_t);
PILI_TYPE_POINTER(struct pili_audio_frame, pili_audio_frame_p);

PILI_TYPE(struct pili_audio_params, pili_audio_params_t);
PILI_TYPE_POINTER(struct pili_audio_params, pili_audio_params_p);

PILI_TYPE(struct pili_error, pili_error_t);
PILI_TYPE_POINTER(struct pili_error, pili_error_p);

#define PILI_STREAM_STATE_UNKNOW        0x00
#define PILI_STREAM_STATE_CONNECTING    0x01
#define PILI_STREAM_STATE_CONNECTED     0x02
#define PILI_STREAM_STATE_DISCONNECTING 0x03
#define PILI_STREAM_STATE_DISCONNECTED  0x04
#define PILI_STREAM_STATE_ERROR         0x05

typedef void (*pili_stream_state_cb)(void *user_data, uint8_t state, pili_error_p error);
typedef void (*pili_stream_connect_cb)(void *user_data, int connect_time, int handshake_time);

/**
 * stream context
 */
PILI_CAMERA_SDK_EXPORT pili_stream_context_p pili_create_stream_context();
PILI_CAMERA_SDK_EXPORT int pili_init_stream_context(pili_stream_context_p ctx,
                             void   *user_data,
                             pili_stream_state_cb stream_state_cb);
PILI_CAMERA_SDK_EXPORT int pili_release_stream_context(pili_stream_context_p ctx);

struct pili_stream_context {
    char                    *push_url;
    PILI_RTMP               *rtmp;
    void                    *user_data;
    pili_stream_state_cb    stream_state_cb;
    int                     state;
    int                     receive_timeout;    // default as 15s
    int                     send_timeout;       // default as 3s
    pili_stream_connect_cb  stream_connect_cb;
};

/**
 * audio frame
 */
struct pili_audio_frame {
    uint8_t *data;
    size_t  length;
};

/**
 * audio params
 */
struct pili_audio_params {
    int sound_format; // FLV_AUDIO_TAG_SOUND_FORMAT_XX
    int sound_rate; // FLV_AUDIO_TAG_SOUND_RATE_XX
    int sound_size; // FLV_AUDIO_TAG_SOUND_SIZE_XX
    int sound_type; // FLV_AUDIO_TAG_SOUND_TYPE_XX
    int is_aac_config; // whether AAC sequence header 
};

/**
 * h264
 */
struct pili_h264_nalu {
    uint8_t *data;
    size_t  length;
};

// Set nalu's length to -1 if there's no such nalu.
struct pili_h264_key_frame {
    pili_h264_nalu_t sps;
    pili_h264_nalu_t pps;
    pili_h264_nalu_t sei;
    pili_h264_nalu_t idr;
};

/**
 * error
 */
PILI_CAMERA_SDK_EXPORT pili_error_p pili_create_error();
PILI_CAMERA_SDK_EXPORT int pili_init_error(pili_error_p error,
                    int code,
                    char *msg);
PILI_CAMERA_SDK_EXPORT int pili_release_error(pili_error_p error);

struct pili_error {
    int  code;
    char *msg;
};

PILI_CAMERA_SDK_EXPORT char *pili_version();

#endif  // __PILI_CAMERA_SDK__TYPE__

#ifndef __HP_COMM_H__
#define __HP_COMM_H__


#include <inttypes.h>
#include <stdio.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

//typedef int(*dev_streamcb)(void *data[], int len[], int count,uint64_t pts,int type,uint8_t chn);
//typedef int(*dev_jpgcb)(void *data, int len,int uuid);

const uint8_t CMD_FACE_RECT = 0x01; /* 视频叠加人脸框 */
const uint8_t CMD_FACE_CROP = 0x02; /* 编码人脸抓拍小图命令 */
const uint8_t CMD_OBJ_RECT = 0x21; /* 物品叠加框 */
const uint8_t MAX_FACE_SUPPORT = 30;
const uint8_t FRAME_ID_LEN = 4;

typedef struct _bd_face_rect {
    uint16_t top;
    uint16_t left;
    uint16_t width;
    uint16_t height;
    unsigned int trackid;
    unsigned char show;
    unsigned char update;
    unsigned char push;
    unsigned char die;
} PROTO_FACE_RECT;

typedef struct _bd_proto_face_result {
    unsigned char frameID[4];
    unsigned char nFaceNum;
    PROTO_FACE_RECT *rcData;

    _bd_proto_face_result() {
        memset(frameID, 0, 4);
        rcData = NULL;
    };
} PROTO_FACE_RESULT;

typedef struct _bd_face_crop {
    uint16_t top;
    uint16_t left;
    uint16_t width;
    uint16_t height;
    unsigned int trackid;
} PROTO_FACE_CROP;

typedef struct _bd_proto_face_crop {
    unsigned char frameID[4];
    unsigned char nFaceNum;
    PROTO_FACE_CROP rcData[MAX_FACE_SUPPORT];
} PROTO_FACE_CROP_RESULT;

typedef struct _bd_obj_rect {
    uint16_t top;
    uint16_t left;
    uint16_t width;
    uint16_t height;
    unsigned int trackid;
    unsigned char show;
    unsigned char update;
    unsigned char push;
    unsigned char die;
    unsigned char category;
} PROTO_OBJ_RECT;

typedef struct _bd_proto_obj_result {
    unsigned char frameID[FRAME_ID_LEN];
    unsigned char nObjNum;
    PROTO_OBJ_RECT *rcData;

    _bd_proto_obj_result() {
        memset(frameID, 0, 4);
        rcData = NULL;
    };
} PROTO_OBJ_RESULT;

typedef struct _bd_result {
    PROTO_FACE_RESULT stFaceRst;
    PROTO_OBJ_RESULT stObjRst;
} PROTO_RESULT;

typedef int(*dev_protocalcb)(uint8_t nCmd, PROTO_FACE_RESULT *stResult);

//typedef int(*stream_cb)(void *data, int *len, int count,uint64_t pts,int type,int chn);

int dev_hw_uart_init(int speed);
int dev_hw_uart_recv(int fd, void *buf, int len, int timeout_ms);
int dev_hw_uart_send(int fd, char *pBuf, int nlen);
int dev_hw_uart_close(int fd);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif

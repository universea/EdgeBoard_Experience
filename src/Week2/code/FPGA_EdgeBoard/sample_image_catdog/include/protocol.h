#ifndef _PROTOCOL__H_
#define _PROTOCOL__H_
#include "dev_common.h"
#include "ssd_detect.hpp"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define BD_PRO_FLAG {0xAA, 0xBB, 0xCD, 0xEF}
#define BD_PRO_VERSION {0x31, 0x30}

typedef struct _bd_proto_header
{
	unsigned char nProFlag[2];
	unsigned char nProVer[2];
	unsigned int  nSize;	
	unsigned int nCmdID;
	//unsigned char u8Reserved[3];
	
}BD_HEADER;

int dq_protocol_init(dev_protocalcb cb);
int dq_protocol_uninit(void);
int dq_protocol_send(char *buff,int len);

int send_obj_rects(std::vector<DetectInfo>* DetectInfos, char* fn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif

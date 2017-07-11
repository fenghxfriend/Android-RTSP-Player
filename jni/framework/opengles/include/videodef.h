/*****************************************************************************
   模块名      : ants360 type
   文件名      :ants360type.h
   相关文件    :
   文件实现功能: 360ants宏定义
   作者        : 魏治兵
   版本        : V3.0  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2004/02/17  3.0         魏治兵        创建
   2012/08/21  4.0         曾昭鸣        修正相关定义
******************************************************************************/
#ifndef _VIDEO_DEF_H_
#define _VIDEO_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define  MAX_WIDTH                      1920
#define  MAX_HEIGHT                     1080
#define  MAX_FRAME_BUFFER_SIZE          ( MAX_WIDTH * MAX_HEIGHT * 3 / 2 )
#define  MAX_H264_FRAME_SIZE            300000
#define  MAX_H264_HEADER_SIZE           1000
#define  BLOCK_QUEUE_SIZE               30

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _VIDEO_DEF_H_ */

/* end of file videodef.h */


#pragma once

//app message
typedef enum
{
    RMsg_BandRTData = 0x10,
    RMsg_BedControl = 0x11,
    RMsg_BedGears = 0x12,
} RecvMsgType_t;


typedef enum 
{
    Msg_BedRTData = 0x90,
} SendMsgType_t;

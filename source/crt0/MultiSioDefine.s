@********************************************************************
@*          MultiSioDefine.s                                        *
@*            Multi-play Communication Library Constants (GAS)      *
@* (Timer Interrupt Send & Non-adjustment of Synchronization)       *
@*                                                                  *
@*          Copyright (C) 1999-2001 NINTENDO Co.,Ltd.               *
@********************************************************************


@ Optimize the following settings based on the software specifications

MULTI_SIO_BLOCK_SIZE            =   16      @ Communication Data Block Size (Max. 24 Bytes)

MULTI_SIO_PLAYERS_MAX           =   4       @ Maximum Number of Players

MULTI_SIO_SYNC_DATA             =   0xfefe  @ Synchronized Data (0x0000/0xfffa~0xffff prohibited)

@ Comment out if no space in CPU internal Work RAM
MULTI_SIO_DI_FUNC_FAST:                     @ SIO Interrupt Prohibit Function High Speed Flag (CPU Internal RAM Execution)


MULTI_SIO_TIMER_NO              =   3       @ Timer No.
MULTI_SIO_TIMER_INTR_FLAG       =   (TIMER0_INTR_FLAG << MULTI_SIO_TIMER_NO)
                                            @ Timer Interrupt Flag
REG_MULTI_SIO_TIMER             =   (REG_TM0CNT + MULTI_SIO_TIMER_NO * 4)
REG_MULTI_SIO_TIMER_L           =    REG_MULTI_SIO_TIMER
REG_MULTI_SIO_TIMER_H           =   (REG_MULTI_SIO_TIMER + 2)
                                            @ Timer Register


@ Multi-play Communication Packet Structure Offset

OFS_MSP_FRAME_COUNTER           =   0       @ Frame Counter
OFS_MSP_RECV_ERROR_FLAGS        =   1       @ Receive Error Flag
OFS_MSP_CHECK_SUM               =   2       @ Checksum
OFS_MSP_DATA                    =   4       @ Communication Data
OFS_MSP_OVERRUN_CATCH           =   (OFS_MSP_DATA + MULTI_SIO_BLOCK_SIZE)
                                            @ Overrun Protect Area

MULTI_SIO_PACKET_SIZE           =   (OFS_MSP_OVERRUN_CATCH + 4)
                                            @ Structure Size


@ Multi-play Communication Work Area Structure Offset

OFS_MS_TYPE                     =   0       @ Connection (Master/Slave)
OFS_MS_STATE                    =   1       @ Communication Function State
OFS_MS_CONNECTED_FLAG           =   2       @ Connection History Flag
OFS_MS_RECV_SUCCESS_FLAGS       =   3       @ Receive Success Flag

OFS_MS_SYNC_RECV_FLAG           =   4       @ Synchronized Data Receive Confirmation Flag
OFS_MS_SYNC_RECV_ADJUST_COUNTER =   8       @ Synchronized Adjustment Counter

OFS_MS_SEND_BUF_COUNTER_INIT    =   10      @ Send Buffer Counter Initialization

OFS_MS_SYNC_SEND_FRAME_COUNTER  =   11      @ Send Frame Counter
OFS_MS_SYNC_RECV_FRAME_COUNTER  =   12      @ Receive Frame Counter

OFS_MS_SEND_BUF_COUNTER         =   20      @ Send Buffer Counter
OFS_MS_RECV_BUF_COUNTER         =   24      @ Receive Buffer Counter

OFS_MS_NEXT_SEND_BUF_P          =   40      @ Send Buffer Pointer
OFS_MS_CURRENT_SEND_BUF_P       =   44
OFS_MS_CURRENT_RECV_BUF_P       =   48      @ Receive Buffer Pointer
OFS_MS_LAST_RECV_BUF_P          =   64
OFS_MS_RECV_CHECK_BUF_P         =   80

OFS_MS_SEND_BUF                 =   96      @ Send Buffer (Double Buffer)
OFS_MS_RECV_BUF                 =   (OFS_MS_SEND_BUF + MULTI_SIO_PACKET_SIZE * 2)
                                            @ Receive Buffer (Triple Buffer)

MULTI_SIO_AREA_SIZE             =   (OFS_MS_RECV_BUF + MULTI_SIO_PACKET_SIZE * 3 * MULTI_SIO_PLAYERS_MAX)
                                            @ Structure Size


    .EXTERN RecvFuncBuf                     @ CPU Internal RAM Execution Buffer
    .EXTERN IntrFuncBuf


@--------------------------------------------------------------------
@-                      Multi-play Communication Initialization     -
@--------------------------------------------------------------------

    .EXTERN MultiSioInit

@* Set serial communication mode to multi-play mode.
@* Initialize register and buffer.

@--------------------------------------------------------------------
@-                      Multi-play Communication Main               -
@--------------------------------------------------------------------

    .EXTERN MultiSioMain

@* First determine if master or slave. If master recognized, initialize
@  timer.
@* Call MultiSioSendDataSet() and set send data.
@* Call MultiSioRecvDataCheck() and check if normal receive done, 
@  and copy receive data to Recvp.
@
@* Set so called with as close timing as possible within 1 frame.
@* Adjustment of synchronized data sometimes takes a little time.
@* Safer not to send data that matches flag data (SIO_SYNC_DATA) prior 
@  to connection determination.
@* If no more replies from unit that was recognized once, synchronization 
@  adjustment continues the receive becomes unstable.
@
@* Arguments:
@    r0:  User Send Buffer Pointer
@    r1:  User Receive Buffer Pointer

@--------------------------------------------------------------------
@-                      Multi-play Communication Interrupt          -
@--------------------------------------------------------------------

    .EXTERN MultiSioIntr

@* During communication interrupt, store receive data from each unit
@  in each receive buffer and set the send buffer data to the register.
@* If master, reset timer and restart send.
@
@* Program so called during communication interrupt.
@* Adjust setting so 1 packet (Except for OverRunCatch[]) can be 
@  transfered with 1 frame.


@--------------------------------------------------------------------
@-                      Set Send Data                               -
@--------------------------------------------------------------------

    .EXTERN MultiSioSendDataSet

@* Set the user send buffer data to send buffer.
@
@* Called from MultiSioMain().
@* Not necessary to call directly.
@
@* Arguments:
@    r0:  User Send Buffer Pointer

@--------------------------------------------------------------------
@-                     Check Receive Data                           -
@--------------------------------------------------------------------

    .EXTERN MultiSioRecvDataCheck

@* Check if receive done normally. If normal, copy the receive data
@  to the user receive buffer.
@
@* Called from MultiSioMain().
@* Do not need to call directly.
@
@* Arguments:
@    r0:  User Receive Buffer Pointer



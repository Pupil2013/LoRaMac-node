/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    �2013 Semtech

Description: LoRa MAC layer implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#ifndef __LORAMAC_H__
#define __LORAMAC_H__

/*!
 * LoRaMac channels definition
 */
#define LORA_NB_CHANNELS                            8
#define LORA_NB_125_CHANNELS                        6

#define LC1                                         868100000
#define LC2                                         868300000
#define LC3                                         868500000
#define LC4                                         868850000
#define LC5                                         869050000
#define LC6                                         869525000
#define LC7                                         868300000
#define FC1                                         868300000

/*!
 * Beacon interval in ms
 */
#define BEACON_INTERVAL                             128000000

/*!
 * Class A&B receive delay in ms
 */
#define CLS2_RECEIVE_DELAY1                         1000000 - RADIO_WAKEUP_TIME
#define CLS2_RECEIVE_DELAY2                         ( 2 * CLS2_RECEIVE_DELAY1 )

/*!
 * Class A&B maximum receive window delay in ms
 */
#define CLS2_MAX_RX_WINDOW                          3000000

/*!
 * Join accept delay in ms
 */
#define JOIN_ACCEPT_DELAY                           5000000

/*!
 * Maximum allowed gap for the FCNT field
 */
#define MAX_FCNT_GAP                                16384

/*!
 * ADR acknowledgement counter limit
 */
#define ADR_ACK_LIMIT                               16

/*!
 * Number of ADR acknowledgement requests before returning to default datarate
 */
#define ADR_ACK_DELAY                               8

/*!
 * Number of seconds after the start of the second reception window without
 * receiving an acknowledge
 */
#define ACK_TIMEOUT                                 4000000

/*!
 * RSSI free threshold
 */
#define RSSI_FREE_TH                                -90 // [dBm]

/*! 
 * Frame direction definition
 */
#define UP_LINK                                     0
#define DOWN_LINK                                   1

/*!
 *
 */
#define LORAMAC_TX_ERROR_TIMEOUT                    0

/*!
 *
 */
#define LORAMAC_RX_ERROR_TIMEOUT                    0
#define LORAMAC_RX_ERROR_RADIO                      1

/*!
 * Sets the length of the LoRaMAC footer field.
 * Mainly indicates the MIC field length
 */
#define LORAMAC_MFR_LEN                             4

/*!
 * LoRaMAC channels parameters definition
 */
typedef struct
{
    uint8_t  Enabled;   // Channel enabled
    uint32_t Frequency; // [Hz]
    uint8_t  Datarate;  // LORA [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
                        // FSK bits/s
    uint8_t  Bw;        // [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved] 
    int8_t   Power;     // [dBm]
}ChannelParams_t;

/*!
 * LoRaMAC frame types
 */
typedef enum
{
    FRAME_TYPE_JOIN_REQ              = 0x00,
    FRAME_TYPE_JOIN_ACCEPT           = 0x01,
    FRAME_TYPE_DATA_UNCONFIRMED      = 0x02,
    FRAME_TYPE_DATA_CONFIRMED        = 0x03,
    FRAME_TYPE_PROPRIETARY           = 0x07,
}LoRaMacFrameType_t;


/*!
 * LoRaMAC mote MAC commands
 */
typedef enum
{
    MOTE_MAC_LINK_CHECK_REQ          = 0x02,
    MOTE_MAC_LINK_ADR_ANS            = 0x03,
    MOTE_MAC_DEV_STATUS_ANS          = 0x06,
}LoRaMacMoteCmd_t;

/*!
 * LoRaMAC server MAC commands
 */
typedef enum
{
    SRV_MAC_LINK_CHECK_ANS           = 0x02,
    SRV_MAC_LINK_ADR_REQ             = 0x03,
    SRV_MAC_DEV_STATUS_REQ           = 0x06,
}LoRaMacSrvCmd_t;

/*!
 * LoRaMAC Battery level indicator
 */
typedef enum
{
    BAT_LEVEL_EXT_SRC                = 0x00,
    BAT_LEVEL_EMPTY                  = 0x01,
    BAT_LEVEL_FULL                   = 0xFE,
    BAT_LEVEL_NO_MEASURE             = 0xFF,
}LoRaMacBatteryLevel_t;

/*!
 * LoRaMAC header field definition
 */
typedef union
{
    uint8_t Value;
    struct
    {
        uint8_t Major           : 2;
        uint8_t RFU             : 3;
        uint8_t MType           : 3;
    }Bits;
}LoRaMacHeader_t;

/*!
 * LoRaMAC frame header field definition
 */
typedef union
{
    uint8_t Value;
    struct
    {
        uint8_t OptionsLength   : 4;
        uint8_t FPending        : 1;
        uint8_t Ack             : 1;
        uint8_t AdrAckReq       : 1;
        uint8_t Adr             : 1;
    }Bits;
}LoRaMacFrameCtrl_t;

/*!
 * LoRaMAC event flags
 */
typedef union
{
    uint8_t Value;
    struct
    {
        uint8_t Tx              : 1;
        uint8_t Rx              : 1;
        uint8_t LinkCheck       : 1;
        uint8_t                 : 4;
        uint8_t JoinAccept      : 1;
    }Bits;
}LoRaMacEventFlags_t;

typedef enum
{
    LORAMAC_EVENT_INFO_STATUS_OK = 0,
    LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT,
    LORAMAC_EVENT_INFO_STATUS_RX_ERROR,
    LORAMAC_EVENT_INFO_STATUS_RX_TIMEOUT,
}LoRaMacEventInfoStatus_t;

/*!
 * LoRaMAC event information
 */
typedef struct
{
    LoRaMacEventInfoStatus_t Status;
    bool TxAckReceived;
    uint8_t TxNbRetries;
    uint8_t TxDatarate;
    uint8_t RxPort;
    uint8_t *RxBuffer;
    uint8_t RxBufferSize;
    int8_t RxRssi;
    uint8_t RxSnr;
    uint16_t Energy;
    uint8_t DemodMargin;
    uint8_t NbGateways;
}LoRaMacEventInfo_t;

/*!
 * LoRaMAC events structure
 * Used to notify upper layers of MAC events
 */
typedef struct sLoRaMacEvent
{
    /*!
     * MAC layer event callback prototype.
     *
     * \param [IN] flags Bit field indicating the MAC events occurred
     * \param [IN] info  Details about MAC events occurred
     */
    void ( *MacEvent )( LoRaMacEventFlags_t *flags, LoRaMacEventInfo_t *info );
}LoRaMacEvent_t;

/*!
 * LoRaMAC layer initialization
 *
 * \param [IN] events        Pointer to a structure defining the LoRaMAC
 *                           callback functions.
 */
void LoRaMacInit( LoRaMacEvent_t *events );

/*!
 * Enables/Disables the ADR (Adaptive Data Rate)
 * 
 * \param [IN] enable [true: ADR ON, false: ADR OFF]
 */
void LoRaMacSetAdrOn( bool enable );

/*!
 * Initializes the network IDs. Device address, 
 * network session AES128 key and application session AES128 key.
 *
 * \remark To be only used when Over-the-Air activation isn't used.
 *
 * \param [IN] devAddr 32 bits devide address on the network 
 *                     (must be unique to the network)
 * \param [IN] nwkSKey Pointer to the network session AES128 key array
 *                     ( 16 bytes )
 * \param [IN] appSKey Pointer to the application session AES128 key array
 *                     ( 16 bytes )
 */
void LoRaMacInitNwkIds( uint32_t devAddr, uint8_t *nwkSKey, uint8_t *appSKey );

/*!
 * Initiates the Over-the-Air activation 
 * 
 * \param [IN] devEui Pointer to the device EUI array ( 8 bytes )
 * \param [IN] appEui Pointer to the application EUI array ( 8 bytes )
 * \param [IN] appKey Pointer to the application AES128 key array ( 16 bytes )
 *
 * \retval status [0: OK, 1: Tx error, 2: Already joined a network]
 */
uint8_t LoRaMacJoinReq( uint8_t *devEui, uint8_t *appEui, uint8_t *appKey );

/*!
 * Sends a LinkCheckReq MAC command on the next uplink frame
 *
 * \retval status Function status [0: OK, 1: Busy]
 */
uint8_t LoRaMacLinkCheckReq( void );

/*!
 * LoRaMAC layer send frame
 *
 * \param [IN] fPort       MAC payload port (must be > 0)
 * \param [IN] fBuffer     MAC data buffer to be sent
 * \param [IN] fBufferSize MAC data buffer size
 *
 * \retval status          [0: OK, 1: Busy, 2: No network joined,
 *                          3: Length or port error, 4: Unknown MAC command
 *                          5: Unable to find a free channel]
 */
uint8_t LoRaMacSendFrame( uint8_t fPort, void *fBuffer, uint16_t fBufferSize );

/*!
 * LoRaMAC layer send frame
 *
 * \param [IN] fPort       MAC payload port (must be > 0)
 * \param [IN] fBuffer     MAC data buffer to be sent
 * \param [IN] fBufferSize MAC data buffer size
 * \param [IN] fBufferSize MAC data buffer size
 * \param [IN] nbRetries   Number of retries to receive the acknowledgement
 *
 * \retval status          [0: OK, 1: Busy, 2: No network joined,
 *                          3: Length or port error, 4: Unknown MAC command
 *                          5: Unable to find a free channel]
 */
uint8_t LoRaMacSendConfirmedFrame( uint8_t fPort, void *fBuffer, uint16_t fBufferSize, uint8_t nbRetries );

/*!
 * ============================================================================
 * = LoRaMac test functions                                                   =
 * ============================================================================
 */

/*!
 * LoRaMAC layer generic send frame
 *
 * \param [IN] macHdr      MAC header field
 * \param [IN] fOpts       MAC commands buffer
 * \param [IN] fPort       MAC payload port
 * \param [IN] fBuffer     MAC data buffer to be sent
 * \param [IN] fBufferSize MAC data buffer size
 * \retval status          [0: OK, 1: Busy, 2: No network joined,
 *                          3: Length or port error, 4: Unknown MAC command
 *                          5: Unable to find a free channel]
 */
uint8_t LoRaMacSend( LoRaMacHeader_t *macHdr, uint8_t *fOpts, uint8_t fPort, void *fBuffer, uint16_t fBufferSize );

/*!
 * LoRaMAC layer generic send frame with channel specification
 *
 * \param [IN] channel     Channel parameters
 * \param [IN] macHdr      MAC header field
 * \param [IN] fCtrl       MAC frame control field
 * \param [IN] fOpts       MAC commands buffer
 * \param [IN] fPort       MAC payload port
 * \param [IN] fBuffer     MAC data buffer to be sent
 * \param [IN] fBufferSize MAC data buffer size
 * \retval status          [0: OK, 1: Busy, 2: No network joined,
 *                          3: Length or port error, 4: Unknown MAC command]
 */
uint8_t LoRaMacSendOnChannel( ChannelParams_t channel, LoRaMacHeader_t *macHdr, LoRaMacFrameCtrl_t *fCtrl, uint8_t *fOpts, uint8_t fPort, void *fBuffer, uint16_t fBufferSize );

/*!
 * Enables the MIC field test
 *
 * \param [IN] txPacketCounter Fixed Tx packet counter value
 */
void LoRaMacSetMicTest( uint16_t txPacketCounter );

#endif // __LORAMAC_H__
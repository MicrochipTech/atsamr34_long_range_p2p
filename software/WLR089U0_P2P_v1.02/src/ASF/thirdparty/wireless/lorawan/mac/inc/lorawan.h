/**
* \file  lorawan.h
*
* \brief LoRaWAN header file
*		
*
* Copyright (c) 2019-2020 Microchip Technology Inc. and its subsidiaries. 
*
* \asf_license_start
*
* \page License
*
* Subject to your compliance with these terms, you may use Microchip
* software and any derivatives exclusively with Microchip products. 
* It is your responsibility to comply with third party license terms applicable 
* to your use of third party software (including open source software) that 
* may accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, 
* WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, 
* INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, 
* AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE 
* LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL 
* LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE 
* SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE 
* POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY 
* RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, 
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*
* \asf_license_stop
*
*/
/*
* Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
*/


#ifndef _LORAWAN_H
#define	_LORAWAN_H

#ifdef	__cplusplus
extern "C" {
#endif



/****************************** INCLUDES **************************************/
#include <stdint.h>
#include <stdlib.h>
#include "compiler.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "stack_common.h"

/***************************** DEFINES ***************************************/
#define INFINITE_CHANNEL_RETRY					0xFFFF

//Data rate (DR) encoding
#define DR0                                     0
#define DR1                                     1
#define DR2                                     2
#define DR3                                     3
#define DR4                                     4
#define DR5                                     5
#define DR6                                     6
#define DR7                                     7
#define DR8                                     8
#define DR9                                     9
#define DR10                                    10
#define DR11                                    11
#define DR12                                    12
#define DR13                                    13
#define DR14                                    14
#define DR15                                    15

#define LAST_NIBBLE                             0x0F
#define FIRST_NIBBLE                            0xF0

#define LORAWAN_SESSIONKEY_LENGTH					(16)

/***************************** TYPEDEFS ***************************************/

/** Features Supported List */
typedef enum _FeaturesSupported
{
	/*None*/
	NONE,
	/* Duty Cycle Feature is Supported */
	DUTY_CYCLE_SUPPORT = 1 << 1u,
	/* Listen Before Talk is Supported */
	LBT_SUPPORT = 1 << 2u,
	/* FHSS is Supported */
	FHSS_SUPPORT = 1<< 3u,
	/* PA is supported in the Board */
	PA_SUPPORT = 1 << 4u,
	/*Join backoff is supported*/
	JOIN_BACKOFF_SUPPORT = 1 << 5u
} FeaturesSupported_t;

/* minimum and maximum data rate */
typedef union _DataRange
{
    uint8_t value;
	COMPILER_PACK_SET(1)
    struct
    {
       uint8_t min:4;
       uint8_t max:4;
    };
	COMPILER_PACK_RESET()
} DataRange_t;

/** event indexes for application interface */
typedef enum _LorawanEvent
{
    /* LORAWAN TX Done Event */
    LORAWAN_EVT_TX_DONE = 1 << 0u,
    /* LORAWAN RX Data Available Event */
    LORAWAN_EVT_RX_DATA_AVAILABLE = 1 << 1u,
    /* LORAWAN Transaction Complete Event */
    LORAWAN_EVT_TRANSACTION_COMPLETE = 1<< 2u,
    /* Unsupported Event */
    LORAWAN_EVT_UNSUPPORTED = 1 << 3u
} LorawanEvent_t;

/* Operation status */
typedef enum _OpStatus
{
    /* LoRaWAN operation failed */
    MAC_NOT_OK = 0,
    /* LoRaWAN operation successful */
    MAC_OK,
    /* Radio operation failed */
    RADIO_NOT_OK,
    /* Radio operation successful */
    RADIO_OK,
    /* During retransmission, we have changed SF and the buffer is too large */
    INVALID_BUFFER_LEN,
	/* Radio Channel is busy */
	RADIO_CHANNEL_BUSY,
	/*Invalid frame counter*/
	INVALID_FCNTR, 
	/*Mic Error*/                     
	MIC_ERROR,        
	/* Invalid Frame type*/                 
	INVALID_MTYPE,  
	/*Invalid mcast header*/                     
	MCAST_HDR_INVALID, 
	/*Ack Not received*/                 
	NO_ACK  
} OpStatus_t;

/* Activation Types*/
typedef enum _ActivationType
{
    /* LoRaWAN Over The Air Activation - OTAA */
    LORAWAN_OTAA = 0,
    /* LoRaWAN Activation By Personalization - ABP */
    LORAWAN_ABP
} ActivationType_t;

/* Transmission Types*/
typedef enum _TransmissionType
{
    /* LoRaWAN Unconfirmed Transmission */
    LORAWAN_UNCNF = 0,
    /* LoRaWAN Confirmed Transmission */
    LORAWAN_CNF
} TransmissionType_t;

/** Error Coding Configuration
 * This structure is used to override the error coding rate
 * in RADIO layer when transmitting the packet.
 * The override variable is used to inform MAC layer to SET
 * ecr value in the structure.
 * In general, ecr value will not be SET for every transmission.
 **/
typedef struct _ecrConfig
{
	uint8_t ecr;
	bool override;
}ecrConfig_t;

typedef struct _radioConfig
{
	uint32_t frequency;
	uint16_t freq_hop_period;
	ecrConfig_t ecrConfig;
	int8_t txPower;
	uint8_t modulation;
	uint8_t bandwidth;
	uint8_t sf;
}radioConfig_t;

typedef enum _EdClass
{
    /* Bidirectional End device - All End device should implement the Class A feature */
    CLASS_A = 1 << 0u,
    /* End device with scheduled receive slots */
    CLASS_B = 1 << 1u,
    /* End device with maximum receive slots - Continuously listening */
    CLASS_C = 1 << 2u
} EdClass_t;

/* GPS Epoch Time values */
typedef struct _GPSEpochTime
{
    /* Time in seconds since GPS Epoch value - 4 bytes */
    uint32_t secondsSinceEpoch;
    /* Fractional second value in milliseconds - 1 byte */
    uint8_t  fractionalSecond;
} GPSEpochTime_t;

/** Device time value in relation to GPS Epoch.
*
* This structure contains the value of GPS Epoch value and
* corresponding system time value when the GPS Epoch
* value is received by the stack
**/
typedef struct _DevTime
{
    /* Flag to indicate Device Time request mac command is sent */
    bool isDevTimeReqSent;
    /* Index of SwTimestamp_t that stores the system time corresponding to `gpsEpochTime` */
    uint8_t sysEpochTimeIndex;
    /* GPS Epoch time received in DeviceTimeAnswer MAC Command */
    GPSEpochTime_t gpsEpochTime;
} DevTime_t;

typedef union _StackVersion_t
{
    uint32_t value;
    COMPILER_PACK_SET(1)
    struct
    {
        uint32_t reserved1 : 14;
        uint32_t iteration :  4;
        uint32_t qualifier :  2;
        uint32_t minor     :  4;
        uint32_t major     :  4;
        uint32_t reserved2 :  4;
    };
    COMPILER_PACK_RESET()
} StackVersion_t;

/* LORAWAN Status information*/
typedef union _LorawanStatus
{
    uint32_t value;
    struct
    {   /* If set, the network is joined */
        unsigned networkJoined :1;
        /* determines the state of trasmission (rx window open, between tx and rx, etc*/
        unsigned macState :3;
        /* If set, ACK and uplink packets sent due to FPending will be sent immediately */
        unsigned automaticReply :1;
        /* If set, adaptive data rate is requested by server or application */
        unsigned adr :1;
        /* If set, the Mac command duty cycle request was received */
        unsigned silentImmediately :1;
        /* If set, the mac Pause function was called. LoRa modulation is not possible */
        unsigned macPause :1;
        /* If set, data is ready for reception */
        unsigned rxDone :1;
        /* If set, linkCheck mechanism is enabled */
        unsigned linkCheck :1;
        /* If set, new channels are added via CFList or NewChannelRequest command or enabled/disabled via Link Adr command */
        unsigned channelsModified :1;
        /* If set, the txPower was modified via Link Adr command */
        unsigned txPowerModified :1;
        /* If set, the number of repetitions for unconfirmed frames has been modified */
        unsigned nbRepModified :1;
        /* If set, the prescaler has changed via duty cycle request */
        unsigned prescalerModified :1;
        /* If set, the second receive window parameters have changed */
        unsigned secondReceiveWindowModified :1;
        /* If set, the delay between the end of the TX uplink and the opening of the first reception slot has changed */
        unsigned rxTimingSetup :1;
        /* If set, the device must be rejoined as a frame counter issue happened */
        unsigned rejoinNeeded :1;
        /* If set, Multicast frame reception feature is enabled */
        unsigned mcastEnabled :1;
    };
} LorawanStatus_t;

/*Structure for storing transmit frame parameters*/
typedef struct _LorawanSendReq
{
    /* Type of Transmission - CNF/UNCNF */
    TransmissionType_t confirmed;
    /* FPORT value (0-255)*/
	uint8_t port;
    /* Pointer to the transmit data*/
	void *buffer;
    /* Length of the transmit buffer*/
	uint8_t bufferLength;
}LorawanSendReq_t;

/* Structure for storing callback parameters*/
typedef struct _appCbParams
{
    /*Event type*/
    LorawanEvent_t evt;
    union
    {   /* Structure for holding TX Done cb parameters */
        struct
        {
            /* currently none */
			uint8_t dummy;
        } txDone;

        /* Structure for holding RX Data Available cb parameters */
        struct
        {
            /* Source address in the payload. DeviceAddr in case of unicast;
               MulticastAddr in cast of multicast */
            uint32_t devAddr;
            /* Pointer to the received data */
            uint8_t* pData;
            /* Received data length */
            uint8_t dataLength;
            /* Status of operation */
            StackRetStatus_t status;
            /* Reserved bytes for packing */
            uint16_t reserved;
        } rxData;

        /* Structure for holding Transaction complete cb parameters */
        struct
        {
            /* Status of Operation */
            StackRetStatus_t status;
        } transCmpl;
    } param;
} appCbParams_t;

/*#ifdef LBT*/
/*Structure for storing listen Before Talk get and set parameters*/
typedef struct _LorawanLBTParams
{
	/* Scan duration of a single channel */
	uint16_t lbtScanPeriod;
	/* Threshold above which channel is assumed to be occupied */
	int16_t lbtThreshold;
	/* This field controls the number of MAX channels to do lbt.
	 * If set to 0xFFFF, lbt will be done till a free channel is found
	 * If set to 0, retry will not be done, so lbt will be done once
	 * If set to 1, retry will be done once, so lbt will be done twice
	 */
	uint16_t maxRetryChannels;
	/* Number of RSSI read samples for a single channel */
	uint8_t lbtNumOfSamples;
	/* Switch for radio to decide if the transmit request is LBT based */
	bool lbtTransmitOn;
} LorawanLBTParams_t;

typedef struct _LorawanMcastDevAddr
{
	uint8_t groupId;
	uint32_t mcast_dev_addr;
}LorawanMcastDevAddr_t;

typedef struct _LorawanMcastAppSkey
{
	uint8_t groupId;
	uint8_t mcastAppSKey[LORAWAN_SESSIONKEY_LENGTH];
}LorawanMcastAppSkey_t;

typedef struct _LorawanMcastNwkSkey
{
	uint8_t groupId;
	uint8_t mcastNwkSKey[LORAWAN_SESSIONKEY_LENGTH];
}LorawanMcastNwkSkey_t;

typedef struct _LorawanMcastFcnt
{
	uint8_t groupId;
	uint32_t fcntValue;
}LorawanMcastFcnt_t;

typedef struct _LorawanMcastDatarate
{
    uint8_t groupId;
    uint8_t datarate;
} LorawanMcastDatarate_t;

typedef struct _LorawanMcastDlFreqeuncy
{
    uint8_t groupId;
    uint32_t dlFrequency;
} LorawanMcastDlFreqeuncy_t;

typedef struct _LorawanMcastPeriodicity
{
    uint8_t groupId;
    uint8_t periodicity;
} LorawanMcastPeriodicity_t;

typedef struct _LorawanMcastStatus
{
	uint8_t groupId;
	bool status;
}LorawanMcastStatus_t;

typedef struct _TimeOnAirParams
{
    uint8_t dr;
    uint8_t impHdrMode;
    uint8_t crcOn;
    uint8_t cr;
    uint8_t pktLen;
    uint16_t preambleLen;
} TimeOnAirParams_t;

/* List of LORAWAN attributes */
typedef enum _LorawanAttributes
{
    /************************ Read & Write Attributes *************************/
    /* Device EUI - a global end-device ID in IEEE EUI64 address space that uniquely identifies end-device */
    DEV_EUI = 0,
    /* Application EUI - a global application ID in IEEE EUI64 address space that uniquely identifies the application provider (i.e., owner) of the end-device. */
    JOIN_EUI,
    /* Device Address - a 32bit identifier of the end-device within the current network */
    DEV_ADDR,
    /* Application Key - an AES-128 application key specific for the end-device
     * that is assigned by the application owner to the end-device */
    APP_KEY,
    /* Network Session Key - specific for the end-device and
     * it is used to calculate and verify the MIC (Message Integrity Code)*/
    NWKS_KEY,
    /* Application Session Key - specific for the end-device;
     * it is used to encrypt/decrypt the payload field of application-specific data messages,
     * and also to calculate/verify an application-level MIC (Message Integrity Code)that may be included in the payload.
     */
    APPS_KEY,
    /* ADR Enable - Adaptive Data Rate (ADR) mode Enable
     * LoRa network allows the end-devices to individually use any of the possible data rates, this is referred to as Adaptive Data Rate (ADR).
     * If the ADR is set, the network will control the data rate of the end-device through the appropriate MAC commands.
     * If the ADR is not set, the network will not attempt to control the data rate of the end-device regardless of the received signal quality.
     */
    ADR,
    /* Data Rate for the next uplink */
    CURRENT_DATARATE,
    /* TX Output Power */
    /* */
    TX_POWER,
    /* Synchronization Word */
    SYNC_WORD,
    /* Uplink Counter - Framecounter for uplink messages*/
    UPLINK_COUNTER,
    /* Downlink Counter - Framecounter for downlink messages*/
    DOWNLINK_COUNTER,
    /* ReceiveWindow1 Delay - Delay between the transmission and the first Reception window.
     * Refer LORa Regional parameters spec for more information on default value
     */
    RX_DELAY1,
    /* JoinAcceptWindow1 Delay - Delay between the transmission of Join req message and first reception window
     * Refer LORa Regional parameters spec for more information on default value
     */
    JOINACCEPT_DELAY1,
    /* JoinAcceptWindow2 Delay - Delay between the transmission of Join req message and second reception window
     * Refer LORa Regional parameters spec for more information on default value
     */
    JOINACCEPT_DELAY2,
    /* Maximum Frame Counter Gap - Refer LORa Regional parameters spec for more information on default value */
    MAX_FCOUNT_GAP,
    /* ADR Acknowledgment Limit - Refer LORa Regional parameters spec for more information on default value */
    ADR_ACKLIMIT,
    /* ADR Acknowledgment Delay - Refer LORa Regional parameters spec for more information on default value */
    ADR_ACKDELAY,
    /* Retranmsission Timeout - Refer LORa Regional parameters spec for more information on default value */
    RETRANSMITTIMEOUT,
    /* No of Retransmission for the confirmed uplink messages */
    CNF_RETRANSMISSION_NUM,
	/* No of Repetition for the unconfirmed uplink messages */
	UNCNF_REPETITION_NUM,
    /* Receive Window2 configurable parameters - RX2 window frequency and Datarate*/
    RX2_WINDOW_PARAMS,
	/* Class C receive window configurable parameters - RXC window frequency and Datarate*/
	RXC_WINDOW_PARAMS,
    /* Automatic Reply - If set, response to ACK and MAC command will be sent immediately */
    AUTOREPLY,
    /* Battery level of the device */
    BATTERY,
    /* Link Check Period - Period at which Link req command will be sent to Server by End device*/
    LINK_CHECK_PERIOD,
    /* Type of End device - Class A/B/C */
    EDCLASS,
    /***********************Channel related parameters*************************/
    /* Frequency of channel */
    CH_PARAM_FREQUENCY,
    /* Datarate range of channel*/
    CH_PARAM_DR_RANGE,
    /* Status of the channel - Enabled/Disabled */
    CH_PARAM_STATUS,
    /* Duty cycle of channel */
    CH_PARAM_DUTYCYCLE,

    /**************************Read Only Attributes****************************/
    /* ReceiveWindow2 Delay - This attribute cannot be configurable because RX2_delay = RX1_delay + 1s */
	RX_DELAY2,
    /* Aggregated Dutycycle sent by the Server to the end device by MAC Command */
	AGGREGATED_DUTYCYCLE,
    /* Status of LORAWAN Layer */
	LORAWAN_STATUS,
    /* Link check Margin - Demodulation margin in dB of the last successfully received LinkCheckAns command*/
	LINK_CHECK_MARGIN,
    /* Link check Gateway count - No. of Gateways that successfully received LinkCheckReq command */
	LINK_CHECK_GWCNT,
    /* Denotes the ISM band in which End device is operating */
	ISMBAND,
	/* EDCLASS_SUPPORTED - Device classes supported in current implementation */
	EDCLASS_SUPPORTED,
    MACVERSION,
	FHSS_CALLBACK,
	/* Lorawan LBT parameter sets and gets both Radio and Lorawan parameters defined in LorawanLBTParams_t */
	LORAWAN_LBT_PARAMS,
	/************************ Multicast related Attributes**********************/
	/* Multicast Enable - This is to enable the multicast packet reception */
	MCAST_ENABLE,
	/* Multicast Address - a 32bit multicast address of the device */
	MCAST_GROUP_ADDR,
	/* Multicast Network session key - 16 byte key to calculate the MIC of received Multicast packet */
	MCAST_NWKS_KEY,
	/* Multicast Application Session Key - 16 byte key to decrypt the Multicast application packets */
	MCAST_APPS_KEY,
    /* Multicast DL Frame counter */
    MCAST_FCNT_DOWN,
	/* Multicast DL Frame counter min */
	MCAST_FCNT_DOWN_MIN,
	/* Multicast DL Frame counter max */
	MCAST_FCNT_DOWN_MAX,
    /* Multicast downlink Frequency */
    MCAST_FREQUENCY,
    /* Multicast downlink datarate */
    MCAST_DATARATE,
    /* Multicast class-B periodicity */
    MCAST_PERIODICITY,
	/*Enables Test Mode which Disable Duty Cycle for a Device, Override Features Supported by Regulatory*/
	TEST_MODE_ENABLE,
	/*Enables Join backoff support as per in Specification */
	JOIN_BACKOFF_ENABLE,
/* Bands enabled at compile time */
    SUPPORTED_BANDS,
	/* RSSI of last recevied packet */
	LAST_PACKET_RSSI,
	/* FPENDING is true */
	IS_FPENDING,
	/* Downlink ACK required is true */
	DL_ACK_REQD,
	/* Last used channel ID */
	LAST_CH_ID,
	/* Duty Cycle pending interval */
	PENDING_DUTY_CYCLE_TIME,
	/* Number of confirmed retries in one transactions */
	RETRY_COUNTER_CNF,
	/* Number of unconfirmed retries in one transactions */
	RETRY_COUNTER_UNCNF,
	/* Next Payload size */
	NEXT_PAYLOAD_SIZE,
	/* Pending Join Back Off time */
	PENDING_JOIN_DUTY_CYCLE_TIME,
	/* Every-time the frame counter reaches the modulo of count value, 
	 * the Frame counter will be updated in PDS.
	 * For eg: if maxFcntPdsUpdateValue is 2 (default value), 
	 * then every time (Frame counter % maxFcntPdsUpdateValue) == 0, 
	 * Frame counter is stored in PDS.
	 * Note: After reset of the device, the PDS restore of frame 
	 * counters will increase the value to times equal to maxFcntPdsUpdateValue
	 * For eg: If the current stored Frame counter in PDS is 10 and maxFcntPdsUpdateValue is 2,
	 * then during restore operation, the new frame counter will be equal to 12
	 * This value is used in terms of power of 2. The max value is 256 (2 ^ 8).
	 */
	MAX_FCNT_PDS_UPDATE_VAL,
	/* Informing MAC that Crypto device is used for keyStorage */
	CRYPTODEVICE_ENABLED,
    /* If set, ED shall send DeviceTimeReq cmd in next TX */
    SEND_DEVICE_TIME_CMD,
    /* Returns the software version of the running stack */
    STACK_VERSION,
    /* Returns the gps epoch time handle */
    DEVICE_GPS_EPOCH_TIME,
    /* Returns the packet time on air */
    PACKET_TIME_ON_AIR,
    /* Enable or disable duty cycle feature separately */
    REGIONAL_DUTY_CYCLE,
    /* If set, ED shall send LinkCheckReq cmd in next TX */
    SEND_LINK_CHECK_CMD,
} LorawanAttributes_t;

/* Structure holding Receive window2 parameters*/
/* This can be used for setting/getting RX2_WINDOW_PARAMS attribute */
typedef struct _ReceiveWindow2Params
{
	uint32_t frequency;
	uint8_t dataRate;
}ReceiveWindow2Params_t;

/* Channel Index - Used for masking the particular channel in a channel list*/
typedef uint8_t ChannelId_t;

/* Various attributes of the particular channel*/
typedef union _ChannelAttr
{
    /* Frequency of the channel - Resolution 100Hz, can be set from 1MHz to 1.6GHz */
	uint32_t frequency;
    /* Dutycycle of the channel*/
	uint16_t dutyCycle;
    /* Data rate range that can be supported by the channel*/
	uint8_t dataRange;
    /* Status of the channel - Enable/Disable*/
	bool status;
}ChannelAttr_t;

/* Structure holding the particular channel attributes*/
/* This can be used for setting/getting Channel parameters like Frequency, Datarate Range,
 * Dutycycle, Status */
typedef struct _ChannelParameters
{   /* Index of the channel */
	ChannelId_t channelId;
    /* One of the channel attributes value*/
	ChannelAttr_t channelAttr;
}ChannelParameters_t;

/* Function Pointer to Application Data callback */
typedef void (*AppDataCb_t)(void *appHandle, appCbParams_t *data);
/* Function Pointer to Activation Data callback */
typedef void (*JoinResponseCb_t)(StackRetStatus_t status);

/* Structure holding function pointers of Application and Activation data callbacks */
COMPILER_PACK_SET(1)
typedef struct
{
	/* pointer to function that gets called after the bidirectional communication ended */
    void (*AppData)(void *appHandle, appCbParams_t *data);
	/* pointer to function that gets called after the activation procedure */
    void (*JoinResponse)(StackRetStatus_t status);
} AppData_t;
COMPILER_PACK_RESET()


typedef union
{
	uint16_t value;
	struct
	{
		unsigned ackRequiredFromNextDownlinkMessage:1;  //if set, the next downlink message should have the ACK bit set because an ACK is needed for the end device
		unsigned ackRequiredFromNextUplinkMessage:1;    //if set, the next uplink message should have the ACK bit set because an ACK is needed for the server
		unsigned joining: 1;
		unsigned fPending:1;
		unsigned adrAckRequest:1;
		unsigned syncronization:1;                      //if set, there is no need to send an immediat packet because the application sent one from the callback
	};
} LorawanMacStatus_t;


/*********************************************************************//**
\brief	The radio callback typedef used to declare callback functions
		for registering to FHSS.
*************************************************************************/
typedef uint32_t (*FHSSCallback_t)(void);


/*************************** FUNCTIONS PROTOTYPE ******************************/
// Initialization functions
/**
 * @Summary
    LoRaWAN Initialization function
 * @Description
    This function initializes LoRaWAN stack and the radio module.
 * @Preconditions
    None
 * @Param
    appdata - pointer to function that gets called after the bidirectional communication ended.
    joindata - pointer to function that gets called after the activation procedure
 * @Return
    None
 * @Example
*/
void LORAWAN_Init(AppDataCb_t appdata, JoinResponseCb_t joindata /*,IsmBand_t ismBand*/);

/**
 * @Summary
    LoRaWAN activation procedure.
 * @Description
    This function starts LoRaWAN activation procedure.
 * @Preconditions
    None
 * @Param
    activationTypeNew - activation type: OTAA or ABP
 * @Returns
    Function returns the status of the operation (StackRetStatus_t).
 * @Example
*/
StackRetStatus_t LORAWAN_Join(ActivationType_t activationTypeNew);

/**
 * @Summary
    Bidirectional communication start.
 * @Description
    This function starts a bidirectional communication process.
 * @Preconditions
    None
 * @Param
    confirmed - represents the transmission type; can be either UNCNF - unconfirmed or CNF - confirmed (TransmissionType_t)
    port - represents the port on which the transmission is being made; it's a number between 0 and 255 (uint8_t)
    buffer - a data buffer used to store the data to be sent
    bufferLength - the length in bytes of the data buffer (uint8_t)
 * @Returns
    Function returns the status of the operation (StackRetStatus_t).
 * @Example
    uint8_t dataToSend = 45;
    LORAWAN_Send (UNCNF, 20, &dataToSend, sizeof(dataToSend));
*/
StackRetStatus_t LORAWAN_Send (LorawanSendReq_t *lorasendreq);

/**
 * @Summary
    Function pauses LoRaWAN stack.
 * @Description
    This function pauses the LoRaWAN stack functionality to allow transceiver (radio) configuration.
    Through the use of "mac pause", radio commands can be generated between a LoRaWAN protocol
    uplink application, and the LoRaWAN protocol Receive windows.
    This function will reply with the time interval in milliseconds that the transceiver can
    be used without affecting the LoRaWAN functionality.
 * @Preconditions
    None
 * @Param
     None
 * @Returns
     Returns the number in milliseconds representing how much it can be paused without affecting the functionality.
     Returns 0 if it cannot be paused, maximum value when in idle mode.
 * @Example
*/
uint32_t LORAWAN_Pause (void);

/**
 * @Summary
    Function resumes LoRaWAN stack functionality.
 * @Description
    This function resumes LoRaWAN stack functionality, in order to continue normal
    functionality after being paused.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    None
 * @Example
*/
void LORAWAN_Resume (void);

/**
 * @Summary
    Function disables the Silent Immediately state.
 * @Description
    The network can issue a certain command that would require the end device to go silent immediately.
    This mechanism disables any further communication of the module, effectively isolating it from the network.
    Using this function, after this network command has been received, restores the modules connectivity
    by allowing it to send data.
 * @Preconditions
    None
 * @Param
    None
 * @Returns
    None
 * @Example
*/
void LORAWAN_ForceEnable (void);

/**
 * @Summary
    This function will automatically reset the software LoRaWAN stack and initialize it with the parameters for the selected ISM band.
 * @Description
    This command will set default values for most of the LoRaWAN parameters.
    Everything set prior to this command will lose its set value,
    being reinitialized to the default value, including setting the cryptographic keys to 0.
 * @Preconditions
    None
 * @Param
    none
 * @Returns
    The success/failure condition for the reset request
 * @Example
*/
StackRetStatus_t LORAWAN_Reset (IsmBand_t ismBand);

/**
 * @Summary
    LORAWAN Set Attribute
 * @Description
    This function is used to set the various Lorawan attributes.
 * @Preconditions
    None
 * @Param
    attrType - represents the attribute type; can be any of the parameters in (LorawanAttributes_t)
	attrValue - pointer that is used to get the attribute value
 * @Returns
    The error condition for getting a given attribute.
 * @Example
*/
StackRetStatus_t LORAWAN_SetAttr(LorawanAttributes_t attrType, void *attrValue);

/**
 * @Summary
    LORAWAN Get Attributes
 * @Description
    This function is used to get the various Lorawan attributes.
 * @Preconditions
    None
 * @Param
    attrType   - represents the attribute type; can be any of the parameters in LorawanAttributes_t
    attrInput  - pointer to the sub-parameter/index of attribute in an array for example to get CH_PARAM_FREQUENCY, channel index is given as input attribute
	attrOutput - pointer containing the attribute value
 * @Returns
    The error condition for getting a given attribute.
 * @Example
*/
StackRetStatus_t LORAWAN_GetAttr(LorawanAttributes_t attrType, void *attrInput, void *attrOutput);

/**
 * @Summary
    LoRaWAN Set Callback Bit mask function.
 * @Description
    This function is used for enabling the particular callback from MAC to application
 * @Preconditions
    None
 * @Param
    evtmask - Bit mask of the particular callback
 * @Returns
    none
 * @Example
 *  LORAWAN_SetCallbackBitmask(LORAWAN_EVT_RX_DATA_AVAILABLE|LORAWAN_EVT_TRANSACTION_COMPLETE);
*/
void LORAWAN_SetCallbackBitmask(uint32_t evtmask);

/**
 * @Summary
    This function returns the readiness of the stack for sleep
 * @Description
    This function is used for querying the readiness of the stack for sleep.
	This function has dependency on RADIO for corresponding readiness check function in TAL.
 * @Preconditions
    None
 * @Param
    \deviceResetAfterSleep -- 'true' means device will reset during wakeup
                              'false' means device does not reset during wakeup
 * @Returns
    'true' if stack is in ready state for sleep, otherwise 'false'
 * @Example
 *  bool status = LORAWAN_ReadyToSleep(false);
*/
bool LORAWAN_ReadyToSleep(bool deviceResetAfterSleep);

/**
 * @Summary
    LORAWAN Set Multicast Param
 * @Description
    This function is used to set the various Mcast attributes specific from application.
 * @Preconditions
    None
 * @Param
    attrType - represents the attribute type; can be any of the MCAST parameters in (LorawanAttributes_t)
	attrValue - pointer that is used to get the attribute value
 * @Returns
    The error condition for getting a given attribute.
 * @Example
*/
StackRetStatus_t LORAWAN_SetMulticastParam(LorawanAttributes_t attrType, void *attrValue);

#ifdef	__cplusplus
}
#endif

#endif	/* _LORAWAN_H */

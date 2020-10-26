# Real-life Use Case Examples
> "Wireless Made Easy!" - Enable Easy to use P2P Protocol for Long-Range & Low Power Applications on SAM R34 LoRa Technology ICs and WLR089U0 Module

[Back to Main page](../README.md)

## A la carte

1. [Introduction](#step1)
1. [Enable P2P Communication between end nodes](#step2)
   1. [Overview](#step2a)
   1. [Hardware Setup](#step2b)
   1. [Configure the Application](#step2c)
   1. [Run the demo](#step2d)
1. [Enable a Network composed of end nodes and a coordinator](#step3)
   1. [Overview](#step3a)
   1. [Hardware Setup](#step3b)
   1. [Configure the Application](#step3c)
   1. [Run the demo](#step3d)
1. [Enable Long-Range Communication](#step4)
   1. [Overview](#step4a)
   1. [Hardware Setup](#step4b)
   1. [Configure the Application](#step4c)


## Introduction<a name="step1"></a>

This section provides guidelines to customize the LoRa P2P reference sample code in order to allow different Peer-to-Peer network deployment and fulfill many use cases.

## Enable P2P Communication between end nodes<a name="step2"></a>

### Overview<a name="step2a"></a>

A typical P2P network is composed of two devices and that topology offers fastest, reliable and direct connection between devices. There is no need for network infrastructure. The message propagation are very limited.

<p align="center">
<img src="resources/media/typical_p2p.png" width=320>
</p>

In a deployment of such simple topology, it is important that the messages exchanged between those two devices will stay in a local and private network and not be understood by another network in the same vicinity. And this is where the PAN identifier can be used to differenciate the networks.

<p align="center">
<img src="resources/media/p2p_network_panid.png" width=520>
</p>

### Hardware Setup<a name="step2b"></a>

To demonstrate this scenario, minimum 4 boards are required. In a P2P network, at least one of the device must be a Full Functional Device (FFD) and the other can be either an FFD or a Reduce Functional Device (RFD).

<p align="center">
<img src="resources/media/setup_01.png" width=720>
</p>

### Configure the Application<a name="step2c"></a>

- Open file `miwi_config.h` and configure devices with same PAN identifier within a network

e.g. Setting for Device 1 and Device 2

```
/*********************************************************************/
// MY_PAN_ID defines the PAN identifier. Use 0xFFFF if prefer a 
// random PAN ID.
/*********************************************************************/
#define MY_PAN_ID                       0x1234
```

e.g. Setting for Device 3 and Device 4

```
/*********************************************************************/
// MY_PAN_ID defines the PAN identifier. Use 0xFFFF if prefer a 
// random PAN ID.
/*********************************************************************/
#define MY_PAN_ID                       0x2345
```

- In `miwi_config.h`, disable the active scan for faster connection process between devices
```
/*********************************************************************/
// ENABLE_ACTIVE_SCAN will enable the device to do an active scan to 
// to detect current existing connection. 
/*********************************************************************/
//#define ENABLE_ACTIVE_SCAN
```

- In file `p2p_demo.c`, the function to process the message received at application layer check already if the message arrived is coming from the same PAN identifier or not.

```
void ReceivedDataIndication (RECEIVED_MESSAGE *ind)
{
    /*******************************************************************/
    // If a packet has been received, handle the information available
    // in rxMessage.
    /*******************************************************************/
	if ((myPANID.v[1] == ind->SourcePANID.v[1]) && (myPANID.v[0] == ind->SourcePANID.v[0]))
	{
		#if defined(ENABLE_CONSOLE)
		DemoOutput_HandleMessage();
		#endif
		DemoOutput_UpdateTxRx(TxNum, ++RxNum);
		// Toggle LED2 to indicate receiving a packet.
		LED_Toggle(LED0);
		DemoOutput_Instruction();
	}
	else
	{
		printf("Packet received from other Source PAN ID : 0x%x%x\r\n",ind->SourcePANID.v[1],ind->SourcePANID.v[0]);
	}
}
```

- Compile and program the code into the different boards


### Run the demo<a name="step2d"></a>

- Turn ON the boards sequentially and observe the boards with the same PAN identifier will connect each others

<p align="center">
<img src="resources/media/setup_01_connection.png" width=720>
</p>

- Transmit a unicast message and check out the distribution of the message

<p align="center">
<img src="resources/media/setup_01_unicast.png" width=720>
</p>

- Transmit a broadcast message and check out the distribution of the message

<p align="center">
<img src="resources/media/setup_01_unicast.png" width=720>
</p>


## Enable a Network composed of end nodes and a central device<a name="step3"></a>

### Overview<a name="step3a"></a>

A typical P2P network is composed of only two devices. However with LoRa P2P protocol and thanks to the FFD and RFD device types, it is possible for multiple sleepy devices to transport their data up to a single central device.

<p align="center">
<img src="resources/media/setup_02.png" width=520>
</p>

The central device creates the network and must stays enable all the time. The end-nodes join the existing network and start communicating with the central device. The FFD exposes the messages received in the same PAN id from the different RFDs.


### Hardware Setup<a name="step3b"></a>

To demonstrate this scenario, minimum 3 boards are required:
* 1 FFD
* 2 RFDs

Here, the tested setup was 3 RFDs and 1 FFD

<p align="center">
<img src="resources/media/setup_02_.png" width=720>
</p>

### Configure the Application<a name="step3c"></a>

- Compile and program the sample application with default configuration for the FFD Device
- Make sure the FFD starts running before the others devices in order to create the network

- For all the 3 RFDs, perform the following steps:
   - Include  the `ENABLE_SLEEP_FEATURE` symbol in **Project Properties -> Toolchain -> ARM/GNU C Compiler -> Symbol**
   - Open the file `p2p_demo.c` and go to line 273
   - Instead of waking-up the device and sending a Data Request to parent device, let's modify the code to transmit an application payload periodically to FFD
   - Comment the line `MiApp_TransceiverPowerState(POWER_STATE_WAKEUP_DR);` and replace by the following snippet of code:
   
   ```
   /* Wakeup the transceiver and send data periodically (every RFD_WAKEUP_INTERVAL) */
	MiApp_TransceiverPowerState(POWER_STATE_WAKEUP);
	p2p_demo_unicast_to_parent() ;
   ```
   - Open file `miwi_config.h` and configure all devices with same PAN identifier
   - Compile and program the code into the different boards

### Run the demo<a name="step3d"></a>

- Turn ON the boards sequentially (start by the FFD)
- Observe the RFDs to perform the cycle: sleep - wakeup - transmit to parent every `RFD_WAKEUP_INTERVAL` and FFD to expose the received data
 
 <p align="center">
<img src="resources/media/setup_02_console.png" width=720>
</p>

## Enable Long-Range Communication<a name="step4"></a>

### Overview<a name="step4a"></a>

The SAMR34 Xplained Pro and the WLR089U0 Module Xplained Pro boards have been designed and optimized to support 868 and 915 MHz bands. WLR089U0 module is based on the ATSAMR34J18 IC.

 <p align="center">
<img src="resources/media/rfswitch_tcxo.png" width=520>
</p>

And both boards contain two essential components for the radio communication:
- A 32 MHz TCXO (Temperature Compensated Crystal Oscillator) which is used to clock the internal RF part. This TCXO helps in reducing the variation in the central frequency due to temperature and thus improve the receiver sensitivity.
- An RF Switch.

<p align="center">
<img src="resources/media/rf_switch.png" width=320>
</p>

The RF Switch is used to route high frequency signals through transmission path according to the V1 and V2 signals. The Xplained Pro boards support 868 MHz and 915 MHz bands and either PA_BOOST signal (high power) or RFO_HF signal (high efficiency transmitter limited to +13dBm) can be routed to the antenna. The Rx path is common for the two bands. The TX_RX signal is automatically controlled by the SAMR34’s transceiver. When transceiver is transmitting, it will drive TX_RX signal to 1 and for receive state, it will drive TX_RX signal to 0.\
Fore more details, check out the [WLR089U0 (SAMR34 Module) Reference Design Package](https://www.microchip.com/wwwproducts/en/WLR089U0) or the [SAMR34 IC Reference Design Package](https://www.microchip.com/wwwproducts/en/ATSAMR34J18).

To fully benefit from the LoRa Modulation and achieve longer range on SAMR34 device, several factors should be taken into consideration:
- Antenna performance
- Device positioning
- Surrounding environment
- Radio transceiver configuration (Tx power mode, ...)

Another points to consider are the different regional spectrum allocations and regulatory requirements. These include allowed frequencies in a particular region and maximum output power.\
e.g. up to +14dBm output power allowed by ETSI in Europe in the 867-869 MHz band\
e.g. +20dBm output power is possible in North America in the 902-928 MHz band

The [LoRa Modem Calculator Tool](https://www.semtech.com/uploads/documents/SX1272LoRaCalculatorSetup1_1.zip) is a useful tool to get a good estimation of the critical factors like time on air per frame, equivalent bitrate, RF performance characteristics and power consumption.

Following are the most important range determinants.
- Spreading Factor\
LoRa modulation converts symbols (binary data) to chirp signals that span the frequency range. The chirp time (symbol time) is roughly proportional to 2<sup>spreading factor</sup>. Each step up in spreading factor doubles the time on air to transmit a symbol. Each unit increase in SF correlates to about 2.5dB extra link budget. Higher spreading factors are more resistant to local noise effects and will be read more reliably at the cost of lower data rate and more congestion.

- Bandwidth\
Higher bandwidth has higher data rates and is more power-efficient, but has more congestion and less range. Each doubling of the bandwidth correlates to almost 3dB less link budget.

- Link Budget: this is how many dBs you can lose between the transmit PA and the receiver IF. Higher link budget directly translate to longer distance (6dB = twice distance).

From software standpoint, the way to get the longest range is by maximizing the link budget. The hardware way includes better antenna and more power.

It is possible to maximize the link budget by:
- lowest possible bandwith
- highest possible Spreading Factor

Unfortunately decreasing the bandwidth increases the risk of miscommunication due to frequency drift and inaccuracy. These settings also affect the data rate.

Effective Data Rate:\
The relationship between these settings and data rate is approximately:
Data Rate = Bandwidth / (2<sup>SF</sup>)

It is a trade-off to find according to the need of the end-application.

### Hardware Setup<a name="step4b"></a>

Ensure the antenna orientation is the same as below to get a good range.

<p align="center">
<img src="resources/media/samr34_xpro_orientation.png" width=>
</p>

Check out the [SAMR34 Xpro and SAMR34 module (WLR089U) range measurement info](https://microchip.wikidot.com/lora:range-test-comparison-between-samr34-and-wlr089u) for more details.

### Configure the Application<a name="step4c"></a>

Following are the LoRa P2P APIs located in `phy.c` which can be used to set the SAM R34 or WLR089U0 devices for maximum possible range.

#### Set max. Output Power

- API: `void PHY_SetTxPower(uint8_t txPower)`
- Settings for Europe: `PHY_SetTxPower(15)`
- Settings for North America: `PHY_SetTxPower(20)`

> PA boost is enabled if power is higher than 15. Check out the details of the API in the code.

#### Set Spreading Factor - SF12

- API: `RadioError_t PHY_SetAttribute(RadioAttribute_t RadioAttr,uint8_t* AttrVal)`
- Set SF12:\
`RadioDataRate_t sf = SF_12;`<br>
`PHY_SetAttribute(SPREADING_FACTOR, &sf);`

#### Set Bandwidth

- API: `RadioError_t PHY_SetAttribute(RadioAttribute_t RadioAttr,uint8_t* AttrVal)`
- BW_125kHz:\
`RadioLoRaBandWidth_t bw = BW_125KHZ;`<br>
`PHY_SetAttribute(BANDWIDTH, &bw);`

#### Get Spreading Factor

- API: `RadioError_t PHY_GetAttribute(RadioAttribute_t RadioAttr,uint8_t* AttrVal)`
- `RadioDataRate_t sf;`<br>
`PHY_GetAttribute(SPREADING_FACTOR, (void *)&sf);`






<a href="#top">Back to top</a>


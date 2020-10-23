# Real-life Use Case Examples
> "Wireless Made Easy!" - Enable Easy to use P2P Protocol for Long-Range & Low Power Applications on SAM R34 LoRa Technology ICs and WLR089U0 Module

[Back to Main page](../README.md)

## A la carte

1. [Introduction](#step1)
1. [Enable P2P Communication between end nodes](#step1)
   1. [Overview](#step1a)
   1. [Hardware Setup](#step1b)
   1. [Configure the Application](#step1c)
   1. [Run the demo](#step1d)
1. [Enable a Network composed of end nodes and a coordinator](#step2)
   1. [Overview](#step2a)
   1. [Hardware Setup](#step2b)
   1. [Configure the Application](#step2c)
   1. [Run the demo](#step2d)


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



<a href="#top">Back to top</a>


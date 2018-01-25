/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 The Boeing Company
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

// 
// This script configures two nodes on an 802.11b physical layer, with
// 802.11b NICs in adhoc mode, and by default, sends one packet of 1000 
// (application) bytes to the other node.  The physical layer is configured
// to receive at a fixed RSS (regardless of the distance and transmit
// power); therefore, changing position of the nodes has no effect. 
//
// There are a number of command-line options available to control
// the default behavior.  The list of available command-line options
// can be listed with the following command:
// ./waf --run "BroadcastTreeProtocolScenario --help"
//
// Note that all ns-3 attributes (not just the ones exposed in the below
// script) can be changed at command line; see the documentation.
//
// This script can also be helpful to put the Wifi layer into verbose
// logging mode; this command will turn on all wifi logging:
// 
// ./waf --run "BroadcastTreeProtocolScenario --verbose=1"
//
// When you are done, you will notice two pcap trace files in your directory.
// If you have tcpdump installed, you can try this:
//
// tcpdump -r BroadcastTreeProtocolScenario-0-0.pcap -nn -tt
//

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
//#include "ns3/internet-module.h"
#include "ns3/energy-module.h"
//#include "ns3/broadcast-tree-protocol-header.h"
//#include "ns3/broadcast-tree-protocol-helper.h"
//#include "ns3/broadcast-tree-protocol.h"


#include <string>


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("BroadcastTreeProtocolScenario");


// FLOODING FUNCTIONS DEFINITION-START-----------------------------------------

bool received ( Ptr<NetDevice>, Ptr<const Packet> rcvPacket, uint16_t protocolNumber, const Address& senderAddress)
{
  

  Ptr<Packet> packet = rcvPacket->Copy ();
  //packet->Print(std::cout);

  BroadcastTreeProtocolHeader btpHeader; 
  
  // if(packet->PeekHeader (btpHeader)){ ... }

  packet->RemoveHeader (btpHeader);

  /*
  uint8_t *buffer = new uint8_t () [packet->GetSize ()];
  packet->CopyData (buffer, packet->GetSize ()); 

  // Now buffer contains the serialized data in the packet
  

  //BroadcastTreeProtocolHeader btpHeader; 

 
  if(packet->PeekHeader (btpHeader))
    {
      // btpHeader present
      
      uint32_t seqNum = btpHeader.GetSequenceNumber();
      
      uint16_t srcFlag = btpHeader.GetSourceFlag(); 
      uint16_t ttl = btpHeader.GetTtl();
      uint16_t frameType = btpHeader.GetFrameType();
      uint16_t pwrFlag = btpHeader.GetPowerFlag();
      uint16_t routeLength = btpHeader.GetRouteLength();

    */

  NS_LOG_UNCOND ("Received one packet!");
 

  return true;
}


static void GenerateFlooding (Ptr<NetDevice> sender, uint16_t protocolNumber, uint32_t pktSize, uint32_t pktCount, Time pktInterval )
{
  
  NS_LOG_UNCOND ( protocolNumber );
  
  if (pktCount > 0)
    {    
      // uint16_t sourceFlag, uint16_t ttl, uint16_t frameType, uint16_t powerFlag, uint16_t routeLength
      BroadcastTreeProtocolHeader btpHeader;   //  ( 1, 1, 1, 0, 0 )
     
      btpHeader.SetSourceFlag(2); 
      btpHeader.SetFinishedFlag(0);
      btpHeader.SetTtl(1);
      btpHeader.SetFrameType(2);
      btpHeader.SetPowerFlag(1);
      btpHeader.SetRouteLength(1);

      // Cast NetDevice to WifiNetDevice
      Ptr<WifiNetDevice> senderDevice = DynamicCast<WifiNetDevice>(sender);
     
      //SET THE DEVICE WIFIPHY TX POWER
      senderDevice->SetTxPower(16);

      //OBTAIN THE TX POWER FROM THE WIFI NET DEVICE WIFIPHY
      // double GetTxPowerEnd (void) const -> Returns the maximum available transmission power level (dBm)
      double highestTxPower = senderDevice->GetPhy()->GetTxPowerEnd ();
      
      // CONVERST/CAST FROM DOUBLE TO UINT(32 o 64)
      uint32_t serializableHighestTxPower = (uint32_t) (highestTxPower + 0.5);

      NS_LOG_UNCOND ("The TX Power is " << highestTxPower  << " dBm" );    
      NS_LOG_UNCOND ("The serializable TX Power is " << serializableHighestTxPower << " dBm" ); 
      
      btpHeader.SetHighestTxPower(111);
      btpHeader.SetScndHighestTxPower(222);
      btpHeader.SetMinimumSenderSnr(333); 
 
 
      // Get sender's MAC address
      Mac48Address senderMac  = senderDevice->GetMac()->GetAddress();
      btpHeader.AddMacAddress(senderMac);

        	
      Ptr <Packet> packet = Create<Packet> (pktSize);
      packet->AddHeader(btpHeader);      

      /*
      BroadcastTreeProtocolHeader rcvBtpHeader;
      packet->RemoveHeader(rcvBtpHeader);
      */

      // senderDevice->Send
      senderDevice->Send (packet, Mac48Address::GetBroadcast (), protocolNumber);
     
      NS_LOG_UNCOND ("\nSENT ONE PACKET!");     


 
      //Simulator::Schedule (pktInterval, &GenerateFlooding, sender, dest, protocolNumber , pktSize,pktCount-1, pktInterval);
    }
}


// TRACE FUNCTIONS ------------------------------------------------------------
// Trace function for remaining energy at node.
void
RemainingEnergy (double oldValue, double remainingEnergy)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds ()
                 << "s Current remaining energy = " << remainingEnergy << "J");
}

// Trace function for total energy consumption at node.
void
TotalEnergy (double oldValue, double totalEnergy)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds ()
                 << "s Total energy consumed by node = " << totalEnergy << "J");
  
}


/****************************** MAIN FUNCTION ********************************/

int main (int argc, char *argv[])
{
 
  NS_LOG_UNCOND ("START MAIN FUNCTION");

  Packet::EnablePrinting ();

//VARIABLE INITIALIZATION-START------------------------------------------------

  std::string phyMode ("OfdmRate6Mbps");
  uint32_t packetSize = 1000;      // bytes
  uint32_t numPackets = 1;
  double interval = 1.0;           // seconds
  const uint16_t protNumber = 0x0101;    //ICMP = 1 // IPv4 encap. = 4
  bool verbose = false;

 
//COMMAND LINE INITIALIZATION-START--------------------------------------------

  CommandLine cmd;

  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("packetSize", "size of application packet sent", packetSize);
  cmd.AddValue ("numPackets", "number of packets generated", numPackets);
  cmd.AddValue ("interval", "interval (seconds) between packets", interval);
  cmd.AddValue ("verbose", "turn on all WifiNetDevice log components", verbose);

  cmd.Parse (argc, argv);


//NETWORK INFRASTRUCTURE SET-UP------------------------------------------------
 
  // Convert to time object
  Time interPacketInterval = Seconds (interval);

  // disable fragmentation for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  // turn off RTS/CTS for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue (phyMode));


  NodeContainer nodes;
  nodes.Create (2);     // create 2 nodes
  
  NodeContainer networkNodes;
  networkNodes.Add (nodes.Get (0));
  networkNodes.Add (nodes.Get (1));


  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi;

  if (verbose)
    {
      wifi.EnableLogComponents ();  // Turn on all Wifi logging
    }

  wifi.SetStandard (WIFI_PHY_STANDARD_80211a);

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();

  /*
  //MUST CONFIRM IF I NEED TO SET THIS VALUES
  wifiPhy.Set ("RxGain", DoubleValue (-10));                    // Reception gain (dB) 
  wifiPhy.Set ("TxGain", DoubleValue (offset + Prss));          // Transmission gain (dB)
  */
  
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss("ns3::LogDistancePropagationLossModel", "Exponent", DoubleValue (3.0));

  // Create the wifi physical channel
  wifiPhy.SetChannel(wifiChannel.Create ());    

  // ADD MAC & DISABLE
  WifiMacHelper wifiMac;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode",StringValue (phyMode), "ControlMode",StringValue (phyMode));
  
  // Set high-MAC to adhoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac");
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, networkNodes);


  //MOBILITY MODEL
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::RandomRectanglePositionAllocator",
                                    "X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=10]"),
                                    "Y", StringValue ("ns3::UniformRandomVariable[Min=0|Max=10]"));

  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);



  /***** BROADCAST TREE PROTOCOL *****/
  /***************************************************************************/

  // Installing broadcast tree protocol
  BroadcastTreeProtocolHelper btpHelper;

  // Install all at once using the NetDeviceContainer (signal the source node)
  btpHelper.InstallProtocolOnDevices (devices, 0);

  /*
  // Install devices one by one
  for (Iterator i = devices.Begin (); i != devices.End (); i++)
  {  
    Ptr<WifiNetDevice> wifiNetDevice = DynamicCast<WifiNetDevice>((*i));
    btpHelper.InstallProtocol (wifiNetDevice);  
  }

  */

  // Install broadcast tree protocol instances
  //Ptr<BroadcastTreeProtocol> tprotocol = CreateObject<BroadcastTreeProtocol> ();


  /*** ENERGY MODEL ***/
  /***************************************************************************/
  /* Energy source -----BEGIN----- */
  BasicEnergySourceHelper basicSourceHelper;

  // Configure energy source
  basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (100));    // Set initial energy stored in basic energy source. ( In Joules )
 
  // Install source 
  // ¿Installs this same source in all nodes or one of this for each node ?
  EnergySourceContainer sources = basicSourceHelper.Install (nodes);
  /* Energy source -----END----- */
  
  /* Device energy model */
  WifiRadioEnergyModelHelper radioEnergyHelper;

  // CONFIGURE RADIO ENERGY MODEL

  /*
   Configure the power spent during transmission
  */ 
  //radioEnergyHelper.Set ("TxCurrentA", DoubleValue (0.0374));    // ( In Amperes )
  
  /***** SET POWER CONSUMPTION OF OTHER STATES *****/
  // IdleCurrentA: The default radio Idle current in Ampere.
  // radioEnergyHelper.Set ("TxCurrentA", DoubleValue (0.0374)); // TxCurrentA: The radio Tx current in Ampere.
  // RxCurrentA: The radio Rx current in Ampere.
  // SwitchingCurrentA: The default radio Channel Switch current in Ampere.

  // Install device model
  DeviceEnergyModelContainer deviceModels = radioEnergyHelper.Install (devices, sources);
  /***************************************************************************/
  

  //PACKET RECEIVAL CALLBACK
  //devices.Get(1)->SetReceiveCallback ( MakeCallback(&received) );
  

  // Output what we are doing
  NS_LOG_UNCOND (numPackets  << " packets sent");


  // TRACING
  wifiPhy.EnablePcap ("BROACAST-TREE-PROTOCOL-SCENEARIO", devices);

  
  Ptr<NetDevice> source = devices.Get(0);
  int sourceId = devices.Get(0)->GetNode ()->GetId ();



  /** CONNECT TRACE SOURCES **/
  /***************************************************************************/
  // All sources are connected to node 1

  // Energy Source
  Ptr<BasicEnergySource> basicSourcePtr = DynamicCast<BasicEnergySource> (sources.Get (1));
  basicSourcePtr->TraceConnectWithoutContext ("RemainingEnergy", MakeCallback (&RemainingEnergy));

  // device energy model
  Ptr<DeviceEnergyModel> basicRadioModelPtr =
    basicSourcePtr->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  NS_ASSERT (basicRadioModelPtr != NULL);
  basicRadioModelPtr->TraceConnectWithoutContext ("TotalEnergyConsumption", MakeCallback (&TotalEnergy));
  /***************************************************************************/

  
  //INTIAL FLOODING
  Simulator::ScheduleWithContext (sourceId, Seconds (1.0), &GenerateFlooding, source, protNumber, packetSize, numPackets, interPacketInterval);
  

  Simulator::Stop (Seconds (3.0)); 
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}




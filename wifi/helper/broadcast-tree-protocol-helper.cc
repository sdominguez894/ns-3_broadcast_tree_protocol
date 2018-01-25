/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 */

#include "ns3/broadcast-tree-protocol-helper.h"
#include "ns3/broadcast-tree-protocol.h"
#include "ns3/object.h"
#include "ns3/log.h"
#include "ns3/wifi-net-device.h"
#include "ns3/net-device.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"



namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("BroadcastTreeProtocolHelper");

NS_OBJECT_ENSURE_REGISTERED (BroadcastTreeProtocolHelper);


TypeId
BroadcastTreeProtocolHelper::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BroadcastTreeProtocolHelper")
    .SetParent<Object> ()
    .SetGroupName ("Wifi")
    .AddConstructor<BroadcastTreeProtocolHelper> ()
  ;
  return tid;
}


BroadcastTreeProtocolHelper::BroadcastTreeProtocolHelper ()
{
  NS_LOG_UNCOND ( "Created BroadcastTreeProtocolHelper" );
}

BroadcastTreeProtocolHelper::~BroadcastTreeProtocolHelper ()
{
}

void
BroadcastTreeProtocolHelper::DoDispose ()
{
}

bool 
BroadcastTreeProtocolHelper::InstallProtocol (Ptr<NetDevice> netDevice)
{    
  // Create a BroadcastTreeProtocol instance that manages this 
  BroadcastTreeProtocol broadcastTreeProtocol;
  //BroadcastTreeProtocol broadcastTreeProtocol = CreateObject<broadcastTreeProtocol> ();
  // Shouldn't a CreateObject return a Ptr<> ?
  
  Ptr<WifiNetDevice> wifiNetDevice = DynamicCast<WifiNetDevice>((netDevice));

  //Call the install method ( -> if pointer)
  broadcastTreeProtocol.Install (wifiNetDevice);
  
  return true;
}


//bool BroadcastTreeProtocolHelper::InstallProtocolOnDevices (NetDeviceContainer devices) 
bool BroadcastTreeProtocolHelper::InstallProtocolOnDevices (NetDeviceContainer devices, uint32_t sourceNodeId) 
{

  NS_LOG_UNCOND ( "Installing broadcast tree protocol on devices");

  NetDeviceContainer::Iterator i;

  for ( i = devices.Begin (); i != devices.End (); ++i )
  {
      
      //For each device in the container

      //1. Cast to WifiNetDevice ¿Are the 2 parenthesis necessary?
      Ptr<WifiNetDevice> wifiNetDevice = DynamicCast<WifiNetDevice>((*i));
      
      //2. Create a BroadcastTreeProtocol instance that manages this 
      BroadcastTreeProtocol broadcastTreeProtocol;
      //BroadcastTreeProtocol broadcastTreeProtocol = CreateObject<broadcastTreeProtocol> ();

      //3. Call the install method ( -> if pointer)
      //bool success = broadcastTreeProtocol.Install (wifiNetDevice);
      broadcastTreeProtocol.Install (wifiNetDevice);

      // Set source node 
      if ( sourceNodeId == ( wifiNetDevice->GetNode () )->GetId () )
        broadcastTreeProtocol.SetSourceNode();

  }

  //The return should be changed
  return true;

}


} // namespace ns3

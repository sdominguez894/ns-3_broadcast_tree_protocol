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

#ifndef BROADCAST_TREE_PROTOCOL_HELPER_H
#define BROADCAST_TREE_PROTOCOL_HELPER_H

//#include "ns3/mesh-stack-installer.h"


#include "ns3/object.h"
#include "ns3/wifi-net-device.h"
#include "ns3/net-device.h"
#include "ns3/net-device-container.h"


namespace ns3 {

/**
 * \ingroup wifi
 *
 * \brief Helper class used to install the broadcast tree protocol 
 */
class BroadcastTreeProtocolHelper : public Object
{
public:

  static TypeId GetTypeId (void);

  /**
   * Construct a BroadcastTreeProtocolHelper helper class.
   */
  BroadcastTreeProtocolHelper (void);

  /**
   * Destroy a BroadcastTreeProtocolHelper helper class.
   */
  ~BroadcastTreeProtocolHelper (void);

  /**
   * Break any reference cycles in the installer helper.  Required for ns-3
   * Object support.
   */
  void DoDispose (void);

  /**
   * \brief Install the protocol on the given WifiNetDevice
   * \param wifiNetDevice The Ptr<WifiNetDevice> to use.
   */
  bool InstallProtocol (Ptr<NetDevice> netDevice);


  /**
   * \brief Install the protocol on every given device in the NetDeviceContainer
   * \param devices The NetDeviceContainer cointaining the devices.
   */
   bool InstallProtocolOnDevices (NetDeviceContainer devices, uint32_t sourceNodeId);

 
};

} // namespace ns3

#endif /* BROADCAST_TREE_PROTOCOL_HELPER_H */


/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef BROADCAST_TREE_PROTOCOL_H
#define BROADCAST_TREE_PROTOCOL_H 

#include <set>
#include <map>
#include <vector>
#include <stdint.h>
#include "ns3/wifi-module.h"
#include "ns3/wifi-net-device.h"

#include "ns3/node.h"
#include "ns3/object.h"
#include "ns3/callback.h"
#include "ns3/broadcast-tree-protocol-header.h"
#include "ns3/broadcast-tree-protocol-point.h"

namespace ns3 {

class Node;
class NetDevice;    
class BroadcastTreeProtocolHeader;


class BroadcastTreeProtocol : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  static const uint16_t PROT_NUMBER; //!< Protocol number (0x0101)

  BroadcastTreeProtocol (void);  
  ~BroadcastTreeProtocol (void);
  
  /**
   * \enum FrameType
   * \brief Different types of frames in the protocol
   */
  enum FrameType
  {
    NEIGHBOR_DISCOVERY_FRAME = 1,
    CHILD_REQUEST_FRAME = 2,
    PARENT_REJECTION_FRAME = 3,
    PARENT_REVOCATION_FRAME = 4,
    PARENT_CONFIRMATION_FRAME = 5,
    LOCAL_END_GAME_FRAME = 6,
    APPLICATION_DATA_FRAME = 7 
  };

  bool Install (Ptr<WifiNetDevice> wifiNetDevice);

  void SetSourceNode ();

  void SetNode (Ptr<Node> node);

  void SetDevice (Ptr<WifiNetDevice> wifiNetDevice);

  Ptr<WifiNetDevice> GetDevice(void);

  void Receive ( Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from, const Address &to, NetDevice::PacketType packetType);


protected:

  virtual void DoDispose (void);

  virtual void NotifyNewAggregate ();

  // SHOULD I HAVE BOTH ??? 
  Ptr<Node> m_node; 
  Ptr<WifiNetDevice> m_wifiNetDevice;

private:
 
 bool IsDuplicate (uint32_t seqNum);  

  /**
   * \brief Construct a BroadcastTreeProtocol header.
   * \
   * \param sequenceNumber unique id of the frame
   * \param sourceFlag if the message comes directly from the source node
   * \param ttl Time to Live
   * \param frameType the type of frame
   * \param finishedFlag if the game is finished for a specific node downwards
   * \param powerFlag if the header carries power information
   * \param routeLength Length in hops, from source node to this node
   * \
   * \param highestTxPower 
   * \param scndHighestTxPower
   * \param minimumSenderSnr 
   * \param minimumSenderSnr
   * \param routeData
   * \
   * \return newly created BroadcastTreeProtocol header
   */

  BroadcastTreeProtocolHeader BuildHeader (
    uint32_t sequenceNumber,
    uint16_t sourceFlag,
    uint16_t ttl,
    uint16_t frameType,
    uint16_t finishedFlag,
    uint16_t powerFlag,
    uint16_t routeLength,
    uint32_t highestTxPower,
    uint32_t scndHighestTxPower,
    uint32_t minimumSenderSnr,
    std::vector<Mac48Address> routeData );


void ProcessForParentFrame ( const Address from, BroadcastTreeProtocolHeader btpHeader );


void ProcessForChildFrame ( const Address from, BroadcastTreeProtocolHeader btpHeader );

 
void ProcessDataFrame ( const Address from, BroadcastTreeProtocolHeader btpHeader, uint16_t ttl );


void RecalculatePowerToReachAllNodes (void);


void RecalculatePowerToReachChilds (void);


std::vector<BroadcastTreeProtocolPoint> GetChildNodes ( void );

/*
  typedef std::map< Mac48Address, BroadcastTreeProtocolPoint > NeighborsMap;
  typedef std::vector<Mac48Address> ChildsList;
  
 
  NeighborsMap m_neighbors;
  ChildsList m_childs;
*/

  bool m_parentEndOfGame;                                               // Signals the stoping of messages processing by this node as a parent
  bool m_childEndOfGame;                                                // Signals the stoping of messages processing by this node as a child 
  bool m_isSourceNode;                                                  // True if the node is the source
  bool m_finishedBranch;                                                // True if the game is finished from this node downwards the tree

  uint32_t m_powerToReachChilds;                                        // Minimum required transmit power to successfully reach all child nodes
  uint32_t m_iterationsUnchanged;  
  uint32_t m_iterationsWithoutNeighbor;                                 // 
  uint32_t m_costOfCurrentConnection;                                   // Cost of transmitting a message to a specific node
  uint32_t m_costOfRequestedConnection;                                 // Cost of connection requested by a specific node *****


  //std::set<uint32_t> &m_sequenceNumbers;
  std::vector<uint32_t> m_sequenceNumbers;                              // Stores all the sequence numbers of packets received by this node

  std::map <Mac48Address, BroadcastTreeProtocolPoint> m_neighbors;    // Map containing all neighbor nodes of this node sorted by address
  
  std::vector<Mac48Address> m_childs;                                   // The addreses of all the child nodes of this node

  std::vector<Mac48Address> m_routeToSource;                            // Current route from this node to the source node


};

} // namespace ns3

#endif /* BROADCAST_TREE_PROTOCOL_H */

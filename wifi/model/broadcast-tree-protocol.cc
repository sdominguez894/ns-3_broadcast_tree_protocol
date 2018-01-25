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

#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/object-vector.h"
#include "ns3/node.h"
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/address.h"
#include "ns3/mac48-address.h"
//#include "ns3/net-device.h" 
#include "ns3/wifi-net-device.h"
//#include "ns3/boolean.h"
#include "ns3/snr-tag.h"
#include "ns3/noise-tag.h"
#include "ns3/rx-power-tag.h"
#include "ns3/wifi-module.h"
#include "ns3/broadcast-tree-protocol.h"
#include "ns3/broadcast-tree-protocol-point.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("BroadcastTreeProtocol");

const uint16_t BroadcastTreeProtocol::PROT_NUMBER = 0x0101;

NS_OBJECT_ENSURE_REGISTERED (BroadcastTreeProtocol);

TypeId
BroadcastTreeProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BroadcastTreeProtocol")
    .SetParent<Object> ()
    .SetGroupName ("Wifi")
    .AddConstructor<BroadcastTreeProtocol> ()
    /*
    .AddAttribute ("SequenceNumbersList",
                   "The set of set of packet sequence numbers received.",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&BroadcastTreeProtocol::m_sequenceNumbers),
                   MakeObjectVectorChecker<uint32_t> ()
                  )
    */
  ;
  return tid;
}


/********************* BROADCAST TREE PROTOCOL *************************/

BroadcastTreeProtocol::BroadcastTreeProtocol () : m_parentEndOfGame(false), m_childEndOfGame(false), m_isSourceNode(false) 
                                                , m_powerToReachChilds(0), m_iterationsUnchanged(0), m_iterationsWithoutNeighbor(0) 
                                                , m_costOfCurrentConnection(0), m_costOfRequestedConnection(0)
                                                , m_sequenceNumbers(), m_neighbors(), m_childs(), m_routeToSource()
{
  //m_iterationsUnchanged = 0;
  
}


BroadcastTreeProtocol::~BroadcastTreeProtocol ()
{
  m_node = 0;
  m_wifiNetDevice = 0;
}


void
BroadcastTreeProtocol::SetDevice (Ptr<WifiNetDevice> wifiNetDevice)
{
  NS_LOG_FUNCTION (this << wifiNetDevice);
  m_wifiNetDevice = wifiNetDevice;
}


Ptr<WifiNetDevice> 
BroadcastTreeProtocol::GetDevice ()
{
  return m_wifiNetDevice;
}

void BroadcastTreeProtocol::SetSourceNode ()
{
  NS_LOG_UNCOND ( "SET " << m_node->GetId () << " AS SOURCE NODE" );
  m_isSourceNode = true;
}


void BroadcastTreeProtocol::Receive ( Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from, const Address &to, NetDevice::PacketType packetType)
{
   NS_LOG_UNCOND ("PACKET RECEIVED BY BROADCAST TREE PROTOCOL from " << from);

   Ptr<Packet> packet = p->Copy ();
   BroadcastTreeProtocolHeader btpHeader;


   // If a broadcast tree protocol header exists
   if(packet->PeekHeader (btpHeader))
   {
      // btpHeader present      
      uint32_t seqNum = btpHeader.GetSequenceNumber();
      NS_LOG_UNCOND ( "SEQUENCE NUMBER " << seqNum );
  
    
      // *********  BROADCAST TREE PROTOCOL TAGS  *********** //
      SnrTag snrTag;
      NoiseTag noiseTag; 
      RxPowerTag rxPowerTag;

      if (packet->PeekPacketTag(snrTag)){
        double tmp_snr = snrTag.Get();
        NS_LOG_UNCOND ("Received Packet with SNR = " << tmp_snr);
      }
      if (packet->PeekPacketTag(noiseTag)){
        double tmp_noise = noiseTag.Get();
        NS_LOG_UNCOND ("Received Packet with noise = " << tmp_noise);
      }
      if (packet->PeekPacketTag(rxPowerTag)){
        double tmp_rxPower = rxPowerTag.Get();
        NS_LOG_UNCOND ("Received Packet with signal power = " << tmp_rxPower);
      }
      // *********  BROADCAST TREE PROTOCOL TAGS  *********** //

      
      // Check for duplicate frame
      if ( IsDuplicate (seqNum) ) {}

      // I'm calling it a second time to check the behaviour
      // when there is actually a duplicate sequence number
      if ( IsDuplicate (seqNum) )
        return;
      

      // Check for ttl
      uint16_t ttl = btpHeader.GetTtl();
      if ( ttl == 0 )
        return;
      
      ttl = ttl-1;
       

      // Process frame depending on type
      uint32_t frameType = btpHeader.GetFrameType();

      switch(frameType)
      {
        case BroadcastTreeProtocol::PARENT_REJECTION_FRAME :
        case BroadcastTreeProtocol::PARENT_REVOCATION_FRAME :
        case BroadcastTreeProtocol::PARENT_CONFIRMATION_FRAME :
          ProcessForParentFrame ( from, btpHeader );          
          break; //optional 

        case BroadcastTreeProtocol::NEIGHBOR_DISCOVERY_FRAME :
        case BroadcastTreeProtocol::CHILD_REQUEST_FRAME :
        case BroadcastTreeProtocol::LOCAL_END_GAME_FRAME :
          ProcessForChildFrame ( from, btpHeader );
          break; //optional
  
        case BroadcastTreeProtocol::APPLICATION_DATA_FRAME :
          ProcessDataFrame ( from, btpHeader, ttl );
          break; //optional

        // you can have any number of case statements.
        default : //Optional
          NS_LOG_UNCOND ("FRAME OF UNKNOWN TYPE");

     // END OF SWITCH FRAME PROCESSING
     }


   // END OF EXSISTING HEADER
   }
   

// END OF RECEIVE FUNCTION
}


bool 
BroadcastTreeProtocol::IsDuplicate (uint32_t seqNum)
{
  NS_LOG_FUNCTION (this);

  //m_sequenceNumbers.clear();

  NS_LOG_UNCOND ( "ENTERING ISDUPLICATE FUNCTION" );  

  
  // Check if vector is empty
  if ( m_sequenceNumbers.empty() )
  {    
    NS_LOG_UNCOND ( "EMPTY VECTOR" );
    m_sequenceNumbers.push_back(seqNum);
    NS_LOG_UNCOND ( "Print vector value: " << m_sequenceNumbers.at(0) );
    
    return false;
  }

  
  if ( std::find(m_sequenceNumbers.begin(), m_sequenceNumbers.end(), seqNum) != m_sequenceNumbers.end() )
  {
    NS_LOG_UNCOND ( "IS DUPLICATE" );
    NS_LOG_UNCOND ( "Print vector value: " << m_sequenceNumbers.at(0) );
    return true;
  }
  else
  {
    NS_LOG_UNCOND ( "IS NOT DUPLICATE" );
    m_sequenceNumbers.push_back(seqNum);
    NS_LOG_UNCOND ( "Print vector value: " << m_sequenceNumbers.at(0) );
    return false;
  }
}


/*
// When m_sequenceNumbers is of type std::set
bool 
BroadcastTreeProtocol::IsDuplicate (uint32_t seqNum)
{
  NS_LOG_FUNCTION (this);

  m_sequenceNumbers.clear();

  NS_LOG_UNCOND ( "ENTERING ISDUPLICATE FUNCTION" );  

  
  // Check if set is empty
  if ( m_sequenceNumbers.empty() )
  {
    // For SET
    //NS_LOG_UNCOND ( "EMPTY SET" );
    
    // For VECTOR
    NS_LOG_UNCOND ( "EMPTY VECTOR" );
    m_sequenceNumbers.push_back(seqNum);
    return false;
  }


  if( m_sequenceNumbers.insert(seqNum).second )
  {
    NS_LOG_UNCOND ( "Number " << seqNum << " inserted sucessfuly" );
    return false;
  }
  else
  {
    NS_LOG_UNCOND ( "Number " << seqNum << " was already present in set" );
    return true;
  }
}
*/


void BroadcastTreeProtocol::ProcessForParentFrame ( const Address from, BroadcastTreeProtocolHeader btpHeader )
{
  NS_LOG_UNCOND ("FRAME OF TYPES 3, 4, 5");

  if( m_parentEndOfGame )
    return;

  NS_LOG_UNCOND ("It's not parent end of game because its value its:  " << m_parentEndOfGame );
 
}


void BroadcastTreeProtocol::ProcessForChildFrame ( const Address from, BroadcastTreeProtocolHeader btpHeader )
{
  NS_LOG_UNCOND ("FRAME OF TYPES 1, 2, 6");

  if( m_childEndOfGame  || m_isSourceNode )
    return;

  NS_LOG_UNCOND ("It's not child end of game because its value its:  " << m_childEndOfGame );

}


void BroadcastTreeProtocol::ProcessDataFrame ( const Address from, BroadcastTreeProtocolHeader btpHeader, uint16_t ttl )
{
  NS_LOG_UNCOND ("FRAME OF TYPE 7");
}


std::vector<BroadcastTreeProtocolPoint> GetChildNodes ( void )
{
  //returns a vector containing the BroadcastProtocolPoint objects of all the child nodes of this node

  /***** MUST BE MODIFIED  *****/
  return std::vector<BroadcastTreeProtocolPoint>();
}


void
BroadcastTreeProtocol::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);
  m_node = node;
}


BroadcastTreeProtocolHeader BuildHeader (
  bool newSequenceNumber,
  uint16_t sourceFlag,
  uint16_t ttl,
  uint16_t frameType,
  uint16_t finishedFlag,
  uint16_t powerFlag,
  uint16_t routeLength,
  uint32_t highestTxPower,
  uint32_t scndHighestTxPower,
  uint32_t minimumSenderSnr,
  std::vector<Mac48Address> routeData )
{

  BroadcastTreeProtocolHeader btpHeader; 

  if (newSequenceNumber)
    btpHeader.NewSequenceNumber();

  btpHeader.SetSourceFlag(sourceFlag); 
  btpHeader.SetTtl(ttl);
  btpHeader.SetFrameType(frameType);
  btpHeader.SetFinishedFlag(finishedFlag);
  btpHeader.SetPowerFlag(powerFlag);
  btpHeader.SetRouteLength(routeLength);
  btpHeader.SetHighestTxPower(highestTxPower);
  btpHeader.SetScndHighestTxPower(scndHighestTxPower);
  btpHeader.SetMinimumSenderSnr(minimumSenderSnr); 
  
  btpHeader.SetRouteData(routeData);

  return btpHeader;
}


bool 
BroadcastTreeProtocol::Install (Ptr<WifiNetDevice> wifiNetDevice)
{
  NS_LOG_UNCOND ("Installing Broadcast Tree protocol on " << wifiNetDevice->GetNode ());
 
  m_wifiNetDevice = wifiNetDevice;
  m_node = wifiNetDevice->GetNode ();

  m_node->RegisterProtocolHandler (MakeCallback (&BroadcastTreeProtocol::Receive, this), BroadcastTreeProtocol::PROT_NUMBER, m_wifiNetDevice);

  //INITIALIZING VARIABLES
  /*
  m_sequenceNumbers.clear();
                             
  m_neighbors = std::map<Mac48Address, BroadcastTreeProtocolPoint>(); 
  m_neighbors.clear();
      
  m_childs = std::vector<Mac48Address>();
  m_childs.clear();
                              
  m_routeToSource = std::vector<Mac48Address>();  
  m_routeToSource.clear();
  */

  return true;
}


void BroadcastTreeProtocol::RecalculatePowerToReachAllNodes (void)
{
  
}


void BroadcastTreeProtocol::RecalculatePowerToReachChilds (void)
{
  
}


void 
BroadcastTreeProtocol::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  
  // TO DO (My own DoDispose code)

  Object::DoDispose ();
}

void
BroadcastTreeProtocol::NotifyNewAggregate ()
{
  NS_LOG_FUNCTION (this);
  if (m_node == 0)
    {
      Ptr<Node>node = this->GetObject<Node>();
      // verify that it's a valid node and that
      // the node has not been set before
      if (node != 0)
        {
          this->SetNode (node);
        }
    }
  Object::NotifyNewAggregate ();
}



} // namespace ns3





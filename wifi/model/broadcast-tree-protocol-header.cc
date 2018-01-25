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

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/network-module.h"
#include "broadcast-tree-protocol-header.h"
//#include "./broadcast-tree-protocol-header.h"

#include <string>
#include <vector>
#include <algorithm>



namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("BroadcastTreeProtocolHeader");

NS_OBJECT_ENSURE_REGISTERED (BroadcastTreeProtocolHeader);

enum
{
  NEIGHBOR_DISCOVERY_FRAME = 0,
  CHILD_REQUEST_FRAME = 1,
  PARENT_REJECTION_FRAME = 2,
  PARENT_REVOCATION_FRAME = 3,
  PARENT_CONFIRMATION_FRAME = 4,
  CHILD_END_FRAME = 5 

};


// static counter inizialization
uint32_t BroadcastTreeProtocolHeader::counter = 1;

/*******************************************************************************/

BroadcastTreeProtocolHeader::BroadcastTreeProtocolHeader ()
{
  m_sequenceNumber = counter;
}


BroadcastTreeProtocolHeader::~BroadcastTreeProtocolHeader ()
{
}


/*
BroadcastTreeProtocolHeader ( uint16_t sourceFlag, uint16_t ttl, uint16_t frameType, uint16_t powerFlag, uint16_t routeLength)
{
  NS_LOG_FUNCTION (this);

  m_sequenceNumber = this.NewSequenceNumber ();
  m_sourceFlag = sourceFlag ; 
  m_ttl = ttl;
  m_frameType = frameType;
  m_powerFlag = powerFlag; 
  m_routeLength = routeLength;
}
*/


void BroadcastTreeProtocolHeader::NewSequenceNumber (void)
{
  NS_LOG_FUNCTION (this);
  m_sequenceNumber = ++counter;
}

void
BroadcastTreeProtocolHeader::SetSequenceNumber (uint32_t sequenceNumber)
{
  m_sequenceNumber = sequenceNumber;
}
 
uint32_t
BroadcastTreeProtocolHeader::GetSequenceNumber (void)
{
  return m_sequenceNumber;
}

void
BroadcastTreeProtocolHeader::SetSourceFlag (uint16_t sourceFlag)
{
  m_sourceFlag = sourceFlag;
}

uint16_t
BroadcastTreeProtocolHeader::GetSourceFlag (void)
{
  return m_sourceFlag;  
}

void BroadcastTreeProtocolHeader::SetFinishedFlag (uint16_t finishedFlag)
{
  m_finishedFlag = finishedFlag;
}

uint16_t BroadcastTreeProtocolHeader::GetFinishedFlag (void)
{
  return m_finishedFlag;  
}
 

void
BroadcastTreeProtocolHeader::SetTtl (uint16_t ttl)
{
  m_ttl = ttl;
}

uint16_t 
BroadcastTreeProtocolHeader::GetTtl (void)
{
  return m_ttl;
}

void 
BroadcastTreeProtocolHeader::SetFrameType (uint16_t frameType)
{
  m_frameType = frameType;
}

uint16_t
BroadcastTreeProtocolHeader::GetFrameType (void)
{
  return m_frameType;
} 

void
BroadcastTreeProtocolHeader::SetPowerFlag (uint16_t powerFlag)
{
  m_powerFlag = powerFlag;
}

uint16_t
BroadcastTreeProtocolHeader::GetPowerFlag (void)
{
  return m_powerFlag;
}

void
BroadcastTreeProtocolHeader::SetRouteLength (uint16_t routeLength)
{
  m_routeLength = routeLength;
}

uint16_t
BroadcastTreeProtocolHeader::GetRouteLength (void)
{
  return m_routeLength;
}

void 
BroadcastTreeProtocolHeader::SetSerializedSize (uint32_t serializedSize)
{
  m_serializedSize = serializedSize;
}   

// POWER RELATED FUNCTIONS
void
BroadcastTreeProtocolHeader::SetHighestTxPower (uint32_t highestTxPower)
{
  m_highestTxPower = highestTxPower;
}

uint32_t
BroadcastTreeProtocolHeader::GetHighestTxPower (void)
{
  return m_highestTxPower;
}

void
BroadcastTreeProtocolHeader::SetScndHighestTxPower (uint32_t scndHighestTxPower)
{
  m_scndHighestTxPower = scndHighestTxPower;
}

uint32_t
BroadcastTreeProtocolHeader::GetScndHighestTxPower (void)
{
  return m_scndHighestTxPower ;
}

void
BroadcastTreeProtocolHeader::SetMinimumSenderSnr (uint32_t minimumSenderSnr)
{
  m_minimumSenderSnr = minimumSenderSnr;
}

uint32_t
BroadcastTreeProtocolHeader::GetMinimumSenderSnr (void)
{
  return m_minimumSenderSnr;
}

// ROUTE RELATED FUNCTIONS

/*
Overrides the current routeData for the one passed in the argument
*/
void
BroadcastTreeProtocolHeader::SetRouteData (std::vector<Mac48Address> routeData)
{
  m_routeData = routeData;
}

std::vector<Mac48Address>
BroadcastTreeProtocolHeader::GetRouteData (void)
{
  return m_routeData;
}

void
BroadcastTreeProtocolHeader::AddMacAddress (Mac48Address macAddress)
{
  m_routeData.push_back(macAddress);
}

Mac48Address 
BroadcastTreeProtocolHeader::RemovePenultimateMac48Address (void)
{
  //Check if vector is not empty and has more than one mac addresses
  //if(!(m_routeData.empty()) && (m_routeData.size()>1))
  if( m_routeData.size()>1 )
  {
    // extract the penultimate mac address from the vector
    return m_routeData.at( m_routeData.size()-2 );
  }
  else 
  {
    return Mac48Address::GetBroadcast ();
  }
  
}


/********************** INHERITED HEADER FUNCTIONS ***************************/

uint32_t 
BroadcastTreeProtocolHeader::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  
  uint32_t size = 16;
  uint32_t bytesPowerData = 3*4;   

  if(m_powerFlag==1)
  {
    size += bytesPowerData;
  }

  uint32_t bytesRouteData = m_routeLength*6;

  size += bytesRouteData;

  NS_LOG_UNCOND ( "Total size: " << size << "\n" <<
                  "Fixed size: " << 16 << "\n" <<              
                  "BytesPowerData: " << bytesPowerData << "\n" << 
                  "BytesRouteData: " << bytesRouteData << "\n" 
                ); 

  return size;  //return m_serializedSize;
}


TypeId 
BroadcastTreeProtocolHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BroadcastTreeProtocolHeader")
    .SetParent<Header> ()
    .SetGroupName("Network")
    .AddConstructor<BroadcastTreeProtocolHeader> ()
  ;
  return tid;
}

TypeId 
BroadcastTreeProtocolHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
void 
BroadcastTreeProtocolHeader::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
  os << "BroadcastTreeProtocolHeader";

}

void
BroadcastTreeProtocolHeader::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION (this << &start);
  Buffer::Iterator i = start;

  /* 
  Write an uint16_t in the buffer and advance the iterator position by two bytes. 
  The data is written in network order and the input data is expected to be in host order. 
  */


  NS_LOG_UNCOND ("Sent packet with:\n" << 
                 "SequenceNumber: " << m_sequenceNumber << "\n" <<
                 "SourceFlag: " << m_sourceFlag << "\n" <<
                 "FinishedFlag: " << m_finishedFlag << "\n" <<          
                 "Ttl: " << m_ttl << "\n" <<
                 "FrameType: " << m_frameType << "\n" <<
                 "PowerFlag: " << m_powerFlag << "\n" <<
                 "RouteLength: " << m_routeLength << "\n"
                 "HighestTxPower: " << m_highestTxPower << "\n"
                 "ScndHighestTxPower: " << m_scndHighestTxPower << "\n"
                 "MinimumSenderSnr: " << m_minimumSenderSnr << "\n"
                );

  // Print route data
  for (unsigned j=0; j<m_routeLength; j++)    
  {                                                 
    NS_LOG_UNCOND ("Addres " << j << ": " << m_routeData.at(j)); 
  }

  /* 
  std::for_each(m_routeData.begin(), m_routeData.end(), [](Mac48Address &address)
  {                                                                    
    NS_LOG_UNCOND (address); 
  });  
  */

  // ADD DATA TO THE ACTUAL PACKET BUFFER
  
  // Add fixed header data
  i.WriteHtonU32 (m_sequenceNumber);   // 4
  i.WriteHtonU16 (m_sourceFlag);       // 2
  i.WriteHtonU16 (m_finishedFlag);     // 2    
  i.WriteHtonU16 (m_ttl);              // 2              
  i.WriteHtonU16 (m_frameType);        // 2 
  i.WriteHtonU16 (m_powerFlag);        // 2 
  i.WriteHtonU16 (m_routeLength);      // 2

  
  // Add powerData to the header when the power flag is set to 1
  if(m_powerFlag == 1)
  {
    i.WriteHtonU32 (m_highestTxPower); 
    i.WriteHtonU32 (m_scndHighestTxPower); 
    i.WriteHtonU32 (m_minimumSenderSnr);
  } 
  
  // Add routeData to the header when the route contains any nodes
  if(m_routeLength > 0)
  {
    for (unsigned j=0; j<m_routeLength; j++)    
    {                                                 
      WriteTo (i, m_routeData.at(j));  

    }      
  }   
  
}


uint32_t
BroadcastTreeProtocolHeader::Deserialize (Buffer::Iterator start)
{
  NS_LOG_UNCOND ("Deserializing");
  NS_LOG_FUNCTION (this << &start);
  Buffer::Iterator i = start;
  
  // Read fixed header data
  m_sequenceNumber = i.ReadNtohU32 ();
  m_sourceFlag = i.ReadNtohU16 ();
  m_finishedFlag = i.ReadNtohU16 ();  
  m_ttl = i.ReadNtohU16 ();
  m_frameType = i.ReadNtohU16 ();
  m_powerFlag = i.ReadNtohU16 ();
  m_routeLength = i.ReadNtohU16 ();


  if(m_powerFlag == 1)
  {
    m_highestTxPower = i.ReadNtohU32 (); 
    m_scndHighestTxPower = i.ReadNtohU32 (); 
    m_minimumSenderSnr = i.ReadNtohU32 ();
  }

  
  if(m_routeLength>0)
  {     
    for (unsigned j=0; j<m_routeLength; j++)    
    {    
      Mac48Address address;                                     
      ReadFrom (i, address);
      AddMacAddress(address);
    }
  }   
 
  NS_LOG_UNCOND ("\nReceived packet with:\n" << 
                 "SequenceNumber: " << m_sequenceNumber << "\n" <<
                 "SourceFlag: " << m_sourceFlag << "\n" <<
                 "FinishedFlag: " << m_finishedFlag << "\n" <<          
                 "Ttl: " << m_ttl << "\n" <<
                 "FrameType: " << m_frameType << "\n" <<
                 "PowerFlag: " << m_powerFlag << "\n" <<
                 "RouteLength: " << m_routeLength << "\n"
                 "HighestTxPower: " << m_highestTxPower << "\n"
                 "ScndHighestTxPower: " << m_scndHighestTxPower << "\n"
                 "MinimumSenderSnr: " << m_minimumSenderSnr << "\n"
                );  


  // Print route data
  for (unsigned j=0; j<m_routeLength; j++)    
  {                                                 
    NS_LOG_UNCOND ("Addres " << j << ": " << m_routeData.at(j)); 
  }

  return GetSerializedSize ();
}


} // namespace ns3

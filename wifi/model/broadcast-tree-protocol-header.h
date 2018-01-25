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

#ifndef BROADCAST_TREE_PROTOCOL_HEADER_H
#define BROADCAST_TREE_PROTOCOL_HEADER_H 

#include <iostream>
#include <vector>
#include <stdint.h>
#include <string>

#include "ns3/header.h"
#include "ns3/network-module.h"

namespace ns3 {

/** 
 * The length in octects of the BTP header
 */
//static const uint16_t BROADCAST_TREE_PROTOCOL_HEADER_LENGTH = sequenceNumberLength + sourceFlagLength + ttlLength + frameTypeLength + optionsLength + highestTxPowerLength + scndHighestTxPowerLength + minimumSenderSnrLength + (6*m_RouteLength);

/**
 * \ingroup network
 *
 * \brief Header for the Broadcast tree protocol
 *
 * For a list of EtherTypes, see http://www.iana.org/assignments/ieee-802-numbers/ieee-802-numbers.xhtml
 */
class BroadcastTreeProtocolHeader : public Header 
{
public:
  BroadcastTreeProtocolHeader ();
  ~BroadcastTreeProtocolHeader ();

  //BroadcastTreeProtocolHeader ( uint16_t sourceFlag, uint16_t ttl, uint16_t frameType, uint16_t powerFlag, uint16_t routeLength);  

  // BroadcastTreeProtocolHeader ( uint32_t sequenceNumber, uint16_t sourceFlag, uint16_t ttl, uint16_t frameType, uint16_t powerFlag);   

  static TypeId GetTypeId (void);
  static uint32_t counter;
  //static bool isFirstFrame;
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const; 
  virtual uint32_t Deserialize (Buffer::Iterator start);


  /***** DEFITION OF ALL PUBLIC METHODS SPECIFIC TO THIS HEADER *****/

  void NewSequenceNumber (void);

  void SetSequenceNumber (uint32_t sequenceNumber);

  uint32_t GetSequenceNumber (void);
   
  void SetSourceFlag (uint16_t sourceFlag);
  
  uint16_t GetSourceFlag (void);

  uint16_t GetFinishedFlag (void); 
 
  void SetFinishedFlag (uint16_t finishedFlag);
   
  void SetTtl (uint16_t ttl);
   
  uint16_t GetTtl (void);  
   
  void SetFrameType (uint16_t frameType);
   
  uint16_t GetFrameType (void);

  void SetPowerFlag (uint16_t powerFlag);
   
  uint16_t GetPowerFlag (void);

  void SetRouteLength (uint16_t frameType);
   
  uint16_t GetRouteLength (void);

  void SetSerializedSize (uint32_t serializedSize);



  // For the following fields, could they be uint32_t instead of double??

/*
  void SetHighestTxPower (uint64_t highestTxPower);

  uint64_t GetHighestTxPower (void);

  void SetScndHighestTxPower (uint64_t scndHighestTxPower);

  uint64_t GetScndHighestTxPower (void);  

  void SetMinimumSenderSnr (uint64_t minimumSenderSnr);

  uint64_t GetMinimumSenderSnr (void);  
*/

  void SetHighestTxPower (uint32_t highestTxPower);

  uint32_t GetHighestTxPower (void);

  void SetScndHighestTxPower (uint32_t scndHighestTxPower);

  uint32_t GetScndHighestTxPower (void);  

  void SetMinimumSenderSnr (uint32_t minimumSenderSnr);

  uint32_t GetMinimumSenderSnr (void);  
 
 
  void AddMacAddress (Mac48Address macAddress);
  
  Mac48Address RemovePenultimateMac48Address (void);

  std::vector<Mac48Address> GetRouteData (void);

  void  SetRouteData (std::vector<Mac48Address> routeData);



private:
  
  /*
  fixHeaderLength = sequenceNumberLength + sourceFlagLength + ttlLength + frameTypeLength + powerFlagLength + m_routeLength;

//highestTxPowerLength + scndHighestTxPowerLength + minimumSenderSnrLength + m_RouteData;

  */
 
  /************************ VARIABLE HEADER PART *************************/
  
  //uint16_t m_headerLength = 4 + 1 + 2 + 2 + 1 + 2;

  uint32_t m_sequenceNumber;           // the unique id of this frame (4 bytes)  
  uint16_t m_sourceFlag;               // indicates if the message comes directly from the source node 1 or not 0 
  uint16_t m_finishedFlag;             // indicates if the game is finished or unfinished from a specific node
                                       // of the broadcast tree downwards 1 if it is finished and 0 if it is not
  uint16_t m_ttl;                      // the time to live of this frame (2 bytes)
  uint16_t m_frameType;                // the type of frame/message (2 bytes)
  uint16_t m_powerFlag;                // indicates whether the message is carrying power information (2 bytes)
  uint16_t m_routeLength;              // the number of nodes that conforms the route from the source node on
  uint32_t m_serializedSize;

  
  /************************ VARIABLE HEADER PART *************************/
/*  
  uint64_t m_highestTxPower;             // the highest transmit power required to reach all the chid nodes of a parent node (8 bytes) 
  uint64_t m_scndHighestTxPower;         // the second highest transmit power required to reach all the chid nodes of a parent node (8 bytes) 
  uint64_t m_minimumSenderSnr;           // the minimum SNR required by the sender of the message
*/

  uint32_t m_highestTxPower;             // the highest transmit power required to reach all the chid nodes of a parent node (8 bytes) 
  uint32_t m_scndHighestTxPower;         // the second highest transmit power required to reach all the chid nodes of a parent node (8 bytes) 
  uint32_t m_minimumSenderSnr;  


  std::vector<Mac48Address> m_routeData;      // Contains the actual mac addresses of each node the packet has passed through
 
};


} // namespace ns3

// STATIC COUNTER INITIALIZATION
//uint32_t ns3::BroadcastTreeProtocolHeader::counter = 0;

#endif /* BROADCAST_TREE_PROTOCOL_HEADER_H */



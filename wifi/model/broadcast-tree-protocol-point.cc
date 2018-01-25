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
#include <map>
#include <vector>
#include <algorithm>
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/object.h"
//#include "ns3/boolean.h"
#include "ns3/mac48-address.h"
#include "ns3/broadcast-tree-protocol.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("BroadcastTreeProtocolPoint");

NS_OBJECT_ENSURE_REGISTERED (BroadcastTreeProtocolPoint);

TypeId
BroadcastTreeProtocolPoint::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BroadcastTreeProtocolPoint")
    .SetParent<Object> ()
    .SetGroupName ("Wifi")
    .AddConstructor<BroadcastTreeProtocolPoint> ()
  ;
  return tid;
}


/********************* BROADCAST TREE PROTOCOL START ************************/

BroadcastTreeProtocolPoint::BroadcastTreeProtocolPoint ()
{
  /*
  std::vector<Mac48Address> m_routeToSource = routeToSource;      // 
  Mac48Address m_address = address;                               // 
  uint32_t m_powerRequiredToSend = powerRequiredToSend;           // 
  m_isFinished = false;                  // 
  bool m_isSourceNode = isSourceNode;
  */
}


BroadcastTreeProtocolPoint::~BroadcastTreeProtocolPoint ()
{
}

bool BroadcastTreeProtocolPoint::CheckRouteToSource ( Mac48Address addressToCheck )
{
  
  /* 
  bool check;

  if ( m_routeToSource.empty() )
    // If m_routeToSource is empty it can not be there -> TRUE
    return true 


  for (std::vector<int>::iterator routeFromAddress = m_routeToSource.begin() ; routeFromAddress != m_routeToSource.end(); ++routeFromAddress)
  {
    if (*routeFromAddress != addressToCheck )
    {
      // If addressToCheck IS NOT found in the m_routeToSource vector -> TRUE
      check = true;
    }    
    else  
    {
      // If addressToCheck IS found in the m_routeToSource vector -> FALSE & stop searching
      check = false;
      break;
    }

  }
  
  return check;
  */

  return true;
}

uint32_t BroadcastTreeProtocolPoint::GetPowerRequiredToSend (void)
{
  return m_powerRequiredToSend;
}

void BroadcastTreeProtocolPoint::SetPowerRequiredToSend ( uint32_t powerRequiredToSend )
{
  m_powerRequiredToSend = powerRequiredToSend;
}

uint32_t BroadcastTreeProtocolPoint::GetCostToReach (void)
{
  return m_costToReach;
}

void BroadcastTreeProtocolPoint::SetCostToReach ( uint32_t costToReach )
{
  m_costToReach = costToReach;
}

bool BroadcastTreeProtocolPoint::GetIsFinished (void)
{
  return m_isFinished;
}

void BroadcastTreeProtocolPoint::SetIsFinished ( bool isFinished )
{
  m_isFinished = isFinished;
}

bool BroadcastTreeProtocolPoint::GetIsSourceNode (void)
{
  return m_isSourceNode;
}

void BroadcastTreeProtocolPoint::SetIsSourceNode ( bool isSourceNode )
{
  m_isSourceNode = isSourceNode;
}

std::vector<Mac48Address> BroadcastTreeProtocolPoint::GetRouteToSource (void)
{
  return m_routeToSource;
}

void BroadcastTreeProtocolPoint::SetRouteToSource ( std::vector<Mac48Address> routeToSource )
{
  m_routeToSource = routeToSource;
}

// NS-3 OBJECT INHERITED FUNCTIONS
void 
BroadcastTreeProtocolPoint::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  // TO DO (My own DoDispose code)
  Object::DoDispose ();
}

void
BroadcastTreeProtocolPoint::NotifyNewAggregate ()
{
  NS_LOG_FUNCTION (this);

  // TO DO (My own NotifyNewAggregate code)
  Object::NotifyNewAggregate ();
}


} // namespace ns3

/*******************************************************************************/



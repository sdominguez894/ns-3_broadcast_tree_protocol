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

#ifndef BROADCAST_TREE_PROTOCOL_POINT_H
#define BROADCAST_TREE_PROTOCOL_POINT_H 

#include <set>
#include <vector>
#include <stdint.h>
#include <algorithm>

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/object.h"
#include "ns3/mac48-address.h"


namespace ns3 {



class BroadcastTreeProtocolPoint : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);


  BroadcastTreeProtocolPoint ();  
  ~BroadcastTreeProtocolPoint ();


bool CheckRouteToSource (Mac48Address addressToCheck);

uint32_t GetPowerRequiredToSend (void);

void SetPowerRequiredToSend ( uint32_t powerRequiredToSend );

uint32_t GetCostToReach (void);

void SetCostToReach ( uint32_t costToReach );

bool GetIsFinished (void);

void SetIsFinished ( bool isFinished );

bool GetIsSourceNode (void);

void SetIsSourceNode ( bool isSourceNode );

std::vector<Mac48Address> GetRouteToSource (void);

void SetRouteToSource ( std::vector<Mac48Address> routeToSource );


protected:

  virtual void DoDispose (void);

  virtual void NotifyNewAggregate ();


private:
 
  std::vector<Mac48Address> m_routeToSource;

  Mac48Address m_address;
  uint32_t m_powerRequiredToSend;
  uint32_t m_costToReach;
  bool m_isFinished;
  bool m_isSourceNode;

};

} // namespace ns3


#endif /* BROADCAST_TREE_PROTOCOL_POINT_H */

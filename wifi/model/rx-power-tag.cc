/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
 * Copyright (c) 2013 University of Surrey
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Mirko Banchi <mk.banchi@gmail.com>
 *          Konstantinos Katsaros <dinos.katsaros@gmail.com>
 */

#include "rx-power-tag.h"
#include "ns3/tag.h"
#include "ns3/double.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RxPowerTag);

TypeId
RxPowerTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RxPowerTag")
    .SetParent<Tag> ()
    .SetGroupName ("Wifi")
    .AddConstructor<RxPowerTag> ()
    .AddAttribute ("RxPower", "The received signal power of the last packet received",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&RxPowerTag::Get),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

TypeId
RxPowerTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

RxPowerTag::RxPowerTag () : m_rxPower (0)
{
}

RxPowerTag::RxPowerTag (double rxPower)
  : m_rxPower (rxPower)
{
}

uint32_t
RxPowerTag::GetSerializedSize (void) const
{
  return sizeof (double);
}

void
RxPowerTag::Serialize (TagBuffer i) const
{
  i.WriteDouble (m_rxPower);
}

void
RxPowerTag::Deserialize (TagBuffer i)
{
  m_rxPower = i.ReadDouble ();
}

void
RxPowerTag::Print (std::ostream &os) const
{
  os << "RxPower= " << m_rxPower;
}

void
RxPowerTag::Set (double rxPower)
{
  m_rxPower = rxPower;
}

double
RxPowerTag::Get (void) const
{
  return m_rxPower;
}

}

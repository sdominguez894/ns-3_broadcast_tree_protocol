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

#include "noise-tag.h"
#include "ns3/tag.h"
#include "ns3/double.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (NoiseTag);

TypeId
NoiseTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NoiseTag")
    .SetParent<Tag> ()
    .SetGroupName ("Wifi")
    .AddConstructor<NoiseTag> ()
    .AddAttribute ("Noise", "The noise sensed when the last packet was received",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&NoiseTag::Get),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

TypeId
NoiseTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

NoiseTag::NoiseTag ()
  : m_noise (0)
{
}

NoiseTag::NoiseTag (double noise)
  : m_noise (noise)
{
}

uint32_t
NoiseTag::GetSerializedSize (void) const
{
  return sizeof (double);
}

void
NoiseTag::Serialize (TagBuffer i) const
{
  i.WriteDouble (m_noise);
}

void
NoiseTag::Deserialize (TagBuffer i)
{
  m_noise = i.ReadDouble ();
}

void
NoiseTag::Print (std::ostream &os) const
{
  os << "Noise= " << m_noise;
}

void
NoiseTag::Set (double noise)
{
  m_noise = noise;
}

double
NoiseTag::Get (void) const
{
  return m_noise;
}

}

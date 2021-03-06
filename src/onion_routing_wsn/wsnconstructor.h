/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

/*
* Copyright (c) 2020 DLTLT 
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
* Corresponding author: Niki Hrovatin <niki.hrovatin@famnit.upr.si>
*/

#include <iostream>
#include <string>
#include <time.h>
#include <cmath>

#include "ns3/enums.h"
#include "ns3/outputmanager.h"
#include "ns3/onion-routing.h"
#include "ns3/sensornode.h"
#include "ns3/sensornode-helper.h"
#include "ns3/sink.h"
#include "ns3/sink-helper.h"
#include "ns3/outputmanager.h"
#include "ns3/onionvalidator.h"

#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/core-module.h"
#include "ns3/wifi-module.h"
#include "ns3/aodv-module.h"
#include "ns3/olsr-module.h"
#include "ns3/dsr-module.h"
#include "ns3/dsdv-module.h"
#include "ns3/stats-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/config-store-module.h"

#ifndef WSNCONSTRUCTOR_H
#define WSNCONSTRUCTOR_H

using namespace ns3;

/**
 *  \defgroup onion_routing_wsn
 * 
 *
 *  Be sure to read repository README.md before going down to the API.
 */

/**
 * \ingroup onion_routing_wsn
 * \class WsnConstructor
 * \brief The class that constructs the WSN, setup applications on nodes and starts the simulation.
 *
 */

class WsnConstructor : public Object
{
public:
  /**
 *  Register this type.
 *  \return The object TypeId.
 */

  static TypeId GetTypeId (void);

  /**
  *
  * \brief Default constructor
  *
  */
  WsnConstructor ();

  /**
  *
  * \brief  Generate a description of attributes given from the config file and from CLI
  *         set the simulation seed and verbosity.
  *
  */
  void Configure ();

  /**
  *
  * \brief  Construct the WSN, install applications on nodes and start the simulation.
  *
  */
  void Run ();

private:
  //simulation attributes
  uint32_t
      m_simulationSeed; //!< seed to use for the random generation of numbers during the simulation
  uint16_t m_numNodes; //!< number of sensor nodes in the WSN
  enum Routing m_routing; //!< routing algorithm for wireless multihop networks
  enum Topology m_topology; //!< network topology
  enum IEEE_80211n m_mac; //!< Carrier frequency of the IEEE 802.11n
  enum Verbosity m_verbosity; //!< verbosity of the simulation
  enum CommunicationStatistics m_stats; //!< setting of recording communication statistics
  uint16_t m_mss; //!< maximum segment size
  uint16_t m_radius; //!< Parameter for the setup of the random disc topology
  uint16_t m_cellSide; //!< Parameter for the setup of the grid topology

  //Classes to manage the simulation
  Ptr<OutputManager> m_outputManager; //!< Manages the output of the simulation
  Ptr<OnionValidator>
      m_onionValidator; //!< Checks if the onion messagess transiting in the network are valid
  std::string
      m_simulationDescription; //!< String holding a description of parameters used in the simulation
  std::string m_simulationName; //!< name of the simulation

  //onion setup
  uint16_t m_numOnionPaths = 0; //!< Number of different onion paths
  uint16_t *m_onionPathsLengths; //!< Array containing one onion path length in each cell
  uint16_t m_onionRepeate; //!< Number of times to generate the onion for each value of path length
  std::string
      m_pathsLengths; //!< String of values delimited by the symbol \",\" each value representing the number of hops the onion will travel to return back to the sink node issuer of the onion.

  DataCollector data; //!< Collect data with the stats framework

  /**
  *
  * \brief  Split the string \p m_pathsLengths 
  *         by the delimiter (,) and save each value into \p m_onionPathsLengths 
  *         and the number of values into \p m_numOnionPaths
  * 
  */
  void ProcessPathString ();

  /**
    *
    * \brief  Instantiate objects for collecting data with the statistical framework.
    *         data sent and received at MAC layer, and data received at Application layer
    * 
    */
  void CaptureStatistics ();

  /**
  *
  * \brief  Create node objects
  * 
  */
  void CreateNodes ();
  /**
  *
  * \brief  Create Network devices and setup the wireless communication based on the IEEE 802.11n
  *         Allow to select from 2.4GHz to 5GHz and setup different MCS using the attribute system
  * 
  */
  void CreateDevices ();
  /**
  *
  * \brief  Deploy nodes at random positions on a disc shaped plane. 
  *         The radius of the disc is selected based on <i>r_disc<sup>2</sup> * &pi; = A</i>.
  *         <i>A</i> being the sum of circular areas covered by \p m_numNodes at radius \p m_radius
  *         Therefore the average sensor node density is maintained fixed if the parameter \p m_numNodes changes.
  *         The sink node is deployed in the center of the disc.    
  * 
  */
  void BuildDiscTopology ();

  /**
  *
  * \brief  Sensor nodes are deployed according to a grid structure;
  *         each sensor node is equidistant from the closest sensor nodes in cardinal directions.
  *         The distance between sensor nodes is defined by the parameter \p m_cellSide
  *         The sink node is deployed in the center of the grid repleacing a sensor node.    
  * 
  */
  void BuildGridTopology ();

  /**
  *
  * \brief  Installing the internet stack on nodes and setting up IP-addresses
  * 
  */
  void InstallInternetStack ();

  /**
  *
  * \brief  Install applications on nodes and setup starting time of the handshake and the onion start time
  * 
  */
  void InstallApplications ();

  /**
  *
  * \brief  Install DSR routing
  * 
  */
  void DSRrouting ();
  /**
  *
  * \brief  Install AODV routing
  * 
  */
  void AODVrouting ();
  /**
  *
  * \brief  Install OLSR routing
  * 
  */
  void OLSRrouting ();
  /**
  *
  * \brief  Install DSDV routing
  * 
  */
  void DSDVrouting ();

  MobilityHelper mobility; //!< Topology helper
  NodeContainer wifiNodes; //!< Container of wireless nodes
  NodeContainer m_sink; //!< Container of the sink node
  NodeContainer sensornodes; //!< Container of sensor nodes
  NetDeviceContainer wifiDevices; //!< Container of wireless devices
  Ipv4InterfaceContainer wifiInterfaces; //!< Container of netork interfaces
  ApplicationContainer sinkApps; //!< Container of sink node applications
  ApplicationContainer sensornodeApps; //!< Container of sensor node applications
};

#endif // WSNCONSTRUCTOR_H

// Copyright (C) 2004, 2009 International Business Machines and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// Authors:  Carl Laird, Andreas Waechter     IBM    2004-08-13

#include "IpLoqoMuOracle.hpp"

#include <cmath>
#include <cstdio>

namespace Ipopt
{

#if IPOPT_VERBOSITY > 0
static const Index dbg_verbosity = 0;
#endif

LoqoMuOracle::LoqoMuOracle()
   : MuOracle()
{ }

LoqoMuOracle::~LoqoMuOracle()
{ }

bool LoqoMuOracle::InitializeImpl(
   const OptionsList& /*options*/,
   const std::string& /*prefix*/
)
{
   return true;
}

bool LoqoMuOracle::CalculateMu(
   Number  mu_min,
   Number  mu_max,
   Number& new_mu
)
{
   DBG_START_METH("LoqoMuOracle::CalculateMu",
                  dbg_verbosity);

   Number avrg_compl = IpCq().curr_avrg_compl();
   Jnlst().Printf(J_DETAILED, J_BARRIER_UPDATE,
                  "  Average complementarity is %lf\n", avrg_compl);

   Number xi = IpCq().curr_centrality_measure();
   Jnlst().Printf(J_DETAILED, J_BARRIER_UPDATE,
                  "  Xi (distance from uniformity) is %lf\n", xi);

   //Number factor = 1.-tau_min_;   //This is the original values
   Number factor = 0.05;   //This is the value I used otherwise
   Number sigma = Number(0.1) * std::pow(Min(factor * (Number(1.) - xi) / xi, Number(2.)), Number(3.));

   Number mu = sigma * avrg_compl;
   Jnlst().Printf(J_DETAILED, J_BARRIER_UPDATE,
                  "  Barrier parameter proposed by LOQO rule is %lf\n", mu);

   /*
   IpData().Append_info_string(" sigma", sigma);
   IpData().Append_info_string(" xi", IpCq().curr_centrality_measure());
   */

   new_mu = Max(Min(mu_max, mu), mu_min);
   return true;
}

} // namespace Ipopt

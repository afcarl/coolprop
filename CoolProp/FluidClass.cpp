#define _CRT_SECURE_NO_WARNINGS
#include "CoolPropTools.h"
#ifdef __ISWINDOWS__
	#define _USE_MATH_DEFINES
	#include "float.h"
#else
	#ifndef DBL_EPSILON
		#include <limits>
		#define DBL_EPSILON std::numeric_limits<double>::epsilon()
	#endif
#endif
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <exception>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include <cmath>

#include "MatrixMath.h"
#include "Helmholtz.h"
#include "FluidClass.h"
#include "CoolProp.h"

#include "PengRobinson.h"
#include "REFPROP.h"
#include "Solvers.h"
#include "CPState.h"
#include "Brine.h"
#include "CriticalSplineConstants.h"

#include "pseudopurefluids/Air.h"
#include "pseudopurefluids/R410A.h"
#include "pseudopurefluids/R404A.h"
#include "pseudopurefluids/R507A.h"
#include "pseudopurefluids/R407C.h"
#include "pseudopurefluids/SES36.h"

#include "purefluids/Water.h"
#include "purefluids/R134a.h"
#include "purefluids/R290.h"
#include "purefluids/R32.h"
#include "purefluids/R744.h"
#include "purefluids/R717.h"
#include "purefluids/Argon.h"
#include "purefluids/R1234yf.h"
#include "purefluids/Nitrogen.h"
#include "purefluids/IndustrialFluids.h"
#include "purefluids/Alkanes.h"
#include "purefluids/Siloxanes.h"
#include "purefluids/R22.h"
#include "purefluids/Hydrogen.h"
#include "purefluids/Propylene.h"
#include "purefluids/Oxygen.h"
#include "purefluids/Helium.h"
#include "purefluids/Ethylene.h"
#include "purefluids/SulfurHexafluoride.h"
#include "purefluids/Ethanol.h"
#include "purefluids/Cyclopentane.h"
#include "purefluids/Span_Short.h"
#include "purefluids/R1234ze.h"
#include "purefluids/Butenes.h"
#include "purefluids/FAME.h"
#include "purefluids/Ether.h"
#include "purefluids/R143A.h"
#include "purefluids/DMC.h"
#include "purefluids/R23.h"
#include "purefluids/Xylene_EthylBenzene.h"
#include "purefluids/R236FA.h"
#include "purefluids/R236EA.h"
#include "purefluids/R227EA_R365MFC.h"
#include "purefluids/R161.h"
#include "purefluids/HFE143m.h"
#include "purefluids/Benzene.h"
#include "purefluids/Undecane.h"
#include "purefluids/R125.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

static bool UseCriticalSpline = true;

void rebuild_CriticalSplineConstants_T()
{
	UseCriticalSpline = false;
	FluidsContainer Fluids = FluidsContainer();
	std::vector<std::string> fluid_names = strsplit(Fluids.FluidList(),',');

	double rhoL=0, rhoV=0, drhodTL=0, drhodTV=0;

	FILE *fp;
	fp = fopen("CriticalSplineConstants_T.h","w");
	for (unsigned int i = 0; i < fluid_names.size(); i++)
	{
		std::cout << format("%s:\n",fluid_names[i].c_str()).c_str();
		CoolPropStateClass CPS = CoolPropStateClass(fluid_names[i]);
		double Tc = CPS.pFluid->reduce.T;
		double Tt = CPS.pFluid->params.Ttriple;
		if (!CPS.pFluid->pure())
		{
			// Skip pseudo-pure fluids
			continue;
		}
		double good;
		try{
			if (Tc-5>Tt)
			{
				CPS.update(iT,Tc-5,iQ,1);
				good = 5;
			}
			else
			{
				CPS.update(iT,Tc-1,iQ,1);
				good = 1;
			}
		}
		catch(std::exception)
		{
			if (CPS.pFluid->reduce.T > 100)
			{
				std::cout << format("%s : failed at 5 K \n",fluid_names[i].c_str()).c_str();
				continue;
			}
			else
			{
				std::cout << format("%s : failed at 5 K \n",fluid_names[i].c_str()).c_str();
				continue;
			}
		}
		
		try{
			for (double b = good; b>0; b -= 0.01)
			{
				CPS.update(iT,Tc-b,iQ,1);
				good = b;
				rhoV = CPS.rhoV(); rhoL = CPS.rhoL();
				drhodTV = CPS.drhodT_along_sat_vapor(); 
				drhodTL = CPS.drhodT_along_sat_liquid();
			}
			for (double b = 0.01; b>0; b -= 0.0001)
			{
				CPS.update(iT,Tc-b,iQ,1);
				good = b;
				rhoV = CPS.rhoV(); rhoL = CPS.rhoL();
				drhodTV = CPS.drhodT_along_sat_vapor(); 
				drhodTL = CPS.drhodT_along_sat_liquid();
			}
		}
		catch(std::exception){
		}
		fprintf(fp,"\tstd::make_pair(std::string(\"%s\"),CriticalSplineStruct_T(%0.12e,%0.12e,%0.12e,%0.12e,%0.12e) ),\n",fluid_names[i].c_str(),Tc-good,rhoL,rhoV,drhodTL,drhodTV);
	}
	fclose(fp);
	UseCriticalSpline = true;
}

CriticalSplineStruct_T::CriticalSplineStruct_T(double Tend, double rhoendL, double rhoendV, double drhoLdT_sat, double drhoVdT_sat){
		this->Tend = Tend;
		this->rhoendL = rhoendL;
		this->rhoendV = rhoendV;
		this->drhoLdT_sat = drhoLdT_sat;
		this->drhoVdT_sat = drhoVdT_sat;
	};

// ------------------------------
// FluidsContainer Implementation
// ------------------------------

FluidsContainer::FluidsContainer()
{
	
	// The pure fluids
	FluidsList.push_back(new WaterClass());	
	FluidsList.push_back(new R134aClass());
	FluidsList.push_back(new HeliumClass());
	FluidsList.push_back(new OxygenClass());
	FluidsList.push_back(new HydrogenClass());
	FluidsList.push_back(new ParaHydrogenClass());
	FluidsList.push_back(new OrthoHydrogenClass());
	FluidsList.push_back(new ArgonClass());
	FluidsList.push_back(new R744Class());
	FluidsList.push_back(new NitrogenClass());
	FluidsList.push_back(new R290Class());
	FluidsList.push_back(new R717Class());
	FluidsList.push_back(new R1234yfClass());
	FluidsList.push_back(new R1234zeClass());
	FluidsList.push_back(new R32Class());
	FluidsList.push_back(new R22Class());
	FluidsList.push_back(new SES36Class());
	FluidsList.push_back(new EthyleneClass());
	FluidsList.push_back(new SulfurHexafluorideClass());
	FluidsList.push_back(new EthanolClass());
	FluidsList.push_back(new DimethylEtherClass());	
	FluidsList.push_back(new DimethylCarbonateClass());	
	FluidsList.push_back(new R143AClass());	
	FluidsList.push_back(new R23Class());
	FluidsList.push_back(new nDodecaneClass());	
	FluidsList.push_back(new PropyleneClass());	
	FluidsList.push_back(new CyclopentaneClass());	
	FluidsList.push_back(new R236FAClass());
	FluidsList.push_back(new R236EAClass());
	FluidsList.push_back(new R227EAClass());
	FluidsList.push_back(new R365MFCClass());
	FluidsList.push_back(new R161Class());
	FluidsList.push_back(new HFE143mClass());
	FluidsList.push_back(new BenzeneClass());
	FluidsList.push_back(new UndecaneClass());
	FluidsList.push_back(new R125Class());

	// The industrial fluids
	FluidsList.push_back(new R245faClass());
	FluidsList.push_back(new R41Class());
	FluidsList.push_back(new CarbonMonoxideClass());
	FluidsList.push_back(new CarbonylSulfideClass());
	FluidsList.push_back(new DecaneClass());
	FluidsList.push_back(new HydrogenSulfideClass());
	FluidsList.push_back(new IsopentaneClass());
	FluidsList.push_back(new NeopentaneClass());
	FluidsList.push_back(new IsohexaneClass());
	FluidsList.push_back(new KryptonClass());
	FluidsList.push_back(new NonaneClass());
	FluidsList.push_back(new TolueneClass());
	FluidsList.push_back(new XenonClass());
	FluidsList.push_back(new R116Class());
	FluidsList.push_back(new AcetoneClass());
	FluidsList.push_back(new NitrousOxideClass());
	FluidsList.push_back(new SulfurDioxideClass());
	FluidsList.push_back(new R141bClass());
	FluidsList.push_back(new R142bClass());
	FluidsList.push_back(new R218Class());
	
	FluidsList.push_back(new MethaneClass());
	FluidsList.push_back(new EthaneClass());
	FluidsList.push_back(new nButaneClass());
	FluidsList.push_back(new IsoButaneClass());
	// Span Non-Polar
	FluidsList.push_back(new nPentaneClass());
	FluidsList.push_back(new nHexaneClass());
	FluidsList.push_back(new nHeptaneClass());
	FluidsList.push_back(new nOctaneClass());
	FluidsList.push_back(new CyclohexaneClass());
	// Span Polar
	FluidsList.push_back(new R152AClass());
	FluidsList.push_back(new R123Class());
	FluidsList.push_back(new R11Class());


	// The Siloxanes
	FluidsList.push_back(new OctamethyltrisiloxaneClass()); //MDM
	FluidsList.push_back(new DecamethyltetrasiloxaneClass()); //MD2M
	FluidsList.push_back(new DodecamethylpentasiloxaneClass()); //MD3M
	FluidsList.push_back(new DodecamethylcyclohexasiloxaneClass()); //D6
	FluidsList.push_back(new HexamethyldisiloxaneClass());//MM
	FluidsList.push_back(new TetradecamethylhexasiloxaneClass()); //MD4M
	FluidsList.push_back(new OctamethylcyclotetrasiloxaneClass()); //D4
	FluidsList.push_back(new DecamethylcyclopentasiloxaneClass()); //D5

	// The butenes
	FluidsList.push_back(new OneButeneClass());
	FluidsList.push_back(new IsoButeneClass());
	FluidsList.push_back(new Cis2ButeneClass());
	FluidsList.push_back(new Trans2ButeneClass());
	
	// The methyl ester components of biodiesel
	FluidsList.push_back(new MethylPalmitateClass());
	FluidsList.push_back(new MethylStearateClass());
	FluidsList.push_back(new MethylOleateClass());
	//FluidsList.push_back(new MethylLinoleateClass());// Something wrong with this fluid NOT WORKING
	FluidsList.push_back(new MethylLinolenateClass());

	// Xylene isomers and EthylBenzene
	FluidsList.push_back(new oXyleneClass());
	FluidsList.push_back(new mXyleneClass());
	FluidsList.push_back(new pXyleneClass());
	FluidsList.push_back(new EthylBenzeneClass());

	// The pseudo-pure fluids
	FluidsList.push_back(new AirClass());
	FluidsList.push_back(new R404AClass());
	FluidsList.push_back(new R410AClass());
	FluidsList.push_back(new R407CClass());
	FluidsList.push_back(new R507AClass());

	

	// Build the map of fluid names mapping to pointers to the Fluid class instances
	for (std::vector<Fluid*>::iterator it = FluidsList.begin(); it != FluidsList.end(); it++)
	{
		//// Load all the parameters related to the Critical point spline
		set_critical_spline_constants((*it));
		// Call the post_load routine
		(*it)->post_load();
		// Load up entry in map
		fluid_name_map[(*it)->get_name()] = *it;
	}
}

// Destructor
FluidsContainer::~FluidsContainer()
{
	while (!FluidsList.empty())
	{
		delete FluidsList.back();
		FluidsList.pop_back();
	}
}

bool FluidsContainer::add_REFPROP_fluid(std::string FluidName, std::vector<double> xmol)
{
	// Some fluids from REFPROP that are not included in CoolProp due to not having a Helmholtz energy EOS
	Fluid * pREFPROPFluid = new REFPROPFluidClass(FluidName,xmol);
	FluidsList.push_back(pREFPROPFluid);
	// Add entry to the fluid name map
	fluid_name_map.insert(std::pair<std::string,Fluid*>(FluidName,pREFPROPFluid));
	return true;
}

Fluid * FluidsContainer::get_fluid(long iFluid)
{
	return FluidsList[iFluid];
}
Fluid * FluidsContainer::get_fluid(std::string name)
{
	std::map<std::string,Fluid*>::iterator it;
	// Try to find using the map if Fluid name is provided
	it = fluid_name_map.find(name);
	// If it is found the iterator will not be equal to end
	if (it != fluid_name_map.end() )
	{
		// Return a pointer to the class
		return (*it).second;
	}

	// Wasn't found, now we need to check for an alias
	for (std::vector<Fluid*>::iterator it = FluidsList.begin(); it != FluidsList.end(); it++)
	{
		if ( (*it)->isAlias(name) )
		{
			return *it;
		}
	}
	return NULL;
}

long FluidsContainer::get_fluid_index(Fluid* pFluid)
{
	int i = 0;
	// Iterate to find the 0-based index of the fluid
	for (std::vector<Fluid*>::iterator it = FluidsList.begin(); it != FluidsList.end(); it++)
	{
		if ((*it) == pFluid)
		{
			return i;
		}
		i++;
	}
	return -1;
}

std::string FluidsContainer::FluidList()
{
	// Return a std::string with the list of fluids
	std::string FL;
	for (std::vector<Fluid*>::iterator it = FluidsList.begin(); it != FluidsList.end(); it++)
	{
		FL+=(*it)->get_name();
		FL+=",";
	}
	//Remove the tailing comma
	FL = FL.substr (0,FL.length()-1);
	return FL;
}


/// A stub class to do the density(T,p) calculations for near the critical point using Brent solver
class DensityTpResids : public FuncWrapper1D
{
private:
	double p,T;
	Fluid *pFluid;
public:
	DensityTpResids(Fluid *pFluid, double T, double p){this->pFluid = pFluid; this->p = p; this->T = T;};
	~DensityTpResids(){};
	
	double call(double rho)
	{
		return this->p - pFluid->pressure_Trho(T,rho);
	}
};
// 

// Destructor needs to free all dynamically allocated objects
// In this case, the phir and phi0 components
Fluid::~Fluid()
{
	while (!phirlist.empty())
	{
		delete phirlist.back();  
		phirlist.pop_back();
	}
	while (!phi0list.empty())
	{
		delete phi0list.back();  
		phi0list.pop_back();
	}
	if (h_ancillary != NULL){ delete h_ancillary; h_ancillary = NULL;}
	if (s_ancillary != NULL){ delete s_ancillary; s_ancillary = NULL;}
	if (cp_ancillary != NULL){ delete cp_ancillary; cp_ancillary = NULL;}
	if (drhodT_p_ancillary != NULL){ delete drhodT_p_ancillary; drhodT_p_ancillary = NULL;}
}
void Fluid::post_load(void)
{
	// Set the reducing values from the pointer
	reduce=*preduce;
	// Set the triple-point pressure if not set in code
	// Use the dewpoint pressure for pseudo-pure fluids
	if ( fabs(params.ptriple)>1e9 ){
		double pL, pV, rhoL, rhoV;
		saturation_T(params.Ttriple,false,&pL,&pV,&rhoL,&rhoV);
		params.ptriple = pV;
	}
	//// Instantiate the ancillary curve classes
	h_ancillary = new AncillaryCurveClass(this,std::string("H"));
	s_ancillary = new AncillaryCurveClass(this,std::string("S"));
	cp_ancillary = new AncillaryCurveClass(this,std::string("C"));
	drhodT_p_ancillary = new AncillaryCurveClass(this,std::string("drhodT|p"));
}
//--------------------------------------------
//    Residual Part
//--------------------------------------------

double Fluid::phir(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phirlist.begin(); it != phirlist.end(); it++)
		summer += (*it)->base(tau,delta);
	return summer;
}
double Fluid::dphir_dDelta(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phirlist.begin(); it != phirlist.end(); it++)
	{
		summer += (*it)->dDelta(tau,delta);
	}
	return summer;
}
double Fluid::d2phir_dDelta2(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phirlist.begin(); it != phirlist.end(); it++)
		summer += (*it)->dDelta2(tau,delta);
	return summer;
}
double Fluid::d3phir_dDelta3(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phirlist.begin(); it != phirlist.end(); it++)
	{
		summer += (*it)->dDelta3(tau,delta);
	}
	return summer;
}
double Fluid::d3phir_dDelta_dTau2(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phirlist.begin(); it != phirlist.end(); it++)
		summer += (*it)->dDelta_dTau2(tau,delta);
	return summer;
}
double Fluid::dphir_dTau(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phirlist.begin(); it != phirlist.end(); it++)
		summer += (*it)->dTau(tau,delta);
	return summer;
}
double Fluid::d2phir_dTau2(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phirlist.begin(); it != phirlist.end(); it++)
		summer += (*it)->dTau2(tau,delta);
	return summer;
}
double Fluid::d3phir_dTau3(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phirlist.begin(); it != phirlist.end(); it++)
		summer += (*it)->dTau3(tau,delta);
	return summer;
}

double Fluid::d2phir_dDelta_dTau(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phirlist.begin(); it != phirlist.end(); it++)
		summer += (*it)->dDelta_dTau(tau,delta);
	return summer;
}
double Fluid::d3phir_dDelta2_dTau(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phirlist.begin(); it != phirlist.end(); it++)
		summer += (*it)->dDelta2_dTau(tau,delta);
	return summer;
}

//--------------------------------------------
//    Ideal Gas Part
//--------------------------------------------
double Fluid::phi0(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phi0list.begin(); it != phi0list.end(); it++){
		summer += (*it)->base(tau,delta);
	}	
	return summer;
}
double Fluid::dphi0_dDelta(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phi0list.begin(); it != phi0list.end(); it++)
		summer += (*it)->dDelta(tau,delta);
	return summer;
}
double Fluid::d2phi0_dDelta2(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phi0list.begin(); it != phi0list.end(); it++)
		summer += (*it)->dDelta2(tau,delta);
	return summer;
}
double Fluid::dphi0_dTau(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phi0list.begin(); it != phi0list.end(); it++){
		summer += (*it)->dTau(tau,delta);
	}
	return summer;
}
double Fluid::d2phi0_dTau2(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phi0list.begin(); it != phi0list.end(); it++){
		summer += (*it)->dTau2(tau,delta);
	}
	return summer;
}
double Fluid::d3phi0_dTau3(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phi0list.begin(); it != phi0list.end(); it++){
		summer += (*it)->dTau3(tau,delta);
	}
	return summer;
}
double Fluid::d2phi0_dDelta_dTau(double tau, double delta)
{
	double summer = 0;
	for (std::vector<phi_BC*>::iterator it = phi0list.begin(); it != phi0list.end(); it++)
		summer += (*it)->dDelta_dTau(tau,delta);
	return summer;
}

bool Fluid::isAlias(std::string name)
{
	// Returns true if name is an alias of the fluid
	for (vector<std::string>::iterator it = aliases.begin(); it != aliases.end(); it++)
		if (name.compare((*it))==0)
		{
			return true;
		}
	return false;
}
// ----------------------------------------
//             Properties
// ----------------------------------------

double Fluid::pressure_Trho(double T, double rho)
{
    double delta,tau,R;
	R=params.R_u/params.molemass;
	tau=reduce.T/T;
	delta=rho/reduce.rho;
	return R*T*rho*(1.0+delta*dphir_dDelta(tau,delta));
}

double Fluid::enthalpy_Trho(double T, double rho)
{
	double delta,tau,R;
    R=params.R_u/params.molemass;
	tau=reduce.T/T;
	delta=rho/reduce.rho;
    return R*T*(1.0+tau*(dphi0_dTau(tau,delta)+dphir_dTau(tau,delta))+delta*dphir_dDelta(tau,delta));
}

double Fluid::internal_energy_Trho(double T, double rho)
{
    double delta,tau,R;
    R=params.R_u/params.molemass;
	tau=reduce.T/T;
	delta=rho/reduce.rho;
    return R*T*tau*(dphi0_dTau(tau,delta)+dphir_dTau(tau,delta));
}

double Fluid::entropy_Trho(double T, double rho)
{
    double delta,tau,R;
    R=params.R_u/params.molemass;
	tau=reduce.T/T;
	delta=rho/reduce.rho;
    return R*(tau*(dphi0_dTau(tau,delta)+dphir_dTau(tau,delta))-phi0(tau,delta)-phir(tau,delta));
}
double Fluid::specific_heat_v_Trho(double T, double rho)
{
    double delta,tau,R;
    R=params.R_u/params.molemass;
	tau=reduce.T/T;
	delta=rho/reduce.rho;
    return -R*tau*tau*(d2phi0_dTau2(tau,delta)+d2phir_dTau2(tau,delta));
}
double Fluid::specific_heat_p_Trho(double T, double rho)
{
    double delta,tau,c1,c2,R,dphir_dDelta_;
    R=params.R_u/params.molemass;
	tau=reduce.T/T;
	delta=rho/reduce.rho;
	dphir_dDelta_=dphir_dDelta(tau,delta);
    c1=pow(1.0+delta*dphir_dDelta_-delta*tau*d2phir_dDelta_dTau(tau,delta),2);
    c2=(1.0+2.0*delta*dphir_dDelta_+pow(delta,2)*d2phir_dDelta2(tau,delta));
	return R*(-pow(tau,2)*(d2phi0_dTau2(tau,delta)+d2phir_dTau2(tau,delta))+c1/c2);
}
double Fluid::specific_heat_p_ideal_Trho(double T)
{
    double tau,R;
    R=params.R_u/params.molemass;
	tau=reduce.T/T;
    return R*(1-tau*tau*d2phi0_dTau2(tau, 1e-12));
}
double Fluid::speed_sound_Trho(double T, double rho)
{
    double delta,tau,c1,c2,R;
    R=params.R_u/params.molemass;
	tau=reduce.T/T;
	delta=rho/reduce.rho;

    c1=-specific_heat_v_Trho(T,rho)/R;
    c2=(1.0+2.0*delta*dphir_dDelta(tau,delta)+pow(delta,2)*d2phir_dDelta2(tau,delta));
    return sqrt(-c2*T*specific_heat_p_Trho(T,rho)*1000/c1);
}
double Fluid::gibbs_Trho(double T,double rho)
{
	double delta,tau,R;
    R=params.R_u/params.molemass;
	tau=reduce.T/T;
	delta=rho/reduce.rho;

    return R*T*(1+phi0(tau,delta)+phir(tau,delta)+delta*dphir_dDelta(tau,delta));
}
double Fluid::dpdT_Trho(double T,double rho)
{
	double delta,tau,R;
    R=params.R_u/params.molemass;
	tau=reduce.T/T;
	delta=rho/reduce.rho;

	return rho*R*(1+delta*dphir_dDelta(tau,delta)-delta*tau*d2phir_dDelta_dTau(tau,delta));
}
double Fluid::drhodT_p_Trho(double T,double rho)
{
	return DerivTerms((char *)"drhodT|p",T,rho,this);
}

/// Get the density using the Soave EOS
double Fluid::density_Tp_Soave(double T, double p, int iValue)
{
	double omega, R, m, a, b, A, B, r, q, D, u, Y, Y1, Y2, Y3, theta, phi, rho;
	omega = params.accentricfactor;
	R = params.R_u/params.molemass;
	m = 0.480+1.574*omega-0.176*omega*omega;
	a = 0.42747*R*R*crit.T*crit.T/crit.p*pow(1+m*(1-sqrt(T/reduce.T)),2);
	b = 0.08664*R*crit.T/crit.p;
	A = a*p/(R*R*T*T);
	B = b*p/(R*T);

	// Terms in reduced cubic equation
	r = (3*(A-B-B*B)-1)/3;
	q = -2/27.0+1.0/3.0*(A-B-B*B)-A*B;

	// Discriminant
	D = pow(r/3,3)+pow(q/2,2);

	if (D>0)
	{
		// One real root
		u = pow(-q/2+sqrt(D),1.0/3.0);
		Y = u-r/(3*u);
		rho = p/(R*T*(Y+1.0/3.0));
		return rho;
	}
	else
	{
		theta = sqrt(-r*r*r/27);
		phi = acos(-q/(2*theta));

		Y1 = 2*pow(theta,1.0/3.0)*cos(phi/3.0);
		Y2 = 2*pow(theta,1.0/3.0)*cos(phi/3.0+2.0*M_PI/3.0);
		Y3 = 2*pow(theta,1.0/3.0)*cos(phi/3.0+4.0*M_PI/3.0);

		double solns [] = {Y1, Y2, Y3};

		// Find the solution that you want to use if there are multiple solutions
		if (iValue == 0)
		{
			Y = *std::min_element(solns, solns+3);
		}
		else if (iValue == 1)
		{
			Y = *std::max_element(solns, solns+3);
		}
		else
		{
			throw ValueError(format("iValue [%d] should be either 0 or 1",iValue));
		}
		
		rho = p/(R*T*(Y+1.0/3.0));
		return rho;
	}
}

/*!

\[\begin{array}{l}
p = \frac{{RT}}{{v - b}} - \frac{{a\alpha }}{{v\left( {v + b} \right)}}\\
\alpha  = \left( {1 + \kappa \left( {1 - \sqrt {{T_r}} } \right)} \right)\left( {1 + \kappa \left( {1 - \sqrt {{T_r}} } \right)} \right) = 1 + 2\kappa \left( {1 - \sqrt {{T_r}} } \right) + {\kappa ^2}{\left( {1 - \sqrt {{T_r}} } \right)^2}\\
\alpha  = 1 + 2\kappa \left( {1 - \sqrt {{T_r}} } \right) + {\kappa ^2}{\left( {1 - \sqrt {{T_r}} } \right)^2}\\
\alpha  = 1 + 2\kappa  - 2\kappa \sqrt {{T_r}}  + {\kappa ^2}\left[ {1 - 2\sqrt {{T_r}}  + {T_r}} \right]\\
T = {T_r}{T_c}\\
p = \frac{{R{T_r}{T_c}}}{{v - b}} - \frac{{a\left( {1 + 2\kappa  - 2\kappa \sqrt {{T_r}}  + {\kappa ^2}\left[ {1 - 2\sqrt {{T_r}}  + {T_r}} \right]} \right)}}{{v\left( {v + b} \right)}}\\
\\
{\rm{Factor in terms of }}\sqrt {{T_r}} \\
\\
p = \frac{{R{T_r}{T_c}}}{{v - b}} - \frac{{a\left( {1 + 2\kappa  + {\kappa ^2} - 2\kappa \sqrt {{T_r}}  + {\kappa ^2}\left[ { - 2\sqrt {{T_r}}  + {T_r}} \right]} \right)}}{{v\left( {v + b} \right)}}\\
p = \frac{{R{T_r}{T_c}}}{{v - b}} - \frac{{a\left( {1 + 2\kappa  + {\kappa ^2} - 2\kappa (1 + \kappa )\sqrt {{T_r}}  + {\kappa ^2}{T_r}} \right)}}{{v\left( {v + b} \right)}}\\
p = \frac{{R{T_r}{T_c}}}{{v - b}} - \frac{{a\left( {1 + 2\kappa  + {\kappa ^2}} \right)}}{{v\left( {v + b} \right)}} + \frac{{2a\kappa (1 + \kappa )}}{{v\left( {v + b} \right)}}\sqrt {{T_r}}  - \frac{{a{\kappa ^2}}}{{v\left( {v + b} \right)}}{T_r}\\
0 = \left[ {\frac{{R{T_c}}}{{v - b}} - \frac{{a{\kappa ^2}}}{{v\left( {v + b} \right)}}} \right]{T_r} + \frac{{2a\kappa (1 + \kappa )}}{{v\left( {v + b} \right)}}\sqrt {{T_r}}  - \frac{{a\left( {1 + 2\kappa  + {\kappa ^2}} \right)}}{{v\left( {v + b} \right)}} - p
\end{array}\]

*/
double Fluid::temperature_prho_PengRobinson(double p, double rho)
{
	double omega, R, kappa, a, b, A, B, C, D, V= 1/rho;
	omega = params.accentricfactor;
	R = params.R_u/params.molemass;

	kappa = 0.37464+1.54226*omega-0.26992*omega*omega;
	a = 0.457235*R*R*crit.T*crit.T/crit.p;
	b = 0.077796*R*crit.T/crit.p;
	double den = V*V+2*b*V-b*b;

	// A sqrt(Tr)^2 + B sqrt(Tr) + C = 0
	A = R*reduce.T/(V-b)-a*kappa*kappa/(den);
	B = +2*a*kappa*(1+kappa)/(den);
	C = -a*(1+2*kappa+kappa*kappa)/(den)-p;

	D = B*B-4*A*C;

	double sqrt_Tr1 = (-B+sqrt(B*B-4*A*C))/(2*A);
	double sqrt_Tr2 = (-B-sqrt(B*B-4*A*C))/(2*A);
	return sqrt_Tr1*sqrt_Tr1*reduce.T;
}

double Fluid::temperature_prho_VanDerWaals(double p, double rho)
{
	double R = 8.314472;
	double a = 27*pow(R*reduce.T,2)/(64*reduce.p);
	double b = 8.314472*reduce.T/(8*reduce.p);
	double Vm = 1/rho*params.molemass;
	return 1.0/R*(p+a/Vm/Vm)*(Vm-b);
}

double Fluid::density_Tp(double T, double p)
{
	// If the guess value is not provided, calculate the guess density using Peng-Robinson
	// This overload is used to pre-calculate the guess for density using PR if possible
	if (debug()>8){
		std::cout<<__FILE__<<':'<<__LINE__<<": Fluid::density_Tp(double T, double p): "<<T<<","<<p<<std::endl;
	}
	return density_Tp(T,p,_get_rho_guess(T,p));
}

double Fluid::density_Tp(double T, double p, double rho_guess)
{
    double delta,tau,dpdrho,error=999,R,p_EOS,rho,change=999;
    R=params.R_u/params.molemass;
	tau=reduce.T/T;

	if (debug()>8){
		std::cout<<__FILE__<<':'<<__LINE__<<": Fluid::density_Tp(double T, double p, double rho_guess): "<<T<<","<<p<<","<<rho_guess<<std::endl;
	}
	// Start with the guess value
	rho=rho_guess;
    int iter=1;
    while (fabs(error)>1e-8 && fabs(change)>1e-10)
    {
        delta=rho/reduce.rho;
		// Run and save to cut down on calculations
		double dphirdDelta = dphir_dDelta(tau,delta);
        // Use Newton's method to find the density since the derivative of pressure w.r.t. density is known from EOS
        dpdrho=R*T*(1+2*delta*dphirdDelta+delta*delta*d2phir_dDelta2(tau,delta));
		// Pressure from equation of state
		p_EOS = rho*R*T*(1+delta*dphirdDelta);
        // Update the step using Newton's method
        rho = rho-(p_EOS-p)/dpdrho;
		change = fabs((p_EOS-p)/dpdrho);
        // Residual
        error=p_EOS-p;
        iter++;
        if (iter>30)
        {
			throw SolutionError(format("Number of steps in density_TP has exceeded 30 with inputs T=%g,p=%g,rho_guess=%g for fluid %s\n",T,p,rho_guess,name.c_str()));
        }
		if (!ValidNumber(rho))
		{
			throw SolutionError(format("Non-numeric density obtained in density_TP with inputs T=%g,p=%g,rho_guess=%g for fluid %s\n",T,p,rho_guess,name.c_str()));
		}
    }	
	if (debug()>8){
		std::cout<<__FILE__<<':'<<__LINE__<<": Fluid::density_Tp(double T, double p, double rho_guess): "<<T<<","<<p<<","<<rho_guess<<" = "<<rho<<std::endl;

	}
    return rho;
}

double Fluid::viscosity_Trho( double T, double rho)
{
	long iR134a = get_Fluid_index(std::string("R134a"));
	// Calculate the ECS
	double mu = viscosity_ECS_Trho(T, rho, get_fluid(iR134a));
	return mu;
}
double Fluid::conductivity_Trho( double T, double rho)
{
	long iR134a = get_Fluid_index(std::string("R134a"));
	// Calculate the ECS
	double lambda = conductivity_ECS_Trho(T, rho, get_fluid(iR134a));
	return lambda;
}



void Fluid::saturation_s(double s, int Q, double *Tsatout, double *rhoout)
{
	class SatFuncClass : public FuncWrapper1D
	{
	private:
		int Q;
		double s;
		Fluid * pFluid;
	public:
		double rho,pL,pV,rhoL,rhoV;
		SatFuncClass(double s, int Q, Fluid *pFluid){
			this->s = s;
			this->Q = Q;
			this->pFluid = pFluid;
		};
		double call(double T){
			pFluid->saturation_T(T, false, &pL, &pV, &rhoL, &rhoV);
			if (Q == 0){
				this->rho = rhoL;
				return pFluid->entropy_Trho(T,rhoL) - this->s;
			}
			else if (Q == 1){
				this->rho = rhoV;
				return pFluid->entropy_Trho(T,rhoV) - this->s;
			}
			else{
				throw ValueError("Q must be 0 or 1");
			}
		};
	} SatFunc(s, Q, this);

	if (debug()>5){
		std::cout<<format("%s:%d: Fluid::saturation_s(%g,%d) \n",__FILE__,__LINE__,s,Q).c_str();
	}
	if (isPure==true)
	{
		if (enabled_TTSE_LUT)
		{
			throw NotImplementedError(format("saturation_s not implemented for TTSE"));
			return;
		}
		else
		{
			std::string errstr;
			*Tsatout = Brent(&SatFunc,params.Ttriple,crit.T,1e-16,1e-8,100,&errstr);
			*rhoout = SatFunc.rho;
		}
	}
	else
	{ 
		//// Pseudo-pure fluid
		//*psatLout = psatL(T);
		//*psatVout = psatV(T);
		//try{
		//	*rhosatLout = density_Tp(T, *psatLout, rhosatL(T));
		//	*rhosatVout = density_Tp(T, *psatVout, rhosatV(T));
		//}
		//catch (std::exception){
		//	// Near the critical point, the behavior is not very nice, so we will just use the ancillary near the critical point
		//	*rhosatLout = rhosatL(T);
		//	*rhosatVout = rhosatV(T);
		//}
		//return;
	}
}

void Fluid::saturation_T(double T, bool UseLUT, double *psatLout, double *psatVout, double *rhosatLout, double *rhosatVout)
{
	double rhoL, rhoV, p;
	if (debug()>5){
		std::cout<<format("%s:%d: Fluid::saturation_T(%g,%d) \n",__FILE__,__LINE__,T,UseLUT).c_str();
	}
	if (isPure==true)
	{
		if (enabled_TTSE_LUT)
		{
			throw NotImplementedError(format("saturation_T not implemented for TTSE"));
			return;
		}
		else
		{
			if (UseCriticalSpline && ValidNumber(CriticalSpline_T.Tend) && T > CriticalSpline_T.Tend)
			{
				//// Use the spline (or linear) interpolation since you are very close to the critical point
				*rhosatLout = CriticalSpline_T.interpolate_rho(this,0,T);
				*rhosatVout = CriticalSpline_T.interpolate_rho(this,1,T);
				*psatLout = pressure_Trho(T,*rhosatLout);
				*psatVout = *psatLout;
				return;
			}
			try{
				rhosatPure_Akasaka(T,rhosatLout,rhosatVout,&p);
				*psatLout = p;
				*psatVout = p;
				return;
			}
			catch (std::exception){
				rhosatPure_Brent(T,&rhoL,&rhoV,&p);
				*rhosatLout = rhoL;
				*rhosatVout = rhoV;
				*psatLout = p;
				*psatVout = p;
				return;
			}
		}
	}
	else
	{ 
		// Pseudo-pure fluid
		*psatLout = psatL(T);
		*psatVout = psatV(T);
		try{
			*rhosatLout = density_Tp(T, *psatLout, rhosatL(T));
			*rhosatVout = density_Tp(T, *psatVout, rhosatV(T));
		}
		catch (std::exception){
			// Near the critical point, the behavior is not very nice, so we will just use the ancillary near the critical point
			*rhosatLout = rhosatL(T);
			*rhosatVout = rhosatV(T);
		}
		return;
	}
}


void Fluid::rhosatPure_Brent(double T, double *rhoLout, double *rhoVout, double *pout)
{
	/*
	Use Brent's method around when Akasaka's method fails.  Slow but reliable
	*/

	class SatFuncClass : public FuncWrapper1D
	{
	private:
		double T,gL,gV;
		Fluid * pFluid;
	public:
		double p,rhoL,rhoV;
		SatFuncClass(double T, Fluid *pFluid){
			this->T=T;
			this->pFluid = pFluid;
		};
		double call(double p){
			// Span, 2000 p 56
			DensityTpResids DTPR = DensityTpResids(this->pFluid,T,p);
			std::string errstr;
			this->rhoL = this->pFluid->rhosatL(T);
			//std::cout << Props("D",'Q',0,'T',T,"REFPROP-R245fa") <<std::endl;
			rhoL = Brent(&DTPR,rhoL+1e-3,rhoL-1e-3,DBL_EPSILON,1e-8,30,&errstr);
			rhoV = Brent(&DTPR,rhoV,pFluid->crit.rho-10*DBL_EPSILON,DBL_EPSILON,1e-8,40,&errstr);
			gL = pFluid->gibbs_Trho(T,rhoL);
			gV = pFluid->gibbs_Trho(T,rhoV);
			return gL-gV;
		};
	} SatFunc(T,this);

	std::string errstr;
	double pmax = reduce.p;
	double pmin = psatV_anc(T-0.1);
	if (pmin < params.ptriple)
		pmin = params.ptriple;
	Brent(&SatFunc,pmin,pmax,DBL_EPSILON,1e-8,30,&errstr);
}

class rhosatPure_BrentrhoVResidClass : public FuncWrapper1D
{
public:
	Fluid * pFluid;
	double T,gL,gV,rhoL,rhoV,p;

	rhosatPure_BrentrhoVResidClass(double T, Fluid *pFluid){
		this->T=T;
		this->pFluid = pFluid;
		this->rhoL = pFluid->rhosatL(T);
	};
	double call(double rhoV){
		double pV = pFluid->pressure_Trho(T,rhoV);
		rhoL = pFluid->density_Tp(T,pV,rhoL);
		gL = pFluid->gibbs_Trho(T,rhoL);
		gV = pFluid->gibbs_Trho(T,rhoV);
		this->p = pV;
		this->rhoV = rhoV;
		return gL-gV;
	};
};

void Fluid::rhosatPure_BrentrhoV(double T, double *rhoLout, double *rhoVout, double *pout)
{
	rhosatPure_BrentrhoVResidClass SF = rhosatPure_BrentrhoVResidClass(T,this);

	std::string errstr;
	Brent(&SF,(reduce.rho+0.95*rhosatV(T))/2,0.95*rhosatV(T),DBL_EPSILON,1e-12,30,&errstr);
	*rhoLout = SF.rhoL;
	*rhoVout = SF.rhoV;
	*pout = SF.p;
}

/// This function implements the method of Akasaka to do analytic Newton-Raphson for the 
/// saturation calcs
void Fluid::rhosatPure_Akasaka(double T, double *rhoLout, double *rhoVout, double *pout)
{
	/*
	This function implements the method of Akasaka 

	R. Akasaka,"A reliable and Useful Method to Determine the Saturation State from 
	Helmholtz Energy Equations of State", 
	Journal of Thermal Science and Technology v3 n3,2008

	Ancillary equations are used to get a sensible starting point
	*/
	double rhoL,rhoV,dphirL,dphirV,ddphirL,ddphirV,phirL,phirV,JL,JV,KL,KV,dJL,dJV,dKL,dKV;
	double DELTA, deltaL=0, deltaV=0, tau=0, gamma = 1, error, PL, PV, stepL, stepV;
	int iter=0;
	// Use the density ancillary function as the starting point for the solver
    try
	{
		rhoL=rhosatL(T);
		rhoV=rhosatV(T);
		deltaL = rhoL/reduce.rho;
		deltaV = rhoV/reduce.rho;
		tau = reduce.T/T;
	}
	catch(NotImplementedError &e)
	{
		std::cout << e.what() << "Ancillary not provided" << std::endl;
	}

	do{
		if (debug()>5){
			std::cout << format("%s:%d: right before the derivs with deltaL = %g deltaV = %g tau = %g\n",__FILE__,__LINE__,deltaL, deltaV, tau).c_str();
		}
		// Calculate once to save on calls to EOS
		dphirL = dphir_dDelta(tau,deltaL);
		dphirV = dphir_dDelta(tau,deltaV);
		ddphirL = d2phir_dDelta2(tau,deltaL);
		ddphirV = d2phir_dDelta2(tau,deltaV);
		phirL = phir(tau,deltaL);
		phirV = phir(tau,deltaV);

		JL = deltaL * (1 + deltaL*dphirL);
		JV = deltaV * (1 + deltaV*dphirV);
		KL = deltaL*dphirL + phirL + log(deltaL);
		KV = deltaV*dphirV + phirV + log(deltaV);
		dJL = 1 + 2*deltaL*dphirL + deltaL*deltaL*ddphirL;
		dJV = 1 + 2*deltaV*dphirV + deltaV*deltaV*ddphirV;
		dKL = 2*dphirL + deltaL*ddphirL + 1/deltaL;
		dKV = 2*dphirV + deltaV*ddphirV + 1/deltaV;
		
		DELTA = dJV*dKL-dJL*dKV;

		error = fabs(KL-KV)+fabs(JL-JV);

		//Get the step
		stepL = gamma/DELTA*( (KV-KL)*dJV-(JV-JL)*dKV);
		stepV = gamma/DELTA*( (KV-KL)*dJL-(JV-JL)*dKL);
		
		if (deltaL+stepL > 1 && deltaV+stepV < 1 && deltaV+stepV > 0)
		{
			deltaL += stepL;
			deltaV += stepV;
		}
		else
		{
			throw ValueError(format("rhosatPure_Akasaka failed"));
		}
		iter=iter+1;
	}
	while (error > 1e-10);
	*rhoLout = deltaL*reduce.rho;
	*rhoVout = deltaV*reduce.rho;
	PL = pressure_Trho(T,*rhoLout);
	PV = pressure_Trho(T,*rhoVout);
	*pout = 0.5*(PL+PV);
	return;
}
void Fluid::rhosatPure(double T, double *rhoLout, double *rhoVout, double *pout)
{
    // Only works for pure fluids (no blends)
    // At equilibrium, saturated vapor and saturated liquid are at the same pressure and the same Gibbs energy
    double rhoL,rhoV,p,error=999,x1=0,x2=0,x3,y1=0,y2,f,p_guess;
    int iter;

    if (T>=crit.T || T<(params.Ttriple-0.0001))
    {
		throw ValueError(format("Temperature of fluid [%g] is out of range from %g K to %g K",T,params.Ttriple,crit.T));
    }

    // Use the density ancillary function as the starting point for the secant solver
    rhoL=rhosatL(T);
	rhoV=rhosatV(T);
	
    p_guess=pressure_Trho(T,rhoV);

    iter=1;
    // Use a secant method to obtain pressure
    while ((iter<=3 || fabs(error)>1e-10) && iter<100)
    {
        if (iter==1){x1=p_guess; p=x1;}
        else if (iter==2){x2=1.0001*p_guess; p=x2;}
        else {p=x2;}
            //Recalculate the densities based on the current pressure
            rhoL=density_Tp(T,p,rhoL);
            rhoV=density_Tp(T,p,rhoV);
            // Residual between saturated liquid and saturated vapor gibbs function
            f=gibbs_Trho(T,rhoL)-gibbs_Trho(T,rhoV);
        if (iter==1){y1=f;}
        else //(iter>1)
        {
            y2=f;
            x3=x2-y2/(y2-y1)*(x2-x1);
            error=f;
            y1=y2; x1=x2; x2=x3;
        }
        iter++;
        if (iter>100)
        {
        	//ERROR
            printf("rhosatPure failed, current values of rhoL and rhoV are %g,%g\n",rhoL,rhoV);
            *rhoLout=_HUGE;
            *rhoVout=_HUGE;
            *pout=_HUGE;
            return;
        }
    }
    *rhoLout=rhoL;
    *rhoVout=rhoV;
    *pout=p;
    return;
}
bool isBetween(double x1,double x2, double x)
{
	if (x2>x1 && x>=x1 && x<=x2) return true;
	else if (x1>x2 && x<=x1 && x>=x2) return true;
	else return false;
}

// Ancillary equations composed by interpolating within 50-point 
// curve that is calculated once
double Fluid::hsatV_anc(double T)
{
	if (!h_ancillary->built) 
		h_ancillary->build(50);
	return h_ancillary->interpolateV(T);
}
double Fluid::ssatV_anc(double T)
{
	if (!s_ancillary->built) 
		s_ancillary->build(50);
	return s_ancillary->interpolateV(T);
}
double Fluid::hsatL_anc(double T)
{
	if (!h_ancillary->built) 
		h_ancillary->build(50);
	return h_ancillary->interpolateL(T);
}
double Fluid::ssatL_anc(double T)
{
	if (!s_ancillary->built) 
		s_ancillary->build(50);
	return s_ancillary->interpolateL(T);
}

double Fluid::_get_rho_guess(double T, double p)
{
    double Tc,rho_simple;

	Tc = reduce.T;

	// Then based on phase, select the useful solution(s)
	double pL,pV,rhoL,rhoV;

	long phase = phase_Tp_indices(T,p,&pL,&pV,&rhoL,&rhoV);
	
	if (debug()>5){
		std::cout<<__FILE__<<format(": Fluid::_get_rho_guess(%g,%g) phase =%d\n",T,p,phase).c_str();
	}
	// These are very simplistic guesses for the density, but they work ok
	if (phase == iGas || phase == iSupercritical)
	{
		// Guess that it is ideal gas
		rho_simple = p/(R()*T);
	}
	else if (phase == iLiquid)
	{
		// Start at the saturation state, with a known density, using the ancillary
		double rhoL = rhosatL(T);
		double pL = psatL_anc(T);
		if (debug()>5){
			std::cout<<format("%d:%d: pL = %g rhoL = %g rhoV %g \n",__FILE__,__LINE__,pL,rhoL, rhoV).c_str();
		}
		double delta = rhoL / reduce.rho;
		double tau = reduce.T/T;
		double dp_drho = R()*T*(1+2*delta*dphir_dDelta(tau,delta)+delta*delta*d2phir_dDelta2(tau,delta));
		double drho_dp = 1/dp_drho;
		rho_simple = rhoL-drho_dp*(pL-p);
	}
	else if (fabs(psatL_anc(T)-p)<1e-8 || fabs(psatV_anc(T)-p)<1e-8)
	{
		throw ValueError(format("Input T [%0.16g] & p [%0.16g] are two-phase or saturated which is thermodynamically undefined\n",T,p));
	}
	else
	{
		// It is two-phase, we are going to skip the process of 
		// solving and just return a value somewhere in the two-phase region.
		return (rhoL+rhoV)/2;
	}
	if (debug()>5){
		std::cout<<__FILE__<<": _get_rho_guess = "<<rho_simple<<std::endl;
	}
	return rho_simple;

	//// Try to use Peng-Robinson to get a guess value for the density
	//std::vector<double> rholist= PRGuess_rho(this,T,p);
	//if (rholist.size()==0){
	//	throw SolutionError(format("PengRobinson could not yield any solutions"));
	//}
	//else if (rholist.size()==1){
	//	rho_guess = rholist[0];
	//}
	//else{
	//	// A list of ok solutions
	//	std::vector<double> goodrholist;
	//	for (unsigned int i=0;i<rholist.size();i++){
	//		pEOS=Props(std::string("P"),'T',T,'D',rholist[i],name.c_str());
	//		if (fabs(pEOS/p-1)<0.03){
	//			goodrholist.push_back(rholist[i]);
	//		}
	//	}
	//	if (goodrholist.size()==1){
	//		rho_guess = goodrholist[0];
	//	}
	//	else{
	//		rho_guess = rho_simple;
	//	}
	//}
	//return rho_guess;
}


std::string Fluid::phase_Tp(double T, double p, double *pL, double *pV, double *rhoL, double *rhoV)
{
	// Get the value from the long-output function
	long iPhase = phase_Tp_indices(T, p, pL, pV, rhoL, rhoV);
	if (get_debug()>7)
	{
		if (debug()>5){
			std::cout<<__FILE__<<": phase index is " << iPhase <<std::endl;
		}

	}
	// Convert it to a std::string
	switch (iPhase)
	{
	case iTwoPhase:
		return std::string("Two-Phase");
	case iSupercritical:
		return std::string("Supercritical");
	case iGas:
		return std::string("Gas");
	case iLiquid:
		return std::string("Liquid");
	default:
		return std::string("");
	}
}
long Fluid::phase_Tp_indices(double T, double p, double *pL, double *pV, double *rhoL, double *rhoV)
{
	/*
		|         |     
		|         |    Supercritical
		|         |
	p	| Liquid (b)------------
		|        /
		|       / 
		|      /       Gas
		|     / 
		|   (a)
		|  -
		|------------------------

		           T

	   a: triple point
	   b: critical point
	   a-b: Saturation line

	*/

	if (debug()>5){
		std::cout<<__FILE__<<format(": phase_Tp_indices(%g,%g)\n",T,p).c_str();
	}

	if (T>crit.T && p>crit.p){
		return iSupercritical;
	}
	else if (T>crit.T && p<crit.p){
		return iGas;
	}
	else if (T<crit.T && p>crit.p){
		return iLiquid;
	}
	else if (p<params.ptriple){
		return iGas;
	}
	else{
		// Now start to think about the saturation stuff
		// First try to use the ancillary equations if you are far enough away
		// Ancillary equations are good to within 1% in pressure in general
		// Some industrial fluids might not be within 3%
		if (isPure && p<0.94*psat(T)){
			return iGas;
		}
		else if (isPure && p>1.06*psat(T)){
			return iLiquid;
		}
		else if (!isPure && p<0.94*psatV(T)){
			return iGas;
		}
		else if (!isPure && p>1.06*psatL(T)){
			return iLiquid;
		}
		else{
			// Actually have to use saturation information sadly
			// For the given temperature, find the saturation state
			// Run the saturation routines to determine the saturation densities and pressures
			saturation_T(T,enabled_TTSE_LUT,pL,pV,rhoL,rhoV);
			if (p>(*pL+10*DBL_EPSILON)){
				return iLiquid;
			}
			else if (p<(*pV-10*DBL_EPSILON)){
				return iGas;
			}
			else{
				return iTwoPhase;
			}
		}
	}
}

std::string Fluid::phase_Trho(double T, double rho, double *pL, double *pV, double *rhoL, double *rhoV)
{
	// Get the value from the long-output function
	long iPhase = phase_Trho_indices(T, rho, pL, pV, rhoL, rhoV);
	if (debug()>5){
		std::cout<<__FILE__<<": phase index is " << iPhase <<std::endl;
	}

	// Convert it to a std::string
	switch (iPhase)
	{
	case iTwoPhase:
		return std::string("Two-Phase");
	case iSupercritical:
		return std::string("Supercritical");
	case iGas:
		return std::string("Gas");
	case iLiquid:
		return std::string("Liquid");
	default:
		return std::string("");
	}
}

long Fluid::phase_Trho_indices(double T, double rho, double *pL, double *pV, double *rhoL, double *rhoV)
{
	/*
        |
	    | Liquid
		|
	    | ---
		|     \
		|      \
	rho	|       a
		|      /
		|     /
		|  --
		|
		| Gas
		|
		|------------------------

		           T

	   a: triple point
	   b: critical point
	   a-b: Saturation line

	*/
	// If temperature is below the critical temperature, it is either 
	// subcooled liquid, superheated vapor, or two-phase
	if (T<reduce.T)
	{
		// Start to think about the saturation stuff
		// First try to use the ancillary equations if you are far enough away
		// Ancillary equations are good to within 1% in pressure in general
		// Some industrial fluids might not be within 3%
		if (rho<0.95*rhosatV(T)){
			return iGas;
		}
		else if (rho>1.05*rhosatL(T)){
			return iLiquid;
		}
		else{
			// Actually have to use saturation information sadly
			// For the given temperature, find the saturation state
			// Run the saturation routines to determine the saturation densities and pressures
			// Use the passed in variables to save calls to the saturation routine so the values can be re-used again
			saturation_T(T,enabled_TTSE_LUT,pL,pV,rhoL,rhoV);
			if (rho>*rhoL){
				return iLiquid;
			}
			else if (rho<*rhoV){
				return iGas;
			}
			else{
				return iTwoPhase;
			}
		}
	}
	// Now check the states above the critical temperature
	double p = pressure_Trho(T,rho);

	if (T>reduce.T && p>reduce.p){
		return iSupercritical;
	}
	else if (T>reduce.T && p<reduce.p){
		return iGas;
	}
	else if (T<reduce.T && p>reduce.p){
		return iLiquid;
	}
	else if (p<params.ptriple){
		return iGas;
	}
	else{
		return -1;
	}
}

long Fluid::phase_prho_indices(double p, double rho, double *T, double *TL, double *TV, double *rhoL, double *rhoV)
{

	// If temperature is below the critical temperature, it is either 
	// subcooled liquid, superheated vapor, or two-phase
	if (p < crit.p)
	{	
		// Actually have to use saturation information sadly
		// For the given temperature, find the saturation state
		// Run the saturation routines to determine the saturation densities and pressures
		// Use the passed in variables to save calls to the saturation routine so the values can be re-used again
		saturation_p(p,false,TL,TV,rhoL,rhoV);
		if (rho>*rhoL){
			return iLiquid;
		}
		else if (rho<*rhoV){
			return iGas;
		}
		else{
			*T = *TL;
			return iTwoPhase;
		}
	}
	// Now check the states above the critical pressure
	double T0 = temperature_prho_PengRobinson(p,rho);
	*T = temperature_prho(p, rho, T0);

	if (*T > crit.T){
		return iSupercritical;
	}
	else{ // T < crit.T
		return iLiquid;
	}
}
double Fluid::temperature_prho(double p, double rho, double T0)
{
	// solve for T to yield the desired pressure
	double error, T, drdT,r;
	int iter = 0;
	CoolPropStateClass CPS = CoolPropStateClass(this);

	T = T0;
	CPS.update(iT,T,iD,rho);

	do
	{
		r = CPS.p()-p;
		drdT = CPS.dpdT_constrho();
		T -= r/drdT;
		CPS.update(iT,T,iD,rho);
		error = fabs(r);
		iter++;
		if (iter > 100)
			throw SolutionError(format("temperature_prho failed with inputs T=%g rho=%g T0=%g for fluid %s",p,rho,T0,name.c_str()));
	}
	while (error > 1e-8 );
	return T;
}

void Fluid::temperature_ph(double p, double h, double *Tout, double *rhoout, double *rhoLout, double *rhoVout, double *TsatLout, double *TsatVout, double T0, double rho0)
{
	int iter;
	bool failed = false;
	double A[2][2], B[2][2],T_guess, omega =1.0;
	double dar_ddelta,da0_dtau,d2a0_dtau2,dar_dtau,d2ar_ddelta_dtau,d2ar_ddelta2,d2ar_dtau2,d2a0_ddelta_dtau;
	double f1,f2,df1_dtau,df1_ddelta,df2_ddelta,df2_dtau;
    double rhoL, rhoV, hsatL,hsatV,TsatL,TsatV,tau,delta,worst_error;
	double h_guess, hc, rho_guess;
	double hsat_tol = 0.5;
	
	// A starting set of temperature and pressure are provided, use them as the guess value
	// Their default values are -1 and -1, which are unphysical values
	if (T0 > 0 && rho0 > 0)
	{
		T_guess = T0;
		delta = rho0/reduce.rho;
	}
	else
	{
		// It is supercritical pressure	(or just below the critical pressure)
		if (p > 0.999*crit.p)
		{
			hc = enthalpy_Trho(crit.T+0.001,crit.rho);
			if (h > hc)
			{
				// Supercritical pseudo-gas - extrapolate to find guess temperature at critical density
				T_guess = crit.T+30;
				h_guess = enthalpy_Trho(T_guess,crit.rho);
				T_guess = (crit.T-T_guess)/(hc-h_guess)*(h-h_guess)+T_guess;
				
				delta = p/(R()*T_guess)/reduce.rho;
			}
			else
			{
				// Supercritical pseudo-liquid
				T_guess = params.Ttriple;
				rho_guess = density_Tp(T_guess,p);
				h_guess = enthalpy_Trho(T_guess,rho_guess);
				// Update the guess with linear interpolation
				T_guess = (crit.T-T_guess)/(hc-h_guess)*(h-h_guess)+T_guess;
				// Solve for the density
				rho_guess = density_Tp(T_guess,p,rho_guess);
				
				delta = rho_guess/reduce.rho;
			}
		}
		else
		{
			// Set to a negative value as a dummy value
			delta = -1;
			
			// If using TTSE, TTSE is the arbiter of two-phase/single-phase boundary
			// 
			if (enabled_TTSE_LUT)
			{
				hsatL = TTSESatL.evaluate(iH,p);
				hsatV = TTSESatV.evaluate(iH,p);
				// If TTSE is enabled and we have gotten to this point
				if (h>hsatV || h<hsatL)
				{
					
					
				}
			}
			else
			{
				//**************************************************
				// Step 1: Try to just use the ancillary equations
				//**************************************************
				TsatL = Tsat_anc(p,0);
				if (pure())
					TsatV = TsatL;
				else
					TsatV = Tsat_anc(p,1);
				rhoL = rhosatL(TsatL);
				rhoV = rhosatV(TsatV);
				hsatL = hsatL_anc(TsatL);
				hsatV = hsatV_anc(TsatV);
				*rhoLout = -1;
				*rhoVout = -1;
				*TsatLout = -1;
				*TsatVout = -1;

				if (h > hsatV + hsat_tol)
				{
					// Using ideal gas cp is a bit faster
					// Can't use an ancillary since it diverges at the critical point
					double cpV = specific_heat_p_ideal_Trho(TsatV);
					//Superheated vapor
					T_guess = TsatV+(h-hsatV)/cpV;
					// Volume expansivity at saturated vapor using the ancillaries
					// Can't use an ancillary since it diverges at the critical point
					double drhodT = DerivTerms((char *)"drhodT|p",TsatV,rhoV,this);
					// Extrapolate to get new density guess
					double rho = rhoV+drhodT*(h-hsatV)/cpV;
					// If the guess is negative for the density, need to make it positive
					if (rho<0)
						rho = 0.1;
					delta = rho / reduce.rho;
				}
				else if (h < hsatL - hsat_tol) 
				{
					T_guess = params.Ttriple;
					rho_guess = density_Tp(T_guess,p);
					h_guess = enthalpy_Trho(T_guess,rho_guess);
					// Update the guess with linear interpolation
					T_guess = (TsatL-T_guess)/(hsatL-h_guess)*(h-h_guess)+T_guess;
					// Solve for the density
					rho_guess = density_Tp(T_guess,p,rho_guess);
					
					delta = rho_guess/reduce.rho;
				}
			}
			
			if (delta<0) // No solution found using ancillary equations (or the saturation LUT is in use) - need to use saturation call (or LUT)
			{
				//**************************************************
				// Step 2: Not far away from saturation (or it is two-phase) - need to solve saturation as a function of p :( - this is slow
				//**************************************************
 				CoolPropStateClass sat = CoolPropStateClass(name);
				sat.update(iP,p,iQ,0);
				rhoL = sat.rhoL();
				rhoV = sat.rhoV();
				hsatL = sat.hL();
				hsatV = sat.hV();
				TsatL = sat.TL();
				TsatV = sat.TV();
				*rhoLout = rhoL;
				*rhoVout = rhoV;
				*TsatLout = TsatL;
				*TsatVout = TsatV;
				
				if (h>hsatV)
				{
					T_guess = TsatV+30;
					h_guess = enthalpy_Trho(T_guess,rhoV);
					T_guess = (TsatV-T_guess)/(hsatV-h_guess)*(h - h_guess)+T_guess;
					
					delta = p/(R()*T_guess)/reduce.rho;
				}
				else if (h<hsatL)
				{
					T_guess = params.Ttriple;
					rho_guess = density_Tp(T_guess,p);
					h_guess = enthalpy_Trho(T_guess,rho_guess);

					// Same thing at the midpoint temperature
					double Tmid = (T_guess + TsatL)/2.0;
					double rho_mid = density_Tp(Tmid,p,(rho_guess+rhoL)/2.0);
					double h_mid = enthalpy_Trho(Tmid,rho_mid);

					// Quadratic interpolation
					T_guess = QuadInterp(h_guess, h_mid, hsatL, T_guess, Tmid, TsatL, h);
					rho_guess = QuadInterp(h_guess, h_mid, hsatL, rho_guess, rho_mid, rhoL, h);

					delta = rho_guess / reduce.rho;
				}
				else
				{
					// It is two-phase
					// Return the quality weighted values
					double quality = (h-hsatL)/(hsatV-hsatL);
					*Tout = quality*TsatV+(1-quality)*TsatL;
					double v = quality*(1/rhoV)+(1-quality)*1/rhoL;
					*rhoout = 1/v;
					return;
				}
			}
		}
	}

	tau=reduce.T/T_guess;
	double Tc = reduce.T;
	double rhoc = reduce.rho;

	// Now we enter into a Jacobian loop that attempts to simultaneously solve 
	// for temperature and density using Newton-Raphson
    worst_error=999;
    iter=0;
    while (worst_error>1e-6 && failed == false)
    {
    	// All the required partial derivatives
    	da0_dtau = dphi0_dTau(tau,delta);
    	d2a0_dtau2 = d2phi0_dTau2(tau,delta);
    	d2a0_ddelta_dtau = 0.0;
    	dar_dtau = dphir_dTau(tau,delta);
		dar_ddelta = dphir_dDelta(tau,delta);
		d2ar_ddelta_dtau = d2phir_dDelta_dTau(tau,delta);
		d2ar_ddelta2 = d2phir_dDelta2(tau,delta);
		d2ar_dtau2 = d2phir_dTau2(tau,delta);

		f1 = delta/tau*(1+delta*dar_ddelta)-p/(rhoc*R()*Tc);
		f2 = (1+delta*dar_ddelta)+tau*(da0_dtau+dar_dtau)-tau*h/(R()*Tc);
		df1_dtau = (1+delta*dar_ddelta)*(-delta/tau/tau)+delta/tau*(delta*d2ar_ddelta_dtau);
		df1_ddelta = (1.0/tau)*(1+2.0*delta*dar_ddelta+delta*delta*d2ar_ddelta2);
		df2_dtau = delta*d2ar_ddelta_dtau+da0_dtau+dar_dtau+tau*(d2a0_dtau2+d2ar_dtau2)-h/(R()*Tc);
		df2_ddelta = (dar_ddelta+delta*d2ar_ddelta2)+tau*(d2a0_ddelta_dtau+d2ar_ddelta_dtau);

		//First index is the row, second index is the column
		A[0][0]=df1_dtau;
		A[0][1]=df1_ddelta;
		A[1][0]=df2_dtau;
		A[1][1]=df2_ddelta;

		//double det = A[0][0]*A[1][1]-A[1][0]*A[0][1];

		MatInv_2(A,B);
		tau -= omega*(B[0][0]*f1+B[0][1]*f2);
		delta -= omega*(B[1][0]*f1+B[1][1]*f2);

        if (fabs(f1)>fabs(f2))
            worst_error=fabs(f1);
        else
            worst_error=fabs(f2);

		iter+=1;
		if (iter>100)
		{
			throw SolutionError(format("Thp did not converge with inputs p=%g h=%g for fluid %s",p,h,(char*)name.c_str()));
			*Tout = _HUGE;
            *rhoout = _HUGE;
		}
    }

	//std::cout<<"Temperature_ph took "<<iter-1<<" steps\n";
	*Tout = reduce.T/tau;
	*rhoout = delta*reduce.rho;
}

void Fluid::temperature_ps(double p, double s, double *Tout, double *rhoout, double *rhoLout, double *rhoVout, double *TsatLout, double *TsatVout)
{
	int iter;
	double A[2][2], B[2][2],T_guess;
	double dar_ddelta,da0_dtau,d2a0_dtau2,dar_dtau,d2ar_ddelta_dtau,d2ar_ddelta2,d2ar_dtau2,d2a0_ddelta_dtau,a0,ar,da0_ddelta;
	double f1,f2,df1_dtau,df1_ddelta,df2_ddelta,df2_dtau;
    double rhoL, rhoV, ssatL,ssatV,TsatL,TsatV,tau,delta,worst_error;
	double s_guess, sc, rho_guess;
	double ssat_tol = 0.1;
	// It is supercritical pressure	(or just below the critical pressure)
	if (p > 0.999*crit.p) 
	{
		sc = entropy_Trho(crit.T+0.001,crit.rho);
		if (s > sc)
		{
			// Supercritical pseudo-gas - extrapolate to find guess temperature at critical density
			T_guess = crit.T+30;
			s_guess = entropy_Trho(T_guess,crit.rho);
			T_guess = (crit.T-T_guess)/(sc-s_guess)*(s-s_guess)+T_guess;
			
			delta = p/(R()*T_guess)/reduce.rho;
		}
		else
		{
			// Supercritical pseudo-liquid
			T_guess = params.Ttriple;
			rho_guess = density_Tp(T_guess,p);
			s_guess = entropy_Trho(T_guess,rho_guess);
			// Update the guess with linear interpolation
			T_guess = (crit.T-T_guess)/(sc-s_guess)*(s-s_guess)+T_guess;
			// Solve for the density
			rho_guess = density_Tp(T_guess,p,rho_guess);
			
			delta = rho_guess/reduce.rho;
		}
	}
	else
	{
		// Set to a negative value as a dummy value
		delta = -1;

		//**************************************************
		// Step 1: Try to just use the ancillary equations
		//**************************************************
		TsatL = Tsat_anc(p,0);
		if (pure())
			TsatV = TsatL;
		else
			TsatV = Tsat_anc(p,1);
		rhoL = rhosatL(TsatL);
		rhoV = rhosatV(TsatV);
		ssatL = ssatL_anc(TsatL);
		ssatV = ssatV_anc(TsatV);
		*rhoLout = -1;
		*rhoVout = -1;
		*TsatLout = -1;
		*TsatVout = -1;

		if (s > ssatV + ssat_tol)
		{
			
			T_guess = TsatV+30;
			s_guess = entropy_Trho(T_guess,rhoV);
			T_guess = (TsatV-T_guess)/(ssatV-s_guess)*(s-s_guess)+T_guess;
			
			delta = p/(R()*T_guess)/reduce.rho;
		}
		else if (s < ssatL - ssat_tol)
		{
			T_guess = params.Ttriple;
			rho_guess = density_Tp(T_guess,p);
			s_guess = entropy_Trho(T_guess,rho_guess);
			// Update the guess with linear interpolation
			T_guess = (TsatL-T_guess)/(ssatL-s_guess)*(s-s_guess)+T_guess;
			// Solve for the density
			rho_guess = density_Tp(T_guess,p,rho_guess);
			
			delta = rho_guess/reduce.rho;
		}
		
		if (delta<0) // No solution found using ancillary equations - need to use saturation call
		{
			//**************************************************
			// Step 2: Not far away from saturation (or it is two-phase) - need to solve saturation as a function of p :( - this is slow
			//**************************************************
			CoolPropStateClass sat = CoolPropStateClass(name);
			sat.update(get_param_index((char *)"P"),p,get_param_index((char *)"Q"),0);
			rhoL = sat.rhoL();
			rhoV = sat.rhoV();
			ssatL = sat.sL();
			ssatV = sat.sV();
			TsatL = sat.TL();
			TsatV = sat.TV();
			*rhoLout = rhoL;
			*rhoVout = rhoV;
			*TsatLout = TsatL;
			*TsatVout = TsatV;

			if (s > ssatV)
			{
				T_guess = TsatV+30;
				s_guess = entropy_Trho(T_guess,rhoV);
				T_guess = (TsatV-T_guess)/(ssatV-s_guess)*(s-s_guess)+T_guess;
				
				delta = p/(R()*T_guess)/reduce.rho;
			}
			else if (s < ssatL)
			{
				T_guess = params.Ttriple;
				rho_guess = density_Tp(T_guess,p);
				s_guess = entropy_Trho(T_guess,rho_guess);
				// Update the guess with linear interpolation
				T_guess = (TsatL-T_guess)/(ssatL-s_guess)*(s-s_guess)+T_guess;
				// Solve for the density
				rho_guess = density_Tp(T_guess,p,rho_guess);
				
				delta = rho_guess/reduce.rho;
			}
			else
			{
				// It is two-phase
				// Return the quality weighted values
				double quality = (s-ssatL)/(ssatV-ssatL);
				*Tout = quality*TsatV+(1-quality)*TsatL;
				double v = quality*(1/rhoV)+(1-quality)*1/rhoL;
				*rhoout = 1/v;
				return;
			}
		}
	}

	tau=reduce.T/T_guess;
	double Tc = reduce.T;
	double rhoc = reduce.rho;

	// Now we enter into a Jacobian loop that attempts to simultaneously solve 
	// for temperature and density using Newton-Raphson
    worst_error=999;
    iter=0;
    while (worst_error>1e-6)
    {
    	// All the required partial derivatives
		a0 = phi0(tau,delta);
    	da0_dtau = dphi0_dTau(tau,delta);
    	d2a0_dtau2 = d2phi0_dTau2(tau,delta);
    	da0_ddelta = dphi0_dDelta(tau,delta);
		d2a0_ddelta_dtau = 0.0;

    	ar = phir(tau,delta);
		dar_dtau = dphir_dTau(tau,delta);
		d2ar_dtau2 = d2phir_dTau2(tau,delta);
		dar_ddelta = dphir_dDelta(tau,delta);
		d2ar_ddelta2 = d2phir_dDelta2(tau,delta);
		d2ar_ddelta_dtau = d2phir_dDelta_dTau(tau,delta);
		
		// Residual and derivatives thereof for entropy
		f1 = tau*(da0_dtau+dar_dtau)-ar-a0-s/R();
		df1_dtau = tau*(d2a0_dtau2 + d2ar_dtau2)+(da0_dtau+dar_dtau)-dar_dtau-da0_dtau;
		df1_ddelta = tau*(d2a0_ddelta_dtau+d2ar_ddelta_dtau)-dar_ddelta-da0_ddelta;

		// Residual and derivatives thereof for pressure
		f2 = delta/tau*(1+delta*dar_ddelta)-p/(rhoc*R()*Tc);
		df2_dtau = (1+delta*dar_ddelta)*(-delta/tau/tau)+delta/tau*(delta*d2ar_ddelta_dtau);
		df2_ddelta = (1.0/tau)*(1+2.0*delta*dar_ddelta+delta*delta*d2ar_ddelta2);

		//First index is the row, second index is the column
		A[0][0]=df1_dtau;
		A[0][1]=df1_ddelta;
		A[1][0]=df2_dtau;
		A[1][1]=df2_ddelta;

		MatInv_2(A,B);
		tau -= B[0][0]*f1+B[0][1]*f2;
		delta -= B[1][0]*f1+B[1][1]*f2;

		
        if (fabs(f1)>fabs(f2))
            worst_error=fabs(f1);
        else
            worst_error=fabs(f2);

		iter+=1;
		if (iter>100)
		{
			throw SolutionError(format("Tsp did not converge with inputs p=%g s=%g for fluid %s",p,s,(char*)name.c_str()));
			*Tout = _HUGE;
            *rhoout = _HUGE;
		}
    }

	*Tout = reduce.T/tau;
	*rhoout = delta*reduce.rho;
}

void Fluid::temperature_hs(double h, double s, double *Tout, double *rhoout, double *rhoLout, double *rhoVout, double *TsatLout, double *TsatVout)
{
	int iter;
	double A[2][2], B[2][2], T_guess;
	double dar_ddelta,da0_dtau,d2a0_dtau2,dar_dtau,d2ar_ddelta_dtau,d2ar_ddelta2,d2ar_dtau2,d2a0_ddelta_dtau,a0,ar,da0_ddelta;
	double f1,f2,df1_dtau,df1_ddelta,df2_ddelta,df2_dtau;
    double tau,delta,worst_error,rho_guess;
	double ssat_tol = 0.1;
	std::string errstr;

	double scrit = entropy_Trho(crit.T, crit.rho);
	double hcrit = enthalpy_Trho(crit.T, crit.rho);

	class SatFuncClass : public FuncWrapper1D
	{
	private:
		double h, s;
		Fluid * pFluid;
	public:
		double rho,pL,pV,rhoL,rhoV,hL,hV,sL,sV,Q;
		SatFuncClass(double h, double s, Fluid *pFluid){
			this->h = h;
			this->s = s;
			this->pFluid = pFluid;
		};
		double call(double T){
			// Try to find a Tsat that yields the same quality when considering enthalpy and entropy
			pFluid->saturation_T(T, false, &pL, &pV, &rhoL, &rhoV);
			hL = pFluid->enthalpy_Trho(T,rhoL);
			hV = pFluid->enthalpy_Trho(T,rhoV);
			sL = pFluid->entropy_Trho(T,rhoL);
			sV = pFluid->entropy_Trho(T,rhoV);
			double r = (this->h-hL)/(hV-hL)-(this->s-sL)/(sV-sL);
			Q = (this->s-sL)/(sV-sL);
			rho = 1/(Q/rhoV+(1-Q)/rhoL);
			return r;
		};
	} SatFunc(h, s, this);

	if (SatFunc.call(params.Ttriple)<0){throw ValueError(format("Value for h,s [%g,%g] is solid",h,s));}

	try{
		*Tout = Brent(&SatFunc,params.Ttriple,reduce.T-1e-5,1e-16,1e-8,100,&errstr);
		if (SatFunc.Q >1 || SatFunc.Q < 0){ throw ValueError("Solution must be within the two-phase region"); } 
		// It is two-phase, we are done, no exceptions were raised
		*rhoout = SatFunc.rho;
		*rhoLout = SatFunc.rhoL;
		*rhoVout = SatFunc.rhoV;
		*TsatLout = *Tout;
		*TsatVout = *Tout;
		return;
	}
	catch(ValueError)
	{
		double Tsat, rhosat, cp, hsat;
		// It is single-phase, either liquid, gas, or supercritical, but we don't know which yet
		try{
			if (s < scrit){ this->saturation_s(s, 0, &Tsat, &rhosat);	}
			else{ this->saturation_s(s, 1, &Tsat, &rhosat); }

			// Check the saturated enthalpy for the given value of the entropy
			hsat = this->enthalpy_Trho(Tsat, rhosat);
			cp = this->specific_heat_p_Trho(Tsat, rhosat);
			if (cp > 10) cp = 10; // Near critical point cp goes to infinity, clip it
			T_guess = Tsat + (h-hsat)/cp;
			rho_guess = rhosat;
		}
		catch(ValueError)
		{
			double cp0 = specific_heat_p_ideal_Trho(reduce.T);
			T_guess = 298;
			rho_guess = 0.001;
		}
		
	}

	

	tau = reduce.T/T_guess;
	delta = rho_guess/reduce.rho;

	//// Now we enter into a Jacobian loop that attempts to simultaneously solve 
	//// for temperature and density using Newton-Raphson
    worst_error=999;
    iter=0;
    while (worst_error>1e-6)
    {
    	// All the required partial derivatives
		a0 = phi0(tau,delta);
    	da0_dtau = dphi0_dTau(tau,delta);
    	d2a0_dtau2 = d2phi0_dTau2(tau,delta);
    	da0_ddelta = dphi0_dDelta(tau,delta);
		d2a0_ddelta_dtau = 0.0;

    	ar = phir(tau,delta);
		dar_dtau = dphir_dTau(tau,delta);
		d2ar_dtau2 = d2phir_dTau2(tau,delta);
		dar_ddelta = dphir_dDelta(tau,delta);
		d2ar_ddelta2 = d2phir_dDelta2(tau,delta);
		d2ar_ddelta_dtau = d2phir_dDelta_dTau(tau,delta);
		
		// Residual and derivatives thereof for entropy
		f1 = tau*(da0_dtau+dar_dtau)-ar-a0-s/R();	
		df1_dtau = tau*(d2a0_dtau2 + d2ar_dtau2)+(da0_dtau+dar_dtau)-dar_dtau-da0_dtau;
		df1_ddelta = tau*(d2a0_ddelta_dtau+d2ar_ddelta_dtau)-dar_ddelta-da0_ddelta;

		// Residual and derivatives thereof for enthalpy
		f2 = (1+delta*dar_ddelta)+tau*(da0_dtau+dar_dtau)-tau*h/(R()*reduce.T);
		df2_dtau = delta*d2ar_ddelta_dtau+da0_dtau+dar_dtau+tau*(d2a0_dtau2+d2ar_dtau2)-h/(R()*reduce.T);
		df2_ddelta = (dar_ddelta+delta*d2ar_ddelta2)+tau*(d2a0_ddelta_dtau+d2ar_ddelta_dtau);

		//First index is the row, second index is the column
		A[0][0]=df1_dtau;
		A[0][1]=df1_ddelta;
		A[1][0]=df2_dtau;
		A[1][1]=df2_ddelta;

		MatInv_2(A,B);
		tau -= B[0][0]*f1+B[0][1]*f2;
		delta -= B[1][0]*f1+B[1][1]*f2;
		
        if (fabs(f1)>fabs(f2))
            worst_error=fabs(f1);
        else
            worst_error=fabs(f2);

		iter+=1;
		if (iter>100)
		{
			throw SolutionError(format("Ths did not converge with inputs h=%g s=%g for fluid %s",h,s,(char*)name.c_str()));
			*Tout = _HUGE;
            *rhoout = _HUGE;
		}
    }

	*Tout = reduce.T/tau;
	*rhoout = delta*reduce.rho;
}

double Fluid::Tsat_anc(double p, double Q)
{
	// This one only uses the ancillary equations

    double Tc,Tmax,Tmin,Tmid;

    Tc=reduce.T;
    Tmax=Tc-10*DBL_EPSILON;
	Tmin=params.Ttriple+10*DBL_EPSILON;
	if (Tmin <= limits.Tmin)
		Tmin = limits.Tmin;
	
	Tmid = (Tmax+Tmin)/2.0;
	
	double tau_max = Tc/Tmin;
	double tau_min = Tc/Tmax;
    
	class SatFuncClass : public FuncWrapper1D
	{
	private:
		double p,Q,Tc;
		std::string name;
		Fluid * pFluid;
	public:
		SatFuncClass(double p_, double Q_, double Tc_, std::string name_,Fluid *_pFluid){
			p=p_;Q=Q_;Tc=Tc_,name=name_,pFluid = _pFluid;
		};
		double call(double tau){
			if (fabs(Q-1)<10*DBL_EPSILON)
				return log(pFluid->psatV_anc(Tc/tau)/p);
			else if (fabs(Q)<10*DBL_EPSILON)
				return log(pFluid->psatL_anc(Tc/tau)/p);
			else
				throw ValueError("Quality must be 1 or 0");
		};
	} SatFunc(p,Q,reduce.T,name,this);

	
	// Use Brent's method to find tau = Tc/T
	std::string errstr;
	try
	{
		double tau = Brent(&SatFunc,tau_min,tau_max,1e-10,1e-4,50,&errstr);
		if (errstr.size()>0)
			throw SolutionError("Saturation calculation failed");
		return reduce.T/tau;
	}
	catch(std::exception)
	{
		// At very low pressures the above solver will sometimes fail due to 
		// the uncertainty of the ancillary pressure equation at low temperature (pressure)
		// Here we just do a quadratic interpolation
		
		double logp_min, logp_mid, logp_max;
		double Tmid = (Tmax+Tmin)/2.0;
		double tau_mid = Tc/Tmid;

		if (fabs(Q-1)<1e-10)
		{
			// reduced pressures
			logp_min = log(psatV_anc(Tmin));
			logp_mid = log(psatV_anc(Tmid));
			logp_max = log(psatV_anc(Tmax));
		}
		else if (fabs(Q)<1e-10)
		{
			logp_min = log(psatL_anc(Tmin));
			logp_mid = log(psatL_anc(Tmid));
			logp_max = log(psatL_anc(Tmax));
		}
		else
		{
			throw ValueError(format("Quality [%g] must be either 1 or 0",Q));
		}
		
		// plotting tc/t versus log(p) tends to give very close to straight line
		// use this fact find t more easily using reverse quadratic interpolation
		double tau_interp = QuadInterp(logp_min,logp_mid,logp_max,tau_max,tau_mid,tau_min,log(p));

		//std::cout << "tsat_anc" << psatV_anc(reduce.T/tau_interp) << '\n';
		return reduce.T/tau_interp;
	}	
}


/// A wrapper class to do the calculations to get densities and saturation temperature
/// as a function of pressure for pure fluids
/// This class has been deprecated as it is in general 4 times slower than the method below that is based on the secant solver
class SaturationFunctionOfPressureResids : public FuncWrapperND
{
private:
	double arL, arV, dar_ddeltaL, dar_ddeltaV, d2ar_ddelta2L, d2ar_ddelta2V, d2ar_ddelta_dtauL, d2ar_ddelta_dtauV;
	double dar_dtauL,dar_dtauV;
	double p,R,rhoc,Tc,deltaL,deltaV,tau;
	Fluid *pFluid;
public:
	SaturationFunctionOfPressureResids(Fluid *pFluid, double p, double R, double rhoc, double Tc){
		this->pFluid = pFluid;
		this->p = p;
		this->R = R;
		this->rhoc = rhoc;
		this->Tc = Tc;
	};
	~SaturationFunctionOfPressureResids(){};
	
	void calculate_parameters(double deltaV, double deltaL, double tau)
	{	
		arL = pFluid->phir(tau,deltaL);
		arV = pFluid->phir(tau,deltaV);
		dar_ddeltaL = pFluid->dphir_dDelta(tau,deltaL);
		dar_ddeltaV = pFluid->dphir_dDelta(tau,deltaV);
		dar_dtauL = pFluid->dphir_dTau(tau,deltaL);
		dar_dtauV = pFluid->dphir_dTau(tau,deltaV);
		d2ar_ddelta2L = pFluid->d2phir_dDelta2(tau,deltaL);
		d2ar_ddelta2V = pFluid->d2phir_dDelta2(tau,deltaV);
		d2ar_ddelta_dtauL = pFluid->d2phir_dDelta_dTau(tau,deltaL);
		d2ar_ddelta_dtauV = pFluid->d2phir_dDelta_dTau(tau,deltaV);
	}
	double J(char Q)
	{
		if (Q=='L')
			return deltaL*(1+deltaL*dar_ddeltaL);
		else
			return deltaV*(1+deltaV*dar_ddeltaV);
	}
	double J_delta(char Q)
	{
		if (Q=='L')
			return 1+2*deltaL*dar_ddeltaL+deltaL*deltaL*d2ar_ddelta2L;
		else
			return 1+2*deltaV*dar_ddeltaV+deltaV*deltaV*d2ar_ddelta2V;
	}
	double J_tau(char Q)
	{
		if (Q=='L')
			return deltaL*deltaL*d2ar_ddelta_dtauL;
		else
			return deltaV*deltaV*d2ar_ddelta_dtauV;
	}
	double K(char Q)
	{
		if (Q=='L')
			return deltaL*dar_ddeltaL+arL+log(deltaL);
		else
			return deltaV*dar_ddeltaV+arV+log(deltaV);
	}
	double K_delta(char Q)
	{
		if (Q=='L')
			return 2*dar_ddeltaL+deltaL*d2ar_ddelta2L+1/deltaL;
		else
			return 2*dar_ddeltaV+deltaV*d2ar_ddelta2V+1/deltaV;
	}
	double K_tau(char Q)
	{
		if (Q=='L')
			return deltaL*d2ar_ddelta_dtauL+dar_dtauL;
		else
			return deltaV*d2ar_ddelta_dtauV+dar_dtauV;
	}

	std::vector<double> call(std::vector<double> x)
	{
		
		deltaV = x[0]; 
		deltaL = x[1];
		tau = x[2];

		// Calculate all the parameters that are needed for the derivatives
		calculate_parameters(deltaV,deltaL,tau);

		std::vector<double> out = std::vector<double>(3,0);
		out[0]=J('V')-J('L');
		out[1]=K('V')-K('L');
		out[2]=1+deltaV*dar_ddeltaV-p*tau/(R*Tc*deltaV*rhoc);
		return out;
	}
	std::vector<std::vector<double> > Jacobian(std::vector<double> x)
	{
		deltaV = x[0]; 
		deltaL = x[1];
		tau = x[2];
		std::vector<std::vector<double> > out;
		out.resize(x.size(),std::vector<double>(x.size(),0));
		
		out[0][0] = J_delta('V');
		out[0][1] = -J_delta('L');
		out[0][2] = J_tau('V')-J_tau('L');
		out[1][0] = K_delta('V');
		out[1][1] = -K_delta('L');
		out[1][2] = K_tau('V')-K_tau('L');
		out[2][0] = deltaV*d2ar_ddelta2V+dar_ddeltaV+p*tau/(R*Tc*deltaV*deltaV*rhoc);
		out[2][1] = 0;
		out[2][2] = deltaV*d2ar_ddelta_dtauV-p/(R*Tc*deltaV*rhoc);
		return out;
	}
};

class SaturationPressureGivenResids : public FuncWrapper1D
{
private:
	double p;
	Fluid *pFluid;
public:
	double rhoL, rhoV;
	SaturationPressureGivenResids(Fluid *pFluid, double p){
		this->pFluid = pFluid; 
		this->p = p;
	};
	~SaturationPressureGivenResids(){};
	
	double call(double T)
	{
		rhoL = pFluid->density_Tp(T,p,rhoL);
		rhoV = pFluid->density_Tp(T,p,rhoV);
		return pFluid->gibbs_Trho(T,rhoL)-pFluid->gibbs_Trho(T,rhoV);
	}
};

class Saturation_p_IterateSaturationT_Resids : public FuncWrapper1D
{
private:
	double p;
	Fluid *pFluid;
public:
	double rhoL, rhoV;
	Saturation_p_IterateSaturationT_Resids(Fluid *pFluid, double p){
		this->pFluid = pFluid; 
		this->p = p;
	};
	~Saturation_p_IterateSaturationT_Resids(){};
	
	double call(double T)
	{
		double psatL,psatV;
		pFluid->saturation_T(T,pFluid->enabled_TTSE_LUT,&psatL,&psatV,&rhoL,&rhoV);
		return psatL-this->p;
	}
};

void Fluid::saturation_p(double p, bool UseLUT, double *TsatL, double *TsatV, double *rhoLout, double *rhoVout)
{
	double Tsat,rhoL,rhoV;

	// Pseudo-critical pressure based on critical density and temperature
	// The highest pressure that be achieved with a temperature <= Tc
	// Some EOS, pc != p(Tc,rhoc)
	double pc_EOS = pressure_Trho(reduce.T,reduce.rho);

	if (fabs(p-reduce.p)<DBL_EPSILON || p > pc_EOS)
	{
		*TsatL = reduce.T;
		*TsatV = reduce.T;
		*rhoLout = reduce.rho;
		*rhoVout = reduce.rho;
		return;
	}

	if (debug()>5){
		std::cout<<format("%s:%d: Fluid::saturation_p(%g,%d) \n",__FILE__,__LINE__,p,UseLUT).c_str();
	}
	if (isPure==true)
	{
		if (UseLUT)
		{
			throw NotImplementedError();
		}
		else
		{
			//// Use Secant method to find T that gives the same gibbs function in both phases - a la REFPROP SATP function
			std::string errstr;
			SaturationPressureGivenResids SPGR = SaturationPressureGivenResids(this,p);
			Tsat = Tsat_anc(p,0);
			rhoL = rhosatL(Tsat);
			rhoV = rhosatV(Tsat);
			SPGR.rhoL = rhoL;
			SPGR.rhoV = rhoV;
			try{
				Tsat = Secant(&SPGR,Tsat,1e-4,1e-9,50,&errstr);
				if (errstr.size()>0 || !ValidNumber(Tsat)|| !ValidNumber(SPGR.rhoV)|| !ValidNumber(SPGR.rhoL))
					throw SolutionError("Saturation calculation failed");
				*rhoVout = SPGR.rhoV;
				*rhoLout = SPGR.rhoL;
				*TsatL = Tsat;
				*TsatV = Tsat;
				return;
			}
			catch(std::exception) // Whoops that failed...
			{
				try{
					// Now try to get Tsat by using Brent's method on saturation_T calls
					Saturation_p_IterateSaturationT_Resids SPISTR = Saturation_p_IterateSaturationT_Resids(this,p);
					Tsat = Tsat_anc(p,0);
					rhoL = rhosatL(Tsat);
					rhoV = rhosatV(Tsat);
					SPGR.rhoL = rhoL;
					SPGR.rhoV = rhoV;
					Tsat = Brent(&SPISTR,Tsat-3,reduce.T,DBL_EPSILON,1e-10,30,&errstr);
					if (errstr.size()>0 || !ValidNumber(Tsat)|| !ValidNumber(SPGR.rhoV)|| !ValidNumber(SPGR.rhoL))
						throw SolutionError("Saturation calculation failed");
					*rhoVout = SPGR.rhoV;
					*rhoLout = SPGR.rhoL;
					*TsatL = Tsat;
					*TsatV = Tsat;
					return;
				}
				catch (std::exception)
				{
					throw SolutionError("saturation_p calculation failed");
				}
			}
		}
	}
	else
	{ 
		// Pseudo-pure fluid
		*TsatL = Tsat_anc(p,0);
		*TsatV = Tsat_anc(p,1);
		*rhoLout = rhosatL(*TsatL);
		*rhoVout = rhosatV(*TsatV);
		return;
	}


	
	
	/*SaturationFunctionOfPressureResids SFPR = SaturationFunctionOfPressureResids(this,p,params.R_u/params.molemass,reduce.rho,reduce.T);
	Eigen::Vector3d x0_initial, x;
	Tsat = Tsat_anc(p,0);
	rhoL = rhosatL(Tsat);
	rhoV = rhosatV(Tsat);
	x0_initial << rhoV/reduce.rho, rhoL/reduce.rho, reduce.T/Tsat;
	std::string errstring;
	x=NDNewtonRaphson_Jacobian(&SFPR,x0_initial,1e-7,30,&errstring);
	*rhoVout = reduce.rho*x(0);
	*rhoLout = reduce.rho*x(1);
	*Tout = reduce.T/x(2);
	if (errstring.size()>0 && !ValidNumber(Tsat))
		throw SolutionError("Saturation calculation failed");
	return;*/
}

double Fluid::Tsat(double p, double Q, double T_guess)
{
	double rhoLout,rhoVout;
	return Tsat(p,Q,T_guess,false,&rhoLout, &rhoVout);
}

double Fluid::Tsat(double p, double Q, double T_guess, bool UseLUT, double *rhoLout, double *rhoVout)
{
	if (isPure && !UseLUT)
	{
		double TL,TV;
		saturation_p(p,UseLUT,&TL,&TV,rhoLout,rhoVout);
		return TL;
	}
	else
	{
		double Tc,Tmax,Tmin;

		Tc=crit.T;
		Tmax=Tc-0.001;
		Tmin=params.Ttriple+1;
		if (Tmin <= limits.Tmin)
			Tmin = limits.Tmin;
	    
		// Plotting Tc/T versus log(p) tends to give very close to straight line
		// Use this fact find T more easily
    
		class SatFuncClass : public FuncWrapper1D
		{
		private:
			double p,Q,Tc;
			std::string name;
			Fluid * pFluid;
		public:
			SatFuncClass(double p_, double Q_, double Tc_, std::string name, Fluid * pFluid){
				p=p_;Q=Q_;Tc=Tc_,this->name=name,this->pFluid = pFluid;
			};
			double call(double tau){
				if (fabs(Q)<10*DBL_EPSILON)
				{
					return log(pFluid->psatL(Tc/tau)/p);
				}
				else if (fabs(Q-1)<10*DBL_EPSILON)
				{
					return log(pFluid->psatV(Tc/tau)/p);
				}
				else
				{
					throw ValueError(format("Must be either saturated liquid or vapor"));
				}
			};
		} SatFunc(p,Q,reduce.T,name,this);

		double tau_max = Tc/Tmin;
		double tau_min = Tc/Tmax;

		// Use Brent's method to find tau = Tc/T
		std::string errstr;
		double tau = Brent(&SatFunc,tau_min,tau_max,1e-10,1e-10,50,&errstr);
		if (errstr.size()>0)
			throw SolutionError("Saturation calculation failed");
		if (!isPure)
		{
			*rhoLout = rhosatL(reduce.T/tau);
			*rhoVout = rhosatV(reduce.T/tau);
		}
		return reduce.T/tau;
	}
}

double Fluid::viscosity_dilute(double T, double e_k, double sigma)
{	
	// T in [K], e_k in [K], sigma in [nm]
	// viscosity returned is in [Pa-s]
	
	/*
	Model for the Viscosity and Thermal Conductivity of Refrigerants,
	Including a New Correlation for the Viscosity of R134a,
	Marcia L. Huber, Arno Laesecke, and Richard A. Perkins
	Ind. Eng. Chem. Res. 2003, 42, 3163-3178
	*/

	double eta_star, Tstar, OMEGA_2_2;
	Tstar = T/e_k;
	// From Neufeld, 1972, Journal of Chemical Physics - checked coefficients
	OMEGA_2_2 = 1.16145*pow(Tstar,-0.14874)+ 0.52487*exp(-0.77320*Tstar)+2.16178*exp(-2.43787*Tstar);
	// Using the leading constant from McLinden, 2000 since the leading term from Huber 2003 gives crazy values
	eta_star = 26.692e-3*sqrt(params.molemass*T)/(pow(sigma,2)*OMEGA_2_2)/1e6;
	return eta_star;
}
double Fluid::conductivity_critical(double T, double rho, double qd)
{
	double k=1.380658e-23, //[J/K]
		R0=1.03,
		gamma=1.239,
		nu=0.63,
		Pcrit = reduce.p, //[kPa]
		Tref = 1.5*reduce.T, //[K]
		GAMMA = 0.0496,
		zeta0=1.94e-10, //[m]
		cp,cv,delta,num,zeta,mu,
		OMEGA_tilde,OMEGA_tilde0,pi=M_PI,tau;

	delta = rho/reduce.rho;

	tau = reduce.T/T;
	double dp_drho=R()*T*(1+2*delta*dphir_dDelta(tau,delta)+delta*delta*d2phir_dDelta2(tau,delta));
	double X = Pcrit/pow(reduce.rho,2)*rho/dp_drho;
	tau = reduce.T/Tref;
	double dp_drho_ref=R()*Tref*(1+2*delta*dphir_dDelta(tau,delta)+delta*delta*d2phir_dDelta2(tau,delta));
	double Xref = Pcrit/pow(reduce.rho,2)*rho/dp_drho_ref*Tref/T;
	num=X-Xref;

	// no critical enhancement if numerator is negative
	if (num<0)
		return 0.0;
	else
		zeta=zeta0*pow(num/GAMMA,nu/gamma); //[m]

	cp=specific_heat_p_Trho(T,rho); //[kJ/kg/K]
	cv=specific_heat_v_Trho(T,rho); //[kJ/kg/K]
	mu=viscosity_Trho(T,rho)*1e6; //[uPa-s]

	OMEGA_tilde=2.0/pi*((cp-cv)/cp*atan(zeta*qd)+cv/cp*(zeta*qd)); //[-]
	OMEGA_tilde0=2.0/pi*(1.0-exp(-1.0/(1.0/(qd*zeta)+1.0/3.0*(zeta*qd)*(zeta*qd)/delta/delta))); //[-]

	double lambda=rho*cp*1e9*(R0*k*T)/(6*pi*mu*zeta)*(OMEGA_tilde-OMEGA_tilde0); //[W/m/K]
	return lambda/1e3; //[kW/m/K]
}
double Fluid::surface_tension_T(double T)
{
	/* Implements the method of Miqeu et al., "An extended scaled equation for the temperature dependence of
    the surface tension of pure compounds inferred from an analysis of experimental data",Fluid Phase 
	Equilibria 172 (2000) 169–182

	This correlation is used as the default, in the absence of other correlation, which can be provided for fluids if desired

	sigma in [mN/m]-->[N/m]
	*/
	double N_A = 6.02214129e23,     //[-] CODATA 2010
		k = 1.3806488e-23,          //[J/K] CODATA 2010
		w = params.accentricfactor, //[-]
		Vc,                         //[cm^3/mol]
		Tc,                         //[K]
		t,                          //[K]
		sigma;                      //[mN/m]
	Tc=reduce.T;
	t=1-T/Tc;
	//[m3/kg]*[kg/kmol]*[0.001 kmol/mol] --> [m3/mol]
	Vc = 1/reduce.rho*params.molemass/1000;
    // N_A has units of 1/mol, Vc has units of m3/mol , (1/m3)^(2/3)->1/m^2
	// k has units of J/K
	// Tc has units of K
	// k*Tc has units of J, or N-m
	sigma = k*Tc*pow(N_A/Vc,2.0/3.0)*(4.35+4.14*w)*pow(t,1.26)*(1+0.19*sqrt(t)-0.25*t);
	return sigma;
}
class ConformalTempResids : public FuncWrapperND
{
private:
	Fluid * InterestFluid, * ReferenceFluid;
	double alpha_j, Z_j;
public:
	ConformalTempResids(Fluid *InterestFluid, Fluid *ReferenceFluid, double alpha_j, double Z_j){
		this->InterestFluid = InterestFluid;
		this->ReferenceFluid = ReferenceFluid;
		this->alpha_j = alpha_j;
		this->Z_j = Z_j;
	};
	~ConformalTempResids(){};
	std::vector<double> call(std::vector<double> x)
	{
		double T0 = x[0]; double rho0 = x[1];
		double alpha_0 = DerivTerms((char *)"phir",T0,rho0,ReferenceFluid);
		double Z_0 = DerivTerms((char *)"Z",T0,rho0,ReferenceFluid);
		std::vector<double> out = std::vector<double>(2,0);
		out[0]=alpha_j-alpha_0;
		out[1]=Z_j-Z_0;
		return out;
	}
	std::vector<std::vector<double> > Jacobian(std::vector<double> x)
	{
		double T0=x[0]; double rho0=x[1];
		double dtau_dT = -ReferenceFluid->reduce.T/T0/T0;
		double ddelta_drho = 1/ReferenceFluid->reduce.rho;
		std::vector<std::vector<double> > out;
		out.resize(x.size(),std::vector<double>(x.size(),0));
		// Terms for the fluid of interest drop out
		double dalpha_dT0 = -DerivTerms((char *)"dphir_dTau",T0,rho0,ReferenceFluid)*dtau_dT;
		out[0][0] = dalpha_dT0;
		double dalpha_drho0 = -DerivTerms((char *)"dphir_dDelta",T0,rho0,ReferenceFluid)*ddelta_drho;
		out[0][1] = dalpha_drho0;
		double dZ_dT0 = -DerivTerms((char *)"dZ_dTau",T0,rho0,ReferenceFluid)*dtau_dT;
		out[1][0] = dZ_dT0;
		double dZ_drho0 = -DerivTerms((char *)"dZ_dDelta",T0,rho0,ReferenceFluid)*ddelta_drho;
		out[1][1] = dZ_drho0;

		return out;
	}
};

std::vector<double> Fluid::ConformalTemperature(Fluid *InterestFluid, Fluid *ReferenceFluid,double T, double rho, double T0, double rho0, std::string *errstring)
{
	int iter=0;
	double error,v0,v1,delta,tau,dp_drho;
	
	//The values for the fluid of interest that are the target
	double alpha_j = DerivTerms((char *)"phir",T,rho,InterestFluid);
	double Z_j = DerivTerms((char *)"Z",T,rho,InterestFluid);
	
	std::vector<double> f0,v,negative_f0;
	std::vector<std::vector<double> > J;
	ConformalTempResids CTR = ConformalTempResids(InterestFluid,ReferenceFluid,alpha_j,Z_j);
	std::vector<double> x0 = std::vector<double>(2,0);
	x0[0] = T0;
	x0[1] = rho0;
	
	// Check whether the starting guess is already pretty good
	error = root_sum_square(CTR.call(x0));

	// Make a copy so that if the calculations fail, we can return the original values
	std::vector<double> x0_initial = x0;
	
	try{
		// First naively try to just use the Newton-Raphson solver without any
		// special checking of the values.
		x0=NDNewtonRaphson_Jacobian(&CTR,x0_initial,1e-10,30,errstring);
		error = root_sum_square(CTR.call(x0));
		if (fabs(error)>1e-2 || x0[0]<0.0  || x0[1]<0.0 || !ValidNumber(x0[0]) || !ValidNumber(x0[1])){
			throw ValueError("Error calculating the conformal state for ECS");
		}
		// convert to STL vector to avoid Eigen library in header
		std::vector<double> xout(2,x0[0]);
		xout[1] = x0[1];
		return xout;
	}
	catch(std::exception){}
	// Ok, that didn't work, so we need to try something more interesting
	// Local Newton-Raphson solver with bounds checking on the step values
	error=999;
	iter=0;
	x0 = x0_initial; // Start back at unity shape factors
	while (iter<30 && fabs(error)>1e-6)
	{
		T = x0[0];
		rho = x0[1];
		tau = reduce.T/T;
		delta = rho/reduce.rho;
		dp_drho=R()*T*(1+2*delta*dphir_dDelta(tau,delta)+delta*delta*d2phir_dDelta2(tau,delta));
		
		//if (dp_drho<0)
		//{
		//	if (rho > ReferenceFluid->reduce.rho)
		//		x0(1)*=1.04;
		//	else
		//		x0(0)*=0.96;
		//}
		//Try to take a step
		f0 = CTR.call(x0);
		J = CTR.Jacobian(x0);

		// Negate f0
		negative_f0 = f0;
		for (unsigned int i = 0; i<f0.size(); i++){ negative_f0[i] *= -1;}

		v = linsolve_Gauss_Jordan(J,negative_f0);
		v0 = v[0]; 
		v1 = v[1];
		if (x0[0]-v[0]>0 && x0[1]-v[1]>0)
		{
			x0[0] -= v[0];
			x0[1] -= v[1];
		}
		else
		{
			x0[0] -= 1.05*x0[0];
			x0[1] -= 1.05*x0[1];
		}
		error = root_sum_square(f0);
		iter += 1;
		if (iter>29)
		{
			*errstring = std::string("ConformalTemperature reached maximum number of steps without reaching solution");
			return x0_initial;
		}
	}
	// convert to STL vector
	std::vector<double> xout(2,x0[0]);
	xout[1] = x0[1];
	return xout;
};

double Fluid::viscosity_ECS_Trho(double T, double rho, Fluid * ReferenceFluid)
{
	/*
	Implements the method of
	Marcia L. Huber, Arno Laesecke, and Richard A. Perkins
	"Model for the Viscosity and Thermal Conductivity of Refrigerants,
	Including a New Correlation for the Viscosity of R134a"
	Ind. Eng. Chem. Res. 2003, 42, 3163-3178
	*/
	double e_k,sigma,e0_k, sigma0, Tc0,rhoc0,T0,rho0,rhoc,Tc,
		eta_dilute,theta,phi,f,h,eta_resid,M,M0,F_eta,eta,psi,
		rhoc0bar,rhobar,rhocbar,rho0bar;
	std::vector<double> x0;

	Tc0=ReferenceFluid->reduce.T;
	rhoc0=ReferenceFluid->reduce.rho;
	M0=ReferenceFluid->params.molemass;
	rhoc0bar = rhoc0/M0;
	Tc = reduce.T;
	rhoc = reduce.rho;
	M = params.molemass;
	rhocbar=rhoc/M;
	rhobar=rho/M;
	
	try{
		// Get the ECS parameters from the reference fluid
		ReferenceFluid->ECSParams(&e0_k,&sigma0);
	}
	catch (const NotImplementedError &){
		// Doesn't have e_k and sigma for reference fluid
		throw NotImplementedError(format("Your reference fluid for ECS [%s] does not have an implementation of ECSParams",(char *)ReferenceFluid->get_name().c_str()));
	}

	try{
		// Get the ECS params for the fluid if it has them
		ECSParams(&e_k,&sigma);
	}
	catch (const NotImplementedError &){ 
		//Estimate the ECS parameters from Huber and Ely, 2003
		e_k = e0_k*Tc/Tc0;
		sigma = sigma0*pow(rhoc/rhoc0,1.0/3.0);
	}

	// The dilute portion is for the fluid of interest, not for the reference fluid
	// It is the viscosity in the limit of zero density
	eta_dilute = viscosity_dilute(T,e_k,sigma);

	if (1)//(T>reduce.T)
	{
		// Get the conformal temperature.  To start out here, assume that the shape factors are unity
		theta=1;
		phi=1;
	}
	else
	{
		/* Use the method from
		Isabel M. Marruchoa, James F. Ely, "Extended corresponding states for pure polar
		and non-polar fluids: an improved method for component shape factor prediction",
		Fluid Phase Equilibria 150–151 1998 215–223

		Reynes and Thodos,
		APPLICATION OF A REDUCED VAPOR
		PRESSURE EQUATION TO
		NONHYDROROCARBON SUBSTANCES
		beta = 5/9*gamma-40/27
		gamma = 9/5*beta + 9/5*40/27
		gamma = 9/5*beta + 8/3
		*/
		double Bstar,Bstar0,Cstar,Cstar0,DELTABstar,DELTACstar,Zc,Zc0;
		double omega = params.accentricfactor;
		double omega0 = ReferenceFluid->params.accentricfactor;
		double Tstar = T/reduce.T;
		Zc = reduce.p/(reduce.rho*R()*reduce.T);
		Zc0 = ReferenceFluid->reduce.p/(ReferenceFluid->reduce.rho*ReferenceFluid->R()*ReferenceFluid->reduce.T);
		Bstar = -6.207612-15.37641*omega-0.574946*pow(10,-omega);
		Bstar0 = -6.207612-15.37641*omega0-0.574946*pow(10,-omega0);
		Cstar = 8/3+9*Bstar/5.0/log(10.0);
		Cstar0 = 8/3+9*Bstar0/5.0/log(10.0);
		DELTABstar = Bstar-Bstar0;
		DELTACstar = Cstar-Cstar0;
		theta = (1-Cstar0+2*pow(1-Tstar,2.0/7.0)*log(Zc/Zc0)-DELTABstar+DELTACstar*log(Tstar)+Bstar/Tstar)/(1-Cstar0+Bstar0/Tstar);
		phi = pow(Zc,pow(1-Tstar,2.0/7.0))/pow(Zc0,pow(1-Tstar/theta,2.0/7.0));
	}

	psi = ECS_psi_viscosity(rho/reduce.rho);

	f=Tc/Tc0*theta;
	//Must be the ratio of MOLAR densities!!
	h=rhoc0bar/rhocbar*phi;
	T0=T/f;
	rho0bar=rhobar*h;
	//Get the mass density for the reference fluid [kg/m3]
	rho0=rho0bar*M0;
	std::string errstring;
	// First check whether you should use this code in the first place.
	// Implementing the method of TRNECS from REFPROP
	double delta = rho/reduce.rho;
	double tau = reduce.T/T;
	double Z = 1+delta*dphir_dDelta(tau,delta);
	double p0 = Z*R()*T0*rho0;
	if (Z<0.3 || p0>1.1*ReferenceFluid->reduce.p || rho0>ReferenceFluid->reduce.rho){
		// Use the code to calculate the conformal state
		x0=ConformalTemperature(this,ReferenceFluid,T,rho,T0,rho0,&errstring);
		T0=x0[0];
		rho0=x0[1];
	}
	rho0bar = rho0/M0;
	h = rho0bar/rhobar;
	f = T/T0;

	eta_resid = ReferenceFluid->viscosity_background(T0,rho0*psi);
	F_eta = sqrt(f)*pow(h,-2.0/3.0)*sqrt(M/M0);
	eta = eta_dilute+eta_resid*F_eta;
	return eta;
}

double Fluid::conductivity_ECS_Trho(double T, double rho, Fluid * ReferenceFluid)
{
	/*
	Implements the method of
	Marcia L. Huber, Arno Laesecke, and Richard A. Perkins
	"Model for the Viscosity and Thermal Conductivity of Refrigerants,
	Including a New Correlation for the Viscosity of R134a"
	Ind. Eng. Chem. Res. 2003, 42, 3163-3178
	*/
	double e_k,sigma,e0_k, sigma0, Tc0,rhoc0,T0,rho0,rhoc,Tc,
		eta_dilute,theta,phi,f,h,lambda_resid,M,M0,F_lambda,lambda,chi,
		f_int,lambda_int,lambda_crit,lambda_star,rhoc0bar,rhobar,rhocbar,rho0bar;
	std::vector<double> x0;

	// Properties for the reference fluid
	Tc0=ReferenceFluid->reduce.T;
	rhoc0=ReferenceFluid->reduce.rho;
	M0=ReferenceFluid->params.molemass;
	rhoc0bar = rhoc0/M0;

	// Properties for the given fluid
	Tc = reduce.T;
	rhoc = reduce.rho;
	M = params.molemass;
	rhocbar=rhoc/M;
	rhobar=rho/M;
	try{
		//Estimate the ECS parameters from Huber and Ely, 2003
		ReferenceFluid->ECSParams(&e0_k,&sigma0);
	}
	catch (NotImplementedError &){
		// Doesn't have e_k and sigma for reference fluid
		throw NotImplementedError(format("Your reference fluid for ECS [%s] does not have an implementation of ECSParams",(char *)ReferenceFluid->get_name().c_str()));
	}
		
	try{
		// Get the ECS params for the fluid if it has them
		ECSParams(&e_k,&sigma);
	}
	catch(NotImplementedError &){
		e_k = e0_k*Tc/Tc0;
		sigma = sigma0*pow(rhoc/rhoc0,1.0/3.0);
	}

	// The dilute portion is for the fluid of interest, not for the reference fluid
	// It is the viscosity in the limit of zero density
	// It has units of Pa-s here
	eta_dilute = viscosity_dilute(T,e_k,sigma);
	
	chi = ECS_chi_conductivity(rho/reduce.rho);
	f_int = ECS_f_int(T);

	// Get the conformal temperature.  To start out here, assume that the shape factors are unity
	theta=1.0;
	phi=1.0;
	f=Tc/Tc0*theta;
	h=rhoc0bar/rhocbar*phi;
	T0=T/f;
	rho0bar=rhobar*h;
	//Get the mass density for the reference fluid [kg/m3]
	rho0=rho0bar*M0;
	
	std::string errstring;
	// First check whether you should use this code in the first place.
	// Implementing the method of TRNECS from REFPROP
	double delta = rho/reduce.rho;
	double tau = reduce.T/T;
	double Z = 1+delta*dphir_dDelta(tau,delta);
	double p0 = Z*R()*T0*rho0;
	if (Z<0.3 || p0 > 1.1*ReferenceFluid->reduce.p || rho0 > ReferenceFluid->reduce.rho){
		// Use the code to calculate the conformal state
		x0=ConformalTemperature(this,ReferenceFluid,T,rho,T0,rho0,&errstring);
		T0=x0[0];
		rho0=x0[1];
	}
	
	rho0bar = rho0/M0;
	h=rho0bar/rhobar;
	f=T/T0;
	
	// Ideal-gas specific heat in the limit of zero density
	double cpstar = specific_heat_p_ideal_Trho(T);
	lambda_star = 15e-3*R()*(eta_dilute*1e6)/4.0; //[W/m/K]
	lambda_int = f_int*(eta_dilute*1e6)*(cpstar-5.0/2.0*R() ); //[W/m/K]
	F_lambda = sqrt(f)*pow(h,-2.0/3.0)*sqrt(M0/M);
	
	//Get the background viscosity from the reference fluid
	lambda_resid = ReferenceFluid->conductivity_background(T0,rho0*chi)*1e3 ;//[W/m/K]
	lambda_crit = conductivity_critical(T,rho);
	lambda = lambda_int+lambda_star+lambda_resid*F_lambda+lambda_crit;
	return lambda/1e3; //[kW/m/K]
}

bool Fluid::build_TTSE_LUT(bool force_build)
{
	if (!built_TTSE_LUT || force_build)
	{
		// No value has been set for the parameters
		if (pmin_TTSE>1e100 && pmax_TTSE > 1e100 && hmin_TTSE > 1e00 && hmax_TTSE > 1e100)
		{
			double psatL,psatV,rhoL,rhoV;
			
			this->disable_TTSE_LUT(); // Disable TTSE to do these calculations
			saturation_T(limits.Tmin,false,&psatL,&psatV,&rhoL,&rhoV);// <<----------------- This is the problem with EES!
			double hL = enthalpy_Trho(limits.Tmin,rhoL);
			double hV = enthalpy_Trho(limits.Tmin,rhoV);
			hmin_TTSE = hL;
			hmax_TTSE = hL+(hV-hL)*2;
			pmin_TTSE = params.ptriple;
			if (pmin_TTSE<psatL)
			{
				pmin_TTSE = psatL;
			}
			pmax_TTSE = 2*reduce.p;
		}

		// Turn off the use of LUT while you are building it, 
		// otherwise you get an infinite recursion
		enabled_TTSE_LUT = false;

		TTSESatL = TTSETwoPhaseTableClass(this,0);
		// Deference the pointer to the table
		TTSESatL.set_size(Nsat_TTSE);

		TTSESatV = TTSETwoPhaseTableClass(this,1);
		TTSESatV.set_size(Nsat_TTSE);
		TTSESatV.build(pmin_TTSE,crit.p-1e-15,&TTSESatL);

		TTSESinglePhase = TTSESinglePhaseTableClass(this);
		TTSESinglePhase.enable_writing_tables_to_files = enable_writing_tables_to_files;
		TTSESinglePhase.set_size_ph(Np_TTSE,Nh_TTSE);
		// T,rho will mirror the size and range of h,p
		TTSESinglePhase.set_size_Trho(Np_TTSE, Nh_TTSE);
		TTSESinglePhase.hmin = hmin_TTSE;
		TTSESinglePhase.hmax = hmax_TTSE;
		TTSESinglePhase.pmin = pmin_TTSE;
		TTSESinglePhase.pmax = pmax_TTSE;
		TTSESinglePhase.SatL = &TTSESatL;
		TTSESinglePhase.SatV = &TTSESatV;

		// If we can read the LUT from file, we are done and don't need to rebuild
		if (!TTSESinglePhase.read_all_from_file(TTSESinglePhase.root_path))
		{
			// Build all the tables
			TTSESinglePhase.build_ph(hmin_TTSE, hmax_TTSE, pmin_TTSE, pmax_TTSE, &TTSESatL, &TTSESatV);
			TTSESinglePhase.build_Trho(-1, -1, -1, -1, &TTSESatL, &TTSESatV);// Allow method to figure out the range using h,p since -1 passed for T and rho limits

			// Write all the matrices and arrays to files
			if (TTSESinglePhase.enable_writing_tables_to_files){
				TTSESinglePhase.write_all_to_file(TTSESinglePhase.root_path);
			}
		}

		built_TTSE_LUT = true;
		enabled_TTSE_LUT = true;
	}
	return true;
}

/// Enable the TTSE
/// If you want to over-ride parameters, must be done before calling this function
void Fluid::enable_TTSE_LUT(void){enabled_TTSE_LUT = true;};
/// Check if TTSE is enabled
bool Fluid::isenabled_TTSE_LUT(void){return enabled_TTSE_LUT;};
/// Disable the TTSE
void Fluid::disable_TTSE_LUT(void){enabled_TTSE_LUT = false;};
/// Enable the writing of TTSE tables to file
void Fluid::enable_TTSE_LUT_writing(void){enable_writing_tables_to_files = true;};
/// Check if the writing of TTSE tables to file is enabled
bool Fluid::isenabled_TTSE_LUT_writing(void){return enable_writing_tables_to_files;};
/// Disable the writing of TTSE tables to file
void Fluid::disable_TTSE_LUT_writing(void){enable_writing_tables_to_files = false;};
/// Over-ride the default size of both of the saturation LUT
void Fluid::set_TTSESat_LUT_size(int Nsat){Nsat_TTSE = Nsat;};
/// Over-ride the default size of the single-phase LUT
void Fluid::set_TTSESinglePhase_LUT_size(int Np, int Nh){Np_TTSE = Np; Nh_TTSE = Nh;};
/// Over-ride the default range of the single-phase LUT
void Fluid::set_TTSESinglePhase_LUT_range(double hmin, double hmax, double pmin, double pmax){hmin_TTSE = hmin; hmax_TTSE = hmax; pmin_TTSE = pmin; pmax_TTSE = pmax;};
/// Get the current range of the single-phase LUT
void Fluid::get_TTSESinglePhase_LUT_range(double *hmin, double *hmax, double *pmin, double *pmax){*hmin = hmin_TTSE; *hmax = hmax_TTSE; *pmin = pmin_TTSE; *pmax = pmax_TTSE;};

void AncillaryCurveClass::update(Fluid *_pFluid, std::string Output)
{
	pFluid = _pFluid;
	iOutput = get_param_index(Output);
}
int AncillaryCurveClass::build(int N)
{
	double T,rhoV,rhoL;

	// Output values
	long iFluid = get_Fluid_index(pFluid->get_name());

	double Tmin	= pFluid->params.Ttriple+1e-6;
	if (Tmin<pFluid->limits.Tmin)
		Tmin = pFluid->limits.Tmin+1e-6;
	double Tmax	= pFluid->reduce.T-10*DBL_EPSILON;
	for(int i = 0; i<N; i++)
	{
		T = Tmin+(Tmax-Tmin)/(N-1)*i;
		xL.push_back(T);
		xV.push_back(T);
		rhoL = pFluid->rhosatL(T);
		rhoV = pFluid->rhosatV(T);
		yL.push_back(IProps(iOutput,iT,T,iD,rhoL,iFluid));
		yV.push_back(IProps(iOutput,iT,T,iD,rhoV,iFluid));
	}
	built = true;
	return 1;
}

double AncillaryCurveClass::interpolateL(double T){
	return interp1d(&xL,&yL,T);
}

double AncillaryCurveClass::interpolateV(double T){
	return interp1d(&xV,&yV,T);
}

double AncillaryCurveClass::reverseinterpolateL(double y){
	return interp1d(&yL,&xL,y);
}

double AncillaryCurveClass::reverseinterpolateV(double y){
	return interp1d(&yV,&xV,y);
}

#if defined(_MSC_VER) || defined(__powerpc__)
// Microsoft version of math.h doesn't include acosh or asinh, so we just define them here
// Neither does the PPC version
static double acosh(double x)
{
 	return log(x + sqrt(x*x - 1.0) );
}
static double asinh(double value)
{
	if(value>0){
		return log(value + sqrt(value * value + 1));
	}
	else{
		return -log(-value + sqrt(value * value + 1));
	}
}
#endif

double CriticalSplineStruct_T::interpolate_rho(Fluid *pFluid, int phase, double T)
{
	// Use the spline interpolation since you are very close to the critical point
	// R = rho/rhoc-1 => we are solving for R for liquid and vapor using a spline
	// since we know that dtaudR|crit = 0, tau|crit = 1, R|crit = 0, and we know 
	// rho and drhodT_sat at Tend, the end of the normal Akasaka solving method
	//
	// Essentially you have tau = aR^3+bR^2+cR+d, where c = 0 and d = 1 from constraints 
	// at critical point
	// Can check cubic solution from http://www.akiti.ca/Quad3Deg.html
	// Also, wikipedia has good docs on cubics: http://en.wikipedia.org/wiki/Cubic_function, especially the "Trigonometric (and hyperbolic) method" section

	double rhoc = pFluid->reduce.rho;
	double Tc = pFluid->reduce.T;
	double tauend = Tc/Tend, tau = Tc/T;

	double k1,k2,R,Rend,a,b,c,d,rhoend;
	// If you are within a microKelvin of critical point, return critical point
	if (fabs(T-Tc)<1e-6)
	{
		return rhoc;
	}
	if (phase == 0)
	{
		k1 = -tauend/Tend*rhoc/drhoLdT_sat; // k1 = dtaudR|sat at Tend
		k2 = tauend;
		Rend = rhoendL/rhoc-1; // R at Tend
		rhoend = rhoendL;
	}
	else if (phase == 1)
	{
		k1 = -tauend/Tend*rhoc/drhoVdT_sat; // k1 = dtaudR|sat at Tend
		k2 = tauend;
		Rend = rhoendV/rhoc-1; // R at Tend
		rhoend = rhoendV;
	}
	else
	{
		throw ValueError();
	}
	
	// Linear system to find constants a,b for the spline cubic
	/*
	k2 = aR^3+bR^2+1
	k1 = 3aR^2+2bR
	*/
	double a11 = Rend*Rend*Rend;
	double a12 = Rend*Rend;
	double b1 = k2-1;
	double a21 = 3*Rend*Rend;
	double a22 = 2*Rend;
	double b2 = k1;

	// Cramer's rule to find a,b
	double det = a11*a22-a21*a12;
	a = (b1*a22-a12*b2)/det;
	b = (b2*a11-a21*b1)/det;

	// Constants from critical point
	c = 0;
	d = 1-tau;

	// Discriminant
	double DELTA = 18*a*b*c*d-4*b*b*b*d+b*b*c*c-4*a*c*c*c-27*a*a*d*d;

	// Coefficients for the depressed cubic t^3+p*t+q = 0
	double p = (3*a*c-b*b)/(3*a*a);
	double q = (2*b*b*b-9*a*b*c+27*a*a*d)/(27*a*a*a);

	if (DELTA<0)
	{
		// One real root
		double t0;
		if (4*p*p*p+27*q*q>0 && p<0)
		{
			t0 = -2.0*fabs(q)/q*sqrt(-p/3.0)*cosh(1.0/3.0*acosh(-3.0*fabs(q)/(2.0*p)*sqrt(-3.0/p)));
		}
		else
		{
			t0 = -2.0*sqrt(p/3.0)*sinh(1.0/3.0*asinh(3.0*q/(2.0*p)*sqrt(3.0/p)));
		}
		R = t0-b/(3*a);
	}
	else //(DELTA>0)
	{
		// Three real roots
		double t0 = 2.0*sqrt(-p/3.0)*cos(1.0/3.0*acos(3.0*q/(2.0*p)*sqrt(-3.0/p))-0*2.0*M_PI/3.0);
		double t1 = 2.0*sqrt(-p/3.0)*cos(1.0/3.0*acos(3.0*q/(2.0*p)*sqrt(-3.0/p))-1*2.0*M_PI/3.0);
		double t2 = 2.0*sqrt(-p/3.0)*cos(1.0/3.0*acos(3.0*q/(2.0*p)*sqrt(-3.0/p))-2*2.0*M_PI/3.0);

		double R0 = t0-b/(3*a);
		double R1 = t1-b/(3*a);
		double R2 = t2-b/(3*a);

		// The solution for R must be bounded between Rend and 0
		if (R0*Rend > 0 && fabs(R0)<=fabs(Rend))
		{
			R = R0;
		}
		else if (R1*Rend > 0 && fabs(R1)<=fabs(Rend))
		{
			R = R1;
		}
		else if (R2*Rend > 0 && fabs(R2)<=fabs(Rend))
		{
			R = R2;
		}
		else
		{
			throw ValueError(format("No solution found for R"));
		}
	}

	return rhoc*(1+R);
}

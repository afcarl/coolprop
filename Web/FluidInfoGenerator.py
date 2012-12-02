import CoolProp.CoolProp as CP
import CoolProp
import textwrap
import os

def PropertyConsistency(Fluid):
    
    return textwrap.dedent(
    """
    Check of p,h and p,s as inputs (X: Failure .: Success)
    =================================================================
    .. plot::
    
        from CoolProp.Plots.Plots import Ph,Ps
        from CoolProp.CoolProp import Props
        from matplotlib import pyplot as plt
        import numpy as np

        Ref = "{Fluid:s}"
        fig = plt.figure(figsize=(10,5))
        ax1 = fig.add_subplot(121)
        ax2 = fig.add_subplot(122)

        Tmin = Props(Ref,'Tmin')+3
        pmin = Props('P','T',Tmin,'Q',0,Ref)
        pmax = Props(Ref,'pcrit')*2
        hmin = Props('H','T',Tmin,'Q',0,Ref)
        hmax = 2*Props('H','T',Props(Ref,'Tcrit')-1,'Q',1,Ref)-hmin
        smin = Props('S','T',Tmin,'Q',0,Ref)
        smax = 2*Props('S','T',Props(Ref,'Tcrit')-1,'Q',1,Ref)-smin
            
        Ph(Ref, axis = ax1, Tmin = Tmin)
        Ps(Ref, axis = ax2, Tmin = Tmin)

        for p in np.linspace(pmin,pmax,10):
            for h in np.linspace(hmin,hmax):
                _bad = False
                try:
                    T = Props('T','H',h,'P',p,Ref)
                    rho = Props('D','H',h,'P',p,Ref)
                    hEOS = Props('H','T',T,'D',rho,Ref)
                except ValueError:
                    _bad = True
                if _bad or abs(hEOS/h-1)>1e-6:
                    ax1.plot(h,p,'x',ms = 10)
                else:
                    ax1.plot(h,p,'k.', ms = 1)
                    
        for p in np.linspace(pmin,pmax,10):
            for s in np.linspace(smin,smax):
                _bad = False
                try:
                    T = Props('T','S',s,'P',p,Ref)
                    rho = Props('D','S',s,'P',p,Ref)
                    sEOS = Props('S','T',T,'D',rho,Ref)
                except ValueError:
                    _bad = True
                if _bad or abs(sEOS/s-1)>1e-6:
                    ax2.plot(s,p,'x',ms = 10)
                else:
                    ax2.plot(s,p,'k.', ms = 1)
                    
        plt.tight_layout()
    """.format(Fluid=Fluid))

def CriticalIsotherm(Fluid):
    
    return textwrap.dedent(
    """
    Along the critical isotherm where T=T\ :sub:`c`
    ================================================
    .. plot::
    
        Fluid = "{Fluid:s}"
        RPFluid = "{RPFluid:s}"
        
        #Critical isotherm
        from CoolProp.CoolProp import Props
        from numpy import linspace,array,abs
        import matplotlib.pyplot as plt

        Tc = Props(Fluid,'Tcrit')
        rhoc = Props(Fluid,'rhocrit')
        rhov = linspace(1e-12,2*rhoc)

        #All the CoolProp calculations
        p = array([Props('P','T',Tc,'D',D,Fluid) for D in rhov])
        rho = array([Props('D','T',Tc,'D',D,Fluid) for D in rhov])
        cp = array([Props('C','T',Tc,'D',D,Fluid) for D in rhov])
        cv = array([Props('O','T',Tc,'D',D,Fluid) for D in rhov])
        h0 = Props('H','T',0.95*Tc,'Q',1,Fluid)
        h = array([Props('H','T',Tc,'D',D,Fluid)-h0 for D in rhov])
        s0 = Props('S','T',0.95*Tc,'Q',1,Fluid)
        s = array([Props('S','T',Tc,'D',D,Fluid)-s0 for D in rhov])
        visc = array([Props('V','T',Tc,'D',D,Fluid) for D in rhov])
            
        Rp = array([Props('P','T',Tc,'D',D,RPFluid) for D in rhov])
        Rrho = array([Props('D','T',Tc,'D',D,RPFluid) for D in rhov])
        Rcp = array([Props('C','T',Tc,'D',D,RPFluid) for D in rhov])
        Rcv = array([Props('O','T',Tc,'D',D,RPFluid) for D in rhov])
        Rh0 = Props('H','T',0.95*Tc,'Q',1,RPFluid)
        Rh = array([Props('H','T',Tc,'D',D,RPFluid)-Rh0 for D in rhov])
        Rs0 = Props('S','T',0.95*Tc,'Q',1,RPFluid)
        Rs = array([Props('S','T',Tc,'D',D,RPFluid)-Rs0 for D in rhov])
        Rvisc = array([Props('V','T',Tc,'D',D,RPFluid) for D in rhov])
            
        fig = plt.figure()

        ax = fig.add_axes((0.15,0.15,0.8,0.8))
        ax.semilogy(rhov/rhoc,abs(p/Rp-1)*100,'o',label='Pressure')
        ax.semilogy(rhov/rhoc,abs(cp/Rcp-1)*100,'o',label='Specific heat (cp)')
        ax.semilogy(rhov/rhoc,abs(cv/Rcv-1)*100,'o',label='Specific heat (cv)')
        ax.semilogy(rhov/rhoc,abs(h/Rh-1)*100,'o',label='Enthalpy')
        ax.semilogy(rhov/rhoc,abs(s/Rs-1)*100,'o',label='Entropy') 
        ax.semilogy(rhov/rhoc,abs(visc/Rvisc-1)*100,'o',label='Viscosity')
        ax.set_ylim(1e-16,100)
        ax.set_title('Critical isotherm Deviations from REFPROP 9.0')
        ax.set_xlabel(r'Reduced density $\\rho/\\rho_c$')
        ax.set_ylabel('Absolute deviation [%]')
        ax.legend(numpoints=1,loc='best')
        plt.show()
    """.format(Fluid=Fluid,RPFluid = 'REFPROP-'+CP.get_REFPROPname(Fluid)))

def SatVaporParity(Fluid):
    
    return textwrap.dedent(
    """
    Saturated Vapor Deviations
    ==========================
    
    .. plot::
    
        Fluid = "{Fluid:s}"
        RPFluid = "{RPFluid:s}"
        
        #Saturated Vapor
        from CoolProp.CoolProp import Props
        from numpy import linspace,array,abs
        import matplotlib.pyplot as plt

        Tt = Props(Fluid,'Ttriple')
        Tc = Props(Fluid,'Tcrit')
        Tv = linspace(Tt+0.01,0.95*Tc,20)

        #All the CoolProp calculations
        p = array([Props('P','T',T,'Q',1,Fluid) for T in Tv])
        rho = array([Props('D','T',T,'Q',1,Fluid) for T in Tv])
        cp = array([Props('C','T',T,'Q',1,Fluid) for T in Tv])
        cv = array([Props('O','T',T,'Q',1,Fluid) for T in Tv])
        h0 = Props('H','T',(Tt+Tc)/2.0,'Q',1,Fluid)
        h = array([Props('H','T',T,'Q',1,Fluid)-h0 for T in Tv])
        s0 = Props('S','T',(Tt+Tc)/2.0,'Q',1,Fluid)
        s = array([Props('S','T',T,'Q',1,Fluid)-s0 for T in Tv])   
        sigma = array([Props('I','T',T,'Q',1,Fluid) for T in Tv])
            
        Rp = array([Props('P','T',T,'Q',1,RPFluid) for T in Tv])
        Rrho = array([Props('D','T',T,'Q',1,RPFluid) for T in Tv])
        Rcp = array([Props('C','T',T,'Q',1,RPFluid) for T in Tv])
        Rcv = array([Props('O','T',T,'Q',1,RPFluid) for T in Tv])
        Rh0 = Props('H','T',(Tt+Tc)/2.0,'Q',1,RPFluid)
        Rh = array([Props('H','T',T,'Q',1,RPFluid)-Rh0 for T in Tv])
        Rs0 = Props('S','T',(Tt+Tc)/2.0,'Q',1,RPFluid)
        Rs = array([Props('S','T',T,'Q',1,RPFluid)-Rs0 for T in Tv])
        Rsigma = array([Props('I','T',T,'Q',1,RPFluid) for T in Tv])
            
        fig = plt.figure()

        ax = fig.add_axes((0.15,0.15,0.8,0.8))
        ax.semilogy(Tv/Tc,abs(p/Rp-1)*100,'o',label='Pressure')
        ax.semilogy(Tv/Tc,abs(rho/Rrho-1)*100,'o',label='Density')
        ax.semilogy(Tv/Tc,abs(cp/Rcp-1)*100,'o',label='Specific heat (cp)')
        ax.semilogy(Tv/Tc,abs(cv/Rcv-1)*100,'o',label='Specific heat (cv)')
        ax.semilogy(Tv/Tc,abs(h/Rh-1)*100,'o',label='Enthalpy')
        ax.semilogy(Tv/Tc,abs(s/Rs-1)*100,'o',label='Entropy')  
        ax.semilogy(Tv/Tc,abs(sigma/Rsigma-1)*100,'o',label='Surface tension')
        ax.set_ylim(1e-16,100)
        ax.set_title('Saturated Vapor Deviations from REFPROP 9.0')
        ax.set_xlabel('Reduced temperature T/Tc')
        ax.set_ylabel('Absolute deviation [%]')
        ax.legend(numpoints=1,loc='best')
        plt.show()
    """.format(Fluid=Fluid,RPFluid = 'REFPROP-'+CP.get_REFPROPname(Fluid)))

def SatLiquidParity(Fluid):
    
    return textwrap.dedent(
    """
    Saturated Liquid Deviations
    ===========================
    
    .. plot::
    
        Fluid = "{Fluid:s}"
        RPFluid = "{RPFluid:s}"
        
        #Saturated Liquid
        from CoolProp.CoolProp import Props
        from numpy import linspace,array,abs
        import matplotlib.pyplot as plt

        Tt = Props(Fluid,'Ttriple')
        Tc = Props(Fluid,'Tcrit')
        Tv = linspace(Tt+0.01,0.95*Tc,20)

        #All the CoolProp calculations
        p = array([Props('P','T',T,'Q',0,Fluid) for T in Tv])
        rho = array([Props('D','T',T,'Q',0,Fluid) for T in Tv])
        cp = array([Props('C','T',T,'Q',0,Fluid) for T in Tv])
        cv = array([Props('O','T',T,'Q',0,Fluid) for T in Tv])
        h0 = Props('H','T',(Tt+Tc)/2.0,'Q',0,Fluid)
        h = array([Props('H','T',T,'Q',0,Fluid)-h0 for T in Tv])
        s0 = Props('S','T',(Tt+Tc)/2.0,'Q',0,Fluid)
        s = array([Props('S','T',T,'Q',0,Fluid)-s0 for T in Tv])    
        sigma = array([Props('I','T',T,'Q',0,Fluid) for T in Tv])
            
        Rp = array([Props('P','T',T,'Q',0,RPFluid) for T in Tv])
        Rrho = array([Props('D','T',T,'Q',0,RPFluid) for T in Tv])
        Rcp = array([Props('C','T',T,'Q',0,RPFluid) for T in Tv])
        Rcv = array([Props('O','T',T,'Q',0,RPFluid) for T in Tv])
        Rh0 = Props('H','T',(Tt+Tc)/2.0,'Q',0,RPFluid)
        Rh = array([Props('H','T',T,'Q',0,RPFluid)-Rh0 for T in Tv])
        Rs0 = Props('S','T',(Tt+Tc)/2.0,'Q',0,RPFluid)
        Rs = array([Props('S','T',T,'Q',0,RPFluid)-Rs0 for T in Tv])
        Rsigma = array([Props('I','T',T,'Q',0,RPFluid) for T in Tv])
            
        fig = plt.figure()

        ax = fig.add_axes((0.15,0.15,0.8,0.8))
        ax.semilogy(Tv/Tc,abs(p/Rp-1)*100,'o',label='Pressure')
        ax.semilogy(Tv/Tc,abs(rho/Rrho-1)*100,'o',label='Density')
        ax.semilogy(Tv/Tc,abs(cp/Rcp-1)*100,'o',label='Specific heat (cp)')
        ax.semilogy(Tv/Tc,abs(cv/Rcv-1)*100,'o',label='Specific heat (cv)')
        ax.semilogy(Tv/Tc,abs(h/Rh-1)*100,'o',label='Enthalpy')
        ax.semilogy(Tv/Tc,abs(s/Rs-1)*100,'o',label='Entropy')
        ax.semilogy(Tv/Tc,abs(sigma/Rsigma-1)*100,'o',label='Surface tension')
        ax.set_ylim(1e-16,100)
        ax.set_title('Saturated Liquid Deviations from REFPROP 9.0')
        ax.set_xlabel('Reduced temperature T/Tc')
        ax.set_ylabel('Absolute deviation [%]')
        ax.legend(numpoints=1,loc='best')
        plt.show()
    """.format(Fluid=Fluid,RPFluid = 'REFPROP-'+CP.get_REFPROPname(Fluid)))
    
def params_table(Fluid):
    params = dict(mm = CP.Props(Fluid,'molemass'),
                  Tt = CP.Props(Fluid,'Ttriple'),
                  pc = CP.Props(Fluid,'pcrit'),
                  )
    
    return textwrap.dedent(
    """
    Fluid Data
    ==========
        
    Fluid Parameters
    
    =========================  ==============================
    Mole Mass [kg/kmol]        {mm:0.5f}
    Triple Point [K]           {Tt:0.3f}
    =========================  ==============================
    """.format(**params))
    
def critical_table(Fluid):
    params = dict(Tc = CP.Props(Fluid,'Tcrit'),
                  rhoc = CP.Props(Fluid,'rhocrit'),
                  pc = CP.Props(Fluid,'pcrit'),
                  )
    
    return textwrap.dedent(
    """
    Critical Parameters
    
    ==============================  ==============================
    Temperature [K]                 {Tc:0.2f}
    Density [kg/m\ :sup:`3`\ ]      {rhoc:0.6f}
    Pressure [kPa]                  {pc:0.5f}
    ==============================  ==============================
    
    """.format(**params))

def buildrst(Fluid):
    pass
    
def index_file(Fluids):
    Fluids=[Fluid+'.rst' for Fluid in Fluids]
    FluidList = '\n    '.join(Fluids)
    return textwrap.dedent( 
"""
.. toctree::
    :maxdepth: 1

    {:s}
""".format(FluidList))
    
def fluid_header(Fluid):
    return textwrap.dedent(
"""
********************
{Fluid:s}
********************

Aliases
================================================================================
{Aliases:s}

Equation of State Reference
===========================
{Reference:s}

Transport Properties Information
================================
{Transport:s}
    
""".format(Fluid=Fluid,
           Aliases = CP.get_aliases(Fluid),
           Reference = CP.get_EOSReference(Fluid),
           Transport = CP.get_TransportReference(Fluid)
           )
           )
    
fp=open(os.path.join('Fluids','FluidInformation.rst'),'w')
fp.write('###########################\nFluid Information\n###########################\n')
fp.write(
"""
CoolProp Fluid Information
==========================
.. toctree::
    :maxdepth: 1

    PseudoPureFluids.rst
    PureFluids.rst
    
There are 4 basic classes of fluids that can be used in CoolProp, an example is provided for each one.

Pure Fluids, Pseudo-Pure Fluids
-------------------------------
All the fluids listed in Pure-Fluids and Pseudo-Pure-Fluids sections above can be used.  To use one of these fluids, do something like

.. ipython::

    In [1]: from CoolProp.CoolProp import Props
    
    #Density of dry air at 1 atm. and 25C
    In [1]: Props('D','T',298,'P',101.325,'Air')
    
You can also use any of the aliases of the fluids that are the listed on the fluid page.  For instance, R717 is the refrigerant number for ammonia

.. ipython::

    In [1]: from CoolProp.CoolProp import Props
    
    #Density of saturated ammonia vapor at 1 atm.
    In [1]: Props('D','Q',1,'P',101.325,'R717')
    
    #Density of saturated ammonia vapor at 1 atm.
    In [1]: Props('D','Q',1,'P',101.325,'Ammonia')
    

REFPROP Fluids and Mixtures
---------------------------
If you are on Windows and have REFPROP installed, you can use it with CoolProp.  REFPROP needs to be installed in c:\\Program Files\\REFPROP.  If it is somewhere else, just copy it to this location.

All the pure fluids in REFPROP are used just like the CoolProp fluids except that "REFPROP-" is added at the beginning of the fluid name.  You can use any fluid that is included in REFPROP, but you must use the REFPROP fluid file name.  For CoolProp Fluids, you can use the ``get_REFPROPName()`` function to get the REFPROP name for the fluid.

.. ipython::

    In [1]: from CoolProp.CoolProp import Props
    
    #Saturated isobutane vapor density at 1 atmosphere
    In [1]: Props('D','Q',1,'P',101.325,'REFPROP-ISOBUTAN')
    
You can also use mixtures in REFPROP, there is a special format for the fluid name.  The fluid name is set up like this: ``"REFPROP-MIX:R32[0.697615]&R125[0.302385]"`` -  this is R410A.  The numbers within the brackets are the mole fractions of the components.  They must add up to 1.0
    
.. ipython::

    In [1]: from CoolProp.CoolProp import Props
    
    #Saturated R410 vapor density at 1 atmosphere using the mixture properties
    In [1]: Props('D','Q',1,'P',101.325,'REFPROP-MIX:R32[0.697615]&R125[0.302385]')

Brines
------
A number of aqueous solutions are implemented using the coefficients from Melinder (2010).  The list of diluents implemented are

==========================   ===================================================
Fluid Name                   Description
==========================   ===================================================
``EG``                       Ethylene Glycol
``PG``                       Propylene Glycol
``EA``                       Ethyl Alcohol (Ethanol)
``MA``                       Methyl Alcohol (Methanol)
``Glycerol``                 Glycerol
``NH3``                      Ammonia
``K2CO3``                    Potassium Carbonate
``CaCl2``                    Calcium Chloride
``MgCl2``                    Magnesium Chloride
``NaCl``                     Sodium Chloride
``KAC``                      Potassium Acetate
``KFO``                      Potassium Formate
``LiCl``                     Lithium Chloride
==========================   ===================================================

To use them the fluid name is something like ``"EG-20%"`` which is a 20% by mass ethylene glycol solution

.. ipython::

    In [1]: from CoolProp.CoolProp import Props
    
    #Specific heat 20% mass ethylene glycol solution at 300 K and 1 atm.
    In [1]: Props('C','T',300,'P',101.325,'EG-20%')
    
Liquids
-------
There are also a selection of incompressible liquids implemented from Melinder 2010.  The list of fluids implemented are

==========================   ===================================================
Fluid Name                   Description
==========================   ===================================================
``DEB``                      Diethyl Benzene
``HCM``                      Hydrocarbon Mixture (Therminol D12 Solutia)
``HFE``                      Hydrofluoroether HFE-7100
``PMS1``                     Polydimethylsiloxan 1.
``PMS2``                     Polydimethylsiloxan 2.
``SAB``                      Synthetic alkyl benzene
``HCB``                      Hydrocarbon blend (Dynalene MV)
``TCO``                      Terpene from citrus oils
==========================   ===================================================

.. ipython::

    In [1]: from CoolProp.CoolProp import Props
    
    #Density of HFE-7100 at 300 K and 1 atm.
    In [1]: Props('D','T',300,'P',101.325,'HFE')

"""
)
fp.close()

pseudo_pure_fluids = ['Air','R404A','R410A','R407C','R507A','SES36']
with open(os.path.join('Fluids','PseudoPureFluids.rst'),'w') as fp:
    fp.write('#######################\nPseudo-Pure Fluids\n#######################\n')
    fp.write(index_file(pseudo_pure_fluids))

for Fluid in pseudo_pure_fluids:
    fp=open(os.path.join('Fluids',Fluid+'.rst'),'w')
    s = fluid_header(Fluid) + params_table(Fluid) + critical_table(Fluid) + SatVaporParity(Fluid) + SatLiquidParity(Fluid) + CriticalIsotherm(Fluid) + PropertyConsistency(Fluid)
    fp.write(s)
    fp.close()

pure_fluids = sorted([fluid for fluid in CoolProp.__fluids__ if fluid not in pseudo_pure_fluids])
with open(os.path.join('Fluids','PureFluids.rst'),'w') as fp:
    fp.write('#######################\nPure Fluids\n#######################\n')
    fp.write(index_file(sorted(pure_fluids)))
    
for Fluid in sorted(pure_fluids):
    fp=open(os.path.join('Fluids',Fluid+'.rst'),'w')
    s = fluid_header(Fluid) + params_table(Fluid) + critical_table(Fluid) + SatVaporParity(Fluid) + SatLiquidParity(Fluid) + CriticalIsotherm(Fluid) + PropertyConsistency(Fluid)
    fp.write(s)
    fp.close()

    
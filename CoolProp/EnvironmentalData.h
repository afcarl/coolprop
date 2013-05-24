// This file contains the environmental data provided by the Danish Techological University (DTU)
//  
//

void syaml_build()
{
    std::map<std::string, double> fluid_map;
    
    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["1-Butene"] = "UNKNOWN";
    syaml_environmental_map()["1-Butene"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 0.5;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Acetone"] = "UNKNOWN";
    syaml_environmental_map()["Acetone"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Air"] = "UNKNOWN";
    syaml_environmental_map()["Air"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 3.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Ammonia"] = "B2";
    syaml_environmental_map()["Ammonia"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 0.0;
    fluid_map["GWP100"] = 0.0;
    fluid_map["GWP500"] = 0.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Argon"] = "A1";
    syaml_environmental_map()["Argon"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Benzene"] = "UNKNOWN";
    syaml_environmental_map()["Benzene"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 3.0;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["n-Butane"] = "A3";
    syaml_environmental_map()["n-Butane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["cis-2-Butene"] = "UNKNOWN";
    syaml_environmental_map()["cis-2-Butene"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 2.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["n-Dodecane"] = "UNKNOWN";
    syaml_environmental_map()["n-Dodecane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 3.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["CarbonMonoxide"] = "UNKNOWN";
    syaml_environmental_map()["CarbonMonoxide"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 1.0;
    fluid_map["GWP100"] = 1.0;
    fluid_map["GWP500"] = 1.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["CarbonDioxide"] = "A1";
    syaml_environmental_map()["CarbonDioxide"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 3.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = 97.0;
    fluid_map["GWP100"] = 27.0;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["CarbonylSulfide"] = "UNKNOWN";
    syaml_environmental_map()["CarbonylSulfide"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["CycloHexane"] = "UNKNOWN";
    syaml_environmental_map()["CycloHexane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Cyclopentane"] = "UNKNOWN";
    syaml_environmental_map()["Cyclopentane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 2.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["CycloPropane"] = "UNKNOWN";
    syaml_environmental_map()["CycloPropane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = 0.0;
    fluid_map["GWP100"] = 0.0;
    fluid_map["GWP500"] = 0.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Deuterium"] = "UNKNOWN";
    syaml_environmental_map()["Deuterium"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["DeuteriumOxide"] = "UNKNOWN";
    syaml_environmental_map()["DeuteriumOxide"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["D4"] = "UNKNOWN";
    syaml_environmental_map()["D4"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["D5"] = "UNKNOWN";
    syaml_environmental_map()["D5"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["D6"] = "UNKNOWN";
    syaml_environmental_map()["D6"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 2.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Decane"] = "UNKNOWN";
    syaml_environmental_map()["Decane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["DimethylCarbonate"] = "UNKNOWN";
    syaml_environmental_map()["DimethylCarbonate"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 2.0;
    fluid_map["GWP20"] = 1.0;
    fluid_map["GWP100"] = 1.0;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["DimethylEther"] = "A3";
    syaml_environmental_map()["DimethylEther"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 2.9;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Ethane"] = "A3";
    syaml_environmental_map()["Ethane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Ethanol"] = "UNKNOWN";
    syaml_environmental_map()["Ethanol"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 2.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 6.8;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Ethylene"] = "A3";
    syaml_environmental_map()["Ethylene"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 4.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Fluorine"] = "UNKNOWN";
    syaml_environmental_map()["Fluorine"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 4.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["HydrogenSulfide"] = "UNKNOWN";
    syaml_environmental_map()["HydrogenSulfide"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Helium"] = "A1";
    syaml_environmental_map()["Helium"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["n-Heptane"] = "UNKNOWN";
    syaml_environmental_map()["n-Heptane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 3.1;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["n-Hexane"] = "UNKNOWN";
    syaml_environmental_map()["n-Hexane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Hydrogen"] = "A3";
    syaml_environmental_map()["Hydrogen"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 2.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Isobutene"] = "UNKNOWN";
    syaml_environmental_map()["Isobutene"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Isohexane"] = "UNKNOWN";
    syaml_environmental_map()["Isohexane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Isopentane"] = "A3";
    syaml_environmental_map()["Isopentane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["IsoButane"] = "UNKNOWN";
    syaml_environmental_map()["IsoButane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Krypton"] = "UNKNOWN";
    syaml_environmental_map()["Krypton"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 2.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["MD2M"] = "UNKNOWN";
    syaml_environmental_map()["MD2M"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 2.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["MD3M"] = "UNKNOWN";
    syaml_environmental_map()["MD3M"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 2.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["MDM"] = "UNKNOWN";
    syaml_environmental_map()["MDM"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 72.0;
    fluid_map["GWP100"] = 25.0;
    fluid_map["GWP500"] = 7.6;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Methane"] = "A3";
    syaml_environmental_map()["Methane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 2.8;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Methanol"] = "UNKNOWN";
    syaml_environmental_map()["Methanol"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["MethylLinoleate"] = "UNKNOWN";
    syaml_environmental_map()["MethylLinoleate"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["MethylLinolenate"] = "UNKNOWN";
    syaml_environmental_map()["MethylLinolenate"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["MM"] = "UNKNOWN";
    syaml_environmental_map()["MM"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["MethylOleate"] = "UNKNOWN";
    syaml_environmental_map()["MethylOleate"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["MethylPalmitate"] = "UNKNOWN";
    syaml_environmental_map()["MethylPalmitate"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["MethylStearate"] = "UNKNOWN";
    syaml_environmental_map()["MethylStearate"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 290.0;
    fluid_map["GWP100"] = 320.0;
    fluid_map["GWP500"] = 180.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["NitrousOxide"] = "UNKNOWN";
    syaml_environmental_map()["NitrousOxide"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Neon"] = "A1";
    syaml_environmental_map()["Neon"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Neopentane"] = "UNKNOWN";
    syaml_environmental_map()["Neopentane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 3.0;
    fluid_map["GWP20"] = 12300.0;
    fluid_map["GWP100"] = 17200.0;
    fluid_map["GWP500"] = 20700.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["NitrogenTrifluoride"] = "UNKNOWN";
    syaml_environmental_map()["NitrogenTrifluoride"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Nitrogen"] = "A1";
    syaml_environmental_map()["Nitrogen"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["n-Nonane"] = "UNKNOWN";
    syaml_environmental_map()["n-Nonane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["n-Octane"] = "UNKNOWN";
    syaml_environmental_map()["n-Octane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["OrthoHydrogen"] = "UNKNOWN";
    syaml_environmental_map()["OrthoHydrogen"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Oxygen"] = "UNKNOWN";
    syaml_environmental_map()["Oxygen"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["ParaHydrogen"] = "UNKNOWN";
    syaml_environmental_map()["ParaHydrogen"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["n-Pentane"] = "UNKNOWN";
    syaml_environmental_map()["n-Pentane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 3.0;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Propane"] = "UNKNOWN";
    syaml_environmental_map()["Propane"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 3.1;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Propylene"] = "A3";
    syaml_environmental_map()["Propylene"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Propyne"] = "UNKNOWN";
    syaml_environmental_map()["Propyne"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = 2330.0;
    fluid_map["GWP100"] = 675.0;
    fluid_map["GWP500"] = 205.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R32"] = "A2";
    syaml_environmental_map()["R32"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 2.0;
    fluid_map["GWP20"] = 323.0;
    fluid_map["GWP100"] = 92.0;
    fluid_map["GWP500"] = 28.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R41"] = "UNKNOWN";
    syaml_environmental_map()["R41"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = 5310.0;
    fluid_map["GWP100"] = 7370.0;
    fluid_map["GWP500"] = 9990.0;
    fluid_map["ODP"] = 0.44;
    ASHRAE34_map()["R115"] = "A1";
    syaml_environmental_map()["R115"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R116"] = "A1";
    syaml_environmental_map()["R116"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 2070.0;
    fluid_map["GWP100"] = 609.0;
    fluid_map["GWP500"] = 185.0;
    fluid_map["ODP"] = 0.022;
    ASHRAE34_map()["R124"] = "A1";
    syaml_environmental_map()["R124"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 6350.0;
    fluid_map["GWP100"] = 3500.0;
    fluid_map["GWP500"] = 1100.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R125"] = "UNKNOWN";
    syaml_environmental_map()["R125"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 2250.0;
    fluid_map["GWP100"] = 725.0;
    fluid_map["GWP500"] = 220.0;
    fluid_map["ODP"] = 0.12;
    ASHRAE34_map()["R141b"] = "UNKNOWN";
    syaml_environmental_map()["R141b"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 5490.0;
    fluid_map["GWP100"] = 2310.0;
    fluid_map["GWP500"] = 705.0;
    fluid_map["ODP"] = 0.07;
    ASHRAE34_map()["R142b"] = "A2";
    syaml_environmental_map()["R142b"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 5890.0;
    fluid_map["GWP100"] = 4470.0;
    fluid_map["GWP500"] = 1590.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R143a"] = "A2";
    syaml_environmental_map()["R143a"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 10.0;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R161"] = "UNKNOWN";
    syaml_environmental_map()["R161"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = 6310.0;
    fluid_map["GWP100"] = 8830.0;
    fluid_map["GWP500"] = 12500.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R218"] = "UNKNOWN";
    syaml_environmental_map()["R218"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 5310.0;
    fluid_map["GWP100"] = 3220.0;
    fluid_map["GWP500"] = 1040.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R227EA"] = "UNKNOWN";
    syaml_environmental_map()["R227EA"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 1200.0;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R236EA"] = "UNKNOWN";
    syaml_environmental_map()["R236EA"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = 8100.0;
    fluid_map["GWP100"] = 9810.0;
    fluid_map["GWP500"] = 7660.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R236FA"] = "UNKNOWN";
    syaml_environmental_map()["R236FA"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R245CA"] = "UNKNOWN";
    syaml_environmental_map()["R245CA"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = 3380.0;
    fluid_map["GWP100"] = 1030.0;
    fluid_map["GWP500"] = 314.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R245fa"] = "UNKNOWN";
    syaml_environmental_map()["R245fa"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = 2520.0;
    fluid_map["GWP100"] = 794.0;
    fluid_map["GWP500"] = 241.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R365MFC"] = "UNKNOWN";
    syaml_environmental_map()["R365MFC"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 3985.0;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R507A"] = "A1";
    syaml_environmental_map()["R507A"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 2.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 4.0;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R1234yf"] = "UNKNOWN";
    syaml_environmental_map()["R1234yf"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 2.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 6.0;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R1234ze"] = "UNKNOWN";
    syaml_environmental_map()["R1234ze"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 16300.0;
    fluid_map["GWP100"] = 22800.0;
    fluid_map["GWP500"] = 32600.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["SulfurHexafluoride"] = "UNKNOWN";
    syaml_environmental_map()["SulfurHexafluoride"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 3.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["SulfurDioxide"] = "B1";
    syaml_environmental_map()["SulfurDioxide"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["trans-2-Butene"] = "UNKNOWN";
    syaml_environmental_map()["trans-2-Butene"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 3.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 3.3;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Toluene"] = "UNKNOWN";
    syaml_environmental_map()["Toluene"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Water"] = "A1";
    syaml_environmental_map()["Water"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 0.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 3.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = _HUGE;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["Xenon"] = "UNKNOWN";
    syaml_environmental_map()["Xenon"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 6730.0;
    fluid_map["GWP100"] = 4750.0;
    fluid_map["GWP500"] = 1620.0;
    fluid_map["ODP"] = 1.0;
    ASHRAE34_map()["R11"] = "A1";
    syaml_environmental_map()["R11"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 11000.0;
    fluid_map["GWP100"] = 10900.0;
    fluid_map["GWP500"] = 5200.0;
    fluid_map["ODP"] = 1.0;
    ASHRAE34_map()["R12"] = "A1";
    syaml_environmental_map()["R12"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = 10800.0;
    fluid_map["GWP100"] = 14400.0;
    fluid_map["GWP500"] = 16400.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R13"] = "A1";
    syaml_environmental_map()["R13"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = 5210.0;
    fluid_map["GWP100"] = 7390.0;
    fluid_map["GWP500"] = 11200.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R14"] = "UNKNOWN";
    syaml_environmental_map()["R14"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = _HUGE;
    fluid_map["FH"] = _HUGE;
    fluid_map["PH"] = _HUGE;
    fluid_map["GWP20"] = 530.0;
    fluid_map["GWP100"] = 151.0;
    fluid_map["GWP500"] = 46.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R21"] = "B1";
    syaml_environmental_map()["R21"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 5160.0;
    fluid_map["GWP100"] = 1810.0;
    fluid_map["GWP500"] = 549.0;
    fluid_map["ODP"] = 0.05;
    ASHRAE34_map()["R22"] = "A1";
    syaml_environmental_map()["R22"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 12000.0;
    fluid_map["GWP100"] = 14800.0;
    fluid_map["GWP500"] = 12200.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R23"] = "A1";
    syaml_environmental_map()["R23"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 6540.0;
    fluid_map["GWP100"] = 6130.0;
    fluid_map["GWP500"] = 2700.0;
    fluid_map["ODP"] = 1.0;
    ASHRAE34_map()["R113"] = "A1";
    syaml_environmental_map()["R113"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 8040.0;
    fluid_map["GWP100"] = 10000.0;
    fluid_map["GWP500"] = 8730.0;
    fluid_map["ODP"] = 1.0;
    ASHRAE34_map()["R114"] = "A1";
    syaml_environmental_map()["R114"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 2.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 273.0;
    fluid_map["GWP100"] = 77.0;
    fluid_map["GWP500"] = 24.0;
    fluid_map["ODP"] = 0.02;
    ASHRAE34_map()["R123"] = "B1";
    syaml_environmental_map()["R123"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = 3830.0;
    fluid_map["GWP100"] = 1430.0;
    fluid_map["GWP500"] = 435.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R134a"] = "A1";
    syaml_environmental_map()["R134a"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 4.0;
    fluid_map["PH"] = 1.0;
    fluid_map["GWP20"] = 437.0;
    fluid_map["GWP100"] = 124.0;
    fluid_map["GWP500"] = 38.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R152A"] = "A2";
    syaml_environmental_map()["R152A"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 3900.0;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0.0;
    ASHRAE34_map()["R404A"] = "A1";
    syaml_environmental_map()["R404A"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 1800.0;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R407C"] = "A1";
    syaml_environmental_map()["R407C"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 1.0;
    fluid_map["PH"] = 0.0;
    fluid_map["GWP20"] = _HUGE;
    fluid_map["GWP100"] = 2088.0;
    fluid_map["GWP500"] = _HUGE;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["R410A"] = "A1";
    syaml_environmental_map()["R410A"] = fluid_map;

    fluid_map.clear();
    fluid_map["HH"] = 1.0;
    fluid_map["FH"] = 0.0;
    fluid_map["PH"] = 2.0;
    fluid_map["GWP20"] = 7310.0;
    fluid_map["GWP100"] = 10300.0;
    fluid_map["GWP500"] = 14700.0;
    fluid_map["ODP"] = 0;
    ASHRAE34_map()["RC318"] = "UNKNOWN";
    syaml_environmental_map()["RC318"] = fluid_map;

}
// File generated by the script dev/mixtures/JSON_to_C++.py

const char mixture_excess_JSON[] = ""
"["
"{"
"  \"Type\" : \"MultipleFluids\","
"  \"BibTeX\" : \"Lemmon-JPCRD-2004\","
"  \"Model\" : \"Lemmon-JPCRD-2004\","
"  \"Coeffs\" : [{\"Name1\" : \"R32\","
"               \"Name2\" : \"R125\","
"               \"CAS1\" : \"75-10-5\","
"               \"CAS2\" : \"354-33-6\","
"               \"N\" : [0.0,-0.0072955, 0.078035, 0.61007, 0.64246, 0.014965, -0.34049, 0.085658, -0.064429],"
"               \"t\" : [0.0,4.5, 0.57, 1.9, 1.2, 0.5, 2.6, 11.4, 4.5],"
"               \"d\" : [0, 2, 5, 1, 3, 9, 2, 3, 6],"
"               \"l\" : [0, 1, 1, 2, 2, 2, 3, 3, 3]"
"               },"
"               {\"Name1\" : \"R32\","
"               \"Name2\" : \"R143a\","
"               \"CAS1\" : \"75-10-5\","
"               \"CAS2\" : \"420-46-2\","
"               \"N\" : [0.0,0.22909, 0.094074, 0.00039876, 0.021113],"
"               \"t\" : [0.0,1.9, 0.25, 0.07, 2.0],"
"               \"d\" : [0, 1, 3, 8, 1],"
"               \"l\" : [0, 1, 1, 1, 2]"
"               },"
"               {\"Names1\" : [\"R125\", \"R125\", \"R134a\", \"R134a\"],"
"               \"Names2\" : [\"R134a\", \"R143a\", \"R143a\", \"R152A\"],"
"               \"CAS1\" : [\"354-33-6\", \"354-33-6\", \"811-97-2\", \"811-97-2\"],"
"               \"CAS2\" : [\"811-97-2\", \"420-46-2\", \"420-46-2\", \"75-37-6\"],"
"               \"N\" : [0.0,-0.013073, 0.018259,0.0000081299,0.0078496],"
"               \"t\" : [0.0,7.4, 0.35, 10.0, 5.3],"
"               \"d\" : [0, 1, 3, 11, 2],"
"               \"l\" : [0, 1, 1, 3, 2]"
"               }"
"            ]"
"}"
","
"{"
"  \"Type\" : \"MultipleFluids\","
"  \"BibTeX\" : \"Lemmon-JPCRD-2000\","
"  \"Model\" : \"Lemmon-JPCRD-2000\","
"  \"Coeffs\" : ["
"               {\"Names1\" : [\"Nitrogen\", \"Nitrogen\", \"Argon\"],"
"               \"Names2\" : [\"Argon\", \"Oxygen\", \"Oxygen\"],"
"               \"CAS1\" : [\"7727-37-9\", \"7727-37-9\", \"7440-37-1\"],"
"               \"CAS2\" : [\"7440-37-1\", \"7782-44-7\", \"7782-44-7\"],"
"               \"N\" : [0.0, -0.00195245, 0.00871334],"
"               \"t\" : [0, -1.4, 1.5],"
"               \"d\" : [0, 2, 2]"
"               }"
"               "
"            ]"
"}"
","
"{"
"  \"Type\" : \"MultipleFluids\","
"  \"BibTeX\" : \"Kunz-JCED-2012\","
"  \"Model\" : \"Kunz-JCED-2012\","
"  \"Coeffs\" : ["
"                {\"Name1\" : \"Methane\","
"                \"Name2\" : \"Nitrogen\","
"                \"CAS1\" : \"74-82-8\","
"                \"CAS2\" : \"7727-37-9\","
"                \"d\" : [0, 1, 4, 1, 2, 2, 2, 2, 2, 3],"
"                \"t\" : [0, 0.0, 1.85, 7.85, 5.4, 0.0, 0.75, 2.8, 4.45, 4.25],"
"                \"n\" : [0, -0.0098038985517335, 0.00042487270143005, -0.034800214576142, -0.13333813013896, -0.011993694974627, 0.069243379775168, -0.31022508148249, 0.24495491753226, 0.22369816716981],"
"                \"eta\" : [0, 0.0, 0.0, 1.0, 1.0, 0.25, 0.0, 0.0, 0.0, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5],"
"                \"beta\": [0, 0.0, 0.0, 1.0, 1.0, 2.5, 3.0, 3.0, 3.0, 3.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5]"
"                },"
"                {\"Name1\" : \"Methane\","
"                \"Name2\" : \"CarbonDioxide\","
"                \"CAS1\" : \"74-82-8\","
"                \"CAS2\" : \"124-38-9\","
"                \"d\" : [0, 1, 2, 3, 1, 2, 3],"
"                \"t\" : [0, 2.6, 1.95, 0.0, 3.95, 7.95, 8.0],"
"                \"n\" : [0, -0.10859387354942, 0.080228576727389, -0.0093303985115717, 0.040989274005848, -0.24338019772494, 0.23855347281124],"
"                \"eta\" : [0, 0.0, 0.0, 0.0, 1.0, 0.5, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5],"
"                \"beta\": [0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5]"
"                },"
"                {\"Name1\" : \"Methane\","
"                \"Name2\" : \"Ethane\","
"                \"CAS1\" : \"74-82-8\","
"                \"CAS2\" : \"74-84-0\","
"                \"d\" : [0, 3, 4, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3],"
"                \"t\" : [0, 0.65, 1.55, 3.1, 5.9, 7.05, 3.35, 1.2, 5.8, 2.7, 0.45, 0.55, 1.95],"
"                \"n\" : [0, -0.00080926050298746, -0.00075381925080059, -0.041618768891219, -0.23452173681569, 0.14003840584586, 0.063281744807738, -0.034660425848809, -0.23918747334251, 0.0019855255066891, 6.1777746171555, -6.9575358271105, 1.0630185306388],"
"                \"eta\" : [0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.875, 0.75, 0.5, 0.0, 0.0, 0.0, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5],"
"                \"beta\": [0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.25, 1.5, 2.0, 3.0, 3.0, 3.0, 3.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5]"
"                },"
"                {\"Name1\" : \"Methane\","
"                \"Name2\" : \"Propane\","
"                \"CAS1\" : \"74-82-8\","
"                \"CAS2\" : \"74-98-6\","
"                \"d\" : [0, 3, 3, 4, 4, 4, 1, 1, 1, 2],"
"                \"t\" : [0, 1.85, 3.95, 0.0, 1.85, 3.85, 5.25, 3.85, 0.2, 6.5],"
"                \"n\" : [0, 0.013746429958576, -0.0074425012129552, -0.0045516600213685, -0.0054546603350237, 0.0023682016824471, 0.18007763721438, -0.44773942932486, 0.0193273748882, -0.30632197804624],"
"                \"eta\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5],"
"                \"beta\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.75, 1.0, 2.0, 3.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5]"
"                },"
"                {\"Name1\" : \"Nitrogen\","
"                \"Name2\" : \"Carbon Dioxide\","
"                \"CAS1\" : \"7727-37-9\","
"                \"CAS2\" : \"Carbon Dioxide is an invalid fluid for get_fluid_param_string\","
"                \"d\" : [0, 2, 3, 1, 1, 1, 2],"
"                \"t\" : [0, 1.85, 1.4, 3.2, 2.5, 8.0, 3.75],"
"                \"n\" : [0, 0.28661625028399, -0.10919833861247, -1.137403208227, 0.76580544237358, 0.0042638000926819, 0.17673538204534],"
"                \"eta\" : [0, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5],"
"                \"beta\": [0, 0.0, 0.0, 0.75, 1.0, 2.0, 3.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5]"
"                },"
"                {\"Name1\" : \"Nitrogen\","
"                \"Name2\" : \"Ethane\","
"                \"CAS1\" : \"7727-37-9\","
"                \"CAS2\" : \"74-84-0\","
"                \"d\" : [0, 2, 2, 3, 1, 2, 2],"
"                \"t\" : [0, 0.0, 0.05, 0.0, 3.65, 4.9, 4.45],"
"                \"n\" : [0, -0.47376518126608, 0.48961193461001, -0.0057011062090535, -0.1996682004132, -0.69411103101723, 0.69226192739021],"
"                \"eta\" : [0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.875],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5],"
"                \"beta\": [0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.25],"
"                \"gamma\": [0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5]"
"                },"
"                {\"Name1\" : \"Methane\","
"                \"Name2\" : \"Hydrogen\","
"                \"CAS1\" : \"74-82-8\","
"                \"CAS2\" : \"1333-74-0\","
"                \"d\" : [0, 1, 3, 3, 4],"
"                \"t\" : [0, 2.0, -1.0, 1.75, 1.4],"
"                \"n\" : [0, -0.25157134971934, -0.0062203841111983, 0.088850315184396, -0.035592212573239],"
"                \"eta\" : [0, 0.0, 0.0, 0.0, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.0, 0.0],"
"                \"beta\": [0, 0.0, 0.0, 0.0, 0.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.0, 0.0]"
"                },"
"                {\"Name1\" : \"Methane\","
"                \"Name2\" : \"n-Butane\","
"                \"CAS1\" : \"74-82-8\","
"                \"CAS2\" : \"106-97-8\","
"                \"d\" : [0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4],"
"                \"t\" : [0, 1.0, 1.55, 1.7, 0.25, 1.35, 0.0, 1.25, 0.0, 0.7, 5.4],"
"                \"n\" : [0, 2.5574776844118, -7.9846357136353, 4.7859131465806, -0.73265392369587, 1.3805471345312, 0.28349603476365, -0.49087385940425, -0.10291888921447, 0.11836314681968, 5.5527385721943e-05],"
"                \"eta\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"beta\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]"
"                },"
"                {\"Name1\" : \"Methane\","
"                \"Name2\" : \"Isobutane\","
"                \"CAS1\" : \"74-82-8\","
"                \"CAS2\" : \"75-28-5\","
"                \"d\" : [0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4],"
"                \"t\" : [0, 1.0, 1.55, 1.7, 0.25, 1.35, 0.0, 1.25, 0.0, 0.7, 5.4],"
"                \"n\" : [0, 2.5574776844118, -7.9846357136353, 4.7859131465806, -0.73265392369587, 1.3805471345312, 0.28349603476365, -0.49087385940425, -0.10291888921447, 0.11836314681968, 5.5527385721943e-05],"
"                \"eta\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"beta\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]"
"                },"
"                {\"Name1\" : \"Ethane\","
"                \"Name2\" : \"Propane\","
"                \"CAS1\" : \"74-84-0\","
"                \"CAS2\" : \"74-98-6\","
"                \"d\" : [0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4],"
"                \"t\" : [0, 1.0, 1.55, 1.7, 0.25, 1.35, 0.0, 1.25, 0.0, 0.7, 5.4],"
"                \"n\" : [0, 2.5574776844118, -7.9846357136353, 4.7859131465806, -0.73265392369587, 1.3805471345312, 0.28349603476365, -0.49087385940425, -0.10291888921447, 0.11836314681968, 5.5527385721943e-05],"
"                \"eta\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"beta\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]"
"                },"
"                {\"Name1\" : \"Ethane\","
"                \"Name2\" : \"n-Butane\","
"                \"CAS1\" : \"74-84-0\","
"                \"CAS2\" : \"106-97-8\","
"                \"d\" : [0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4],"
"                \"t\" : [0, 1.0, 1.55, 1.7, 0.25, 1.35, 0.0, 1.25, 0.0, 0.7, 5.4],"
"                \"n\" : [0, 2.5574776844118, -7.9846357136353, 4.7859131465806, -0.73265392369587, 1.3805471345312, 0.28349603476365, -0.49087385940425, -0.10291888921447, 0.11836314681968, 5.5527385721943e-05],"
"                \"eta\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"beta\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]"
"                },"
"                {\"Name1\" : \"Ethane\","
"                \"Name2\" : \"Isobutane\","
"                \"CAS1\" : \"74-84-0\","
"                \"CAS2\" : \"75-28-5\","
"                \"d\" : [0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4],"
"                \"t\" : [0, 1.0, 1.55, 1.7, 0.25, 1.35, 0.0, 1.25, 0.0, 0.7, 5.4],"
"                \"n\" : [0, 2.5574776844118, -7.9846357136353, 4.7859131465806, -0.73265392369587, 1.3805471345312, 0.28349603476365, -0.49087385940425, -0.10291888921447, 0.11836314681968, 5.5527385721943e-05],"
"                \"eta\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"beta\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]"
"                },"
"                {\"Name1\" : \"Propane\","
"                \"Name2\" : \"n-Butane\","
"                \"CAS1\" : \"74-98-6\","
"                \"CAS2\" : \"106-97-8\","
"                \"d\" : [0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4],"
"                \"t\" : [0, 1.0, 1.55, 1.7, 0.25, 1.35, 0.0, 1.25, 0.0, 0.7, 5.4],"
"                \"n\" : [0, 2.5574776844118, -7.9846357136353, 4.7859131465806, -0.73265392369587, 1.3805471345312, 0.28349603476365, -0.49087385940425, -0.10291888921447, 0.11836314681968, 5.5527385721943e-05],"
"                \"eta\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"beta\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]"
"                },"
"                {\"Name1\" : \"Propane\","
"                \"Name2\" : \"Isobutane\","
"                \"CAS1\" : \"74-98-6\","
"                \"CAS2\" : \"75-28-5\","
"                \"d\" : [0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4],"
"                \"t\" : [0, 1.0, 1.55, 1.7, 0.25, 1.35, 0.0, 1.25, 0.0, 0.7, 5.4],"
"                \"n\" : [0, 2.5574776844118, -7.9846357136353, 4.7859131465806, -0.73265392369587, 1.3805471345312, 0.28349603476365, -0.49087385940425, -0.10291888921447, 0.11836314681968, 5.5527385721943e-05],"
"                \"eta\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"beta\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]"
"                },"
"                {\"Name1\" : \"n-Butane\","
"                \"Name2\" : \"Isobutane\","
"                \"CAS1\" : \"106-97-8\","
"                \"CAS2\" : \"75-28-5\","
"                \"d\" : [0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4],"
"                \"t\" : [0, 1.0, 1.55, 1.7, 0.25, 1.35, 0.0, 1.25, 0.0, 0.7, 5.4],"
"                \"n\" : [0, 2.5574776844118, -7.9846357136353, 4.7859131465806, -0.73265392369587, 1.3805471345312, 0.28349603476365, -0.49087385940425, -0.10291888921447, 0.11836314681968, 5.5527385721943e-05],"
"                \"eta\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"epsilon\" : [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"beta\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],"
"                \"gamma\": [0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]"
"                }"
"            ]"
"}"
"]";

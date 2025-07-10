# buoySummerProject
Summer 2025 Buoy Project for MESA / COD

## team composition
- John Henry Sarrouf (Computer Engineer)
- Glenn Houston Mathews (Environmental Engineer)

## resources for researching buoys
[Trelleborg Guide to Navigation Buoys](https://www.trelleborg.com/marine-systems/~/media/marine-systems/resources/guides-and-design-manual/downloads/guidetonavigationbuoys.pdf)
[OpenMetBuoy‑V2021 (Preprint PDF)](https://github.com/jerabaul29/OpenMetBuoy-v2021a/blob/main/latest_preprint_MDPI_2022.pdf)
[OpenMetBuoy‑V2021 (MDPI Journal Article)](https://www.mdpi.com/2076-3263/12/3/110)
[OpenMetBuoy‑V2021 End User Manual](https://github.com/jerabaul29/OpenMetBuoy-v2021a/tree/main/end_user_manual)



## buoy, tests water quality (pH, Salt Levels, Toxins)
**for the salton sea**
*ideally data is relayed through radio or through a sim card inserted into an external module, so that way the data can be collected without having to retrieve the buoy*
*simultaneously, the data can be stored via micro sd card, as csv files take up relatively little space, and as a result, even a moderately sized storage piece can hold weeks if not months worth of data at once*

### possible applications
**predicting algal blooms**
we can predict algal blooms by monitoring the water's temperature and oxygen content
surprisingly, algal blooms are somwhat common to the salton sea, and can be harmful
- [algal blooms nasa](https://visibleearth.nasa.gov/images/69055/algal-bloom-in-the-salton-sea-california/69057l)

### misc resources (water / parts)
[pH Sensor](https://store-usa.arduino.cc/products/gravity-analog-ph-sensor-meter-kit?srsltid=AfmBOoqeRMmGsmS6jF-vOOj_Sy8lIsTamM6BH9_96V_TSciFYV3JBH8x)
[how to measure salinity](https://www.reddit.com/r/arduino/comments/zhhk92/how_can_i_measure_salinity_of_water/)
[through conductivity](https://hackaday.io/project/7008-hacking-the-way-to-growing-food/log/24646-three-dollar-ec-ppm-meter-arduino)
[arduino water quality monitoring](https://www.instructables.com/Arduino-Water-Quality-Monitoring-System/)
[water quality sensor](https://www.instructables.com/Water-Quality-Sensor/)
[temp sensor](https://www.adafruit.com/product/3721?gQT=1)
[oxy sensor](https://store-usa.arduino.cc/collections/dfrobot/products/gravity-i2c-oxygen-sensor?gQT=1)
[salinity meter tutorial](https://www.instructables.com/Water-Salinity-meter/)


### buoy manufacturing process and other info
[corilla manufacturing](https://youtu.be/BqNsHRRcBX0?si=UaqvQV13RhbOa356)
[how buoys gather data](https://youtu.be/uI6qDKzxA8I?si=GbQfDtwYY16eQNXN)

### sensor ideas
- pH Sensors
- water conductivity (salt levels)
- oxygen levels
- temperature (water and air)
- gps

### chat how to do buoy legally?

**tldr: it wont be too bad, but for independent researchers theres a relatively short form we can fill out**

All buoy‑placement activities on the Salton Sea require federal authorization under the U.S. Army Corps of Engineers’ regulatory program. Mooring buoys in the Salton Sea fall under Nationwide Permit 10 (“Mooring Buoys”), which authorizes placement of buoys in waters of the United States, provided impacts to wetlands, mudflats or vegetated shallows do not exceed 0.10 acre and no Section 401 water quality certification is required [US Army Corps of Engineers](https://www.spk.usace.army.mil/Portals/12/documents/regulatory/nwp/2021-nwps/Attach.1-Final_2021_NWP_CA_Regional_Conditions.pdf?ver=a8cyqIUFwmGnd5gDIlwY0w%3D%3D&utm_source=chatgpt.com). For Salton Sea–specific projects, the Corps’ Los Angeles District has also established a streamlined Letter of Permission (LOP) process under its Salton Sea Management Plan – you can contact the Regulatory Division at the U.S. Army Corps of Engineers, Los Angeles District to discuss whether your buoy qualifies for an LOP [U.S. Army Corps of Engineers](https://www.spl.usace.army.mil/Missions/Regulatory/Projects-Programs/Salton-Sea-Management-Plan/?utm_source=chatgpt.com).

Any private aid to navigation (PATON), including buoys placed for anchorage or marking, must obtain a U.S. Coast Guard permit. Applicants use Form CG‑2554 (“Private Aids to Navigation Application”), providing buoy type, charted position, markings, and evidence of Corps authorization [Pacific Area USCG](https://www.pacificarea.uscg.mil/Our-Organization/District-11/Prevention-Division/PatonOne/?utm_source=chatgpt.com)[eCFR](https://www.ecfr.gov/current/title-33/chapter-I/subchapter-C/part-66?utm_source=chatgpt.com). You must submit the completed CG‑2554 to the Commander, U.S. Coast Guard District 11 (Southern California); the District’s PATON Program Manager assists with paperwork and reviews plan submittals for both temporary (≤ 6 months) and permanent aids [Pacific Area USCG](https://www.pacificarea.uscg.mil/Our-Organization/District-11/Prevention-Division/PatonOne/?utm_source=chatgpt.com).

At the state level, buoys on California waters must comply with Title 14, California Code of Regulations, § 7002 (Waterway Markers), which prescribes buoy color, numbering, and light‑characteristic standards consistent with federal ATON rules [Legal Information Institute](https://www.law.cornell.edu/regulations/california/14-CCR-7002?utm_source=chatgpt.com). Additionally, the California State Parks Division of Boating and Waterways (DBW) oversees boating safety and issues the California Boater Card, but does not itself issue buoy‑placement permits; for any vessel activities you must be properly registered with DMV and hold a Boater Card when operating watercraft [California State Parks](https://dbw.parks.ca.gov/?utm_source=chatgpt.com).

Within the Salton Sea State Recreation Area, special‑event or film permits may be required for activities that could disturb the shoreline environment; contact the Ocotillo Wells District Special Events/Film Permit Coordinator at SpecialEvents.OcotilloWells@parks.ca.gov to determine if your buoy deployment triggers a park permit [California State Parks](https://www.parks.ca.gov/?page_id=29646&utm_source=chatgpt.com).

**Consequences of Non‑Compliance**  
Unauthorized buoys are subject to removal at the owner’s expense, and the Coast Guard may issue discrepancy reports or direct removal under 33 CFR 66.01‑25 [U.S. Department of Defense](https://media.defense.gov/2017/Nov/20/2001846135/-1/-1/0/CG_2554.pdf?utm_source=chatgpt.com). Violations of Corps regulations can incur civil penalties and require after‑the‑fact permits, which carry higher fees and more onerous review [US Army Corps of Engineers](https://www.spk.usace.army.mil/Portals/12/documents/regulatory/nwp/2021-nwps/Attach.1-Final_2021_NWP_CA_Regional_Conditions.pdf?ver=a8cyqIUFwmGnd5gDIlwY0w%3D%3D&utm_source=chatgpt.com).

---

#### Who to Contact

- **U.S. Army Corps of Engineers, Los Angeles District (Regulatory Division)**  
    For Nationwide Permit 10 or LOP: (213) 452‑3800 or lac-regulatory@usace.army.mil [U.S. Army Corps of Engineers](https://www.spl.usace.army.mil/Missions/Regulatory/Projects-Programs/Salton-Sea-Management-Plan/?utm_source=chatgpt.com)
    
- **U.S. Coast Guard District 11 (PATON Program Manager)**  
    For Private Aids to Navigation (Form CG‑2554) and ATON guidance: sector11prevention@uscg.mil [Pacific Area USCG](https://www.pacificarea.uscg.mil/Our-Organization/District-11/Prevention-Division/PatonOne/?utm_source=chatgpt.com)
    
- **California State Parks, Division of Boating and Waterways**  
    For boater registration and safety regulation questions: https://dbw.parks.ca.gov/ [California State Parks](https://dbw.parks.ca.gov/?utm_source=chatgpt.com)
    
- **California State Parks, Ocotillo Wells District**  
    For special‑event/film permits at Salton Sea SRA: SpecialEvents.OcotilloWells@parks.ca.gov [California State Parks](https://www.parks.ca.gov/?page_id=29646&utm_source=chatgpt.com)
    
- **National Wildlife Refuge Found By Houston :)** 
Permit for Research Activties:
https://www.fws.gov/service/special-use-permits-national-wildlife-refuges
Email of Refuge Manager/Project Leader: Jonathan_Shore@fws.gov

By securing these federal and state approvals in advance, you ensure your buoy is a safe, lawful private aid to navigation on the Salton Sea.

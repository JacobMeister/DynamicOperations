class SCR_DynamicOperationsConstants
{	
	const int AMOUNT_OF_MISSIONS = 3;
	
	static const ref array<string> POSSIBLE_MISSION_LOCATIONS = 
	{
		// locations that have the standardized SCR_SiteSlotEntity elemements in the hierarchy
		
		// Towns
		"C_Location_St_Pierre", "C_LocationChotain", 
		"C_LocationDurras", "C_LocationEntreDeux", 
		"C_LocationFigari", "C_LocationGravette", "C_LocationLamentin", "C_LocationLaruns", 
		"C_LocationLeMoule", "C_LocationLevie", "C_LocationMeaux", "C_LocationMontignac", 
		"C_LocationMorton", "C_LocationProvins", "C_LocationRegina", "C_LocationStPhilippe", 
		"C_LocationTyrone", "C_LocationVernon", "C_LocationVilleneuve",
		
		// Villages
		"S_Kermovan",
		
		// Points of interest
		"G_Airport", 
		"G_Farm2", "G_Farm3", "G_Industrial", "G_Industrial3", "G_Landfill",
		"G_Military", "G_Military3", "G_Military5", "G_Military6", "G_Quarry", "G_Sawmill", 
		"G_MontfortCastle", "Barracks"
	};

	static const ref array<string> PATROL_LOCATIONS = 
	{
		// smaller locations that have some strategic value
		
		// Villages
		"S_Benac", "S_Camurac", "S_Etoupe", "S_Gorey", "S_Lacourt", "S_Lavalette", 
		"S_LesCreux", "S_Perelle", "S_Redon", "S_Richemont", "S_LeBosc", 

		// Misc
		"G_Briars", "G_Factory", "G_Farm", "G_Farm4", "G_Harbour", "G_Harbour2", "G_Harbour4", 
		"G_Harbour5", "G_Harbour6", "G_Industrial2", "G_Military2", "G_Military4", "G_PennantsPass", 
		"G_PetersPass", "L_RamtopMeadows", "G_Sawmill2"	
	};
	
	static const ref array<string> SPAWN_LOCATIONS = 
	{
		// location in the wilderness where teams could plausibly infiltrate via car/boat/heli/foot
		"G_HuntsmansHeath", "L_LaRoue", "L_Scythe", "L_CragPoint", "L_Thollevast", "L_SimonsWood", 
		"L_HelmansSip",	"L_BoulderCape", "L_RaccoonRock", "L_AnresBeacon", "L_JuniperPoint", 
		"L_MartinsWatch",	"L_DriftwoodSands", "H_SixBells", "V_OldWood", 
		"V_NewWood", "V_OreRidge", "V_TyroneRidge", "V_MortonValley", "V_WesternHeights", "H_LongHill", 
		"H_ProwPeak", "H_Highstone", "H_SpruceHill", "H_CalvaryHill", "H_GallowsHill", "H_BonfireHill",
		"H_HumboldtHill", "H_PickMountain", "H_HedgehogHill", "H_SwellMountain", "H_MargaretsMount"
	};

	static const ref array<string> EXFIL_LOCATIONS = 
	{
		// same type as spawnlocations but these are too rugged to sensibly place a starting vehicle and arsenal
		"L_Coubet", "L_LaChalette", "L_CoalmansBrow", "L_RockweedCape", "L_SeagullPoint", "L_StubwoodPoint", "H_LacansHead", "H_MarysMount", "L_ChevalGinLodge", "L_SchoonersEnd"
	};
}
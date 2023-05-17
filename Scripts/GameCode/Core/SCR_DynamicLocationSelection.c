class SCR_DynamicLocationSelection
{
	protected ref RandomGenerator m_random;
	protected ref map<string,string> m_locationEntities = new map<string,string>();
	
	protected static const ref array<string> m_possibleMissionLocations = 
	{
		// locations that have the standardized SCR_SiteSlotEntity elemements in the hierarchy
		
		// Towns
		"C_Location_St_Pierre", "C_LocationChotain", "C_LocationDurras", "C_LocationEntreDeux", 
		"C_LocationFigari", "C_LocationGravette", "C_LocationLamentin", "C_LocationLaruns", 
		"C_LocationLeMoule", "C_LocationLevie", "C_LocationMeaux", "C_LocationMontignac", 
		"C_LocationMorton", "C_LocationProvins", "C_LocationRegina", "C_LocationStPhilippe", 
		"C_LocationTyrone", "C_LocationVernon", "C_LocationVilleneuve",
		
		// Villages
		"S_Kermovan", "S_Lancre", 
		
		// Misc
		"G_Airport", "G_Farm2", "G_Farm3", "G_Industrial", "G_Industrial3", "G_Landfill",
		"G_Military", "G_Military3", "G_Military5", "G_Military6", "G_Quarry", "G_Sawmill", 
		"G_MontfortCastle", "Barracks"
	};
	
	protected static const ref array<string> m_possibleMissionLocations1 = 
	{
		"C_LocationChotain","C_LocationLaruns","Barracks"
	};

	protected static const ref array<string> m_patrolLocations = 
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
	
	protected static const ref array<string> m_spawnLocations = 
	{
		// location in the wilderness where teams could plausibly infiltrate via car/boat/heli/foot
		
		"G_HuntsmansHeath", "L_LaRoue", "L_Scythe", "L_CragPoint", "L_Thollevast", "L_SimonsWood", 
		"L_HelmansSip",	"L_BoulderCape", "L_RaccoonRock", "L_AnresBeacon", "L_JuniperPoint", "L_SeagullPoint", 
		"L_SchoonersEnd", "L_MartinsWatch", "L_ChevalGinLodge",	"L_DriftwoodSands", "H_SixBells", "V_OldWood", 
		"V_NewWood", "V_OreRidge", "V_TyroneRidge", "V_MortonValley", "V_WesternHeights", "H_LongHill", 
		"H_ProwPeak", "H_Highstone", "H_SpruceHill", "H_CalvaryHill", "H_GallowsHill", "H_BonfireHill",
		"H_HumboldtHill", "H_PickMountain", "H_HedgehogHill", "H_SwellMountain", "H_MargaretsMount"
	};

	protected static const ref array<string> m_exfilLocations = 
	{
		// same as spawnlocation but these are too rugged to sensibly place a starting vehicle and arsnenal
		"L_Coubet", "L_LaChalette", "L_CoalmansBrow", "L_RockweedCape", "L_StubwoodPoint", "H_LacansHead", "H_MarysMount"
	};
	
	void SCR_DynamicLocationSelection()
	{
		m_random = new RandomGenerator();
		FillLocationMap();
	}
	
	void ~SCR_DynamicLocationSelection()
	{
		
	}
	
	void GetLocations(out array<ref SCR_Location> missionLocationArray)
	{
		// select random Location
		int randomIndex = m_random.RandInt(0, m_possibleMissionLocations.Count());
		IEntity initialLocationEntity = GetGame().GetWorld().FindEntityByName(m_possibleMissionLocations[randomIndex]);
		array<IEntity> possibleSecondaryMissionLocations = new array<IEntity>();
		
		// filter all locations based on range to get other mission locations not too close or too far
		foreach(string missionLocation : m_possibleMissionLocations)
		{
			IEntity locationEntity = GetGame().GetWorld().FindEntityByName(missionLocation);
			int distance = GetDistanceBetweenEntities(initialLocationEntity, locationEntity);
			
			if(distance < 2500 && distance > 500)
			{
				possibleSecondaryMissionLocations.Insert(locationEntity);
			}
		}
		
		missionLocationArray.Insert(GetLocation(initialLocationEntity));
		for (int i = 1; i < SCR_DynamicOperations.AMOUNT_OF_MISSIONS; i++)
		{
			int randomInt = m_random.RandInt(0, possibleSecondaryMissionLocations.Count());
			ref SCR_Location loc = GetLocation(possibleSecondaryMissionLocations[randomInt]);
			missionLocationArray.Insert(loc);
			possibleSecondaryMissionLocations.Remove(randomInt);
		}
				
		foreach(SCR_Location location : missionLocationArray)
		{
			GetPatrolLocations(location);
		}
	}
	
	SCR_Location GetLocation(IEntity locationEntity)
	{
		string entityName = locationEntity.GetName();
		string mappedName = m_locationEntities.Get(entityName);
		IEntity mappedEntity = GetGame().GetWorld().FindEntityByName(mappedName);
		ref SCR_Location loc = new SCR_Location(entityName, locationEntity, mappedEntity);
		return loc;
	}
	
	// Get all patrol locations between 300 and 1000 meters of the location and add them to the location
	protected void GetPatrolLocations(SCR_Location location)
	{
		foreach(string patrolLocation : m_patrolLocations)
		{
			IEntity patrolLocationEntity = GetGame().GetWorld().FindEntityByName(patrolLocation);
			int distance = GetDistanceBetweenEntities(location.GetEntity(), patrolLocationEntity);
			
			if(distance < 1000 && distance > 300 )
			{
				location.AddPatrolLocation(patrolLocationEntity.GetOrigin());
			}
		}
	}
	
	array<IEntity> GetSpawnLocations(array<ref SCR_Location> missionLocations, bool includeExfil)
	{
		array<IEntity> spawnArray = new array<IEntity>();
		// get all locations within 1500 meters from all objectives
		array<IEntity> possibleSpawnLocations = new array<IEntity>();
		
		foreach(SCR_Location missionLocation : missionLocations)
		{
			foreach(string spawnLocation : m_spawnLocations)
			{
				IEntity locationEntity = GetGame().GetWorld().FindEntityByName(spawnLocation);
				int distance = GetDistanceBetweenEntities(missionLocation.GetEntity(), locationEntity);
				
				// filter out initial location
				if(distance < 1500 && distance > 1)
				{
					possibleSpawnLocations.Insert(locationEntity);
				}
			}
			if(includeExfil)
			{
				foreach(string spawnLocation : m_exfilLocations)
				{
					IEntity locationEntity = GetGame().GetWorld().FindEntityByName(spawnLocation);
					int distance = GetDistanceBetweenEntities(missionLocation.GetEntity(), locationEntity);
					
					// filter out initial location
					if(distance < 1500 && distance > 1)
					{
						possibleSpawnLocations.Insert(locationEntity);
					}
				}
			}
		}
		
		array<IEntity> spawnLocations = new array<IEntity>();
		
		// remove all locations within 1000 meters from all objectives
		foreach(SCR_Location missionLocation : missionLocations)
		{
			foreach(IEntity spawnLocation : possibleSpawnLocations)
			{
				int distance = GetDistanceBetweenEntities(missionLocation.GetEntity(), spawnLocation);
				
				// filter out initial location
				if(distance > 1000)
				{
					spawnLocations.Insert(spawnLocation);
				}
			}
		}
		
		// select 2 random Locations
		if(spawnLocations.Count() == 0)
		{
			spawnArray.Insert(GetGame().GetWorld().FindEntityByName("L_SimonsWood"));
			return spawnArray;
		}
		int firstRandom = m_random.RandInt(0, spawnLocations.Count());
		spawnArray.Insert(spawnLocations[firstRandom]);
		int spawnLocationIndex = m_spawnLocations.Find(spawnLocations[firstRandom].GetName());
		m_spawnLocations.Remove(spawnLocationIndex);
		spawnLocations.Remove(firstRandom);
		
		
		if(spawnLocations.Count() == 0)
		{
			return spawnArray;
		}
		int secondRandom = m_random.RandInt(0, spawnLocations.Count());
		spawnArray.Insert(spawnLocations[secondRandom]);
		spawnLocationIndex = m_spawnLocations.Find(spawnLocations[secondRandom].GetName());
		m_spawnLocations.Remove(spawnLocationIndex);
		return spawnArray;
	}
	
		
	protected int GetDistanceBetweenEntities(IEntity entity1, IEntity entity2)
	{
		vector location1 = entity1.GetOrigin();
		vector location2 = entity2.GetOrigin();
		return vector.Distance(location1, location2);
	}
				
	protected void FillLocationMap()
	{
		m_locationEntities.Set("C_Location_St_Pierre", "SaintPierreSlots");
		m_locationEntities.Set("C_LocationChotain", "Chotain");
		m_locationEntities.Set("C_LocationDurras", "Durras");
		m_locationEntities.Set("C_LocationEntreDeux", "EntreDeux");
		m_locationEntities.Set("C_LocationFigari", "Figari");
		m_locationEntities.Set("C_LocationGravette", "Gravette");
		m_locationEntities.Set("C_LocationLamentin", "Lamentin");
		m_locationEntities.Set("C_LocationLaruns", "Laruns");
		m_locationEntities.Set("C_LocationLeMoule", "LeMoule");
		m_locationEntities.Set("C_LocationLevie", "Levie");
		m_locationEntities.Set("C_LocationMeaux", "Meaux");
		m_locationEntities.Set("C_LocationMontignac", "Montignac");
		m_locationEntities.Set("C_LocationMorton", "Morton");
		m_locationEntities.Set("C_LocationProvins", "Provins");
		m_locationEntities.Set("C_LocationRegina", "Regina");
		m_locationEntities.Set("C_LocationStPhilippe", "SaintPhillipe");
		m_locationEntities.Set("C_LocationTyrone", "Tyrone");
		m_locationEntities.Set("C_LocationVernon", "Vernon");
		m_locationEntities.Set("C_LocationVilleneuve", "Villeneuf");
		m_locationEntities.Set("S_Kermovan", "FishingSettlement_N");
		m_locationEntities.Set("Lancre", "FishingSettlement_S");
		m_locationEntities.Set("G_Airport", "Airfield");
		m_locationEntities.Set("G_Farm2", "Farm_Meaux");
		m_locationEntities.Set("G_Farm3", "Farm_Montignac");
		m_locationEntities.Set("G_Industrial", "CementFactory_01");
		m_locationEntities.Set("G_Industrial3", "CementFactory_02");
		m_locationEntities.Set("G_Landfill", "Landfill");
		m_locationEntities.Set("G_Military", "Base_Chotain");
		m_locationEntities.Set("G_Military3", "ShootingRange");
		m_locationEntities.Set("G_Military5", "Base_SaintPhillipe");
		m_locationEntities.Set("G_Military6", "Base_Levie");
		m_locationEntities.Set("G_Quarry", "Quarry");
		m_locationEntities.Set("G_Sawmill", "Sawmill");
		m_locationEntities.Set("L_MontfortCastle", "Castle");
		m_locationEntities.Set("Barracks", "Barracks");
	}
}
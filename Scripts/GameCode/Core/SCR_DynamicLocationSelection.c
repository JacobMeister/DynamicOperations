class SCR_DynamicLocationSelection
{
	protected ref RandomGenerator m_random;
	protected ref map<string,string> m_locationEntities = new map<string,string>();
	
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
		int randomIndex = m_random.RandInt(0, SCR_DynamicOperationsConstants.POSSIBLE_MISSION_LOCATIONS.Count());
		string initialLocationName = SCR_DynamicOperationsConstants.POSSIBLE_MISSION_LOCATIONS[randomIndex];
		IEntity initialLocationEntity = GetGame().GetWorld().FindEntityByName(initialLocationName);
		array<string> possibleSecondaryMissionLocations = new array<string>();
		
		// filter all locations based on range to get other mission locations not too close or too far
		foreach(string missionLocation : SCR_DynamicOperationsConstants.POSSIBLE_MISSION_LOCATIONS)
		{
			IEntity locationEntity = GetGame().GetWorld().FindEntityByName(missionLocation);
			int distance = GetDistanceBetweenEntities(initialLocationEntity, locationEntity);
			
			if(distance < 2500 && distance > 500)
			{
				possibleSecondaryMissionLocations.Insert(missionLocation);
			}
		}
		
		missionLocationArray.Insert(GetLocation(initialLocationName));
		for (int i = 1; i < SCR_DynamicOperationsConstants.AMOUNT_OF_MISSIONS; i++)
		{
			int randomInt = m_random.RandInt(0, possibleSecondaryMissionLocations.Count());
			string possibleSecondaryMissionLocation = possibleSecondaryMissionLocations[randomInt];
			// if for any possible reason this string is null, remove it from the list, and do it again
			if(!possibleSecondaryMissionLocation)
			{
				possibleSecondaryMissionLocations.RemoveOrdered(randomInt);
				i--;
				continue;
			}
			ref SCR_Location loc = GetLocation(possibleSecondaryMissionLocation);
			missionLocationArray.Insert(loc);
			possibleSecondaryMissionLocations.RemoveOrdered(randomInt);
		}
				
		foreach(SCR_Location location : missionLocationArray)
		{
			GetPatrolLocations(location);
			GetDeliveryLocations(location);
		}
	}
	
	SCR_Location GetLocation(string locationName)
	{
		IEntity locationEntity = GetGame().GetWorld().FindEntityByName(locationName);
		string mappedName = m_locationEntities.Get(locationName);
		IEntity mappedEntity = GetGame().GetWorld().FindEntityByName(mappedName);
		if(!locationEntity || !mappedEntity)
		{
			if(!locationEntity)
			{
				Print("Could not find entity in world: " + locationName, LogLevel.ERROR);
			}
			else
			{
				Print("Could not find entity in world: " + mappedName, LogLevel.ERROR);
			}
			
		}
		ref SCR_Location loc = new SCR_Location(locationName, locationEntity, mappedEntity);
		return loc;
	}
	
	// Get all patrol locations between 300 and 1500 meters of the location and add them to the location
	protected void GetPatrolLocations(SCR_Location location)
	{
		foreach(string patrolLocation : SCR_DynamicOperationsConstants.PATROL_LOCATIONS)
		{
			IEntity patrolLocationEntity = GetGame().GetWorld().FindEntityByName(patrolLocation);
			int distance = GetDistanceBetweenEntities(location.GetEntity(), patrolLocationEntity);
			
			if(distance < 1500 && distance > 300 )
			{
				location.AddPatrolLocation(patrolLocationEntity.GetOrigin());
			}
		}
	}
	
	// Get all patrol locations between 2500 and 3500 meters of the location and add them to the location
	protected void GetDeliveryLocations(SCR_Location location)
	{
		foreach(string patrolLocation : SCR_DynamicOperationsConstants.PATROL_LOCATIONS)
		{
			IEntity patrolLocationEntity = GetGame().GetWorld().FindEntityByName(patrolLocation);
			int distance = GetDistanceBetweenEntities(location.GetEntity(), patrolLocationEntity);
			
			if(distance < 3500 && distance > 2500 )
			{
				location.AddDeliveryLocation(patrolLocationEntity.GetOrigin());
			}
		}
	}
	
	array<IEntity> GetSpawnLocations(array<ref SCR_Location> missionLocations, bool includeExfil)
	{
		// This method attempts to get two spawn/exfil locations that are close enough to the AO to be helpful, 
		// but not too close to cause patrols to instantly find and kill the spawning players
		
		array<IEntity> spawnArray = new array<IEntity>();
		// get all locations within 2000 meters from all objectives
		set<IEntity> possibleSpawnLocations = new set<IEntity>();
		
		foreach(SCR_Location missionLocation : missionLocations)
		{
			foreach(string spawnLocation : SCR_DynamicOperationsConstants.SPAWN_LOCATIONS)
			{
				IEntity locationEntity = GetGame().GetWorld().FindEntityByName(spawnLocation);
				int distance = GetDistanceBetweenEntities(missionLocation.GetEntity(), locationEntity);
				
				if(distance < 2000 && distance > 1)
				{
					possibleSpawnLocations.Insert(locationEntity);
				}
			}
			if(includeExfil)
			{
				foreach(string spawnLocation : SCR_DynamicOperationsConstants.EXFIL_LOCATIONS)
				{
					IEntity locationEntity = GetGame().GetWorld().FindEntityByName(spawnLocation);
					int distance = GetDistanceBetweenEntities(missionLocation.GetEntity(), locationEntity);
					
					if(distance < 2000 && distance > 1)
					{
						possibleSpawnLocations.Insert(locationEntity);
					}
				}
			}
		}
		
		// remove all locations within 800 meters from all objectives
		foreach(SCR_Location missionLocation : missionLocations)
		{
			for(int i = possibleSpawnLocations.Count() - 1; i > 0; i--)
			{
				if(!possibleSpawnLocations[i])
				{
					continue;
				}
				int distance = GetDistanceBetweenEntities(missionLocation.GetEntity(), possibleSpawnLocations[i]);
				
				if(distance < 800)
				{
					possibleSpawnLocations.Remove(i);
				}
			}
		}
		
		// select 2 random Locations
		// Select Simonswood as backup location for both spawn and extraction location if no locations were found
		if(possibleSpawnLocations.Count() == 0)
		{
			spawnArray.Insert(GetGame().GetWorld().FindEntityByName("L_SimonsWood"));
			return spawnArray;
		}
		int firstRandom = m_random.RandInt(0, possibleSpawnLocations.Count());
		spawnArray.Insert(possibleSpawnLocations[firstRandom]);
		string name = possibleSpawnLocations[firstRandom].GetName();
		int spawnLocationIndex = SCR_DynamicOperationsConstants.SPAWN_LOCATIONS.Find(name);
		if(spawnLocationIndex != -1)
		{
			// remove it from original list so an exfil location cannot be created on a place where a spawn is located
			SCR_DynamicOperationsConstants.SPAWN_LOCATIONS.Remove(spawnLocationIndex);
			possibleSpawnLocations.Remove(firstRandom);
		}
		else
		{
			spawnLocationIndex = SCR_DynamicOperationsConstants.EXFIL_LOCATIONS.Find(name);
			SCR_DynamicOperationsConstants.EXFIL_LOCATIONS.Remove(spawnLocationIndex);
			possibleSpawnLocations.Remove(firstRandom);
		}
		
		if(possibleSpawnLocations.Count() == 0)
		{
			return spawnArray;
		}
		int secondRandom = m_random.RandInt(0, possibleSpawnLocations.Count());
		spawnArray.Insert(possibleSpawnLocations[secondRandom]);
		spawnLocationIndex = SCR_DynamicOperationsConstants.SPAWN_LOCATIONS.Find(possibleSpawnLocations[secondRandom].GetName());
		if(spawnLocationIndex != -1)
		{
			SCR_DynamicOperationsConstants.SPAWN_LOCATIONS.Remove(spawnLocationIndex);
			possibleSpawnLocations.Remove(secondRandom);
		}
		else
		{
			spawnLocationIndex = SCR_DynamicOperationsConstants.EXFIL_LOCATIONS.Find(name);
			SCR_DynamicOperationsConstants.EXFIL_LOCATIONS.Remove(spawnLocationIndex);
			possibleSpawnLocations.Remove(secondRandom);
		}
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
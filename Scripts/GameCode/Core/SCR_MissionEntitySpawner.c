class SCR_MissionEntitySpawner
{
	protected ref SCR_AISpawner m_aiSpawner;
	protected ref RandomGenerator m_random;
	protected ref array<string> m_vehicleTypes = 
	{
		"{259EE7B78C51B624}Prefabs/Vehicles/Wheeled/UAZ469/UAZ469.et",
		"{99F1610551D54D17}Prefabs/Vehicles/Wheeled/UAZ469/UAZ469_base.et",
		"{16C1F16C9B053801}Prefabs/Vehicles/Wheeled/Ural4320/Ural4320_transport.et",
		"{D9B91FAB817A6033}Prefabs/Vehicles/Wheeled/Ural4320/Ural4320_transport_covered.et"
	};
	
	protected ref array<string> m_smallSlotPrefabs = 
	{
		"{7C20041DA3207191}Prefabs/Compositions/Slotted/SlotFlatSmall/AmmoStorage_S_USSR_01.et",
		"{68ED84218EA1BDC3}Prefabs/Compositions/Slotted/SlotFlatSmall/Antenna_S_USSR_01.et",
		"{7492BAA88AFCEDCD}Prefabs/Compositions/Slotted/SlotFlatSmall/Bunker_S_USSR_01.et",
		"{E7C062E5BE16FA11}Prefabs/Compositions/Slotted/SlotFlatSmall/FuelStorage_S_USSR_01.et",
		"{FD952CFF9CAD3F2A}Prefabs/Compositions/Slotted/SlotFlatSmall/LivingArea_S_USSR_01.et",
		"{114DE81321786CD9}Prefabs/Compositions/Slotted/SlotFlatSmall/MachineGunNest_S_USSR_01_PKM.et",
		"{FFD9E8A7CD2592B5}Prefabs/Compositions/Slotted/SlotFlatSmall/SandbagPosition_S_USSR_03.et",
		"{4093AE33DCEFAD67}Prefabs/Compositions/Slotted/SlotFlatSmall/SupplyStorage_S_USSR_01.et",
		"{A047DA062D3DA650}Prefabs/Compositions/Slotted/SlotFlatSmall/VehicleMaintenance_S_USSR_01.et"
	};
	
	protected const string smallRoadCheckpoint = "{9483333BFD9E2D0F}Prefabs/Compositions/Slotted/SlotRoadSmall/Checkpoint_S_USSR_01.et";
	protected const string mediumRoadCheckPoint = "{7C85836D444E3797}Prefabs/Compositions/Slotted/SlotRoadMedium/Checkpoint_M_USSR_01.et";
	protected const string largeRoadCheckPoint = "{9F9924B626C5FA2C}Prefabs/Compositions/Slotted/SlotRoadLarge/Checkpoint_L_USSR_01.et";
	
	void SCR_MissionEntitySpawner()
	{
		m_aiSpawner = new SCR_AISpawner();
		m_random = new RandomGenerator();
	}
	
	void ~SCR_MissionEntitySpawner()
	{
		
	}
	
	void SpawnMissionEntities(array<ref SCR_Location> missionLocations)
	{
		foreach(SCR_Location location : missionLocations)
		{
			SCR_SlotManager slotManager = location.GetSlotManager();
			SpawnRoadEntities(slotManager);
			SpawnVehiclesInMissionArea(slotManager);
			SpawnSoldiersInMissionArea(location);
			SpawnBuildingsInMissionArea(slotManager);
			Print("Spawned mission area in " + location.GetName());
		}
	}
	
	protected void SpawnRoadEntities(SCR_SlotManager slotManager)
	{
		// spawn road slots + soldiers
		int count = 0;
		while(true)
		{
			SCR_SiteSlotEntity slot = slotManager.GetRoadSlot();
			
			if(!slot)
			{
				break;
			}
			
			//Spawn correct size checkpoint on slot
			string prefabName = slot.GetPrefabData().GetPrefabName();
			string resourceName;
			
			switch (prefabName)
			{
				case "{66265A506DF1FE2B}PrefabsEditable/Slots/Road/E_SlotRoadSmall.et":
					resourceName = smallRoadCheckpoint;
					break;
		
				case "{F4066B8425BEC47C}PrefabsEditable/Slots/Road/E_SlotRoadMedium.et":
					resourceName = mediumRoadCheckPoint;
					break;
				
				case "{881CE7B0098504C2}PrefabsEditable/Slots/Road/E_SlotRoadLarge.et":
					resourceName = largeRoadCheckPoint;
					break;
			
				default:
					resourceName = smallRoadCheckpoint;
					break;
			}
		
			SCR_SpawnSetup.SpawnPrefabOnSlot(slot, resourceName, "SpawnRoadEntities");
			
			// only spawn 3 soldiers on checkpoints to prevent enormous amount of soldiers in larger towns with more road slots
			if(count < 3)
			{
				m_aiSpawner.SpawnAI("{CB58D90EA14430AD}Prefabs/Groups/OPFOR/Group_USSR_SentryTeam.et", slot.GetOrigin(), "{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et", slot.GetOrigin());
			}
			count++;
			slotManager.MarkAsUsed(slot);
		}
	}
	
	protected void SpawnVehiclesInMissionArea(SCR_SlotManager slotManager)
	{
		// Spawn at least 2 vehicles in every mission area, 2 more if mission area is > 25 slots
		int vehicleCount = 2;
		if(slotManager.GetAmountOfSlots() > 25)
		{
			vehicleCount + 2;
		}
		
		for(int i = 0; i < vehicleCount; i++)
		{
			
			SCR_SiteSlotEntity slot = slotManager.GetRandomSlot();
			string randomVehicle = m_vehicleTypes[m_random.RandInt(0,m_vehicleTypes.Count())];
			IEntity success = SCR_SpawnSetup.SpawnPrefabOnSlot(slot, randomVehicle, "SpawnVehiclesInMissionArea");
			
			if(success)
			{
				slotManager.MarkAsUsed(slot);
			}
		}
	}
	
	protected void SpawnSoldiersInMissionArea(SCR_Location location)
	{
		// get total amount of soldiers that can be spawned in locaiton
		int amountOfSoldiers = location.GetAmountOfSoldiers();
				
		// divide soldiers between the different available slots
		array<int> slots = new array<int>();
		while (amountOfSoldiers >= 0)
		{
			if(amountOfSoldiers < 4)
			{
				slots.Insert(amountOfSoldiers);
				amountOfSoldiers = 0;
				break;
			}
			
			// we don't want groups < 2 or > 4
			int groupSize = m_random.RandIntInclusive(2,4);
			slots.Insert(groupSize);
			amountOfSoldiers -= groupSize;
		}
		
		SCR_SlotManager slotManager = location.GetSlotManager();
		foreach(int soldierCount : slots)
		{
			SCR_SiteSlotEntity slot = slotManager.GetRandomSlot();
			
			// spawn prefab on slots
			SCR_SpawnSetup.SpawnPrefabOnSlot(slot, m_smallSlotPrefabs[m_random.RandInt(0,m_smallSlotPrefabs.Count())], "SpawnSoldiersInMissionArea/Prefabs");
			
			// spawn soldiers on slots 
			switch (soldierCount)
			{
				case 2:
					m_aiSpawner.SpawnAIWithoutWaypoint(SCR_Prefab_Constants.ussr_medicalSection, slot.GetOrigin());
					break;
				case 3:
					m_aiSpawner.SpawnAIWithoutWaypoint(SCR_Prefab_Constants.ussr_lightFireTeam, slot.GetOrigin());
					break;
				case 4:
					m_aiSpawner.SpawnAIWithoutWaypoint(SCR_Prefab_Constants.ussr_fireGroup, slot.GetOrigin());
					break;
				default:
					break;
			}
			slotManager.MarkAsUsed(slot);
		}
	}
	
	protected void SpawnBuildingsInMissionArea(SCR_SlotManager slotManager)
	{
		// Fill a third of leftover slots with buildings for players to clear
		int amountOfSlotsAvailable = slotManager.GetAvailableSlots();
		for(int i = 0; i < Math.Round(amountOfSlotsAvailable/3); i++)
		{
			SCR_SiteSlotEntity slot = slotManager.GetRandomSlot();
			IEntity success = SCR_SpawnSetup.SpawnPrefabOnSlot(slot, m_smallSlotPrefabs[m_random.RandInt(0,m_smallSlotPrefabs.Count())], "SpawnBuildingsInMissionArea");
			if(success)
			{
				slotManager.MarkAsUsed(slot);
			}
			
		}
	}
}

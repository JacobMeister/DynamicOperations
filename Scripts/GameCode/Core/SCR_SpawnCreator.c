class SCR_SpawnCreator
{
	private int m_iGeneratedSpawnLocationOffset = 300;
	private ref RandomGenerator m_random;
	
	void SCR_SpawnCreator()
	{
		m_random = new RandomGenerator();
	}
	
	void ~SCR_SpawnCreator()
	{
		
	}
	
	void CreateExtraSpawnPointTrigger(SCR_Location location)
	{
		vector locationOrigin = location.GetEntity().GetOrigin();
		//Get 4 points surrounding location
		map<string,vector> vectors = new map<string,vector>();
		vector vector1 = locationOrigin;
		vector1[0] = vector1[0] + m_iGeneratedSpawnLocationOffset;
		vector1[1] = GetGame().GetWorld().GetSurfaceY(vector1[0], vector1[2]);
		vectors.Insert("east", vector1);
		vector vector2 = locationOrigin;
		vector2[0] = vector2[0] - m_iGeneratedSpawnLocationOffset;
		vector2[1] = GetGame().GetWorld().GetSurfaceY(vector2[0], vector2[2]);
		vectors.Insert("west", vector2);
		vector vector3 = locationOrigin;
		vector3[2] = vector3[2] + m_iGeneratedSpawnLocationOffset;
		vector3[1] = GetGame().GetWorld().GetSurfaceY(vector3[0], vector3[2]);
		vectors.Insert("north", vector3);
		vector vector4 = locationOrigin;
		vector4[2] = vector4[2] - m_iGeneratedSpawnLocationOffset;
		vector4[1] = GetGame().GetWorld().GetSurfaceY(vector4[0], vector4[2]);
		vectors.Insert("south", vector4);
		
		// check if they are in the ocean
		foreach(auto name, auto vec : vectors)
		{
			if(vec[1] > 1)
			{
				string layerName = location.GetName() + name;
				// Create area
				IEntity area = SCR_SpawnSetup.SpawnEntity(vec, "{C72F956E4AC6A6E7}Prefabs/ScenarioFramework/Components/Area.et", "CreateExtraSpawnPointTrigger/SpawnArea");
				// Create Layer with spawn slot
				vector z = {0,0,0};
				IEntity layerEntity = SCR_SpawnSetup.SpawnEntity(z, "{5F9FFF4BF027B3A3}Prefabs/ScenarioFramework/Components/Layer.et", "CreateExtraSpawnPointTrigger/SpawnLayer", false, ETransformMode.LOCAL);
				SCR_ScenarioFrameworkLayerBase layer = SCR_ScenarioFrameworkLayerBase.Cast(layerEntity.FindComponent(SCR_ScenarioFrameworkLayerBase));
				layer.SetActivationType(SCR_ScenarioFrameworkEActivationType.ON_TRIGGER_ACTIVATION);
				layerEntity.SetName(layerName);
				area.AddChild(layerEntity, -1);
				
				IEntity spawnSlotEntity =  SCR_SpawnSetup.SpawnEntity(z, "{AA01691FDC4E9167}Prefabs/ScenarioFramework/Components/Slot.et", "CreateExtraSpawnPointTrigger/SpawnSlot", false, ETransformMode.LOCAL);
				SCR_ScenarioFrameworkSlotBase slot = SCR_ScenarioFrameworkSlotBase.Cast(spawnSlotEntity.FindComponent(SCR_ScenarioFrameworkSlotBase));
				slot.SetObjectToSpawn("{CEA2B24051A44525}PrefabsEditable/SpawnPoints/E_SpawnPoint_US.et");
				layerEntity.AddChild(spawnSlotEntity, -1);
				
				// create SlotTrigger
				IEntity slotTriggerEntity =  SCR_SpawnSetup.SpawnEntity(z, "{D183AFE8DAF9793D}Prefabs/ScenarioFramework/Components/SlotTrigger.et", "CreateExtraSpawnPointTrigger/SpawnSlotTrigger", false, ETransformMode.LOCAL);
				SCR_ScenarioFrameworkSlotTrigger slotTrigger = SCR_ScenarioFrameworkSlotTrigger.Cast(slotTriggerEntity.FindComponent(SCR_ScenarioFrameworkSlotTrigger));
				area.AddChild(slotTriggerEntity, -1);
								
				// Add trigger to area to activate slotTrigger
				SCR_ScenarioFrameworkPluginTrigger trigger = new SCR_ScenarioFrameworkPluginTrigger();
				trigger.CreateCustomTriggerArray();
				trigger.SetRadius(Math.Sqrt(2) * m_iGeneratedSpawnLocationOffset / 2);
				trigger.SetTriggerOnce(true);
				slotTrigger.AddPlugin(trigger);
				
				// add spawnslotlayer to spawn object when triggered
				SCR_ScenarioFrameworkActionSpawnObjects triggerAction = new SCR_ScenarioFrameworkActionSpawnObjects();
				triggerAction.AddObjectsToSpawn(layerName);
				slotTrigger.AddAction(triggerAction);
			}
		}
	}
	
	void FurnishSpawnPoint(vector spawnPosition)
	{
		string arsenalEntityId = "{BE99D77D1155B127}Prefabs/Systems/Arsenal/ArsenalBoxes/US/ArsenalBox_US.et";
		string carEntityId = "{5168FEA3054D6D15}Prefabs/Vehicles/Wheeled/M151A2/M151A2_M2HB_MERDC.et";
		string tentEntityId = "{3A6623EADBD13358}Prefabs/Structures/Military/Camps/TentSmallUS_01/TentSmallUS_01_Base.et";
		string fireplaceEntityId = "{D9842C11742C00CF}Prefabs/Props/Civilian/Fireplace_01.et";
		string campFireEntityId = "{899360C930DA3EE3}Prefabs/Particles/Campfire_burning.et";
		
		Resource arsenalResource = SCR_SpawnSetup.GenerateAndValidateResource(arsenalEntityId);
		Resource carResource = SCR_SpawnSetup.GenerateAndValidateResource(carEntityId);
		
		if (!arsenalResource || !carResource)
		{
			Print(("[SpawnPoint] Unable able to load resource for the entities: " + arsenalEntityId + " or " + carEntityId), LogLevel.ERROR);
			return;
		}
				
		// move it some distance from the spawnpoint
		vector arsenalSpawnPosition = spawnPosition;
		arsenalSpawnPosition[0] = arsenalSpawnPosition[0] + 5;
		vector carSpawnPosition = spawnPosition;
		carSpawnPosition[0] = carSpawnPosition[0] - 5;
		vector tentSpawnPosition = spawnPosition;
		tentSpawnPosition[2] = tentSpawnPosition[2] + 5;
		vector campFireSpawnPosition = spawnPosition;
		campFireSpawnPosition[2] = campFireSpawnPosition[2] - 5;
		
		array<IEntity> spawnedEntities = new array<IEntity>();
		
		spawnedEntities.Insert(SCR_SpawnSetup.SpawnEntity(arsenalSpawnPosition, arsenalEntityId, "FurnishSpawnPoint/Arsenal"));
		spawnedEntities.Insert(SCR_SpawnSetup.SpawnEntity(carSpawnPosition, carEntityId, "FurnishSpawnPoint/Car"));
		spawnedEntities.Insert(SCR_SpawnSetup.SpawnEntity(tentSpawnPosition, tentEntityId, "FurnishSpawnPoint/Tent"));
		spawnedEntities.Insert(SCR_SpawnSetup.SpawnEntity(campFireSpawnPosition, fireplaceEntityId, "FurnishSpawnPoint/Fireplace"));
		spawnedEntities.Insert(SCR_SpawnSetup.SpawnEntity(campFireSpawnPosition, campFireEntityId, "FurnishSpawnPoint/Campfire"));
		
		foreach(IEntity entity : spawnedEntities)
		{
			vector angleVector = {0, m_random.RandInt(0,360), 0};
			entity.SetAngles(angleVector);
			SCR_SpawnSetup.SnapToTerrain(entity);
		}
	}
	
	// When entity is found this method will move it underground, thus clearing the area
	bool HideEntities(IEntity ent)
	{
		vector origin = ent.GetOrigin();
		origin[1] = 0;
		ent.SetOrigin(origin);
		return true;
	}	
	
	void CreateSpawnPoint(vector spawnPosition)
	{
		// hide all entities with a radius of 6 meters
		GetGame().GetWorld().QueryEntitiesBySphere(spawnPosition,6, HideEntities, null, EQueryEntitiesFlags.ALL);
		SCR_SpawnSetup.SpawnEntity(spawnPosition, "{5A2220DD0C16958B}Prefabs/MP/Spawning/SpawnPoint_US.et", "CreateSpawnPoint");
	}
};
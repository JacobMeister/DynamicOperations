class SCR_SpawnCreator
{
	private int m_iGeneratedSpawnLocationOffset = 300;
	
	void SCR_SpawnCreator()
	{
		
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
		
		Resource arsenalResource = SCR_SpawnSetup.GenerateAndValidateResource(arsenalEntityId);
		Resource carResource = SCR_SpawnSetup.GenerateAndValidateResource(carEntityId);
		
		if (!arsenalResource || !carResource)
		{
			Print(("[SpawnPoint] Unable able to load resource for the entities: " + arsenalEntityId + " or " + carEntityId), LogLevel.ERROR);
			return;
		}
		
		// move it some distance from the spawnpoint
		vector arsenalSpawnPosition = spawnPosition;
		arsenalSpawnPosition[0] = arsenalSpawnPosition[0] + 3;
		vector carSpawnPosition = spawnPosition;
		carSpawnPosition[0] = carSpawnPosition[0] - 3;
		
		if(GetGame().GetWorld().QueryEntitiesBySphere(carSpawnPosition,1, null))
		{
			carSpawnPosition[0] = carSpawnPosition[0] + 6;
		}
		
		IEntity arsenal = IEntity.Cast(GetGame().SpawnEntityPrefab(arsenalResource, null, SCR_SpawnSetup.GenerateSpawnParameters(arsenalSpawnPosition)));
		IEntity car = IEntity.Cast(GetGame().SpawnEntityPrefab(carResource, null, SCR_SpawnSetup.GenerateSpawnParameters(carSpawnPosition)));
		
		if (!arsenal || !car)
		{
			Print("[SpawnPoint] Unable to create entity!", LogLevel.ERROR);
			return;
		}
	}
	
	void CreateSpawnPoint(vector spawnPosition)
	{
		SCR_SpawnPoint spawnPoint = SCR_SpawnPoint.Cast(SCR_SpawnSetup.SpawnEntity(spawnPosition, "{5A2220DD0C16958B}Prefabs/MP/Spawning/SpawnPoint_US.et", "CreateSpawnPoint"));
		spawnPoint.ShowOnMap();
	}
};
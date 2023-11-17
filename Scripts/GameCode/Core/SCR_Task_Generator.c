class SCR_TaskGenerator
{
	protected ref array<string> m_missions = {};
	protected ref RandomGenerator m_random;
	protected ref SCR_AISpawner m_aiSpawner;
	protected ref map<string,string> m_destroyVehicles = new map<string,string>();
	protected ref map<string,string> m_hitList = new map<string,string>();
	protected ref array<ref array<string>> m_itemList = new array<ref array<string>>();
	
	void SCR_TaskGenerator()
	{
		m_random = new RandomGenerator();
		m_aiSpawner = new SCR_AISpawner();
		SetupValues();
	}
	
	void ~SCR_TaskGenerator()
	{
		
	}
	
	void CreateTasks(array<ref SCR_Location> missionLocations)
	{
		foreach(SCR_Location location : missionLocations)
		{
			SCR_SlotManager slotManager = location.GetSlotManager();
			SCR_SiteSlotEntity randomSlot = slotManager.GetRandomSlotSuitableForMissionTask();
			vector origin =  randomSlot.GetOrigin();
			int randomMissionIndex = m_random.RandInt(0,m_missions.Count());
			string selectedMission = m_missions[randomMissionIndex];
			m_missions.Remove(randomMissionIndex);
			IEntity area = SCR_SpawnSetup.SpawnEntity(randomSlot.GetOrigin(), SCR_Prefab_Constants.area, "CreateTasks/SpawnArea");
			slotManager.MarkAsUsed(randomSlot);
			
			switch (selectedMission)
			{
				case "Destroy Vehicle":
					CreateDestroyVehicleTask(randomSlot, area);
					break;
		
				case "Destroy Radar":
					CreateDestroyRadarTask(randomSlot, area);
					break;
				
				case "Kill Enemy":
					CreateKillEnemyTask(randomSlot, area);
					break;
				
				case "Clear Area":
					CreateClearAreaTask(randomSlot, area);
					break;
				
				case "Retrieve Item":
					CreateRetrieveTask(location, randomSlot);
					break;
		
				default:
					CreateKillEnemyTask(randomSlot, area);
					break;
			}
			Print("Created Task: " + selectedMission + " in " + location.GetName());
		}
	}
	
	void CreateDestroyVehicleTask(SCR_SiteSlotEntity slot, IEntity area)
	{
		vector origin =  slot.GetOrigin();
		
		// Spawn ScenarioFramework area/layer/slot
		IEntity layerTaskDestroy = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), SCR_Prefab_Constants.layerTaskDestroy, "CreateDestroyVehicleTask/SpawnLayerTaskDestroy", false);
		area.AddChild(layerTaskDestroy, -1);
		
		IEntity slotDestroy = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), SCR_Prefab_Constants.slotDestroy, "CreateDestroyVehicleTask/SpawnSLotDestroy", false);
		layerTaskDestroy.AddChild(slotDestroy, -1);
		
		SCR_ScenarioFrameworkSlotDestroy slotDestroyComponent = SCR_ScenarioFrameworkSlotDestroy.Cast(slotDestroy.FindComponent(SCR_ScenarioFrameworkSlotDestroy));
		string vehicleResourceName = m_destroyVehicles.GetKey(m_random.RandInt(0,m_destroyVehicles.Count()));
		string vehicleTaskDescription = m_destroyVehicles.Get(vehicleResourceName);
		
		IEntity vehicle = SCR_SpawnSetup.SpawnPrefabOnSlot(slot, vehicleResourceName, "CreateDestroyVehicleTask/SpawnVehicle");
		slotDestroyComponent.SetEntity(vehicle);
		slotDestroyComponent.SetTitleAndDescription("Destroy Vehicle", vehicleTaskDescription);
		//slotDestroyComponent.UseExistingAsset(true);
		//slotDestroyComponent.SetObjectToSpawn(vehicleResourceName);
		
		SpawnProtectionDetail(SCR_Prefab_Constants.ussr_machineGunTeam, origin);
		
		// Spawn camo net
		GetGame().GetCallqueue().CallLater(SCR_SpawnSetup.SpawnEntity, 10000, false, origin, SCR_Prefab_Constants.camoNet_large, "CreateDestroyVehicleTask", true, ETransformMode.WORLD);
	}
	
	void CreateDestroyRadarTask(SCR_SiteSlotEntity slot, IEntity area)
	{
		vector origin =  slot.GetOrigin();
		
		// Spawn ScenarioFramework area/layer/slot
		IEntity layerTaskDestroy = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), SCR_Prefab_Constants.layerTaskDestroy, "CreateDestroyVehicleTask/SpawnLayerTaskDestroy", false);
		area.AddChild(layerTaskDestroy, -1);
		
		IEntity slotDestroy = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), SCR_Prefab_Constants.slotDestroy, "CreateDestroyVehicleTask/SpawnSLotDestroy", false);
		layerTaskDestroy.AddChild(slotDestroy, -1);
		
		SCR_ScenarioFrameworkSlotDestroy slotDestroyComponent = SCR_ScenarioFrameworkSlotDestroy.Cast(slotDestroy.FindComponent(SCR_ScenarioFrameworkSlotDestroy));
		
		IEntity stand = SCR_SpawnSetup.SpawnAndMoveTo(origin, SCR_Prefab_Constants.approachRadar_tall, "CreateDestroyRadarTask/SpawnEntity");
		IEntity cables = SCR_SpawnSetup.SpawnAndMoveTo(origin, SCR_Prefab_Constants.approachRadar_cables, "CreateDestroyRadarTask/SpawnEntity");
		IEntity generator = SCR_SpawnSetup.SpawnAndMoveTo(origin, SCR_Prefab_Constants.approachRadar_generator, "CreateDestroyRadarTask/SpawnEntity");				
		vector topOfStandLocation = origin;
		topOfStandLocation[1] = topOfStandLocation[1] + 4.8;
		IEntity dish = SCR_SpawnSetup.SpawnAndMoveTo(topOfStandLocation, SCR_Prefab_Constants.approachRadar, "CreateDestroyRadarTask/SpawnEntity");
		slotDestroyComponent.SetEntity(dish);
		slotDestroyComponent.SetTitleAndDescription("Destroy Radar", "The soviets are setting up a radar system. Destroy it before it becomes operational.");
		//slotDestroyComponent.UseExistingAsset(true);
		//slotDestroyComponent.SetObjectToSpawn(SCR_Prefab_Constants.approachRadar);
		SpawnProtectionDetail(SCR_Prefab_Constants.ussr_atTeam, origin);
	}
		
	void CreateKillEnemyTask(SCR_SiteSlotEntity slot, IEntity area)
	{
		vector origin =  slot.GetOrigin();
		SCR_SpawnSetup.SpawnPrefabOnSlot(slot, SCR_Prefab_Constants.building_headquarters, "CreateKillEnemyTask/spawntent");
		
		// Spawn ScenarioFramework area/layer/slot
		IEntity layerTaskKillEnemy = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), SCR_Prefab_Constants.layerTaskKill, "CreateKillEnemyTask/SpawnLayerTaskKill", false);
		area.AddChild(layerTaskKillEnemy, -1);
		
		IEntity slotKill = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), SCR_Prefab_Constants.slotKill, "CreateKillEnemyTask/SpawnSLotKill", false);
		layerTaskKillEnemy.AddChild(slotKill, -1);
		
		SCR_ScenarioFrameworkSlotKill slotKillComponent = SCR_ScenarioFrameworkSlotKill.Cast(slotKill.FindComponent(SCR_ScenarioFrameworkSlotKill));
		
		string characterResourceName = m_hitList.GetKey(m_random.RandInt(0,m_hitList.Count()));
		string characterTaskDescription = m_hitList.Get(characterResourceName);
		IEntity character = IEntity.Cast(SCR_SpawnSetup.SpawnEntity(origin, characterResourceName, "CreateKillEnemyTask/SpawnEntity"));
		slotKillComponent.SetEntity(character);
		slotKillComponent.SetTitleAndDescription("Kill Enemy", characterTaskDescription);
		//slotKillComponent.UseExistingAsset(true);
		//slotKillComponent.SetObjectToSpawn(characterResourceName);
		
		SCR_ChimeraCharacter castCharacter = SCR_ChimeraCharacter.Cast(character);
		if(characterTaskDescription.Contains("spy"))
		{
			FactionAffiliationComponent fac = FactionAffiliationComponent.Cast(castCharacter.FindComponent(FactionAffiliationComponent));
			fac.SetAffiliatedFactionByKey("USSR");
		}
		SpawnProtectionDetail(SCR_Prefab_Constants.ussr_machineGunTeam, origin);
	}
	
	void CreateClearAreaTask(SCR_SiteSlotEntity slot, IEntity area)
	{
		vector origin =  slot.GetOrigin();
		SCR_SpawnSetup.SpawnPrefabOnSlot(slot, SCR_Prefab_Constants.building_supply_storage, "CreateClearAreaTask/spawntent");
		
		// Spawn ScenarioFramework area/layer/slot
		IEntity layerTaskClearArea = SCR_SpawnSetup.SpawnEntity(vector.Zero, SCR_Prefab_Constants.layerTaskClearArea, "CreateClearAreaTask/SpawnLayerTaskClearArea", false, ETransformMode.LOCAL);
		area.AddChild(layerTaskClearArea, -1);
		
		IEntity slotClearArea = SCR_SpawnSetup.SpawnEntity(vector.Zero, SCR_Prefab_Constants.slotClearArea, "CreateClearAreaTask/SpawnSLotClearArea", false, ETransformMode.LOCAL);
		layerTaskClearArea.AddChild(slotClearArea, -1);
		
		IEntity character = IEntity.Cast(SCR_SpawnSetup.SpawnEntity(origin, SCR_Prefab_Constants.ussr_rifleSquad, "CreateClearAreaTask/SpawnEntity"));
		
		SCR_ScenarioFrameworkSlotClearArea slotClearAreaComponent = SCR_ScenarioFrameworkSlotClearArea.Cast(slotClearArea.FindComponent(SCR_ScenarioFrameworkSlotClearArea));
		SCR_ScenarioFrameworkPluginTrigger pluginTrigger = new SCR_ScenarioFrameworkPluginTrigger();
		pluginTrigger.Setup();
		pluginTrigger.SetRadius(200);
		slotClearAreaComponent.SetTitleAndDescription("Clear Area", "Enemy forces are gathering for an attack on US forces, catch them off guard and take them out.");
		slotClearAreaComponent.AddPlugin(pluginTrigger);
		SpawnProtectionDetail(SCR_Prefab_Constants.ussr_machineGunTeam, origin);
	}
	
	void CreateRetrieveTask(SCR_Location location, SCR_SiteSlotEntity slot)
	{
		IEntity area = SCR_SpawnSetup.SpawnEntity(vector.Zero, SCR_Prefab_Constants.area, "CreateRetrieveTask/SpawnArea", false);
		// spawn command tent where objective will be found
		vector origin = slot.GetOrigin();
		SCR_SpawnSetup.SpawnEntity(origin, SCR_Prefab_Constants.building_headquarters, "CreateRetrieveTask/spawntent");
		
		// Spawn ScenarioFramework area/layer/slot
		string layerTaskName = "layerTaskDeliver_" + location.GetName();
		IEntity layerTaskDeliver = SCR_SpawnSetup.SpawnEntity(origin, SCR_Prefab_Constants.layerTaskDeliver, "CreateRetrieveTask/SpawnLayerTaskDeliver", true, ETransformMode.LOCAL);
		layerTaskDeliver.SetName(layerTaskName);
		area.AddChild(layerTaskDeliver, -1);
		
		int index = m_random.RandInt(0,m_itemList.Count());
		string itemResourceName = m_itemList[index][0];
		string taskItemName = m_itemList[index][1];
		string taskDescription = m_itemList[index][2];
		
		// spawn slotpick
		IEntity slotPickEntity = SCR_SpawnSetup.SpawnEntity(vector.Up, SCR_Prefab_Constants.slotPick, "CreateRetrieveTask/SpawnSlotPick", false, ETransformMode.LOCAL);
		layerTaskDeliver.AddChild(slotPickEntity, -1);
		SCR_ScenarioFrameworkSlotPick slotPick = SCR_ScenarioFrameworkSlotPick.Cast(slotPickEntity.FindComponent(SCR_ScenarioFrameworkSlotPick));
		slotPick.SetTitleAndDescriptions("Find " + taskItemName, taskDescription, "Deliver " + taskItemName, taskDescription, "Pick up " + taskItemName, taskDescription);
		slotPick.SetObjectToSpawn(itemResourceName);
		
		array<vector> deliveryLocations = location.GetDeliveryLocations();
		vector deliveryLocation = deliveryLocations[m_random.RandInt(0, deliveryLocations.Count())];
		
		IEntity slotDeliveryEntity = SCR_SpawnSetup.SpawnEntity(deliveryLocation, SCR_Prefab_Constants.slotDelivery, "CreateRetrieveTask/SpawnSlotDelivery");
		SCR_ScenarioFrameworkSlotDelivery slotDelivery = SCR_ScenarioFrameworkSlotDelivery.Cast(slotDeliveryEntity.FindComponent(SCR_ScenarioFrameworkSlotDelivery));
		SCR_ScenarioFrameworkPluginTrigger pluginTrigger = new SCR_ScenarioFrameworkPluginTrigger();
		pluginTrigger.Setup();
		pluginTrigger.SetRadius(15);
		pluginTrigger.SetTriggerOnce(false);
		slotDelivery.AddPlugin(pluginTrigger);
		slotDelivery.AddAssociatedLayerTask(layerTaskName);
		slotDelivery.DisableDebug();
		layerTaskDeliver.AddChild(slotDeliveryEntity, -1, EAddChildFlags.RECALC_LOCAL_TRANSFORM);
		
		// spawn delivery location friendlies + Vehicle
		SCR_SpawnSetup.SpawnEntity(deliveryLocation, SCR_Prefab_Constants.camo_humvee, "CreateRetrieveTask/SpawnDeliveryVehicle");
		SpawnProtectionDetail(SCR_Prefab_Constants.us_fireTeam, deliveryLocation);
		
		
		SpawnProtectionDetail(SCR_Prefab_Constants.ussr_machineGunTeam, slot.GetOrigin());
	}
	
	private void SpawnProtectionDetail(string resourceName, vector origin)
	{
		// spawn protection detail 5m to the side because they keep getting crushed by vehicle spawn
		vector protectionDetailSpawn = origin;
		protectionDetailSpawn[0] = protectionDetailSpawn[0] + 5;
		m_aiSpawner.SpawnAI(resourceName, protectionDetailSpawn, "{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et", origin);
	}
	
	private void SetupValues()
	{
		m_missions.Insert("Destroy Vehicle");
		m_missions.Insert("Destroy Radar");
		m_missions.Insert("Kill Enemy");
		m_missions.Insert("Retrieve Item");
		m_missions.Insert("Clear Area");
		m_destroyVehicles.Insert(SCR_Prefab_Constants.command_truck,"The soviets have set up a mobile command center. Destroy their command truck.");
		m_destroyVehicles.Insert(SCR_Prefab_Constants.btr,"A BTR70 has been spotten in the area of operations. Destroy the armored vehicle.");
		m_destroyVehicles.Insert(SCR_Prefab_Constants.fuel_truck,"Destroy the fuel truck to disrupt soviet logistics.");
		m_destroyVehicles.Insert(SCR_Prefab_Constants.supply_truck,"Destroy the supply truck to disrupt soviet logistics.");
		m_hitList.Insert(SCR_Prefab_Constants.us_rifleman, "A spy has been impersonating an american soldier to infiltrate US command. Kill the soviet spy.");
		m_hitList.Insert(SCR_Prefab_Constants.ussr_officer, "An enemy officer has spotted near your location. Take out the officer.");
		m_hitList.Insert(SCR_Prefab_Constants.ussr_sharpShooter, "A sniper has been targeting US officers. Kill the enemy sniper");
		array<string> intel = new array<string>();
		intel.InsertAll({SCR_Prefab_Constants.intelligence_folder, "Intelligence", "An enemy officer has been seen near your location. See if you can find more information on their plans."});
		m_itemList.Insert(intel);
		array<string> weapon = {SCR_Prefab_Constants.rifle, "Weapon", "A spy has stolen a US weapon. Retrieve the weapon before it can be shipped back to the USSR to be reverse engineered."};
		m_itemList.Insert(weapon);
		array<string> backPack = {SCR_Prefab_Constants.backpack, "BackPack", "A soviet messenger has hidden secret intelligence inside his backpack liner. Take it back to base so it can be searched."};
	}
};
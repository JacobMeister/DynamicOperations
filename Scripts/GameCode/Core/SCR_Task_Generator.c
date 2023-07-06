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
			IEntity area = SCR_SpawnSetup.SpawnEntity(randomSlot.GetOrigin(), "{C72F956E4AC6A6E7}Prefabs/ScenarioFramework/Components/Area.et", "CreateTasks/SpawnArea");
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
		IEntity layerTaskDestroy = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), "{5EDF39860639027D}Prefabs/ScenarioFramework/Components/LayerTaskDestroy.et", "CreateDestroyVehicleTask/SpawnLayerTaskDestroy", false);
		area.AddChild(layerTaskDestroy, -1);
		
		IEntity slotDestroy = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), "{7586595959BA2D99}Prefabs/ScenarioFramework/Components/SlotDestroy.et", "CreateDestroyVehicleTask/SpawnSLotDestroy", false);
		layerTaskDestroy.AddChild(slotDestroy, -1);
		
		SCR_ScenarioFrameworkSlotDestroy slotDestroyComponent = SCR_ScenarioFrameworkSlotDestroy.Cast(slotDestroy.FindComponent(SCR_ScenarioFrameworkSlotDestroy));
		string vehicleResourceName = m_destroyVehicles.GetKey(m_random.RandInt(0,m_destroyVehicles.Count()));
		string vehicleTaskDescription = m_destroyVehicles.Get(vehicleResourceName);
		
		IEntity vehicle = SCR_SpawnSetup.SpawnPrefabOnSlot(slot, vehicleResourceName, "CreateDestroyVehicleTask/SpawnVehicle");
		slotDestroyComponent.SetEntity(vehicle);
		slotDestroyComponent.SetTitleAndDescription("Destroy Vehicle", vehicleTaskDescription);
		
		SpawnProtectionDetail("{A2F75E45C66B1C0A}Prefabs/Groups/OPFOR/Group_USSR_MachineGunTeam.et", origin);
		
		// Spawn camo net
		GetGame().GetCallqueue().CallLater(SCR_SpawnSetup.SpawnEntity, 10000, false, origin, "{3C9119EDA99AAA76}Prefabs/Structures/Military/CamoNets/Soviet/CamoNet_Large_Top_Soviet.et", "CreateDestroyVehicleTask", true, ETransformMode.WORLD);
	}
	
	void CreateDestroyRadarTask(SCR_SiteSlotEntity slot, IEntity area)
	{
		//int r = m_random.RandIntInclusive(0,1);
		//if(r == 0)
		//{
		//	CreateDestroyAntennaTask(slot, area);
		//	return;
		//}
		vector origin =  slot.GetOrigin();
		
		// Spawn ScenarioFramework area/layer/slot
		IEntity layerTaskDestroy = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), "{5EDF39860639027D}Prefabs/ScenarioFramework/Components/LayerTaskDestroy.et", "CreateDestroyVehicleTask/SpawnLayerTaskDestroy", false);
		area.AddChild(layerTaskDestroy, -1);
		
		IEntity slotDestroy = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), "{7586595959BA2D99}Prefabs/ScenarioFramework/Components/SlotDestroy.et", "CreateDestroyVehicleTask/SpawnSLotDestroy", false);
		layerTaskDestroy.AddChild(slotDestroy, -1);
		
		SCR_ScenarioFrameworkSlotDestroy slotDestroyComponent = SCR_ScenarioFrameworkSlotDestroy.Cast(slotDestroy.FindComponent(SCR_ScenarioFrameworkSlotDestroy));
		
		IEntity stand = SCR_SpawnSetup.SpawnAndMoveTo(origin, "{979A564D476B5AAE}Prefabs/Structures/Military/Radar/ApproachRadar_RPL5_01/ApproachRadar_RPL5_01_tall.et", "CreateDestroyRadarTask/SpawnEntity");
		IEntity cables = SCR_SpawnSetup.SpawnAndMoveTo(origin, "{689E4B964E7FE377}Prefabs/Structures/Military/Radar/ApproachRadar_RPL5_01/ApproachRadar_RPL5_01_cables_tall.et", "CreateDestroyRadarTask/SpawnEntity");
		IEntity generator = SCR_SpawnSetup.SpawnAndMoveTo(origin, "{F15B99F6B06E7200}Prefabs/Structures/Military/Radar/ApproachRadar_RPL5_01/ApproachRadar_RPL5_01_generator.et", "CreateDestroyRadarTask/SpawnEntity");				
		vector topOfStandLocation = origin;
		topOfStandLocation[1] = topOfStandLocation[1] + 4.8;
		IEntity dish = SCR_SpawnSetup.SpawnAndMoveTo(topOfStandLocation, "{DED4DB7D08E6E0BE}Prefabs/Structures/Military/Radar/ApproachRadar_RPL5_01/ApproachRadar_RPL5_01.et", "CreateDestroyRadarTask/SpawnEntity");
		slotDestroyComponent.SetEntity(dish);
		slotDestroyComponent.SetTitleAndDescription("Destroy Radar", "The soviets are setting up a radar system. Destroy it before it becomes operational.");
		SpawnProtectionDetail("{96BAB56E6558788E}Prefabs/Groups/OPFOR/Group_USSR_Team_AT.et", origin);
	}
	
	// TODO can't get the antenna to work with destruction
	void CreateDestroyAntennaTask(SCR_SiteSlotEntity slot, IEntity area)
	{
		vector origin =  slot.GetOrigin();
		
		// Spawn ScenarioFramework area/layer/slot
		IEntity layerTaskDestroy = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), "{5EDF39860639027D}Prefabs/ScenarioFramework/Components/LayerTaskDestroy.et", "CreateDestroyVehicleTask/SpawnLayerTaskDestroy", false);
		area.AddChild(layerTaskDestroy, -1);
		
		IEntity slotDestroy = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), "{7586595959BA2D99}Prefabs/ScenarioFramework/Components/SlotDestroy.et", "CreateDestroyVehicleTask/SpawnSLotDestroy", false);
		layerTaskDestroy.AddChild(slotDestroy, -1);
		
		SCR_ScenarioFrameworkSlotDestroy slotDestroyComponent = SCR_ScenarioFrameworkSlotDestroy.Cast(slotDestroy.FindComponent(SCR_ScenarioFrameworkSlotDestroy));
		
		IEntity antenna = SCR_SpawnSetup.SpawnPrefabOnSlot(slot, "{B9AEDBE988F20A1A}Prefabs/Structures/Infrastructure/Towers/AntennaR404_01/AntennaR404_01.et", "CreateDestroyRadarTask/SpawnEntity");
		slotDestroyComponent.SetEntity(antenna);
		slotDestroyComponent.SetTitleAndDescription("Destroy Antenna", "The soviets are setting up a communication tower. Destroy it before it becomes operational.");
		SpawnProtectionDetail("{96BAB56E6558788E}Prefabs/Groups/OPFOR/Group_USSR_Team_AT.et", origin);
	}
	
	void CreateKillEnemyTask(SCR_SiteSlotEntity slot, IEntity area)
	{
		vector origin =  slot.GetOrigin();
		SCR_SpawnSetup.SpawnPrefabOnSlot(slot, "{2A3B53114DEAA60A}Prefabs/Compositions/Slotted/SlotFlatSmall/Headquarters_S_USSR_01.et", "CreateKillEnemyTask/spawntent");
		
		// Spawn ScenarioFramework area/layer/slot
		IEntity layerTaskKillEnemy = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), "{2008B4EE6C4D528E}Prefabs/ScenarioFramework/Components/LayerTaskKill.et", "CreateKillEnemyTask/SpawnLayerTaskKill", false);
		area.AddChild(layerTaskKillEnemy, -1);
		
		IEntity slotKill = SCR_SpawnSetup.SpawnEntity(SCR_SpawnSetup.CreateZeroVector(origin), "{C70DC6CBD1AAEC9A}Prefabs/ScenarioFramework/Components/SlotKill.et", "CreateKillEnemyTask/SpawnSLotKill", false);
		layerTaskKillEnemy.AddChild(slotKill, -1);
		
		SCR_ScenarioFrameworkSlotKill slotKillComponent = SCR_ScenarioFrameworkSlotKill.Cast(slotKill.FindComponent(SCR_ScenarioFrameworkSlotKill));
		
		string characterResourceName = m_hitList.GetKey(m_random.RandInt(0,m_hitList.Count()));
		string characterTaskDescription = m_hitList.Get(characterResourceName);
		IEntity character = IEntity.Cast(SCR_SpawnSetup.SpawnEntity(origin, characterResourceName, "CreateKillEnemyTask/SpawnEntity"));
		slotKillComponent.SetEntity(character);
		slotKillComponent.SetTitleAndDescription("Kill Enemy", characterTaskDescription);
		SCR_ChimeraCharacter castCharacter = SCR_ChimeraCharacter.Cast(character);
		if(characterTaskDescription.Contains("spy"))
		{
			FactionAffiliationComponent fac = FactionAffiliationComponent.Cast(castCharacter.FindComponent(FactionAffiliationComponent));
			fac.SetAffiliatedFactionByKey("USSR");
		}
		SpawnProtectionDetail("{A2F75E45C66B1C0A}Prefabs/Groups/OPFOR/Group_USSR_MachineGunTeam.et", origin);
	}
	
	void CreateClearAreaTask(SCR_SiteSlotEntity slot, IEntity area)
	{
		vector origin =  slot.GetOrigin();
		SCR_SpawnSetup.SpawnPrefabOnSlot(slot, "{4093AE33DCEFAD67}Prefabs/Compositions/Slotted/SlotFlatSmall/SupplyStorage_S_USSR_01.et", "CreateClearAreaTask/spawntent");
		
		// Spawn ScenarioFramework area/layer/slot
		IEntity layerTaskClearArea = SCR_SpawnSetup.SpawnEntity(vector.Zero, "{CDC0845AD90BA073}Prefabs/ScenarioFramework/Components/LayerTaskClearArea.et", "CreateClearAreaTask/SpawnLayerTaskClearArea", false, ETransformMode.LOCAL);
		area.AddChild(layerTaskClearArea, -1);
		
		IEntity slotClearArea = SCR_SpawnSetup.SpawnEntity(vector.Zero, "{E53456990A756229}Prefabs/ScenarioFramework/Components/SlotClearArea.et", "CreateClearAreaTask/SpawnSLotClearArea", false, ETransformMode.LOCAL);
		layerTaskClearArea.AddChild(slotClearArea, -1);
		
		IEntity character = IEntity.Cast(SCR_SpawnSetup.SpawnEntity(origin, "{E552DABF3636C2AD}Prefabs/Groups/OPFOR/Group_USSR_RifleSquad.et", "CreateClearAreaTask/SpawnEntity"));
		
		SCR_ScenarioFrameworkSlotClearArea slotClearAreaComponent = SCR_ScenarioFrameworkSlotClearArea.Cast(slotClearArea.FindComponent(SCR_ScenarioFrameworkSlotClearArea));
		SCR_ScenarioFrameworkPluginTrigger pluginTrigger = new SCR_ScenarioFrameworkPluginTrigger();
		pluginTrigger.Setup();
		pluginTrigger.CreateCustomTriggerArray();
		pluginTrigger.SetRadius(200);
		slotClearAreaComponent.SetTitleAndDescription("Clear Area", "Enemy forces are gathering for an attack on US forces, catch them off guard and take them out.");
		slotClearAreaComponent.AddPlugin(pluginTrigger);
		SpawnProtectionDetail("{A2F75E45C66B1C0A}Prefabs/Groups/OPFOR/Group_USSR_MachineGunTeam.et", origin);
	}
	
	void CreateRetrieveTask(SCR_Location location, SCR_SiteSlotEntity slot)
	{
		IEntity area = SCR_SpawnSetup.SpawnEntity(vector.Zero, "{C72F956E4AC6A6E7}Prefabs/ScenarioFramework/Components/Area.et", "CreateRetrieveTask/SpawnArea", false);
		// spawn command tent where objective will be found
		vector origin = slot.GetOrigin();
		SCR_SpawnSetup.SpawnEntity(origin, "{2A3B53114DEAA60A}Prefabs/Compositions/Slotted/SlotFlatSmall/Headquarters_S_USSR_01.et", "CreateRetrieveTask/spawntent");
		
		// Spawn ScenarioFramework area/layer/slot
		string layerTaskName = "layerTaskDeliver_" + location.GetName();
		IEntity layerTaskDeliver = SCR_SpawnSetup.SpawnEntity(origin, "{88821DCA414AF4C7}Prefabs/ScenarioFramework/Components/LayerTaskDeliver.et", "CreateRetrieveTask/SpawnLayerTaskDeliver", true, ETransformMode.LOCAL);
		layerTaskDeliver.SetName(layerTaskName);
		area.AddChild(layerTaskDeliver, -1);
		
		int index = m_random.RandInt(0,m_itemList.Count());
		string itemResourceName = m_itemList[index][0];
		string taskItemName = m_itemList[index][1];
		string taskDescription = m_itemList[index][2];
		
		// spawn slotpick
		IEntity slotPickEntity = SCR_SpawnSetup.SpawnEntity(vector.Zero, "{9F70B00322910AED}Prefabs/ScenarioFramework/Components/SlotPick.et", "CreateRetrieveTask/SpawnSlotPick", false, ETransformMode.LOCAL);
		layerTaskDeliver.AddChild(slotPickEntity, -1);
		SCR_ScenarioFrameworkSlotPick slotPick = SCR_ScenarioFrameworkSlotPick.Cast(slotPickEntity.FindComponent(SCR_ScenarioFrameworkSlotPick));
		slotPick.SetTitleAndDescriptions("Find " + taskItemName, taskDescription, "Deliver " + taskItemName, taskDescription, "Pick up " + taskItemName, taskDescription);
		
		// Spawn objective item and assign to slot
		origin[1] = origin[1] + 1.1;
		origin[2] = origin[2] - 1.5;
		IEntity retrieveItemEntity = SCR_SpawnSetup.SpawnEntity(origin, itemResourceName, "CreateRetrieveTask/SpawnRetrievableEntity", false, ETransformMode.WORLD);
		switch (taskItemName)
		{
			case "Weapon":
				retrieveItemEntity.SetYawPitchRoll("90 0 90");
				break;
		
			case "BackPack":
				retrieveItemEntity.SetYawPitchRoll("90 0 0");
				break;
			
			default:
				break;
		}
		slotPick.SetEntity(retrieveItemEntity);
				
		array<vector> deliveryLocations = location.GetDeliveryLocations();
		vector deliveryLocation = deliveryLocations[m_random.RandInt(0, deliveryLocations.Count())];
		
		IEntity slotDeliveryEntity = SCR_SpawnSetup.SpawnEntity(deliveryLocation, "{4C2EF5C1E53FE511}Prefabs/ScenarioFramework/Components/SlotDelivery.et", "CreateRetrieveTask/SpawnSlotDelivery", true, ETransformMode.LOCAL);
		SCR_ScenarioFrameworkSlotDelivery slotDelivery = SCR_ScenarioFrameworkSlotDelivery.Cast(slotDeliveryEntity.FindComponent(SCR_ScenarioFrameworkSlotDelivery));
		slotDelivery.AddAssociatedLayerTask(layerTaskName);
		slotDelivery.SetDebugVisibility(false);
		area.AddChild(slotDeliveryEntity, -1);
		
		// spawn delivery location friendlies + Vehicle
		SCR_SpawnSetup.SpawnEntity(deliveryLocation, "{27E2E58E734A80EC}Prefabs/Vehicles/Wheeled/M998/M1025_MERDC.et", "CreateRetrieveTask/SpawnDeliveryVehicle");
		SpawnProtectionDetail("{84E5BBAB25EA23E5}Prefabs/Groups/BLUFOR/Group_US_FireTeam.et", deliveryLocation);
		
		
		SpawnProtectionDetail("{A2F75E45C66B1C0A}Prefabs/Groups/OPFOR/Group_USSR_MachineGunTeam.et", slot.GetOrigin());
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
		//m_missions.Insert("Destroy Vehicle");
		//m_missions.Insert("Destroy Radar");
		m_missions.Insert("Kill Enemy");
		//m_missions.Insert("Steal Vehicle");
		m_missions.Insert("Retrieve Item");
		m_missions.Insert("Clear Area");
		m_destroyVehicles.Insert("{1BABF6B33DA0AEB6}Prefabs/Vehicles/Wheeled/Ural4320/Ural4320_command.et","The soviets have set up a mobile command center. Destroy their command truck.");
		m_destroyVehicles.Insert("{C012BB3488BEA0C2}Prefabs/Vehicles/Wheeled/BTR70/BTR70.et","A BTR70 has been spotten in the area of operations. Destroy the armored vehicle.");
		m_destroyVehicles.Insert("{4C81D7ED8F8C0D87}Prefabs/Vehicles/Wheeled/Ural4320/Ural4320_tanker.et","Destroy the fuel truck to disrupt soviet logistics.");
		m_destroyVehicles.Insert("{9DD4FD3527CB46C8}Prefabs/Vehicles/Wheeled/Ural4320/Ural4320_transport_covered_closed.et","Destroy the soviet supply truck to disrupt soviet logistics.");
		m_hitList.Insert("{26A9756790131354}Prefabs/Characters/Factions/BLUFOR/US_Army/Character_US_Rifleman.et", "A spy has been impersonating an american soldier to infiltrate US command. Kill the soviet spy.");
		m_hitList.Insert("{5117311FB822FD1F}Prefabs/Characters/Factions/OPFOR/USSR_Army/Character_USSR_HVT.et", "An enemy officer has spotted near your location. Take out the officer.");
		m_hitList.Insert("{976AC400219898FA}Prefabs/Characters/Factions/OPFOR/USSR_Army/Character_USSR_Sharpshooter.et", "A sniper has been targeting US officers. Kill the enemy sniper");
		array<string> intel = new array<string>();
		intel.InsertAll({"{6D56FED1E55A8F84}Prefabs/Items/Misc/IntelligenceFolder_E_01/IntelligenceFolder_E_01.et", "Intelligence", "An enemy officer has been seen near your location. See if you can find more information on their plans."});
		//m_itemList.Insert(intel);
		array<string> weapon = {"{B1482FB64E3D2D45}Prefabs/Weapons/Rifles/M16/Rifle_M16A2_4x20.et", "Weapon", "A spy has stolen a US weapon. Retrieve the weapon before it can be shipped back to the USSR to be reverse engineered."};
		m_itemList.Insert(weapon);
		array<string> backPack = {"{3DE0155EC9767B98}Prefabs/Items/Equipment/Backpacks/Backpack_Veshmeshok.et", "BackPack", "A soviet messenger has hidden secret intelligence inside his backpack liner. Take it back to base so it can be searched."};
		//m_itemList.Insert(backPack);
		//m_itemList.Insert("{50A68300B537EDFA}Prefabs/Items/Food/ArmyCrackers_Soviet_01.et", {"Crackers", "The general is hungry for Soviet army crackers. Get them for him or you'll be demoted."});
	}
};
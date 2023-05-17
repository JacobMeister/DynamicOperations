class SCR_TaskGenerator
{
	protected ref array<string> m_missions = {};
	protected ref RandomGenerator m_random;
	protected ref SCR_AISpawner m_aiSpawner;
	protected ref map<string,string> m_destroyVehicles = new map<string,string>();
	protected ref map<string,string> m_hitList = new map<string,string>();
	
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
			// Get slot to spawn mission vehicle on
			SCR_SlotManager slotManager = location.GetSlotManager();
			SCR_SiteSlotEntity randomSlot = slotManager.GetRandomSlotSuitableForMissionTask();
			vector origin =  randomSlot.GetOrigin();
			int randomMissionIndex = m_random.RandInt(0,m_missions.Count());
			string selectedMission = m_missions[randomMissionIndex];
			//m_missions.Remove(randomMissionIndex);
			IEntity area = SCR_SpawnSetup.SpawnEntity(randomSlot.GetOrigin(), "{C72F956E4AC6A6E7}Prefabs/ScenarioFramework/Components/Area.et", "CreateDestroyVehicleTask/SpawnArea");
		
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
		
				default:
					CreateKillEnemyTask(randomSlot, area);
					break;
			}
			
			// spawn protection detail 5m to the side because they keep getting crushed by vehicle spawn
			vector protectionDetailSpawn = origin;
			protectionDetailSpawn[0] = protectionDetailSpawn[0] + 5;
			m_aiSpawner.SpawnAI("{A2F75E45C66B1C0A}Prefabs/Groups/OPFOR/Group_USSR_MachineGunTeam.et", protectionDetailSpawn, "{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et", origin);
			slotManager.MarkAsUsed(randomSlot);
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
		
		IEntity stand = SCR_SpawnSetup.SpawnPrefabOnSlot(slot, "{979A564D476B5AAE}Prefabs/Structures/Military/Radar/ApproachRadar_RPL5_01/ApproachRadar_RPL5_01_tall.et", "CreateDestroyRadarTask/SpawnEntity");				
		vector topOfStandLocation = origin;
		topOfStandLocation[1] = topOfStandLocation[1] + 4.8;
		vector angleOfSlot = slot.GetAngles();
		IEntity dish = SCR_SpawnSetup.SpawnEntity(topOfStandLocation, "{DED4DB7D08E6E0BE}Prefabs/Structures/Military/Radar/ApproachRadar_RPL5_01/ApproachRadar_RPL5_01.et", "CreateDestroyRadarTask/SpawnEntity", false);
		dish.SetAngles(angleOfSlot);
		slotDestroyComponent.SetEntity(dish);
		slotDestroyComponent.SetTitleAndDescription("Destroy Radar", "The soviets are setting up a radar system. Destroy it before it becomes operational.");
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
		
		IEntity character = IEntity.Cast(SCR_SpawnSetup.SpawnEntity(origin, "{E552DABF3636C2AD}Prefabs/Groups/OPFOR/Group_USSR_RifleSquad.et", "CreateKillEnemyTask/SpawnEntity"));
		
		SCR_ScenarioFrameworkSlotClearArea slotClearAreaComponent = SCR_ScenarioFrameworkSlotClearArea.Cast(slotClearArea.FindComponent(SCR_ScenarioFrameworkSlotClearArea));
		SCR_ScenarioFrameworkPluginTrigger pluginTrigger = new SCR_ScenarioFrameworkPluginTrigger();
		pluginTrigger.CreateCustomTriggerArray();
		pluginTrigger.SetRadius(200);
		slotClearAreaComponent.SetTitleAndDescription("Clear Area", "Enemy forces are gathering for an attack on US forces, catch them off guard and take them out.");
		slotClearAreaComponent.AddPlugin(pluginTrigger);
	}
	
	private void SetupValues()
	{
		m_missions.Insert("Destroy Vehicle");
		m_missions.Insert("Destroy Radar");
		m_missions.Insert("Kill Enemy");
		//m_missions.Insert("Steal Vehicle");
		//m_missions.Insert("Retrieve Intel");
		m_missions.Insert("Clear Area");
		m_destroyVehicles.Insert("{1BABF6B33DA0AEB6}Prefabs/Vehicles/Wheeled/Ural4320/Ural4320_command.et","The soviets have set up a mobile command center. Destroy their command truck.");
		m_destroyVehicles.Insert("{C012BB3488BEA0C2}Prefabs/Vehicles/Wheeled/BTR70/BTR70.et","A BTR70 has been spotten in the area of operations. Destroy the armored vehicle.");
		m_destroyVehicles.Insert("{4C81D7ED8F8C0D87}Prefabs/Vehicles/Wheeled/Ural4320/Ural4320_tanker.et","Destroy the fuel truck to disrupt soviet logistics.");
		m_destroyVehicles.Insert("{9DD4FD3527CB46C8}Prefabs/Vehicles/Wheeled/Ural4320/Ural4320_transport_covered_closed.et","Destroy the soviet supply truck to disrupt soviet logistics.");
		m_hitList.Insert("{26A9756790131354}Prefabs/Characters/Factions/BLUFOR/US_Army/Character_US_Rifleman.et", "A spy has been impersonating an american soldier to infiltrate US command. Kill the soviet spy.");
		m_hitList.Insert("{5117311FB822FD1F}Prefabs/Characters/Factions/OPFOR/USSR_Army/Character_USSR_HVT.et", "An enemy officer has spotted near your location. Take out the officer.");
		m_hitList.Insert("{976AC400219898FA}Prefabs/Characters/Factions/OPFOR/USSR_Army/Character_USSR_Sharpshooter.et", "A sniper has been targeting US officers. Kill the enemy sniper");
	}
};
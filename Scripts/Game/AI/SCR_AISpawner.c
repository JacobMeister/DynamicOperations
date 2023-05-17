class SCR_AISpawner
{
	void SCR_AISpawner()
	{
		
	}
	
	void ~SCR_AISpawner()
	{
		
	}
	
	SCR_AIGroup SpawnAIWithoutWaypoint(string spawnGroup, vector spawnPosition)
	{
		Resource resource = SCR_SpawnSetup.GenerateAndValidateResource(spawnGroup);
		
		if (!resource)
		{
			Print(("[SpawnAI] Unable to load resource for the spawn group: " + spawnGroup), LogLevel.ERROR);
			return null;
		}
		
		SCR_AIGroup group = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(resource, null,  SCR_SpawnSetup.GenerateSpawnParameters(spawnPosition)));
				
		if (!group)
		{
			Print("[SpawnAI] Unable to spawn group!", LogLevel.ERROR);
			return null;
		}
		
		return group;
	}
	
	void SpawnAI(string spawnGroup, vector spawnPosition, string waypointType, vector waypointPosition)
	{
		SCR_AIGroup group = SpawnAIWithoutWaypoint(spawnGroup, spawnPosition);
		AIWaypoint waypoint = CreateWaypoint(waypointType, waypointPosition);
		if(!group || !waypoint)
		{
			return;
		}
		group.AddWaypoint(waypoint);
	}
	
	private AIWaypoint CreateWaypoint(string waypointType, vector waypointPosition)
	{
		Resource resource =  SCR_SpawnSetup.GenerateAndValidateResource(waypointType);
		
		if (!resource)
		{
			Print(("[CreateWaypoint] Unable to load resource for the waypoint: " + waypointType), LogLevel.ERROR);
			return null;
		}
		
		AIWaypoint waypoint = AIWaypoint.Cast(GetGame().SpawnEntityPrefab(resource, null,  SCR_SpawnSetup.GenerateSpawnParameters(waypointPosition)));
		
		if (!waypoint)
		{
			Print("[CreateWaypoint] Unable to create waypoint!", LogLevel.ERROR);
			return null;
		}
		
		return waypoint;
	}
	
	void SpawnPatrollingAI(string spawnGroup, vector startWaypointPosition, vector endWaypointPosition)
	{
		SCR_AIGroup group = SpawnAIWithoutWaypoint(spawnGroup, startWaypointPosition);
		CreatePatrolWaypoint(group, startWaypointPosition, endWaypointPosition);
	}
	
	private void CreatePatrolWaypoint(SCR_AIGroup group, vector startWaypointPosition, vector endWaypointPosition)
	{
		string patrolWaypointType = "{22A875E30470BD4F}Prefabs/AI/Waypoints/AIWaypoint_Patrol.et";
		string cycleWaypointType = "{35BD6541CBB8AC08}Prefabs/AI/Waypoints/AIWaypoint_Cycle.et";
		AIWaypoint startWaypoint = CreateWaypoint(patrolWaypointType, startWaypointPosition);
		AIWaypoint endWaypoint = CreateWaypoint(patrolWaypointType, endWaypointPosition);
				
		Resource resource =  SCR_SpawnSetup.GenerateAndValidateResource(cycleWaypointType);
		
		if (!resource)
		{
			Print(("[CreatePatrolWaypoint] Unable to load resource for the waypoint: " + cycleWaypointType), LogLevel.ERROR);
			return;
		}
		
		AIWaypointCycle cycleWaypoint = AIWaypointCycle.Cast(GetGame().SpawnEntityPrefab(resource, null,  SCR_SpawnSetup.GenerateSpawnParameters(startWaypointPosition)));
		if (!cycleWaypoint)
		{
			Print("[CreatePatrolWaypoint] Unable to create waypoint!", LogLevel.ERROR);
			return;
		}
		array<AIWaypoint> waypoints = {startWaypoint, endWaypoint};
		
		cycleWaypoint.SetWaypoints(waypoints);
		
		group.AddWaypoint(cycleWaypoint);
	}
};
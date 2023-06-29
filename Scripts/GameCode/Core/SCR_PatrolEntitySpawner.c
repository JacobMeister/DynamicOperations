class SCR_PatrolEntitySpawner
{
	private ref SCR_AISpawner m_aiSpawner;
	
	void SCR_PatrolEntitySpawner()
	{
		m_aiSpawner = new SCR_AISpawner();
	}
	
	void ~SCR_PatrolEntitySpawner()
	{
		
	}
	
	void SpawnPatrolEntities(array<ref SCR_Location> locations)
	{
		foreach(SCR_Location location: locations)
		{
			Print("Spawning patrols going from " + location.GetName() + " to:");
			foreach(vector patrolLocation : location.GetPatrolLocations())
			{
				SpawnPatrol(location.GetEntity().GetOrigin(), patrolLocation);
				Print("    " + patrolLocation);
			}
		}
	}
	
	private void SpawnPatrol(vector start, vector end)
	{
		string spawngroup = "{1A5F0D93609DA5DA}Prefabs/Groups/OPFOR/Group_USSR_ManeuverGroup.et";
		m_aiSpawner.SpawnPatrollingAI(spawngroup, start, end);
	}
}
class SCR_DynamicOperations
{
	const int AMOUNT_OF_MISSIONS = 3;
	protected ref RandomGenerator m_random;
	protected ref SCR_DynamicLocationSelection m_locationSelection; // selects 3 locations where missions will take place
	protected ref SCR_SpawnCreator m_spawnCreator; // will determine player spawn and exfil positions and create them in world
	protected ref SCR_ExtractionCreator m_extrationCreator;
	protected ref SCR_TaskGenerator m_taskGenerator; // will spawn mission objectives and create tasks
	protected ref SCR_MissionEntitySpawner m_missionEntitySpawner; // will spawn ai, vehicles and prefabs in mission locations
	protected ref SCR_PatrolEntitySpawner m_patrolEntitySpawner;	
	protected ref array<ref SCR_Location> m_missionLocations = {};
	
	void SCR_DynamicOperations()
	{
		m_random = new RandomGenerator();
		m_locationSelection = new SCR_DynamicLocationSelection();
		m_spawnCreator = new SCR_SpawnCreator();
		m_extrationCreator = new SCR_ExtractionCreator();
		m_taskGenerator = new SCR_TaskGenerator();
		m_missionEntitySpawner = new SCR_MissionEntitySpawner();
		m_patrolEntitySpawner = new SCR_PatrolEntitySpawner();
	}
	
	void ~SCR_DynamicOperations()
	{
		
	}
	
	void GenerateDynamicOperation()
	{
		DetermineAO();
		m_taskGenerator.CreateTasks(m_missionLocations);
		m_missionEntitySpawner.SpawnMissionEntities(m_missionLocations);
		m_patrolEntitySpawner.SpawnPatrolEntities(m_missionLocations);
		CreateSpawns();
	}
	
	protected void DetermineAO()
	{
		m_locationSelection.GetLocations(m_missionLocations);
		Print("Determined area of operations");
		Print("Mission locations: " + m_missionLocations[0].GetName() + ", " + m_missionLocations[1].GetName() + ", " + m_missionLocations[2].GetName());
	}
	
	protected void CreateSpawns()
	{	
		// get locations for spawns and exfils
		array<IEntity> spawnLocations = m_locationSelection.GetSpawnLocations(m_missionLocations, false);
		array<IEntity> exfilLocations = m_locationSelection.GetSpawnLocations(m_missionLocations, true);
		
		foreach(IEntity spawnLocation : spawnLocations)
		{
			m_spawnCreator.CreateSpawnPoint(spawnLocation.GetOrigin());
			m_spawnCreator.FurnishSpawnPoint(spawnLocation.GetOrigin());
		}
		
		// set up new spawn locations when players near mission location
		foreach (SCR_Location location : m_missionLocations)
		{
			m_spawnCreator.CreateExtraSpawnPointTrigger(location);
		}
		
		// set up exfil locations
		m_extrationCreator.CreateExtractionPoints(exfilLocations);
	}
}
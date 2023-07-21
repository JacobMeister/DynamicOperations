class SCR_SpawnSetup
{
	void SCR_SpawnSetup()
	{
		
	}
	
	void ~SCR_SpawnSetup()
	{
		
	}

	static Resource GenerateAndValidateResource(string resourceToLoad)
	{
		Resource resource = Resource.Load(resourceToLoad);
		
		if (!resource.IsValid())
		{
			Print(("[Generate And Validate Resource] Resource is invalid: " + resourceToLoad), LogLevel.ERROR);
			return null;
		}
		
		return resource;
	}
	
	static EntitySpawnParams GenerateSpawnParameters(vector position, bool snapToGround = true, ETransformMode mode = ETransformMode.WORLD)
	{
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = mode;
		
		// snap to Ground
		if(snapToGround)
		{
			float groundLevel = GetGame().GetWorld().GetSurfaceY(position[0], position[2]);
			position[1] = groundLevel;
		}
		
		params.Transform[3] = position;
		return params;
	}
	
	static IEntity SpawnPrefabOnSlot(SCR_SiteSlotEntity slot, string resourceName, string callerName)
	{
		Resource resource = SCR_SpawnSetup.GenerateAndValidateResource(resourceName);
			
		if (!resource)
		{
			Print("[" + callerName + "] Unable to load resource for the entity: " + resourceName);
			return null;
		}
		
		IEntity spawnedPrefab = slot.SpawnEntityInSlot(resource);
		if (!spawnedPrefab)
		{
			Print("[" + callerName + "] Unable to create entity!" + resourceName);
			return null;
		}
		return spawnedPrefab;
	}
	
	static IEntity SpawnEntity(vector origin, string resourceName, string callerName,  bool snapToGround = true, ETransformMode mode = ETransformMode.WORLD)
	{
		Resource resource = SCR_SpawnSetup.GenerateAndValidateResource(resourceName);
			
		if (!resource)
		{
			Print("[" + callerName + "] Unable to load resource for the entity: " + resourceName);
			return null;
		}
		
		IEntity spawnedPrefab =  IEntity.Cast(GetGame().SpawnEntityPrefab(resource, null,  SCR_SpawnSetup.GenerateSpawnParameters(origin, snapToGround, mode)));
		if (!spawnedPrefab)
		{
			Print("[" + callerName + "] Unable to create entity!" + resourceName);
			return null;
		}
		return spawnedPrefab;
	}
	
	static IEntity SpawnAndMoveTo(vector origin, string resourceName, string callerName)
	{
		Resource resource = SCR_SpawnSetup.GenerateAndValidateResource(resourceName);
		if (!resource)
		{
			Print("[" + callerName + "] Unable to load resource for the entity: " + resourceName);
			return null;
		}
		
		vector movedOrigin = origin;
		movedOrigin[1] = movedOrigin[1];
		
		IEntity spawnedPrefab =  IEntity.Cast(GetGame().SpawnEntityPrefab(resource, null,  SCR_SpawnSetup.GenerateSpawnParameters(movedOrigin, false)));
		if (!spawnedPrefab)
		{
			Print("[" + callerName + "] Unable to create entity!" + resourceName);
			return null;
		}
		
		spawnedPrefab.SetOrigin(origin);
		
		return spawnedPrefab;
	}
	
	static vector CreateZeroVector(vector origin)
	{
		int locationHeight = GetGame().GetWorld().GetSurfaceY(origin[0], origin[2]) + 3;
		return {0,locationHeight,0};
	}
	
	static void SnapToTerrain(IEntity entity)
	{
		float height = 0;
		height = entity.GetLocalTransformAxis(3)[1];
		BaseWorld world = entity.GetWorld();
		vector transform[4];
		entity.GetWorldTransform(transform);
		SCR_TerrainHelper.SnapAndOrientToTerrain(transform, world, false);
		entity.SetWorldTransform(transform);
	}
}
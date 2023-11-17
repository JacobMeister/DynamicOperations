class SCR_ExtractionCreator
{
	
	private ref RandomGenerator m_random;

	void SCR_ExtractionCreator()
	{
		m_random = new RandomGenerator();
	}
	
	void ~SCR_ExtractionCreator()
	{
		
	}
	
	void CreateExtractionPoints(array<IEntity> exfilLocations)
	{
		// create area
		IEntity area = SCR_SpawnSetup.SpawnEntity(vector.Zero, SCR_Prefab_Constants.area, "CreateExtractionPoint", false);
		
		// create layer with counter
		IEntity counterLayerEntity = SCR_SpawnSetup.SpawnEntity(vector.Zero, SCR_Prefab_Constants.layer, "CreateExtractionPoint", false, ETransformMode.LOCAL);
		area.AddChild(counterLayerEntity, -1);
		// create logicCounter entity
		SCR_ScenarioFrameworkLogicCounter logicCounter = SCR_ScenarioFrameworkLogicCounter.Cast(SCR_SpawnSetup.SpawnEntity(vector.Zero, SCR_Prefab_Constants.logicCounter, "CreateExtractionPoint", false, ETransformMode.LOCAL));
		logicCounter.SetCountTo(SCR_DynamicOperationsConstants.AMOUNT_OF_MISSIONS -1);
		counterLayerEntity.AddChild(logicCounter, -1);
		
		// create scneario input action
		SCR_ScenarioFrameworkActionInputOnTaskEventIncreaseCounter inputAction = new SCR_ScenarioFrameworkActionInputOnTaskEventIncreaseCounter();
		inputAction.SetEventName(SCR_TaskState.FINISHED);
		
		// create logic input entity
		SCR_ScenarioFrameworkLogicInput input = new SCR_ScenarioFrameworkLogicInput();
		input.AddInputAction(inputAction);
		
		logicCounter.AddInput(input);
		for(int i = 0; i < exfilLocations.Count(); i++)
		{
			vector v = exfilLocations[i].GetOrigin();
			string layerName = CreateExtractionLayers(area, v, i);
			// add onactivate
			// add spawnslotlayer to spawn object when triggered
			SCR_ScenarioFrameworkActionSpawnObjects triggerAction = new SCR_ScenarioFrameworkActionSpawnObjects();
			triggerAction.Setup();
			triggerAction.AddObjectsToSpawn(layerName);
			logicCounter.AddAction(triggerAction);
		}
	}
	
	string CreateExtractionLayers(IEntity area, vector v, int i)
	{
		// create layer with extraction slot
		IEntity extractionLayerEntity = SCR_SpawnSetup.SpawnEntity(v, SCR_Prefab_Constants.layerTaskMove, "CreateExtractionPoint", true, ETransformMode.LOCAL);
		SCR_ScenarioFrameworkLayerTask layer = SCR_ScenarioFrameworkLayerTask.Cast(extractionLayerEntity.FindComponent(SCR_ScenarioFrameworkLayerTask));
		layer.SetActivationType(SCR_ScenarioFrameworkEActivationType.ON_TRIGGER_ACTIVATION);
		string layerName = "ExtractionLayer" + i;
		extractionLayerEntity.SetName(layerName);
		area.AddChild(extractionLayerEntity, -1);
		
		IEntity slotExtractionEntity =  SCR_SpawnSetup.SpawnEntity(vector.Zero, SCR_Prefab_Constants.slotMoveTo, "CreateExtractionPoint", false, ETransformMode.LOCAL);
		extractionLayerEntity.AddChild(slotExtractionEntity, -1);
		SCR_ScenarioFrameworkSlotExtraction slotExtraction = SCR_ScenarioFrameworkSlotExtraction.Cast(slotExtractionEntity.FindComponent(SCR_ScenarioFrameworkSlotExtraction));
		SCR_ScenarioFrameworkPluginTrigger trigger = new SCR_ScenarioFrameworkPluginTrigger();
		trigger.Setup();
		trigger.SetRadius(50);
		//trigger.SetTriggerOnce(true);
		trigger.EnableCountDown(20);
		SCR_ScenarioFrameworkActionEndMission endMissionAction = SCR_ScenarioFrameworkActionEndMission();
		endMissionAction.SetGameOverType(EGameOverTypes.COMBATPATROL_VICTORY);
		endMissionAction.Setup();
		slotExtraction.AddAction(endMissionAction);
		slotExtraction.AddPlugin(trigger);
		slotExtraction.SetTitleAndDescription("Extract", "Move to location to leave area of operations.");
		
		array<IEntity> spawnedEntities = new array<IEntity>();		
		spawnedEntities.Insert(SCR_SpawnSetup.SpawnEntity(slotExtractionEntity.GetOrigin(), SCR_Prefab_Constants.jeep_covered, "CreateExtractionPoint", true, ETransformMode.WORLD));
		
		foreach(IEntity entity : spawnedEntities)
		{
			vector angleVector = {0, m_random.RandInt(0,360), 0};
			entity.SetAngles(angleVector);
			SCR_SpawnSetup.SnapToTerrain(entity);
		}
				
		return layerName;
	}
}
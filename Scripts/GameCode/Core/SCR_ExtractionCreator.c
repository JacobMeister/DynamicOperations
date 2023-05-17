class SCR_ExtractionCreator
{
	void SCR_ExtractionCreator()
	{
	}
	
	void ~SCR_ExtractionCreator()
	{
		
	}
	
	void CreateExtractionPoints(array<IEntity> exfilLocations)
	{
		// create area
		IEntity area = SCR_SpawnSetup.SpawnEntity(vector.Zero, "{C72F956E4AC6A6E7}Prefabs/ScenarioFramework/Components/Area.et", "CreateExtractionPoint", false);
		
		// create layer with counter
		IEntity counterLayerEntity = SCR_SpawnSetup.SpawnEntity(vector.Zero, "{5F9FFF4BF027B3A3}Prefabs/ScenarioFramework/Components/Layer.et", "CreateExtractionPoint", false, ETransformMode.LOCAL);
		area.AddChild(counterLayerEntity, -1);
		// create logicCounter entity
		SCR_ScenarioFrameworkLogicCounter logicCounter = SCR_ScenarioFrameworkLogicCounter.Cast(SCR_SpawnSetup.SpawnEntity(vector.Zero, "{6C1792C71DCAFD55}Prefabs/ScenarioFramework/Components/LogicCounter.et", "CreateExtractionPoint", false, ETransformMode.LOCAL));
		logicCounter.SetCountTo(SCR_DynamicOperations.AMOUNT_OF_MISSIONS -1);
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
			triggerAction.AddObjectsToSpawn(layerName);
			logicCounter.AddAction(triggerAction);
		}
	}
	
	string CreateExtractionLayers(IEntity area, vector v, int i)
	{
		// create layer with extraction slot
		IEntity extractionLayerEntity = SCR_SpawnSetup.SpawnEntity(v, "{246BEC080F393398}Prefabs/ScenarioFramework/Components/LayerTaskMove.et", "CreateExtractionPoint", true, ETransformMode.LOCAL);
		SCR_ScenarioFrameworkLayerTask layer = SCR_ScenarioFrameworkLayerTask.Cast(extractionLayerEntity.FindComponent(SCR_ScenarioFrameworkLayerTask));
		layer.SetActivationType(SCR_ScenarioFrameworkEActivationType.ON_TRIGGER_ACTIVATION);
		string layerName = "ExtractionLayer" + i;
		extractionLayerEntity.SetName(layerName);
		area.AddChild(extractionLayerEntity, -1);
		
		IEntity slotExtractionEntity =  SCR_SpawnSetup.SpawnEntity(vector.Zero, "{A44004A770A5D8BE}Prefabs/ScenarioFramework/Components/SlotMoveTo.et", "CreateExtractionPoint", false, ETransformMode.LOCAL);
		extractionLayerEntity.AddChild(slotExtractionEntity, -1);
		SCR_ScenarioFrameworkSlotExtraction slotExtraction = SCR_ScenarioFrameworkSlotExtraction.Cast(slotExtractionEntity.FindComponent(SCR_ScenarioFrameworkSlotExtraction));
		SCR_ScenarioFrameworkPluginTrigger trigger = new SCR_ScenarioFrameworkPluginTrigger();
		trigger.CreateCustomTriggerArray();
		trigger.SetRadius(50);
		trigger.SetTriggerOnce(true);
		trigger.EnableCountDown(20);
		SCR_ScenarioFrameworkActionEndMission endMissionAction = SCR_ScenarioFrameworkActionEndMission();
		endMissionAction.SetGameOverType(EGameOverTypes.COMBATPATROL_VICTORY);
		slotExtraction.AddAction(endMissionAction);
		slotExtraction.AddPlugin(trigger);
		slotExtraction.SetTitleAndDescription("Extract", "Move to location to leave area of operations.");
		
		SCR_SpawnSetup.SpawnEntity(slotExtractionEntity.GetOrigin(), "{47D94E1193A88497}Prefabs/Vehicles/Wheeled/M151A2/M151A2_transport.et", "CreateExtractionPoint", true, ETransformMode.WORLD);
				
		return layerName;
	}
}
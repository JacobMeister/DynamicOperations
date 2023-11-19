modded class SCR_ScenarioFrameworkActionSpawnObjects
{
	void AddObjectsToSpawn(string objectName)
	{
		if (!m_aNameOfObjectsToSpawnOnActivation)
		{
			m_aNameOfObjectsToSpawnOnActivation = new array<string>();
		}
		m_aNameOfObjectsToSpawnOnActivation.Insert(objectName);
	}
}

modded class SCR_ScenarioFrameworkPluginTrigger
{
	void Setup()
	{
		m_sActivatedByThisFaction = "US";
		m_fUpdateRate = 1;
		m_aCustomTriggerConditions = {};
		m_aSpecificClassNames = {};
		m_aPrefabFilter = {};
	}
	
	void SetVehicleTrigger()
	{
		m_eActivationPresence = TA_EActivationPresence.SPECIFIC_CLASS;
		m_aSpecificClassNames = { "Vehicle" };
	}

	void SetRadius(float radius)
	{
		m_fAreaRadius = radius;
	}

	void SetTriggerOnce(bool once)
	{
		m_bOnce = once;
	}

	void EnableCountDown(float seconds)
	{
		m_bEnableAudio = true;
		m_sCountdownAudio = "SOUND_RESPAWNMENU";
		m_fActivationCountdownTimer = seconds;
		m_bNotificationEnabled = true;
		m_sPlayerActivationNotificationTitle = "Extraction in " + seconds + " seconds";
	}
}

modded class SCR_ScenarioFrameworkActionShowHint
{
	void SetText(string header, string content)
	{
		m_sTitle = header;
		m_sText = content;
	}
}

modded class SCR_ScenarioFrameworkSlotTrigger
{
	void AddAction(SCR_ScenarioFrameworkActionBase action)
	{
		m_aTriggerActions.Insert(action);
	}

	void AddPlugin(SCR_ScenarioFrameworkPlugin plugin)
	{
		m_aPlugins = new array<ref SCR_ScenarioFrameworkPlugin>();
		m_aPlugins.Insert(plugin);
	}
}

modded class SCR_ScenarioFrameworkSlotClearArea
{
	void AddPlugin(SCR_ScenarioFrameworkPlugin plugin)
	{
		m_aPlugins = new array<ref SCR_ScenarioFrameworkPlugin>();
		m_aPlugins.Insert(plugin);
	}

	void SetTitleAndDescription(string taskTitle, string taskDescription)
	{
		m_sTaskTitle = taskTitle;
		m_sTaskDescription = taskDescription;
	}
}

modded class SCR_ScenarioFrameworkSlotExtraction
{
	void AddPlugin(SCR_ScenarioFrameworkPlugin plugin)
	{
		m_aPlugins = new array<ref SCR_ScenarioFrameworkPlugin>();
		m_aPlugins.Insert(plugin);
	}

	void SetTitleAndDescription(string taskTitle, string taskDescription)
	{
		m_sTaskTitle = taskTitle;
		m_sTaskDescription = taskDescription;
	}

	void AddAction(SCR_ScenarioFrameworkActionBase action)
	{
		m_aActionsOnFinished.Insert(action);
	}
}

modded class SCR_ScenarioFrameworkSlotBase
{
	void SetObjectToSpawn(string resourceName)
	{
		m_sObjectToSpawn = resourceName;
	}

	void SetDebugVisibility(bool visible)
	{
		m_bShowDebugShapesDuringRuntime = visible;
	}
	
	void UseExistingAsset(bool useExistingAsset)
	{
		m_bUseExistingWorldAsset = useExistingAsset;
	}
}

modded class SCR_ScenarioFrameworkSlotKill
{
	void SetTitleAndDescription(string taskTitle, string taskDescription)
	{
		m_sTaskTitle = taskTitle;
		m_sTaskDescription = taskDescription;
	}
	
	override void Init(SCR_ScenarioFrameworkArea area = null, SCR_ScenarioFrameworkEActivationType activation = SCR_ScenarioFrameworkEActivationType.SAME_AS_PARENT)
	{
		if (m_bInitiated)
			return;
		
		if (!m_bDynamicallyDespawned && activation != m_eActivationType)
			return;
		
		bool tempTerminated = m_bIsTerminated;
		m_bIsTerminated = false;
		
		if (m_bIsTerminated)
			return;
						
		// Handles inheritance of faction settings from parents
		if (m_sFactionKey.IsEmpty() && m_ParentLayer && !m_ParentLayer.GetFactionKey().IsEmpty())
			SetFactionKey(m_ParentLayer.GetFactionKey());
		
		
		GetOnAllChildrenSpawned().Insert(AfterAllChildrenSpawned);
		
		
		if (!m_Entity)
		{
			InvokeAllChildrenSpawned();
			return;
		}
		
		if (!m_sID.IsEmpty())
			m_Entity.SetName(m_sID);	
		
		ScriptedDamageManagerComponent objectDmgManager = ScriptedDamageManagerComponent.Cast(m_Entity.FindComponent(ScriptedDamageManagerComponent));
		if (objectDmgManager)
			objectDmgManager.GetOnDamageStateChanged().Insert(OnObjectDamage);
		
		if (Vehicle.Cast(m_Entity))
		{
			EventHandlerManagerComponent ehManager = EventHandlerManagerComponent.Cast(m_Entity.FindComponent(EventHandlerManagerComponent));
			if (ehManager)
				ehManager.RegisterScriptHandler("OnCompartmentEntered", this, OnCompartmentEntered, false, true);
		}
			
		StoreTaskSubjectToParentTaskLayer();
		m_bIsTerminated = tempTerminated;
		
		InvokeAllChildrenSpawned();
	}
}

modded class SCR_ScenarioFrameworkSlotDestroy
{
	void SetTitleAndDescription(string taskTitle, string taskDescription)
	{
		m_sTaskTitle = taskTitle;
		m_sTaskDescription = taskDescription;
	}
	
	override void Init(SCR_ScenarioFrameworkArea area = null, SCR_ScenarioFrameworkEActivationType activation = SCR_ScenarioFrameworkEActivationType.SAME_AS_PARENT)
	{
		if (m_bInitiated)
			return;
		
		if (!m_bDynamicallyDespawned && activation != m_eActivationType)
			return;
		
		bool tempTerminated = m_bIsTerminated;
		m_bIsTerminated = false;
		
		if (m_bIsTerminated)
			return;
						
		// Handles inheritance of faction settings from parents
		if (m_sFactionKey.IsEmpty() && m_ParentLayer && !m_ParentLayer.GetFactionKey().IsEmpty())
			SetFactionKey(m_ParentLayer.GetFactionKey());
		
		
		GetOnAllChildrenSpawned().Insert(AfterAllChildrenSpawned);
		
		
		if (!m_Entity)
		{
			InvokeAllChildrenSpawned();
			return;
		}
		
		if (!m_sID.IsEmpty())
			m_Entity.SetName(m_sID);	
		
		ScriptedDamageManagerComponent objectDmgManager = ScriptedDamageManagerComponent.Cast(m_Entity.FindComponent(ScriptedDamageManagerComponent));
		if (objectDmgManager)
			objectDmgManager.GetOnDamageStateChanged().Insert(OnObjectDamage);
		
		if (Vehicle.Cast(m_Entity))
		{
			EventHandlerManagerComponent ehManager = EventHandlerManagerComponent.Cast(m_Entity.FindComponent(EventHandlerManagerComponent));
			if (ehManager)
				ehManager.RegisterScriptHandler("OnCompartmentEntered", this, OnCompartmentEntered, false, true);
		}
			
		StoreTaskSubjectToParentTaskLayer();
		m_bIsTerminated = tempTerminated;
		
		InvokeAllChildrenSpawned();
	}
}

modded class SCR_ScenarioFrameworkSlotPick
{
	void SetTitleAndDescriptions(string taskTitle, string taskDescription, string titleUpdate1, string descriptionUpdate1, string titleUpdate2, string descriptionUpdate2)
	{
		m_sTaskTitle = taskTitle;
		m_sTaskDescription = taskDescription;
		m_sTaskTitleUpdated1 = titleUpdate1;
		m_sTaskDescriptionUpdated1 = descriptionUpdate1;
		m_sTaskTitleUpdated2 = titleUpdate2;
		m_sTaskDescriptionUpdated2 = descriptionUpdate2;
	}
}

modded class SCR_ScenarioFrameworkSlotDelivery
{	
	void AddAssociatedLayerTask(string layerTaskName)
	{
		if (!m_aAssociatedTaskLayers)
		{
			m_aAssociatedTaskLayers = new array<string>();
		}
		m_aAssociatedTaskLayers.Insert(layerTaskName);
	}

	void AddPlugin(SCR_ScenarioFrameworkPlugin plugin)
	{
		m_aPlugins = new array<ref SCR_ScenarioFrameworkPlugin>();
		m_aPlugins.Insert(plugin);
	}
	
	void DisableDebug()
	{
		m_bShowDebugShapesDuringRuntime = false;
	}
}

modded class SCR_ScenarioFrameworkLogicCounter
{
	void AddInput(SCR_ScenarioFrameworkLogicInput input)
	{
		m_aInputs.Insert(input);
	}

	void AddAction(SCR_ScenarioFrameworkActionBase action)
	{
		m_aActions.Insert(action);
	}

	void SetCountTo(int countTo)
	{
		m_iCountTo = countTo;
	}
}

modded class SCR_ScenarioFrameworkLogicInput
{
	void AddInputAction(SCR_ScenarioFrameworkActionInputBase actionInput)
	{
		m_InputAction = actionInput;
	}
}

modded class SCR_ScenarioFrameworkActionInputOnTaskEventIncreaseCounter
{
	void SetEventName(SCR_TaskState taskState)
	{
		m_eEventName = taskState;
	}
}

modded class SCR_ScenarioFrameworkActionEndMission
{
	void Setup()
	{
		m_iMaxNumberOfActivations = 1;
	}
	void SetGameOverType(EGameOverTypes type)
	{
		m_bOverrideGameOverType = true;
		m_eOverriddenGameOverType = type;
	}
}

modded class SCR_TaskDeliver
{
	override void Init()
	{
		super.Init();
		if (Vehicle.Cast(m_Asset))
		{
			EventHandlerManagerComponent ehManager = EventHandlerManagerComponent.Cast(m_Asset.FindComponent(EventHandlerManagerComponent));
			if (ehManager)
			{
				ehManager.RegisterScriptHandler("OnCompartmentEntered", this, OnCompartmentEntered, false);
				ehManager.RegisterScriptHandler("OnCompartmentLeft", this, OnCompartmentLeft, false);
			}
		}
	}
	
	// if player is killed while in vehicle, do nothing if there are other players inside, otherwise set task to state 5
	override void OnDestroyed(IEntity destroyedEntity)
	{
		super.OnDestroyed(destroyedEntity);
		int amountOfPlayersInCar = 0;
		SCR_BaseCompartmentManagerComponent comp = SCR_BaseCompartmentManagerComponent.Cast(m_Asset.FindComponent(SCR_BaseCompartmentManagerComponent));
		if(comp)
		{
			array<IEntity> occupants;
			comp.GetOccupants(occupants);
			foreach(IEntity occupant: occupants)
			{
				if(!EntityUtils.IsPlayer(occupant))
				{
					amountOfPlayersInCar++;
				}
			}
			if(amountOfPlayersInCar == 0)
			{
				SetState(SCR_TaskState.UPDATED);
				UpdateTaskTitleAndDescription(0);
			}
		}
	}
	
	void OnCompartmentEntered(IEntity vehicle, BaseCompartmentManagerComponent mgr, IEntity occupant, int managerId, int slotID)
	{
		if(!EntityUtils.IsPlayer(occupant))
		{
			return;
		}
		UpdateTaskTitleAndDescription(1);
		if (!m_bDeliveryItemFound)
		{
			m_bDeliveryItemFound = true;
			SetState(SCR_TaskState.PROGRESSED);
		}
	}
	
	void OnCompartmentLeft(IEntity vehicle, BaseCompartmentManagerComponent mgr, IEntity occupant, int managerId, int slotID)
	{
		if(!EntityUtils.IsPlayer(occupant))
		{
			return;
		}
		SetState(SCR_TaskState.UPDATED);
		UpdateTaskTitleAndDescription(0);
	}
}

modded class SCR_ScenarioFrameworkActionSpawnObjects
{
	void Setup()
	{
		m_iMaxNumberOfActivations = 1;
	}
}

modded class SCR_CharacterTriggerEntity
{
	//------------------------------------------------------------------------------------------------
	//! Returns number of players from the selected faction that are inside this trigger
	override int GetPlayersCountByFactionInsideTrigger(Faction faction)
	{
		int iCnt = 0;
		m_aPlayersInside.Clear();
		FactionAffiliationComponent factionAffiliationComponent;
		GetEntitiesInside(m_aEntitiesInside);
		foreach (IEntity entity : m_aEntitiesInside)
		{
			if (!entity)
				continue;
			
			if (!faction)
			{
				iCnt++;			//Faction not set == ANY faction
				m_aPlayersInside.Insert(entity);
			}
			else
			{
				factionAffiliationComponent = FactionAffiliationComponent.Cast(entity.FindComponent(FactionAffiliationComponent));
				if (!factionAffiliationComponent)
					continue;

				if (factionAffiliationComponent.GetAffiliatedFaction() != faction)
					continue;
				
				iCnt++;
				m_aPlayersInside.Insert(entity);
			}
		}

		return iCnt;
	}
}

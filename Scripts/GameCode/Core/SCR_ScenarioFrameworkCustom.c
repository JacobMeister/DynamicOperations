modded class SCR_ScenarioFrameworkActionSpawnObjects
{
	void AddObjectsToSpawn(string objectName)
	{
		if(!m_aNameOfObjectsToSpawnOnActivation)
		{
			m_aNameOfObjectsToSpawnOnActivation = new array<string>();
		}
		m_aNameOfObjectsToSpawnOnActivation.Insert(objectName);
	}
}

modded class SCR_ScenarioFrameworkPluginTrigger
{	
	void SetRadius(float radius)
	{
		m_fAreaRadius = radius;
	}
	
	void SetTriggerOnce(bool once)
	{
		m_bOnce = once;
	}
		
	void CreateCustomTriggerArray()
	{
		m_aCustomTriggerConditions = new array<ref SCR_CustomTriggerConditions>();
	}
	
	void EnableCountDown(float seconds)
	{
		m_bEnableAudio = true;
		m_sCountdownAudio = "SOUND_RESPAWNMENU";
		m_fActivationCountdownTimer = seconds;
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
}

modded class SCR_ScenarioFrameworkSlotKill
{
	void SetTitleAndDescription(string taskTitle, string taskDescription)
	{
		m_sTaskTitle = taskTitle;
		m_sTaskDescription = taskDescription;
	}
}

modded class SCR_ScenarioFrameworkSlotDestroy
{
	void SetTitleAndDescription(string taskTitle, string taskDescription)
	{
		m_sTaskTitle = taskTitle;
		m_sTaskDescription = taskDescription;
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
	void SetGameOverType(EGameOverTypes type)
	{
		m_bOverrideGameOverType = true;
		m_eOverriddenGameOverType = type;
	}
}
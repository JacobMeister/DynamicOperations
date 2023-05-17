class SCR_Location
{
	private string m_name;
	private IEntity m_entity;
	private IEntity m_slotParentEntity;
	private ref SCR_SlotManager m_slotManager;
	private ref array<vector> m_patrolLocations = new array<vector>();
	
	void SCR_Location(string name, IEntity entity, IEntity slotParentEntity)
	{
		m_name = name;
		m_entity = entity;
		m_slotParentEntity = slotParentEntity;
		
		m_slotManager = new SCR_SlotManager();
		m_slotManager.PopulateSlotsForLocation(slotParentEntity);
	}
	
	void ~SCR_Location()
	{
		
	}
	
	SCR_SlotManager GetSlotManager()
	{
		return m_slotManager;
	}
	
	string GetName()
	{
		return m_name; 
	}
	
	IEntity GetEntity()
	{
		return m_entity;
	}
	
	int GetAmountOfSoldiers()
	{
		int slotAmount = m_slotManager.GetAmountOfSlots();
		return Math.Round(slotAmount/2.5);
	}
	
	void AddPatrolLocation(vector patrolLocation)
	{
		m_patrolLocations.Insert(patrolLocation);
	}
	
	array<vector> GetPatrolLocations()
	{
		return m_patrolLocations;
	}
}
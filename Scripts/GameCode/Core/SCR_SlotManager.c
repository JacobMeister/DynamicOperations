class SCR_SlotManager
{
	private ref array<SCR_SiteSlotEntity> m_slots = new array<SCR_SiteSlotEntity>();
	private ref array<SCR_SiteSlotEntity> m_usedSlots = new array<SCR_SiteSlotEntity>();
	private ref RandomGenerator m_random;
	
	void SCR_SlotManager()
	{
		m_random = new RandomGenerator();
	}
	
	void ~SCR_SlotManager()
	{
		
	}
	
	// Retrieve all slots from hierarchy
	void PopulateSlotsForLocation(IEntity slotParentEntity)
	{
		SCR_SiteSlotEntity entity = SCR_SiteSlotEntity.Cast(slotParentEntity.GetChildren());
		m_slots.Insert(entity);
		
		while(entity)
		{
			entity = SCR_SiteSlotEntity.Cast(entity.GetSibling());
			if(entity)
			{
				m_slots.Insert(entity);
			}
		}
	}
	
	// Get total amount of slots
	int GetAmountOfSlots()
	{
		return m_slots.Count() + m_usedSlots.Count();
	}
	
	// Get amount of available slots
	int GetAvailableSlots()
	{
		return m_slots.Count();
	}
	
	// Get only road slots
	SCR_SiteSlotEntity GetRoadSlot()
	{
		foreach(SCR_SiteSlotEntity slot : m_slots)
		{
			if(!slot)
			{
				continue;
			}
			string prefabName = slot.GetPrefabData().GetPrefabName();
			if(prefabName== "{66265A506DF1FE2B}PrefabsEditable/Slots/Road/E_SlotRoadSmall.et" || prefabName == "{F4066B8425BEC47C}PrefabsEditable/Slots/Road/E_SlotRoadMedium.et" || prefabName== "{881CE7B0098504C2}PrefabsEditable/Slots/Road/E_SlotRoadLarge.et")
			{
						return slot;
			}
		}
		return null;
	}	
	
	// Get non road slots
	SCR_SiteSlotEntity GetRandomSlotSuitableForMissionTask()
	{
		SCR_SiteSlotEntity randomSlot =  m_slots[m_random.RandInt(0, m_slots.Count())];
		while(true)
		{
			if(!randomSlot)
			{
				continue;
			}
			string prefabName = randomSlot.GetPrefabData().GetPrefabName();
			if(prefabName!= "{66265A506DF1FE2B}PrefabsEditable/Slots/Road/E_SlotRoadSmall.et" && prefabName != "{F4066B8425BEC47C}PrefabsEditable/Slots/Road/E_SlotRoadMedium.et" && prefabName!= "{881CE7B0098504C2}PrefabsEditable/Slots/Road/E_SlotRoadLarge.et")
			{
						return randomSlot;
			}
			randomSlot =  m_slots[m_random.RandInt(0, m_slots.Count())];
		}
		return randomSlot;
	}	
	
	// Get random slot
	SCR_SiteSlotEntity GetRandomSlot()
	{
		return m_slots[m_random.RandInt(0, m_slots.Count())];
	}
	
	// move slot from slots to used slots array
	void MarkAsUsed(SCR_SiteSlotEntity slot)
	{
		int index = m_slots.Find(slot);
		m_usedSlots.Insert(m_slots[index]);
		m_slots.Remove(index);
	}
}
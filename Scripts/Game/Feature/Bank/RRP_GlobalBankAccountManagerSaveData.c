[BaseContainerProps()]
class RRP_GlobalBankAccountManagerSaveDataClass : EPF_EntitySaveDataClass
{
};

[EDF_DbName.Automatic()]
class RRP_GlobalBankAccountManagerSaveData : EPF_EntitySaveData
{
    ref array<ref RRP_BankAccount> m_aBankAccounts;
	
    //------------------------------------------------------------------------------------------------
    override EPF_EReadResult ReadFrom(IEntity entity, EPF_EntitySaveDataClass attributes)
    {
       ReadMetaData(EPF_Component<EPF_PersistenceComponent>.Find(entity));
		RRP_GlobalBankAccountManager BankManager = RRP_GlobalBankAccountManager.Cast(entity);
		m_aBankAccounts = BankManager.GetBankAccounts();

        return EPF_EReadResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override EPF_EApplyResult ApplyTo(IEntity entity, EPF_EntitySaveDataClass attributes)
    {	
        RRP_GlobalBankAccountManager BankManager = RRP_GlobalBankAccountManager.Cast(entity);
		BankManager.SetBankAccounts(m_aBankAccounts);
		
        return EPF_EApplyResult.OK;
    }
	
	//------------------------------------------------------------------------------------------------
    override protected bool SerializationSave(BaseSerializationSaveContext saveContext)
    {
        //if (!super.SerializationSave(saveContext))
        //    return false;
		SerializeMetaData(saveContext);
        saveContext.WriteValue("m_aBankAccounts", m_aBankAccounts);

        return true;
    }

    //------------------------------------------------------------------------------------------------
    override protected bool SerializationLoad(BaseSerializationLoadContext loadContext)
    {
       // if (!super.SerializationLoad(loadContext))
       //     return false;
		DeserializeMetaData(loadContext);
        loadContext.ReadValue("m_aBankAccounts", m_aBankAccounts);
        
        return true;
    }
};
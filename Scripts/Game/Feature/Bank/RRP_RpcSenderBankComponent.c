class RRP_RpcSenderBankComponentClass : ScriptComponentClass
{
}

class RRP_RpcSenderBankComponent  : ScriptComponent
{
	
	RRP_GlobalBankAccountManager m_BankManager;
	protected bool m_bIsMaster;
	
	
	//------------------------------------------------------------------------------------------------
	void AskPlayerBankAccount(RRP_BankAccount account)
	{
		Rpc(Rpc_SendPlayerBankAccount, account);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_SendPlayerBankAccount(RRP_BankAccount account)
	{
		m_BankManager.m_LocalBankAccount = null;
		
		m_BankManager.m_LocalBankAccount.m_iAccountId = 666;
		m_BankManager.m_LocalBankAccount.m_sAccountOwner = "jesus";
		m_BankManager.m_LocalBankAccount.m_iAccountBalance = 333;
		//m_BankManager.m_LocalBankAccount = account;
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void Rpc_LoadBankAccount()
	{
		m_BankManager.LoadPlayerBankAcount(GetOwner());
	}
	
	//------------------------------------------------------------------------------------------------
	void AskProcessTransaction(int amount, int transactionType, string reason, int targetAccount)
	{
		if (m_bIsMaster)
		{
			Rpc_SendProcessTransaction(amount, transactionType, reason, targetAccount);
		}
		else
		{
			Rpc(Rpc_SendProcessTransaction, amount, transactionType, reason, targetAccount);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void Rpc_SendProcessTransaction(int amount, int transactionType, string reason, int targetAccount)
	{
		RRP_BankAccount updatedAccount = m_BankManager.NewAccountTransaction(EL_Utils.GetPlayerUID(GetOwner()), transactionType, reason, amount, targetAccount);
		
		AskPlayerBankAccount(updatedAccount);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Init after Replication
	void LateInit(IEntity owner)
	{
		Print("Delayed Bank Account Check", LogLevel.DEBUG);
		m_bIsMaster = EL_NetworkUtils.IsMaster(owner);
		if (m_bIsMaster)
			Rpc_LoadBankAccount();
		else
			Rpc(Rpc_LoadBankAccount);
	}

	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		GetGame().GetCallqueue().CallLater(LateInit, 4000, false, owner);
		m_BankManager = RRP_GlobalBankAccountManager.GetInstance();
	}

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
	}
}
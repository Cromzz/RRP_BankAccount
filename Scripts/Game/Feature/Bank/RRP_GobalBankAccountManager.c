[EntityEditorProps(category: "ReactiveRoleplay/Bank", description: "Global bank account manager to manage to creation and storage of bank related activities.")]
class RRP_GlobalBankAccountManagerClass : GenericEntityClass
{
};

class RRP_GlobalBankAccountManager : GenericEntity
{
	[Attribute("1000", UIWidgets.Slider, "Set the starting Bank Account amount", "1000 15000 1")]
	private int m_iBankAccountStartingAmount;
	
	protected static RRP_GlobalBankAccountManager s_pInstance;
	
	protected ref array<ref RRP_BankAccount> m_aBankAccounts;
	
	RRP_BankAccount m_LocalBankAccount;

	
	//------------------------------------------------------------------------------------------------
	static RRP_GlobalBankAccountManager GetInstance()
	{
		return s_pInstance;
	}
	
	//------------------------------------------------------------------------------------------------
	array<ref RRP_BankAccount> GetBankAccounts()
	{
		return m_aBankAccounts;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetBankAccounts(array<ref RRP_BankAccount> accounts)
	{
		m_aBankAccounts = accounts;
		
	}
	
	//------------------------------------------------------------------------------------------------
	//Called From Auth
	void LoadPlayerBankAcount(IEntity player)
	{
		PrintFormat("Attempting to load Bank Account for Player: %1", EL_Utils.GetPlayerUID(player));
		PlayerManager pm = GetGame().GetPlayerManager();
		// All players because the message could be from someone who already disconnected
		array<int> players = {};
		pm.GetAllPlayers(players);
		foreach(int user : players)
		{
			PrintFormat("PlayerID:%1", user);
			PrintFormat("### %1", EL_Utils.GetPlayerUID(GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player)));
			PrintFormat("---------------------------------");
		}
		
		RRP_BankAccount foundAccount = GetBankAccount(EL_Utils.GetPlayerUID(player));
		RRP_RpcSenderBankComponent bankRpcComponent = EPF_Component<RRP_RpcSenderBankComponent>.Find(GetInstance());
		
		if (foundAccount != null)
		{
			bankRpcComponent.AskPlayerBankAccount(foundAccount); //we found the players account
		}
		else
		{
			PrintFormat("No Bank Account found for %1, Creating Bank Account", EL_Utils.GetPlayerUID(player));
			RRP_BankAccount newAccount = CreateBankAccount(player); 
			bankRpcComponent.AskPlayerBankAccount(newAccount); //set local bankaccount on new spawn
		}
	}
	
	//------------------------------------------------------------------------------------------------
	RRP_BankAccount GetPlayerBankAccount(IEntity player)
	{
		RRP_BankAccount foundAccount = GetBankAccount(EL_Utils.GetPlayerUID(GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player)));
		if (foundAccount != null)
		{
			return foundAccount;
		}		
		
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	RRP_BankAccount CreateBankAccount(IEntity player)
	{
		return CreateBankAccount(GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player));
	}
	
	//------------------------------------------------------------------------------------------------
	RRP_BankAccount CreateBankAccount(int playerId)
	{
		int newAccountId = GetRandomFreeAccountId();
		RRP_BankAccount newAccount = RRP_BankAccount.Create(EL_Utils.GetPlayerUID(playerId), newAccountId, m_iBankAccountStartingAmount);
		m_aBankAccounts.Insert(newAccount);
		PrintFormat("New bank account created for: %1, account number: %2", GetGame().GetPlayerManager().GetPlayerName(playerId), newAccountId);
		return newAccount;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Gets a random free 8 digit id
	
	int GetRandomFreeAccountId()
	{
		int newAccountId = Math.RandomInt(111, 999);
		foreach (RRP_BankAccount account : m_aBankAccounts)
		{
			if (account.getId() == newAccountId)
				return GetRandomFreeAccountId();
		}
		return newAccountId;
	}
	
	//------------------------------------------------------------------------------------------------
	// Transaction Type 0 = Deposit / 1 = Withdraw

	RRP_BankAccount NewAccountTransaction(string accountOwner, int transactionType, string reason, int amount, int targetAccount)
	{	
		RRP_BankAccount sourceAccount = GetBankAccount(accountOwner);
		
		if (sourceAccount != null) 
		{
			if (sourceAccount.getId() == targetAccount)
			{
				//If the account we are transfering to is our own then deposit to our own
				if (transactionType == 0) 
				{
					bool depositResult = sourceAccount.TryToDeposit(amount, reason, sourceAccount.getId(), targetAccount);
					if(!depositResult)
					Print("Failed to Deposit", LogLevel.NORMAL);
				}
				else
				{
					if (transactionType == 1)
					{
						sourceAccount.TryToWithdraw(amount, reason, sourceAccount.getId(), targetAccount);
					}
					else
					{
						Print("INVALID TRANSACTION TYPE", LogLevel.ERROR);
					}
				}
				return sourceAccount;
			}
			else
			{
				//If it is not our own then transfer to the target account
				sourceAccount = GetBankAccountById(targetAccount);
				if (transactionType == 0) 
				{
					bool depositResult = sourceAccount.TryToDeposit(amount, reason, sourceAccount.getId(), targetAccount);
					if(!depositResult)
					Print("Failed to Deposit", LogLevel.NORMAL);
				}
				else
				{
					if (transactionType == 1)
					{
						sourceAccount.TryToWithdraw(amount, reason, sourceAccount.getId(), targetAccount);
					}
					else
					{
						Print("INVALID TRANSACTION TYPE", LogLevel.ERROR);
					}
				}
				return sourceAccount;
			}
			
		}
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	RRP_BankAccount GetBankAccount(string accountOwner)
	{
		PrintFormat("--------------Checking Account-----------------");
		PrintFormat("Accounts loaded: %1", m_aBankAccounts.Count());
		
		foreach (RRP_BankAccount account : m_aBankAccounts)
		{
			PrintFormat("checking if %1 is equal to %2", account.GetAccountOwnerId(), accountOwner);
			if (account.GetAccountOwnerId() == accountOwner)
			{
				PrintFormat("%1 is equal to %2", account.GetAccountOwnerId(), accountOwner);
				return account;
			}
			
		}
		PrintFormat("Unable to find %1 NOT EQUAL", accountOwner);
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	RRP_BankAccount GetBankAccountById(int accountId)
	{
		foreach (RRP_BankAccount account : m_aBankAccounts)
		{
			if (account.getId() == accountId)
			{
				return account;
			}
			return null;
		}
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	void RRP_GlobalBankAccountManager(IEntitySource src, IEntity parent)
	{
		s_pInstance = this;
		m_aBankAccounts = new array<ref RRP_BankAccount>();
		SetFlags(EntityFlags.ACTIVE, false);
	}
	


	
	
};
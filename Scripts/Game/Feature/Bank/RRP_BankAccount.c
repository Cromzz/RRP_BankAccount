class RRP_BankAccount
{


	[RplProp()]
	string m_sAccountOwner;
	
	[RplProp()]
	int m_iAccountBalance = 0;
	
	[RplProp()]
	int m_iAccountId;
	
  
	protected static const int MAX_REPLICATED_TRANSACTIONS = 10;
	
	//[RplProp(onRplName: "onBankAccountChanged")]
	ref array<ref RRP_BankTransaction> m_aTransactions = new array<ref RRP_BankTransaction>();
	
	//------------------------------------------------------------------------------------------------
	//Create the bank accout and return it
	static RRP_BankAccount Create(string accountOwner, int accountId, int startingBalance)
	{
		RRP_BankAccount bankAccount();
		
		bankAccount.m_iAccountBalance = startingBalance;
		bankAccount.m_sAccountOwner = accountOwner;
		bankAccount.m_iAccountId = accountId;
		
		return bankAccount;
	}
	
	//------------------------------------------------------------------------------------------------
	//Return the Account ID
	int getId()
	{
		return m_iAccountId;
	}
	
	//------------------------------------------------------------------------------------------------
	//Return the Owner of the Account
	IEntity GetAccountOwner()
	{
		return EL_Utils.GetPlayerByUID(m_sAccountOwner);
	}
	
	string GetAccountOwnerId()
	{
		return m_sAccountOwner;
	}
	
	//------------------------------------------------------------------------------------------------
	//Get the balance of the account
	int GetBalance()
	{
		return m_iAccountBalance;
	}
		
	//------------------------------------------------------------------------------------------------
	//Try to deposit cash into Account
	bool TryToDeposit(int amount, string reason, int sourceAccount, int targetAccount)
	{
		PrintFormat("Tring to Deposit: %1, into account: %2, for reason: %3", amount, sourceAccount, reason);
		
		if (EL_MoneyUtils.GetCash(GetAccountOwner()) < amount)
			return false;
		
		int cashRemoved = EL_MoneyUtils.RemoveCash(GetAccountOwner(), amount);
		if (cashRemoved == 0)
		{
			Print("Cash could not be removed from player when depositing to bank", LogLevel.ERROR);
			return false;
		}
		m_iAccountBalance += cashRemoved;
		NewTransaction(amount, sourceAccount, targetAccount, reason, 0);
		PrintFormat("Deposited: %1, into account: %2, for reason: %3", amount, sourceAccount, reason);
		//Use RestAPI to push Log to Discord
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	//Try to deposit cash into Account
	bool TryToWithdraw(int amount, string reason, int sourceAccount, int targetAccount)
	{
		PrintFormat("Tring to Withdraw: %1, from account: %2, for reason: %3", amount, sourceAccount, reason);
		if (m_iAccountBalance < amount)
		{
			Print("DID NOT HAVE ENOUGH FUNDS TO WITHDRAW", LogLevel.NORMAL);
			return false;
		}
		m_iAccountBalance -= EL_MoneyUtils.AddCash(GetAccountOwner(), amount);
		NewTransaction(amount, sourceAccount, targetAccount, reason, 1);
		PrintFormat("Withdrew: %1, from account: %2, for reason: %3", amount, sourceAccount, reason);
		//Use RestAPI to push Log to Discord
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	//Try to deposit cash into Account
	void NewTransaction(int amount, int sourceAccount, int targetAccount, string reason, int type)
	{
		PrintFormat("Created New Transaction for account: %1, for amount: %2, for reason: %3", sourceAccount, amount, reason);
		RRP_BankTransaction newTransaction = RRP_BankTransaction.Create(amount, SCR_DateTimeHelper.GetDateTimeLocal(), sourceAccount, targetAccount, reason, type);
		m_aTransactions.InsertAt(newTransaction, 0);
	}
	
	
	
	//------------------------------------------------------------------------------------------------
	/*!
		Serialize this class using provided ScriptBitWriter.
	*/
	bool RplSave(ScriptBitWriter writer)
	{
		writer.WriteInt(m_iAccountId);
		writer.WriteInt(m_iAccountBalance);
		writer.WriteString(m_sAccountOwner);
		
		return true;
	}
	
	/*!
		Deserialize this class using provided ScriptBitWriter.
	*/
    bool RplLoad(ScriptBitReader reader)
	{
		reader.ReadInt(m_iAccountId);
		reader.ReadInt(m_iAccountBalance);
		reader.ReadString(m_sAccountOwner);
		
		return true;
	}	
	
	//################################################################################################
	//! Codec methods
	//------------------------------------------------------------------------------------------------
	
	static bool Extract(RRP_BankAccount instance, ScriptCtx ctx, SSnapSerializerBase snapshot) 
	{
		
		snapshot.SerializeInt(instance.m_iAccountId);
		snapshot.SerializeInt(instance.m_iAccountBalance);
		snapshot.SerializeString(instance.m_sAccountOwner);
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	static bool Inject(SSnapSerializerBase snapshot, ScriptCtx ctx, RRP_BankAccount instance) 
	{
		snapshot.SerializeInt(instance.m_iAccountId);
		snapshot.SerializeInt(instance.m_iAccountBalance);
		snapshot.SerializeString(instance.m_sAccountOwner);	
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	static void Encode(SSnapSerializerBase snapshot, ScriptCtx ctx, ScriptBitSerializer packet) 
	{
		PrintFormat("Size of snap, %1", snapshot.GetSizeOf());
		snapshot.EncodeInt(packet); 	//m_iAccountId
		snapshot.EncodeInt(packet); 	//m_iAccountBalance
		snapshot.EncodeString(packet); 	//m_sAccountOwner

	}
	
	//------------------------------------------------------------------------------------------------
	static bool Decode(ScriptBitSerializer packet, ScriptCtx ctx, SSnapSerializerBase snapshot) 
	{
		snapshot.DecodeInt(packet); 	//m_iAccountId
		snapshot.DecodeInt(packet); 	//m_iAccountBalance
		snapshot.DecodeString(packet); 	//m_sAccountOwner
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx ctx) 
	{		
		return lhs.CompareSnapshots(rhs, 4 + 4)
		&& lhs.CompareStringSnapshots(rhs);
		
	}

	//------------------------------------------------------------------------------------------------
	static bool PropCompare(RRP_BankAccount instance, SSnapSerializerBase snapshot, ScriptCtx ctx) 
	{
		return snapshot.CompareInt(instance.m_iAccountId)
			&& snapshot.CompareInt(instance.m_iAccountBalance)
			&& snapshot.CompareString(instance.m_sAccountOwner);
	}
			

	



}



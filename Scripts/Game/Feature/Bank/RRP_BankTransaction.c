class RRP_BankTransaction
{
	int m_iAmount;
	string m_iDate;
	int m_iSourceAccount;
	int m_iTargetAccount;
	string m_sReason;
	int m_iType;
	
	//------------------------------------------------------------------------------------------------
	static RRP_BankTransaction Create(int amount, string date, int sourceAccount, int targetAccount, string reason, int type)
	{
		RRP_BankTransaction bankTransaction();
		
		bankTransaction.m_iAmount = amount;
		bankTransaction.m_iDate = date;
		bankTransaction.m_iSourceAccount = sourceAccount;
		bankTransaction.m_iTargetAccount = targetAccount;
		bankTransaction.m_sReason = reason;
		bankTransaction.m_iType = type;
		
		return bankTransaction;
	}
}
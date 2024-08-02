class RRP_BankCheckBalanceAction : ScriptedUserAction
{
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		RRP_RpcSenderBankComponent RpcBankComponent = EL_NetworkUtils.GetLocalBankRpcSender();
		RRP_GlobalBankAccountManager BankManager = RRP_GlobalBankAccountManager.GetInstance();
		RRP_BankAccount playerAccount = BankManager.m_LocalBankAccount;
		if (playerAccount != null)
		{
			PrintFormat("My account balance is: %1", playerAccount.GetBalance());
		}
		else
		{
			Print("Bank Account MISSING!", LogLevel.ERROR);
		}
	}
	
}

class RRP_BankIncreaseBalanceAction : ScriptedUserAction
{
	[Attribute(defvalue: "1", uiwidget: UIWidgets.EditBox, desc: "Amount")]
	protected int m_iAmount;
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		RRP_RpcSenderBankComponent RpcBankComponent = EL_NetworkUtils.GetLocalBankRpcSender();
		RRP_GlobalBankAccountManager BankManager = RRP_GlobalBankAccountManager.GetInstance();
		RRP_BankAccount playerAccount = BankManager.m_LocalBankAccount;
		if (playerAccount != null)
		{
			RpcBankComponent.AskProcessTransaction(m_iAmount, 1, "Processed deposit request by test action", BankManager.m_LocalBankAccount.getId()) 
			//BankManager.NewAccountTransaction(EL_Utils.GetPlayerUID(GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(pUserEntity)), 0, "Processed deposit request by test action", m_iAmount, BankManager.GetBankAccount(EL_Utils.GetPlayerUID(GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(pUserEntity))).getId());
		}
		else
		{
			Print("Bank Account MISSING!", LogLevel.ERROR);
			//BankManager.CreateBankAccount(pUserEntity);
		}
	}

}
	
class RRP_BankDecreaseBalanceAction : ScriptedUserAction
{
	[Attribute(defvalue: "1", uiwidget: UIWidgets.EditBox, desc: "Amount")]
	protected int m_iAmount;
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		RRP_RpcSenderBankComponent RpcBankComponent = EL_NetworkUtils.GetLocalBankRpcSender();
		RRP_GlobalBankAccountManager BankManager = RRP_GlobalBankAccountManager.GetInstance();
		RRP_BankAccount playerAccount = BankManager.m_LocalBankAccount;
		if (playerAccount != null)
		{
			RpcBankComponent.AskProcessTransaction(m_iAmount, 0, "Processed withdraw request by test action", BankManager.m_LocalBankAccount.getId()) 
			//BankManager.NewAccountTransaction(EL_Utils.GetPlayerUID(GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(pUserEntity)), 1, "Processed Withdraw request by test action", m_iAmount, BankManager.GetBankAccount(EL_Utils.GetPlayerUID(GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(pUserEntity))).getId());
		}
		else
		{
			Print("Bank Account MISSING!", LogLevel.ERROR);
			//BankManager.CreateBankAccount(pUserEntity);
		}
	}

	

}

class RRP_BankOpenBankUIAction : ScriptedUserAction
{

    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        if(GetGame().GetPlayerController().GetControlledEntity() == pUserEntity)
            GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.BankAccount);    
    }
	
	//if HasLocalEffectOnly returns true, it will be executing only on the client where the action has been trigerred 
	//if HasLocalEffectOnly returns false, then it will be exeucted only on the client where the action has been trigered and server 
	//if HasLocalEffectOnly returns false and CanBroadcast returns true, then it will be exeucted on client where the action has been trigerred and server and everybody else.        
	    
	//------------------------------------------------------------------------------------------------
	override bool HasLocalEffectOnlyScript()
	{
	    return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBroadcastScript()
	{
	    return false;
	} 

}







	


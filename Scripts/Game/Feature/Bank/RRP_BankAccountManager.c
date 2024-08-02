[EntityEditorProps(category: "Persistence", description: "Global bank account manager to manage to creation and storage of bank related activities.")]
class RRP_BankAccountManagerClass : GenericEntityClass
{
	
}

class RRP_BankAccountManager : GenericEntity
{
	[Attribute(defvalue: "1000", desc: "Starting Amount", category: "Account")]
	private int m_iBankAccountStartingAmount;

}

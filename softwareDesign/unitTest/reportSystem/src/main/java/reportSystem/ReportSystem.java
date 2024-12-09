package reportSystem;

public class ReportSystem {
	private LoginManager loginmgr;
    
    // Constructor that allows injection of a LoginManager or its subclass
    public ReportSystem(LoginManager loginmgr) {
        this.loginmgr = loginmgr;
    }
    
	public boolean submit(String report) {
		if (loginmgr.isValid()) {
			// submit the report
			return true;
		}
		return false;
	}

}

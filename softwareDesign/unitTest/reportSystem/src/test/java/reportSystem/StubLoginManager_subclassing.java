package reportSystem;

public class StubLoginManager_subclassing extends LoginManager {
	@Override
    public boolean isValid() {
        return true; // Always return true for testing
    }
}

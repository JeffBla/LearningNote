package reportSystem;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

class ReportSubmissionTest {

	@Test
	void testIsValidLogFileName() {
        ReportSystem reportSystem = 
        		new ReportSystem(new StubLoginManager_subclassing());
        
        boolean result = reportSystem.submit("test report");
        assertTrue(result);
    }
}

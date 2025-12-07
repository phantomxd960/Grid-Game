package tests_java;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

public class TestBaseBackendJNI {

    static class MockBaseBackend extends BaseBackendJNI {
        private int gold = 0;

        @Override
        public native long createBase() { return 1; }

        @Override
        public native void addGold(long ptr, int amount) { gold += amount; }

        @Override
        public native int getGold(long ptr) { return gold; }
    }

    @Test
    void testGoldMock() {
        MockBaseBackend mock = new MockBaseBackend();
        mock.addGold(1, 300);
        assertEquals(300, mock.getGold(1));
    }
}

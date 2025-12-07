package tests_java;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

public class TestDragonGameBackendJNI {

    static class MockDragonBackend extends DragonGameBackendJNI {
        int hp = 4000;

        @Override
        public native long createDragon() { return 1; }

        @Override
        public native int getDragonHealth(long ptr) { return hp; }

        @Override
        public native boolean attackDragon(long p, long d, int dmg) {
            hp -= dmg;
            return hp <= 0;
        }
    }

    @Test
    void testDragonDamage() {
        MockDragonBackend mock = new MockDragonBackend();
        mock.attackDragon(1,1,500);
        assertEquals(3500, mock.getDragonHealth(1));
    }
}

package tests_java;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

public class TestDragonGameFrontend {

    @Test
    void testDragonGameLoads() {
        DragonGameFrontend f = new DragonGameFrontend();
        assertNotNull(f);
    }

    @Test
    void testDragonHealth() {
        DragonGameFrontend f = new DragonGameFrontend();
        assertEquals(4000, f.getDragonHealth());
    }

    @Test
    void testPlayerAttackDoesNotCrash() {
        DragonGameFrontend f = new DragonGameFrontend();
        assertDoesNotThrow(() -> f.attackDragon(200));
    }
}

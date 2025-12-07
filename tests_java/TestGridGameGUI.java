package tests_java;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

public class TestGridGameGUI {

    @Test
    void testGUIConstructs() {
        GridGameGUI gui = new GridGameGUI();
        assertNotNull(gui);
    }

    @Test
    void testInitialPlayerPosition() {
        GridGameGUI gui = new GridGameGUI();
        int[] pos = gui.getPlayerPosition();
        assertEquals(0, pos[0]);
        assertEquals(0, pos[1]);
    }

    @Test
    void testMovePlayerDoesNotCrash() {
        GridGameGUI gui = new GridGameGUI();
        assertDoesNotThrow(() -> gui.movePlayer("RIGHT"));
    }
}

package tests_java;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

public class TestGridGameBackendJNI {

    @Test
    void testEnemyMoveMock() {
        GridGameBackendJNI backend = new GridGameBackendJNI();

        char[][] grid = new char[10][10];
        int[] player = {5,5};
        int[] enemy = {7,7};

        assertDoesNotThrow(() -> backend.calculateEnemyMove(grid, player, enemy));
    }
}

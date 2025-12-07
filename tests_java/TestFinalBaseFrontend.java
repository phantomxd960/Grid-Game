package tests_java;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

public class TestFinalBaseFrontend {

    @Test
    void testFrontendConstructs() {
        FinalBaseFrontend f = new FinalBaseFrontend();
        assertNotNull(f);
    }

    @Test
    void testGoldStartsAtZero() {
        FinalBaseFrontend f = new FinalBaseFrontend();
        assertEquals(0, f.getGold());
    }

    @Test
    void testAddGoldIncreasesAmount() {
        FinalBaseFrontend f = new FinalBaseFrontend();
        f.addGold(500);
        assertEquals(500, f.getGold());
    }
}

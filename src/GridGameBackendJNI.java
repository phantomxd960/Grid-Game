package src;
public class GridGameBackendJNI {
    // Load the shared library that contains the native methods
    static {
        System.loadLibrary("GridGameBackend"); 
    }

    /**
     * Native method to calculate the enemy's next move.
     *
     * @param grid     The current game grid represented as a 2D character array.
     * @param playerPos The player's current position as a 2-element array [x, y].
     * @param enemyPos  The enemy's current position as a 2-element array [x, y].
     * @return The enemy's new position as a 2-element array [x, y].
     */
    public native int[] calculateEnemyMove(char[][] grid, int[] playerPos, int[] enemyPos);

    /**
     * A utility method to convert the 2D grid to a format suitable for debugging (if needed).
     */
    public static void printGrid(char[][] grid) {
        for (char[] row : grid) {
            for (char cell : row) {
                System.out.print(cell + " ");
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        // Example usage of the JNI functionality
        char[][] grid = {
                { '.', '.', '.', '.', '.', '.', '.', '.', '.', '.' },
                { '.', '.', '.', '.', '.', '.', '.', '.', '.', '.' },
                { '.', '.', '.', '.', '.', '.', '.', '.', '.', '.' },
                { '.', '.', '.', '.', '.', '.', '.', '.', '.', '.' },
                { '.', '.', '.', '.', '.', '.', '.', '.', '.', '.' },
                { '.', '.', '.', '.', '.', '.', '.', '.', '.', '.' },
                { '.', '.', '.', '.', '.', '.', '.', '.', '.', '.' },
                { '.', '.', '.', '.', '.', '.', '.', '.', '.', '.' },
                { '.', '.', '.', '.', '.', '.', '.', '.', '.', '.' },
                { '.', '.', '.', '.', '.', '.', '.', '.', '.', '.' },
        };

        int[] playerPos = { 0, 0 };
        int[] enemyPos = { 9, 9 };

        GridGameBackendJNI game = new GridGameBackendJNI();
        int[] newEnemyPos = game.calculateEnemyMove(grid, playerPos, enemyPos);

        System.out.println("New enemy position: [" + newEnemyPos[0] + ", " + newEnemyPos[1] + "]");
    }
}
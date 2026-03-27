package src;

import javax.swing.*;
import java.awt.*;
import java.util.*;

public class GridGameGUI extends JFrame {
    static final int GRID_SIZE = 10;
    static final char EMPTY = '.';
    static final char PLAYER = 'P';
    static final char ENEMY = 'E';
    static final char WALL = 'x';

    private char[][] grid;
    private int[] playerPos;
    private int[] enemyPos;
    private JLabel[][] gridLabels;
    private int moveCounter = 0;
    private Queue<String> moveHistory = new LinkedList<>(); // To track repeated movements

    static {
        System.loadLibrary("GridGameBackend"); // Load the native library
    }

    // Paths to images
    private final String PLAYER_IMAGE_PATH = "images/player.png";
    private final String ENEMY_IMAGE_PATH = "images/enemy.png";
    private final String WALL_IMAGE_PATH = "images/wall.png";
    private final String EMPTY_IMAGE_PATH = "images/empty.png";

    private ImageIcon playerIcon;
    private ImageIcon enemyIcon;
    private ImageIcon wallIcon;
    private ImageIcon emptyIcon;

    public GridGameGUI() {
        grid = new char[GRID_SIZE][GRID_SIZE];
        playerPos = new int[]{0, 0};
        enemyPos = new int[]{GRID_SIZE - 1, GRID_SIZE - 1};

        loadIcons();
        initializeGrid(grid, playerPos, enemyPos);
        initializeUI();
    }

    private void loadIcons() {
        try {
            playerIcon = new ImageIcon(PLAYER_IMAGE_PATH);
            enemyIcon = new ImageIcon(ENEMY_IMAGE_PATH);
            wallIcon = new ImageIcon(WALL_IMAGE_PATH);
            emptyIcon = new ImageIcon(EMPTY_IMAGE_PATH);
        } catch (Exception e) {
            System.err.println("Error loading images: " + e.getMessage());
        }
    }

    private void initializeUI() {
        setTitle("Grid Game");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        JPanel gridPanel = new JPanel(new GridLayout(GRID_SIZE, GRID_SIZE));
        gridLabels = new JLabel[GRID_SIZE][GRID_SIZE];

        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                gridLabels[i][j] = new JLabel();
                gridLabels[i][j].setHorizontalAlignment(SwingConstants.CENTER);
                gridLabels[i][j].setBorder(BorderFactory.createLineBorder(Color.BLACK));
                gridLabels[i][j].setPreferredSize(new Dimension(50, 50));
                updateLabelIcon(i, j);
                gridPanel.add(gridLabels[i][j]);
            }
        }

        add(gridPanel, BorderLayout.CENTER);

        JPanel controlPanel = new JPanel();
        controlPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 10, 10));
        JButton upButton = new JButton("Up");
        JButton downButton = new JButton("Down");
        JButton leftButton = new JButton("Left");
        JButton rightButton = new JButton("Right");

        controlPanel.add(upButton);
        controlPanel.add(downButton);
        controlPanel.add(leftButton);
        controlPanel.add(rightButton);
        add(controlPanel, BorderLayout.SOUTH);

        upButton.addActionListener(e -> handleMove('w'));
        downButton.addActionListener(e -> handleMove('s'));
        leftButton.addActionListener(e -> handleMove('a'));
        rightButton.addActionListener(e -> handleMove('d'));

        pack();
        setLocationRelativeTo(null);
        setVisible(true);
    }

    private void initializeGrid(char[][] grid, int[] playerPos, int[] enemyPos) {
        do {
            for (int i = 0; i < GRID_SIZE; i++) {
                Arrays.fill(grid[i], EMPTY);
            }

            grid[playerPos[0]][playerPos[1]] = PLAYER;
            grid[enemyPos[0]][enemyPos[1]] = ENEMY;

            Random random = new Random();
            for (int i = 0; i < GRID_SIZE; i++) {
                for (int j = 0; j < GRID_SIZE; j++) {
                    if (random.nextInt(10) < 2 && grid[i][j] == EMPTY) {
                        grid[i][j] = WALL;
                    }
                }
            }
        } while (!isPathAvailable());
    }

    private boolean isPathAvailable() {
        boolean[][] visited = new boolean[GRID_SIZE][GRID_SIZE];
        return dfs(playerPos[0], playerPos[1], visited);
    }

    private boolean dfs(int x, int y, boolean[][] visited) {
        if (x == enemyPos[0] && y == enemyPos[1]) {
            return true;
        }

        if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE || visited[x][y] || grid[x][y] == WALL) {
            return false;
        }

        visited[x][y] = true;

        return dfs(x + 1, y, visited) || dfs(x - 1, y, visited) || dfs(x, y + 1, visited) || dfs(x, y - 1, visited);
    }

    private void updateLabelIcon(int i, int j) {
        char cell = grid[i][j];
        ImageIcon icon = switch (cell) {
            case PLAYER -> playerIcon;
            case ENEMY -> enemyIcon;
            case WALL -> wallIcon;
            default -> emptyIcon;
        };

        if (icon != null) {
            Image scaledImage = icon.getImage().getScaledInstance(50, 50, Image.SCALE_SMOOTH);
            gridLabels[i][j].setIcon(new ImageIcon(scaledImage));
        } else {
            gridLabels[i][j].setIcon(null);
        }
    }

    private void handleMove(char move) {
        int newX = playerPos[0];
        int newY = playerPos[1];
    
        if (move == 'w') newX--;
        else if (move == 's') newX++;
        else if (move == 'a') newY--;
        else if (move == 'd') newY++;
    
        if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE && grid[newX][newY] != WALL) {
            grid[playerPos[0]][playerPos[1]] = EMPTY;
            playerPos[0] = newX;
            playerPos[1] = newY;
            grid[playerPos[0]][playerPos[1]] = PLAYER;
    
            moveCounter++;
            recordMove(move);
    
            // Check for overlap after player's move
            if (playerPos[0] == enemyPos[0] && playerPos[1] == enemyPos[1]) {
                refreshGrid();
                gameOver();
                return;
            }
    
            moveEnemy();
            refreshGrid();
        }
    }
    

    private void recordMove(char move) {
        if (moveHistory.size() == 4) {
            moveHistory.poll();
        }
        moveHistory.offer(String.valueOf(move));
    }

    private boolean isRepeatedMovePattern() {
        if (moveHistory.size()%4 !=0) return false;

        String[] moves = moveHistory.toArray(new String[0]);
        return moves[0].equals(moves[1]) && moves[1].equals(moves[2]) && moves[2].equals(moves[3]);
    }

    private void moveEnemy() {
        grid[enemyPos[0]][enemyPos[1]] = EMPTY;
        if (isRepeatedMovePattern()) {
            // System.out.println("Enemy counters repetitive player movement!");
            enemyPos[0] = Math.max(0, Math.min(GRID_SIZE - 1, enemyPos[0] + (int) (Math.random() * 3 - 1)));
            enemyPos[1] = Math.max(0, Math.min(GRID_SIZE - 1, enemyPos[1] + (int) (Math.random() * 3 - 1)));
        } else {
            int[] newEnemyPos = calculateEnemyMove(grid, playerPos, enemyPos);
            enemyPos[0] = newEnemyPos[0];
            enemyPos[1] = newEnemyPos[1];
        }
        grid[enemyPos[0]][enemyPos[1]] = ENEMY;
    
        // Check for overlap after enemy's move
        if (playerPos[0] == enemyPos[0] && playerPos[1] == enemyPos[1]) {
            refreshGrid();
            gameOver();
        }
    }
    

    private void refreshGrid() {
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                updateLabelIcon(i, j);
            }
        }
    }

    private void gameOver() {
        JOptionPane.showMessageDialog(this, "Game Over! Total Moves: " + moveCounter, "Game Over", JOptionPane.INFORMATION_MESSAGE);
        System.out.println("Total moves: " + moveCounter);
        dispose();  // End the game

    }

    public int getMoveCount() {
        return moveCounter;
    }
    
    public void playGame() {
        // Set the game window visible
        SwingUtilities.invokeLater(() -> setVisible(true));
    
        // Busy-wait until the game is complete (blocking)
        while (isVisible()) {
            try {
                Thread.sleep(100); // Sleep briefly to reduce CPU usage
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                System.out.println("Game interrupted.");
                break;
            }
        }
    }
    

    private native int[] calculateEnemyMove(char[][] grid, int[] playerPos, int[] enemyPos);

    

    public static void main(String[] args) {
        SwingUtilities.invokeLater(GridGameGUI::new);
    }
}

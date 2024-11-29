package src;

import java.util.Scanner;

public class FinalBaseFrontend {
    private static BaseBackendJNI backend = new BaseBackendJNI();
    private static long base;

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        base = backend.createBase();

        // Display story introduction
        displayStory();

        while (true) {
            System.out.println("\n--- Base Menu ---");
            System.out.println("1. Add Gold");
            System.out.println("2. View Gold");
            System.out.println("3. Build a Structure");
            System.out.println("4. Display Status");
            System.out.println("5. Fight the Dragon");
            System.out.println("6. Quit");
            System.out.print("Enter your choice: ");
            int choice = scanner.nextInt();
            scanner.nextLine(); // Consume newline

            switch (choice) {
                case 1 -> addGold();
                case 2 -> viewGold();
                case 3 -> buildStructure(scanner);
                case 4 -> displayStatus();
                case 5 -> {
                    if (canFightDragon()) {
                        fightDragon();
                    } else {
                        System.out.println("Dragon battle unlocks only after Townhall reaches level 5 and maximum structures are built!");
                    }
                }
                case 6 -> {
                    System.out.println("Exiting the game. Goodbye!");
                    backend.destroyBase(base);
                    scanner.close();
                    return;
                }
                default -> System.out.println("Invalid choice. Please try again.");
            }
        }
    }

    private static void displayStory() {
        String content = """
            Welcome to Your Base: A Realm of Epic Building and Strategy!
            
            Prepare to embark on an extraordinary journey as you build your fortress to take on the ultimate challenge—the mighty DRAGON!
            
            Are You Ready to Build Your Legacy?
            - Each click takes you closer to glory.
            - Your strategy, timing, and decision-making will determine your fate.
            
            The fate of your base—and the world—rests in your hands. Build wisely, fight bravely, and conquer the dragon!
        """;

        for (char c : content.toCharArray()) {
            System.out.print(c);
            try {
                Thread.sleep(30); // Typing effect
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        System.out.println();
    }

    private static void addGold() {
        GridGameGUI gameGUI = new GridGameGUI();
        gameGUI.playGame();

        int numMoves = gameGUI.getMoveCount();
        int goldToAdd = numMoves * 10 * (backend.getTownhallLevel(base) + 1);
        backend.addGold(base, goldToAdd);
        System.out.println("Gold added successfully: " + goldToAdd + " coins!");
    }

    private static void viewGold() {
        int gold = backend.getGold(base);
        System.out.println("Current gold: " + gold);
    }

    private static void buildStructure(Scanner scanner) {
        System.out.println("\n--- Build a Structure ---");
        System.out.println("Available Structures:");
        System.out.println("1. Townhall - Cost: " + (200 + (150 * backend.getTownhallLevel(base))));
        System.out.println("2. Knight Camp - Cost: 200 Gold");
        System.out.println("3. Mage Tower - Cost: 300 Gold");
        System.out.println("4. Defence Tower - Cost: 500 Gold");
        System.out.println("5. Saint Tower - Cost: 400 Gold");
        System.out.print("Enter the Choice of the structure to build: ");
        int structureChoice = scanner.nextInt();
        String structureName;
        switch (structureChoice) {
            case 1 -> structureName = "Townhall";
            case 2 -> structureName = "Knight Camp";
            case 3 -> structureName = "Mage Tower";
            case 4 -> structureName = "Defence Tower";
            case 5 -> structureName = "Saint Tower";
            default -> {
                System.out.println("Invalid choice");
                return;
            }
        }

        int cost = backend.getStructureCost(base, structureName);
        if (cost == -1) {
            System.out.println("Invalid structure name. Please try again.");
            return;
        }

        int result = backend.build(base, structureName, cost);
        switch (result) {
            case 1 -> System.out.println(structureName + " built successfully!");
            case -2 -> System.out.println("Failed to build " + structureName + ". Structure limit reached.");
            case -1 -> System.out.println("Failed to build " + structureName + ". Insufficient gold.");
            default -> System.out.println("Failed to build " + structureName + ". Unknown error.");
        }
    }

    private static void displayStatus() {
        System.out.println("\n--- Base Status ---");
        System.out.println("Gold Coins: " + backend.getGold(base));
        System.out.println("Townhall Level: " + backend.getTownhallLevel(base));
        System.out.println("Structures: ");
        System.out.println("- Knight Camps: " + backend.getStructureCount(base, "Knight Camp"));
        System.out.println("- Mage Towers: " + backend.getStructureCount(base, "Mage Tower"));
        System.out.println("- Defence Towers: " + backend.getStructureCount(base, "Defence Tower"));
        System.out.println("- Saint Towers: " + backend.getStructureCount(base, "Saint Tower"));
        System.out.println("-------------------\n");
    }

    private static boolean canFightDragon() {
        if (backend.getTownhallLevel(base) < 5) {
            return false;
        }
        String[] structures = {"Knight Camp", "Mage Tower", "Defence Tower", "Saint Tower"};
        for (String structure : structures) {
            if (backend.getStructureCount(base, structure) < backend.getMaxLimit(base, structure)) {
                return false;
            }
        }
        return true;
    }

    private static void fightDragon() {
        DragonGameFrontend dragonGame = new DragonGameFrontend();
        dragonGame.startDragonBattle(backend, base);
    }
}

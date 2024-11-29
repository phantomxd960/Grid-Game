import java.io.*;
import java.util.Scanner;

public class Frontend {
    static {
        System.loadLibrary("gamebackend");
    }

    public native void startLevel1();

    public native void startLevel2();

    public native void startLevel3();

    public native void movePlayer(String direction, String level);

    public native void performAction(String action);

    public native void performDragonAction(String action, String dragonName);

    public native void performStoreAction(String action);

    public native void performCaveAction(String monster);

    public native void exitGame();

    public static void main(String[] args) {
        Frontend frontend = new Frontend();
        try {

            ProcessBuilder processBuilder = new ProcessBuilder(
                    "/Users/kaushalrajpuwar/dev/projects/GridHero/controller/game");
            Process backendProcess = processBuilder.start();

            BufferedReader backendOutput = new BufferedReader(new InputStreamReader(backendProcess.getInputStream()));

            Scanner scanner = new Scanner(System.in);
            boolean fmain = true;
            System.out.println("Welcome to the Game!");
            while (fmain) {
                System.out.println("\nChoose an option:");
                System.out.println("1️⃣ Play Level 1");
                System.out.println("2️⃣ Play Level 2");
                System.out.println("3️⃣ Play Level 3");
                System.out.println("4️⃣ Exit");
                System.out.print("> ");

                String input = scanner.nextLine();
                String response;

                switch (input) {
                    case "1":
                        frontend.startLevel1();
                        while ((response = backendOutput.readLine()) != null) {
                            System.out.println("[Backend]: " + response);
                            if (response.contains(
                                    "Level 1: Avoid the enemy on the grid and collect coins as much as possible to build the town hall!")) {
                                break;
                            }
                        }
                        level1Actions(scanner, backendOutput, frontend);
                        break;

                    case "2":
                        frontend.startLevel2();
                        while ((response = backendOutput.readLine()) != null) {
                            System.out.println("[Backend]: " + response);
                            if (response.contains("Level 2: Avoid the enemy with restricted areas!")) {
                                break;
                            }
                        }
                        level2Actions(scanner, backendOutput, frontend);
                        break;

                    case "3":
                        frontend.startLevel3();
                        while ((response = backendOutput.readLine()) != null) {
                            System.out.println("[Backend]: " + response);
                            if (response.contains("Level 3: Welcome to the Dragon Slayer game!")) {
                                break;
                            }
                        }
                        level3Actions(scanner, backendOutput, frontend);
                        break;

                    case "4":
                        System.out.println("Exiting game");
                        System.out.println("Goodbye!");
                        return;

                    default:
                        System.out.println("Invalid input. Please try again.");
                        continue;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void level1Actions(Scanner scanner, BufferedReader backendOutput,
            Frontend frontend)
            throws IOException {

        boolean flag = true;
        while (flag) {
            System.out.println("\nLevel 1 - Choose your move:");
            System.out.println("W: Move Up");
            System.out.println("A: Move Left");
            System.out.println("S: Move Down");
            System.out.println("D: Move Right");
            System.out.println("E: Exit");

            System.out.print("> ");

            String move = scanner.nextLine();
            System.out.println(move);
            if (move.equals("E")) {
                frontend.exitGame();
                System.out.println("Goodbye!");
                flag = false;
                return;
            }
            frontend.movePlayer(move, "1");

            String response;
            while ((response = backendOutput.readLine()) != null) {
                System.out.println("[Backend]: " + response);
                if (response.contains("Level 1 complete!")) {
                    level2Actions(scanner, backendOutput, frontend);
                    flag = false;
                    break;
                }
                if (response.contains("Movement done.")) {
                    break;
                }
                if (response.contains("You exited the game")) {
                    flag = false;
                    break;
                }

            }
        }
    }

    private static void level2Actions(Scanner scanner, BufferedReader backendOutput,
            Frontend frontend)
            throws IOException {

        boolean flag2 = true;
        System.out.println("\nLevel 2 - Choose your move:");
        System.out.println("W: Move Up");
        System.out.println("A: Move Left");
        System.out.println("S: Move Down");
        System.out.println("D: Move Right");
        System.out.println("E: Exit");
        System.out.print("> ");
        while (flag2) {

            String move = scanner.nextLine();
            if (move.equals("E")) {
                frontend.exitGame();
                System.out.println("Goodbye!");
                return;
            }
            frontend.movePlayer(move, "2");

            String response;
            while ((response = backendOutput.readLine()) != null) {
                System.out.println("[Backend]: " + response);
                if (response.contains("Level 2 complete!")) {
                    // flag2 = false;
                    return;
                }
                if (response.contains("Movement done.")) {
                    break;
                }
                if (response.contains("You exited the game")) {
                    flag2 = false;
                    break;
                }
            }
        }

    }

    private static void level3Actions(Scanner scanner, BufferedReader backendOutput,
            Frontend frontend)
            throws IOException {

        boolean lrun = true;
        while (lrun) {
            System.out.println("\nLevel 3 - What would you like to do?");
            System.out.println("1️⃣ Fight Dragon");
            System.out.println("2️⃣ Enter Store");
            System.out.println("3️⃣ Go to Cave");
            System.out.println("4️⃣ Exit");
            System.out.print("> ");

            String action = scanner.nextLine();
            if (action.equals("4")) {
                frontend.exitGame();
                break;
            }
            frontend.performAction(action);

            if (action.equals("1")) {
                fightDragon(scanner, backendOutput, "Dragon", frontend);
                break;
            } else if (action.equals("2")) {
                enterStore(scanner, backendOutput, frontend);
                break;
            } else if (action.equals("3")) {
                enterCave(scanner, backendOutput, frontend);
                break;
            }

            String response;
            while ((response = backendOutput.readLine()) != null) {
                if (response.contains("Invalid choice. Try again.")) {
                    break;
                }
                System.out.println("[Backend]: " + response);
            }
        }
    }

    private static void fightDragon(Scanner scanner, BufferedReader backendOutput,
            String dragonName, Frontend frontend) throws IOException {
        boolean frun = true;
        while (frun) {
            System.out.println("\nYou are fighting the " + dragonName + "\nChoose an action:");
            System.out.println("1️⃣ Attack");
            System.out.println("2️⃣ Dodge");
            System.out.println("3️⃣ Go to Store");
            System.out.println("4️⃣ Exit Fight");
            System.out.print("> ");

            String action = scanner.nextLine();
            if (action.equals("4")) {
                frontend.exitGame();
                level3Actions(scanner, backendOutput, frontend);
                break;
            }
            frontend.performDragonAction(action, dragonName);
            if (action.equals("3")) {
                enterStore(scanner, backendOutput, frontend);
                break;
            }
            String response;
            while ((response = backendOutput.readLine()) != null) {

                System.out.println("[Backend]: " + response);
                if (response.contains("Invalid choice. Try again.")) {
                    break;
                }
                if (response.contains("you attacked the dragon")) {
                    break;
                }
                if (response.contains("You dodged the dragon")) {
                    break;
                }
                if (response.contains("You defeated the dragon") || response.contains("Game over")) {
                    frun = false;
                    break;
                }

            }
        }
    }

    private static void enterStore(Scanner scanner, BufferedReader backendOutput,
            Frontend frontend)
            throws IOException {
        boolean run = true;
        while (run) {
            System.out.println("\nWelcome to the store. Choose an action:");
            System.out.println("1️⃣ Buy Health");
            System.out.println("2️⃣ Buy Weapon");
            System.out.println("3️⃣ Exit Store");
            System.out.print("> ");

            String action = scanner.nextLine();
            if (action.equals("3")) {
                frontend.exitGame();
                level3Actions(scanner, backendOutput, frontend);
                break;
            }
            frontend.performStoreAction(action);

            String response;
            while ((response = backendOutput.readLine()) != null) {
                System.out.println("[Backend]: " + response);
                if (response.contains("Invalid store action!")) {
                    break;
                }
                if (response.contains("Exiting store")) {
                    break;
                }
                if (response.contains("Go back to fight")) {

                    run = false;
                    break;
                }

            }
        }
    }

    private static void enterCave(Scanner scanner, BufferedReader backendOutput,
            Frontend frontend)
            throws IOException {
        boolean crun = true;
        while (crun) {

            System.out.println("1️⃣ Slime");
            System.out.println("2️⃣ Fanged Beast");
            System.out.println("3️⃣ Exit Cave");
            System.out.print("> ");

            String choice = scanner.nextLine();
            if (choice.equals("3")) {
                System.out.println("Goodbye!");
                level3Actions(scanner, backendOutput, frontend);
                break;
            }
            String dragonName = "";
            if (choice.equals("1")) {
                dragonName = "Slime";
                fightDragon(scanner, backendOutput, "Slime", frontend);
                break;
            }
            if (choice.equals("2")) {
                dragonName = "FangedBeast";
                fightDragon(scanner, backendOutput, "FangedBeast", frontend);
                break;
            }

            frontend.performCaveAction(dragonName);
            String response;
            while ((response = backendOutput.readLine()) != null) {
                System.out.println("[Backend]: " + response);
                if (response.contains("Battle completed with " + dragonName)) {
                    crun = false;
                    break;
                }
                if (response.contains("Invalid monster")) {
                    break;
                }

            }
        }
    }
}

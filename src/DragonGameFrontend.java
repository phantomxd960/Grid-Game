package src;

import java.util.Scanner;

public class DragonGameFrontend {
    public static void main(String[] args) {
        BaseBackendJNI backend = new BaseBackendJNI();
        long base = backend.createBase();

        startDragonBattle(backend, base);
    }
    private static void displayEnd() {
        String content = """
            THANK YOU FOR PLAYING !
            YOU SUCCESSFULLY WON THE GAME!!!!
        """;

        for (char c : content.toCharArray()) {
            System.out.print(c);
            try {
                Thread.sleep(50); // Typing effect
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        System.out.println();
    }
    public static void startDragonBattle(BaseBackendJNI backend, long base) {
        DragonGameBackendJNI dragonBackend = new DragonGameBackendJNI();
        long dragon = dragonBackend.createDragon();

        Scanner scanner = new Scanner(System.in);
        while (dragonBackend.getDragonHealth(dragon) > 0) {
            System.out.println("\n--- Dragon Battle ---");
            System.out.println("Dragon Health: " + dragonBackend.getDragonHealth(dragon));
            System.out.println("Structures Status:");
            System.out.println(dragonBackend.getStructureStatus(base)); // Show structure health and counts

            System.out.println("Choose your action:");
            System.out.println(dragonBackend.getAvailableStructures(base)); // Show available structures
            System.out.print("Enter the number corresponding to the structure: ");
            int choice = scanner.nextInt();

            if (!backend.hasTownhall(base)) {
                System.out.println("You don't have any structures to fight with. Build a Townhall and try again.");
                break;
            }

            int damage = dragonBackend.attackDragonWithDamage(base, dragon, choice);
            if (damage < 0) { // Attack failed
                System.out.println("Invalid action or the structure does not exist.");
                continue;
            }

            if (choice == 4) { // Saint Tower special action
                boolean revived = dragonBackend.reviveUnits(base);
                if (revived) {
                    System.out.println("Saint Tower revived health for one Mage Tower and Knight Camp!");
                } else {
                    System.out.println("Troops already at maximum health. No regeneration performed.");
                }
            } else {
                System.out.println("You attacked the Dragon and dealt " + damage + " damage!");
            }

            dragonBackend.dragonAttack(base, dragon);
            System.out.println("Dragon attacks!");

            if (dragonBackend.allStructuresDestroyed(base)) {
                System.out.println("All your structures have been destroyed. You lost!");
                backend.resetGame(base);
                dragonBackend.destroyDragon(dragon);
                return;
            }

            if (dragonBackend.getDragonHealth(dragon) <= 0) {
                System.out.println("Congratulations! You defeated the Dragon!");
                dragonBackend.destroyDragon(dragon);
                displayEnd();
                System.exit(0); 
                return;
            }

            if (dragonBackend.getPlayerFailures(dragon) > 3) {
                System.out.println("You have failed too many times. Game is reset.");
                backend.resetGame(base);
                dragonBackend.destroyDragon(dragon);
                return;
            }
        }
    }
}

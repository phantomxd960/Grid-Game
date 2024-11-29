package src;

public class DragonGameBackendJNI {
    static {
        System.loadLibrary("DragonGameBackend");
    }

    public native long createDragon();
    public native int getDragonHealth(long dragon);
    public native boolean attackDragon(long base, long dragon, int choice);
    public native void dragonAttack(long base, long dragon);
    public native boolean allStructuresDestroyed(long base);
    public native int getPlayerFailures(long dragon);
    public native void destroyDragon(long dragon);
    public native void resetGame(long base);
    public native String getStructureStatus(long base); // Get structure health statuses
    public native String getAvailableStructures(long base); // Get structures that can attack
    public native boolean reviveUnits(long base); // Revive troops with Saint Tower
    public native int attackDragonWithDamage(long base, long dragon, int choice); // Returns the damage dealt

}


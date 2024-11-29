package src;

public class BaseBackendJNI {
    static {
        System.loadLibrary("BaseBackend"); // Load the C++ shared library
    }

    // Native method declarations
    public native long createBase();
    public native void destroyBase(long basePointer);
    public native void addGold(long basePointer, int amount);
    public native int build(long basePointer, String structureName, int cost);
    public native int getGold(long basePointer);
    public native boolean hasTownhall(long basePointer);
    public native int getTownhallLevel(long basePointer);
    public native int getStructureCount(long basePointer, String structureName);
    public native int getStructureCost(long basePointer, String structureName);
    public native void resetGame(long basePointer);

    // New method to get maximum structure limit based on Townhall level
    public native int getMaxLimit(long basePointer, String structureName);

    public static void main(String[] args) {
        BaseBackendJNI backend = new BaseBackendJNI();

        // Create a backend instance
        long base = backend.createBase();

        // Example usage
        backend.addGold(base, 1000);
        System.out.println("Gold: " + backend.getGold(base));

        // Build a Townhall
        System.out.println("Building Townhall: " + backend.build(base, "Townhall", 200));
        System.out.println("Townhall Level: " + backend.getTownhallLevel(base));

        // Build a Knight Camp
        System.out.println("Building Knight Camp: " + backend.build(base, "Knight Camp", 200));
        System.out.println("Knight Camp Count: " + backend.getStructureCount(base, "Knight Camp"));

        backend.destroyBase(base);
    }
}

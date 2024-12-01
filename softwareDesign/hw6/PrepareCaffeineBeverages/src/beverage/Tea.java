package beverage;

class Tea extends CaffeineBeverage {
    private boolean wantsCondiments;

    public Tea(boolean wantsCondiments) {
        this.wantsCondiments = wantsCondiments;
    }

    @Override
    void brew() {
        System.out.println("Steeping tea bag...");
    }

    @Override
    void addCondiments() {
        System.out.println("Adding lemon...");
    }

    @Override
    String getBeverageName() {
        return "Tea";
    }

    @Override
    boolean customerWantsCondiments() {
        return wantsCondiments;
    }
}
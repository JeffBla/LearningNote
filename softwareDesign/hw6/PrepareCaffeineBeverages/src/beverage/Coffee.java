package beverage;

class Coffee extends CaffeineBeverage {
    private boolean wantsCondiments;

    public Coffee(boolean wantsCondiments) {
        this.wantsCondiments = wantsCondiments;
    }
    
    @Override
    void brew() {
        System.out.println("Brewing coffee grounds...");
    }

    @Override
    void addCondiments() {
        System.out.println("Adding sugar and milk...");
    }

    @Override
    String getBeverageName() {
        return "Coffee";
    }

    @Override
    boolean customerWantsCondiments() {
        return wantsCondiments;
    }
}
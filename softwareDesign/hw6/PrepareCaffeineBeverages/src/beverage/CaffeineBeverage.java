package beverage;

abstract class CaffeineBeverage {

 public final void prepareBeverage() {
     boilWater();
     brew();
     pourInCup();
     if (customerWantsCondiments()) {
         addCondiments();
     }
     System.out.println("Your " + getBeverageName() + " is ready!\n");
 }

 abstract void brew();
 abstract void addCondiments();
 abstract String getBeverageName();

 private void boilWater() {
     System.out.println("Boiling water...");
 }

 private void pourInCup() {
     System.out.println("Pouring into cup...");
 }

 boolean customerWantsCondiments() {
     return true;
 }
}
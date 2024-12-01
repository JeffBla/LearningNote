package machine;

class HasQuarterState implements State {
    @Override
    public State insertQuarter() {
        System.out.println("You can't insert another quarter");
        return this;
    }

    @Override
    public State ejectQuarter() {
        System.out.println("Quarter returned");
        return new NoQuarterState();
    }

    @Override
    public State turnCrank() {
        System.out.println("You turned the crank...");
        return new SoldState();
    }

    @Override
    public State dispense(GumballMachine machine) {
        System.out.println("No gumball dispensed");
        return this;
    }

    @Override
    public String getStatusMessage() {
        return "waiting for crank to be turned";
    }
}
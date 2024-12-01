package machine;

class NoQuarterState implements State {
    @Override
    public State insertQuarter() {
        System.out.println("You inserted a quarter");
        return new HasQuarterState();
    }

    @Override
    public State ejectQuarter() {
        System.out.println("You haven't inserted a quarter");
        return this;
    }

    @Override
    public State turnCrank() {
        System.out.println("You turned the crank, but there's no quarter");
        return this;
    }

    @Override
    public State dispense(GumballMachine machine) {
        System.out.println("You need to pay first");
        return this;
    }

    @Override
    public String getStatusMessage() {
        return "waiting for quarter";
    }
}
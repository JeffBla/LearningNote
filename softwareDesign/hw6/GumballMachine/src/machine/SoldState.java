package machine;

class SoldState implements State {
    @Override
    public State insertQuarter() {
        System.out.println("Please wait, we're already giving you a gumball");
        return this;
    }

    @Override
    public State ejectQuarter() {
        System.out.println("Sorry, you already turned the crank");
        return this;
    }

    @Override
    public State turnCrank() {
        System.out.println("Turning twice doesn't get you another gumball!");
        return this;
    }

    @Override
    public State dispense(GumballMachine machine) {
        machine.releaseBall();
        if (machine.getCount() > 0) {
            return new NoQuarterState();
        } else {
            System.out.println("Oops, out of gumballs!");
            return new SoldOutState();
        }
    }

    @Override
    public String getStatusMessage() {
        return "delivering a gumball";
    }
}
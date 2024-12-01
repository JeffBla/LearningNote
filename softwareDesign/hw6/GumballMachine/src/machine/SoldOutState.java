package machine;

class SoldOutState implements State {
    @Override
    public State insertQuarter() {
        System.out.println("You can't insert a quarter, the machine is sold out");
        return this;
    }

    @Override
    public State ejectQuarter() {
        System.out.println("You can't eject, you haven't inserted a quarter yet");
        return this;
    }

    @Override
    public State turnCrank() {
        System.out.println("You turned, but there are no gumballs");
        return this;
    }

    @Override
    public State dispense(GumballMachine machine) {
        System.out.println("No gumball dispensed");
        return this;
    }

    @Override
    public String getStatusMessage() {
        return "sold out";
    }
}
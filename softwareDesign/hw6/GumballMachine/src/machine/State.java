package machine;

interface State {
    State insertQuarter();
    State ejectQuarter();
    State turnCrank();
    State dispense(GumballMachine machine);
    String getStatusMessage();
}
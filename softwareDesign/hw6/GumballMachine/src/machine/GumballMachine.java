package machine;

class GumballMachine {
    private State currentState;
    private int count = 0;

    public GumballMachine(int numberGumballs) {
        this.count = numberGumballs;
        if (numberGumballs > 0) {
            currentState = new NoQuarterState();
        } else {
            currentState = new SoldOutState();
        }
    }

    public void insertQuarter() {
        currentState = currentState.insertQuarter();
    }

    public void ejectQuarter() {
        currentState = currentState.ejectQuarter();
    }

    public void turnCrank() {
        currentState = currentState.turnCrank();
    }
    
	public void dispense() {
        currentState = currentState.dispense(this);
	}

    public void releaseBall() {
        System.out.println("A gumball comes rolling out the slot...");
        if (count > 0) {
            count = count - 1;
        }
    }

    public int getCount() {
        return count;
    }

    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();
        result.append("\nMighty Gumball, Inc.");
        result.append("\nJava-enabled Standing Gumball Model #2024");
        result.append("\nInventory: ").append(count).append(" gumball");
        if (count != 1) {
            result.append("s");
        }
        result.append("\nMachine is ").append(currentState.getStatusMessage());
        result.append("\n");
        return result.toString();
    }
}
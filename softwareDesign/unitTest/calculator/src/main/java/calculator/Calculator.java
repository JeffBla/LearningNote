package calculator;

public class Calculator {
	private int sum=0;
	
	public void cumulate(int number) {
		sum+=number;
	}
	
	public int getSum() {
		return sum;
	}
	
	public int addTwoNumber(int i, int j) {
		return i + j;
	}
}
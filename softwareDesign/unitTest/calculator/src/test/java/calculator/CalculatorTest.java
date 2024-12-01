package calculator;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

class CalculatorTest {

	@Test
	void cumulateMultiplePositiveNumbers_IncreasesSum() {
		Calculator calculator = new Calculator();
		calculator.cumulate(5); 
		calculator.cumulate(10);
		assertEquals(15, calculator.getSum());
	}
	
	@Test
	void addTwoPositiveNumbers_ReturnsSum() {
		Calculator calculator = new Calculator();
		int result = calculator.addTwoNumber(3, 5);
		assertEquals(8, result);
	}
	
	@Test
    void addPositiveAndNegativeNumber_ReturnsSum() {
        Calculator calculator = new Calculator();
        int result = calculator.addTwoNumber(3, -2);
        assertEquals(1, result);
    }

    @Test
    void addTwoNegativeNumbers_ReturnsSum() {
        Calculator calculator = new Calculator();
        int result = calculator.addTwoNumber(-3, -5);
        assertEquals(-8, result);
    }

    @Test
    void addZeroAndNumber_ReturnsNumber() {
        Calculator calculator = new Calculator();
        int result = calculator.addTwoNumber(0, 5);
        assertEquals(5, result);
    }

    @Test
    void addNumberAndZero_ReturnsNumber() {
        Calculator calculator = new Calculator();
        int result = calculator.addTwoNumber(5, 0);
        assertEquals(5, result);
    }
}
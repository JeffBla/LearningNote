public class Book {
    private String title;
    private String author;
    private String isbn;
    private int publicationYear;
    private int availableQuantity;

    public void Borrow() throws Exception {
    	 if (availableQuantity > 0) {
    	        availableQuantity--;
    	    } else {
    	        throw new Exception("書籍已無可用數量。");
    	    }
    }
    public void Return() {
    	availableQuantity++;
    }
}

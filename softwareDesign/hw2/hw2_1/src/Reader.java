import java.util.Date;
import java.util.ArrayList;

public class Reader {
    private String name;
    private String readerID;
    private String phoneNumber;
    private Date registrationDate;
    private ArrayList<Book> borrowedBooks;
    private ArrayList<BorrowRecord> borrowHistory;

    public void BorrowBook(Book book) throws Exception {
    	if (borrowedBooks.size() >= 3) {
    	    throw new Exception("借閱上限為三本書。");
    	}
    }
    public void ReturnBook(Book book) {
        //TODO Implement returning logic
    }
    public ArrayList<BorrowRecord> GetBorrowHistory() {
        //TODO Implement viewing borrowed books logic
        return null;
    }
}

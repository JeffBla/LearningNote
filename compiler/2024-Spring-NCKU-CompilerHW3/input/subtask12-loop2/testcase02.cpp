int main(string argv[]) {
    int a[10] = { 10, 21, 30, 41, 50 };
    
    for( int i = 0; i < 9; i++ ) {
        int j = a[i];
        cout << a[i] << endl;

        if( j % 2 == 0 ) {
            cout << "Even" << endl;
        }
    }
}
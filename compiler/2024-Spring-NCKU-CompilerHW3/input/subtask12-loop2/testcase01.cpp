int main(string argv[]) {
    int counter = 0;

    while( counter < 100 ) {
        counter += 1;
        if( counter == 99 ) {
            break;
        }

        if( counter % 9 == 0 ) {
            cout << "Hello World " << counter % 2 << endl;
        }

        cout << 998244353 % counter << endl;
    }

    cout << counter << endl;
}
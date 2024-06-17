// Calculate Pi using the Nilakantha series
float calculate_pi(int terms) {
    float pi = 3.0;   // Start with the first term of the series
    float two = 2.0;  // Cached value
    bool add = true;  // Control addition or subtraction

    for (int k = 1; k < terms; k++) {
        float term = (float)4 / (two * (float)k * (two * (float)k + 1.0) * (two * (float)k + two));
        if (add) {
            pi += term;
            add = false;
        } else {
            pi -= term;
            add = true;
        }
    }

    return pi;
}

int main(string argv[]) {
    int terms = 100;  // Number of terms to sum
    float pi = calculate_pi(terms);
    cout << "Approximation of Pi after " << terms << " terms: " << pi << endl;
    return 0;
}
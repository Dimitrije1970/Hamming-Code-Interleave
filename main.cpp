#include <iostream>
#include <ctime>

using namespace std;

void fill_matrix(bool** matrix, int n, int k, bool G[]) {
	int i, j;
	for (i = 0; i < k; i++) {
		for (j = 0; j < n; j++) {
			matrix[i][j] = G[i*n + j];
		}
	}
}

void print_matrix(bool** matrix, int n, int k) {
	int i, j;
	for (i = 0; i < k;i++) {
		for (j = 0; j < n;j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

void print_numbers(bool** matrix, bool** other_matrix, int n, int k) {
	int i, j;
	for (i = 0; i < k; i++) {
		if (i % 4 == 0) cout << endl;

		for (j = 0; j < n; j++) {
			cout << matrix[i][j];
		}

		if (other_matrix) {
			cout << " [";
			for (j = 0; j < n; j++) {
				cout << other_matrix[i][j];
			}
			cout << "]";
		}

		cout << "    ";
	}
}

bool* int_to_bin(int num, int k) {
	int i;
	bool * bin = new bool[k];

	for (i = 0; i < k; i++) {
		bin[k - i - 1] = (num) & 1;
		num >>= 1;
	}

	return bin;
}

bool* matrix_mul(bool* data, bool** matrix, int n, int k) {
	bool* code = new bool[n];
	int i, j, index = 0;

	for (i = 0; i < n;i++) {
		int res = 0;
		for (j = 0; j < k;j++) {
			res ^= (data[j] & matrix[j][i]);
		}

		code[index++] = res;
	}

	return code;
}

int num_of_ones(bool* code, int n) {
	int res = 0, i;
	for (i = 0; i < n; i++) if (code[i] & 1) res++;

	return res;
}

bool** generate_coded_data(bool** hamming_matrix, int N, bool** numbers) {
	int min = 0, max = pow(2, 6) - 1;
	bool** data = new bool*[N];

	int i, index = 0;
	for (i = 0; i < N; i++) {
		int random_int = min + (rand() % (max - min));
		bool* nb = int_to_bin(random_int, 6);

		numbers[index++] = nb;

		bool* code = matrix_mul(nb, hamming_matrix, 10, 6);
		data[i] = code;
	}

	return data;
}

bool* decode(bool* code, int n, int k) {
	bool* info = new bool[k];

	int sindrome = (code[0] ^ code[2] ^ code[4] ^ code[6] ^ code[8]) + 2 * (code[1] ^ code[2] ^ code[5] ^ code[6] ^ code[9]) +
		4 * (code[3] ^ code[4] ^ code[5] ^ code[6]) + 8 * (code[7] ^ code[8] ^ code[9]);
	
	if(sindrome != 0 && sindrome != 7) code[sindrome-1] ^= 1;

	info[0] = code[2];
	info[1] = code[4];
	info[2] = code[5];
	info[3] = code[6];
	info[4] = code[8];
	info[5] = code[9];

	return info;
}

int main() {
	cout << "1. ZADATAK" << endl << endl;

	int n = 6,k = 3;
	bool** matrix = new bool*[k];
	for (int i = 0; i < k; i++) matrix[i] = new bool[n];
	bool G[18] = { 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1 };
	int weights[8];


	fill_matrix(matrix, n, k, G);
	cout << "Generisuca matrica (G):" << endl;
	print_matrix(matrix, n, k);

	cout << endl << "Sve kodne reci:";

	for (int i = 0; i < 8;i++) {
		if (i % 4 == 0) cout << endl;
		cout << i+1 << ". ";

		bool* bin = int_to_bin(i, k);
		bool* code = matrix_mul(bin, matrix, n, k);


		weights[i] = num_of_ones(code, n);

		for (int j = 0; j < n; j++) cout << code[j];
		cout << " (d = " << weights[i] << ")" << "   ";

		delete[] bin;
		delete[] code;
	}
	cout << endl << endl;

	int d = 0;

	int* sorted = new int[n];
	for (int i = 0; i < 8; i++) sorted[i] = 0;
	for (int i = 0; i < 8; i++) sorted[weights[i]]++;
	for (int i = 0; i <= n; i++) {
		cout << "d = " << i << " [" << sorted[i] << "]   " << endl;
		if (!d && i && sorted[i]) d = i;
	}

	int g = -1;
	int c = -1;

	while (d >= 2 * (++c + 1) + 1);
	while (d >= c + (++g + 1) + 1);

	cout << endl << "Ovaj kod moze da detektuje " << g << " gresku, a ispravi " << c << " gresaka." << endl << endl;

	for (int i = 0; i < k; i++) delete[] matrix[i];
	delete[] matrix;

	cout << "2. ZADATAK" << endl << endl;

	n = 10, k = 6;
	bool** hamming_matrix = new bool* [k];
	for (int i = 0; i < k; i++) hamming_matrix[i] = new bool[n];
	bool hamming_G[60] = { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
						   1, 0, 0, 1, 1, 0, 0, 0, 0, 0,
						   0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
						   1, 1, 0, 1, 0, 0, 1, 0, 0, 0,
						   1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
						   0, 1, 0, 0, 0, 0, 0, 1, 0, 1};

	fill_matrix(hamming_matrix, n, k, hamming_G);
	cout << "Generisuca matrica za Hemingov kod (10, 6): " << endl;
	print_matrix(hamming_matrix, n, k);

	//PREDAJNIK
	srand(time(nullptr));
	int N = 1000;
	bool** numbers = new bool* [N];
	bool** data = generate_coded_data(hamming_matrix, N, numbers);

	cout << endl << "IZVOR (N = 1000):" << endl;
	print_numbers(numbers, nullptr, k, N);

	cout << endl << endl << "KODIRANE INFORMACIJE:" << endl;
	print_numbers(data, nullptr, n, N);

	//INTERLIVER
	int interleaver_param, interleaver_counter = 0;
	cout << endl << endl << "Unesi parametar interlivera: "; cin >> interleaver_param;
	bool** interleaver = new bool*[n];
	for (int i = 0; i < n; i++) interleaver[i] = new bool[interleaver_param];

	int* channel = new int[N * n], channel_counter = 0;

	while (interleaver_counter < N) {
		for (int i = 0; i < n;i++) { //Punjenje interlivera
			for (int j = 0; j < interleaver_param; j++) {
				interleaver[i][j] = data[interleaver_counter+j][i];
			}
		}

		for (int i = 0; i < n;i++) {
			for (int j = 0; j < interleaver_param;j++) {
				channel[channel_counter++] = interleaver[i][j];
			}
		}

		interleaver_counter += interleaver_param;
	}

	//KANAL SA GRESKOM
	int error_pos = 0, error_len = 4, error_period = 50;

	cout << "Unesi od kog bita pocinje paket gresaka: "; cin >> error_pos;

	for (int i = error_pos; i < channel_counter;i+=error_period) {
		if (i + error_len < channel_counter) {
			channel[i - 1] ^= 1;
			channel[i] ^= 1;
			channel[i + 1] ^= 1;
			channel[i + 2] ^= 1;
		}
	}

	//DEINTERLIVER
	channel_counter = 0; interleaver_counter = 0;
	bool** deinterleaver = new bool* [interleaver_param];
	for (int i = 0; i < interleaver_param; i++) deinterleaver[i] = new bool[n];

	bool** result = new bool* [N];
	for (int i = 0; i < N; i++) result[i] = new bool[k];

	while (interleaver_counter < N) {
		for (int i = 0; i < n; i++) { //Punjenje deinterlivera
			for (int j = 0; j < interleaver_param; j++) {
				deinterleaver[j][i] = channel[channel_counter++];
			}
		}

		for (int i = 0; i < interleaver_param;i++) {
			for (int j = 0; j < k;j++) {
				result[interleaver_counter + i][j] = decode(deinterleaver[i], n, k)[j];
			}

		}

		interleaver_counter += interleaver_param;
	}

	cout << endl << endl << "DEKODOVANA [ORIGINALNA]" << endl << endl;
	print_numbers(result, numbers, k, N);

	double num_of_errors = 0;

	for (int i = 0; i < N;i++) {
		for (int j = 0; j < k; j++) {
			if (result[i][j] != numbers[i][j]) num_of_errors++;
		}
	}

	cout << endl << endl << "Procenat lose prenetih bitova je: " << (num_of_errors / (N * k)) * 100 << "%" << endl;
	cout << "Da bi ovo bilo moguce parametar interlivera mora da bude veci od duzine paketa greske i broj N mora biti deljiv ovim brojem (u ovom slucaju >= 4)." << endl;


	for (int i = 0; i < k; i++) delete[] hamming_matrix[i];
	delete[] hamming_matrix;

	for (int i = 0; i < N; i++) delete[] data[i];
	delete[] data;

	for (int i = 0; i < N; i++) delete[] numbers[i];
	delete[] numbers;

	for (int i = 0; i < n; i++) delete[] interleaver[i];
	delete[] interleaver;

	delete[] channel;

	for (int i = 0; i < interleaver_param; i++) delete[] deinterleaver[i];
	delete[] deinterleaver;

	for (int i = 0; i < N; i++) delete[] result[i];
	delete[] result;

	return 0;
}
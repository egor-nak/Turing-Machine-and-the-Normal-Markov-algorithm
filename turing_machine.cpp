/* ВАЖНО!!!!! Инструкция
 * Сами параметры для машины задаются в отдельном файле txt (путь к которому обязательно нужно указывать глобальный, НЕ ЗАБУДЬТЕ ПОМЕНЯТЬ ПУТЬ в строке 213)
 * Формат задания алгоритма:
4 - размер алфавита
a b c - алфавит
4 - количество состояний
1: (#, R, 2) (#, R, 3) (#, R, 3) () - дальше идёт таблица
2: (, R,) (,R,) (,R,) (a, , !)
3: (, R,) (,R,) (,R,) (b, , !)
4: (, R,) (,R,) (,R,) (c, , !)

 Ясейка таблице задаётся в скобках по формату "(знак, передвиженеи, состояние)", обзательно между параметрами должны быть запятые и каждая ячейка должна быть заключена в скобки
 Если ячейка записана как "()", то это означает, что в эту ячейку невозможно попасть

 На месте занка может быть:
 - Любой char
 - Пустота или пробелы, это будет считаться как команда оставить пржний знак
 - # - что означает лямбду

На месте передвижения может быть:
 - R - вправо
 - L - влево
 - N или ничего или пробел - это остаться на том же месте

 На месте состояния может быть:
 - Любое число
 - Пустота или пробелы - это будет осзначать, остаться в том же состоянии
 - ! - это означает завершить программу

 После того, как вы заполнили txt файл с программой, вы можете вводить хводные данные через консоль, просто запустив программу.
 Сначала вы вводите строку, которую вы хотите обработать, а потом mode {1, 2}. Если mode == 1, то выводится только финальное состояние ленты,
 а еси mode == 2, то выводится состояние ленты после каждого шага.

 Переменная ANS_LENGHT отвечает за размер участка ленты, который выводится в консоль.
 */

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>

using namespace std;

#define ll long long

const ll MAX_LENGHT = 1e8;
const ll ANS_LENGHT = 50;

struct cell{
	char sign;
	ll movement, staus;
};

ostream& operator<<(ostream &os, const cell &d) {
	return os << "( " << d.sign << ", " << d.movement << ", " << d.staus << ")";
}

vector<string> splitter(string s, set<char> divisor, bool flag_reg = false) { // flag_reg = false - это режим, когда мы не считаем за новое слово "", это нужно для грамотного разиения строки
	vector<string> ans;

	string tmp_ans;
	for (int i = 0; i < s.size(); ++i) {
		if (s[i] == ' ') {
			continue;
		}
		if (divisor.find(s[i]) == divisor.end()) {
			tmp_ans += s[i];
		} else {
			if (tmp_ans == "" && !flag_reg) {
				continue;
			}
			ans.push_back(tmp_ans);
			tmp_ans = "";
		}
	}
	if (!flag_reg) {
		if (divisor.find(s[s.size() - 1]) == divisor.end()) {
			ans.push_back(tmp_ans);
		}
	} else {
		ans.push_back(tmp_ans);
	}

	return ans;
}

void print_table(vector<vector<cell>> &table) {
	for (auto x : table) {
		for (auto y : x) {
			cout << y << " ";
		}
		cout << "\n";
	}
	return;
}

void get_data_of_emulator(vector<char> &alphabet, vector<vector<cell>> &table, string path) { // Путь нужно указывать глобально
	ifstream myfile(path);
	ll alphabet_size;
	myfile >> alphabet_size;
	for (int i = 0; i < alphabet_size; ++i) {
		char tmp;
		myfile >> tmp;
		alphabet.push_back(tmp);
	}

	ll q_count;
	myfile >> q_count;

	map<string, ll> converte_move_sign_to_value;
	converte_move_sign_to_value["R"] = 1;
	converte_move_sign_to_value["L"] = -1;
	converte_move_sign_to_value["N"] = 0;
	converte_move_sign_to_value[""] = 0;

	vector<cell> tmp_row;
	table.push_back(tmp_row);

	for (int row = 1; row <= q_count; ++row) {
		string s;
		getline(myfile, s);
		if (s == "") {
			row--;
			continue;
		}
		vector<string> split_to_cells = splitter(s, {'(', ')'});

		vector<cell> new_row;
		for (int i = 0; i < split_to_cells.size(); ++i) {
			if (split_to_cells[i][split_to_cells[i].size() - 1] == ':') { // это случай обработки номера состояния
				continue;
			}

			vector<string> cells_values = splitter(split_to_cells[i], {','}, true);

			cell tmp_values;

			tmp_values.movement = converte_move_sign_to_value[cells_values[1]];
			tmp_values.sign = cells_values[0][0];
			if (cells_values[0] == "") {
				tmp_values.sign = ' ';
			}

			if (cells_values[2][0] == '!') {
				tmp_values.staus = -1; // это будет означать такт остановки
			} else if (cells_values[2] == ""){
				tmp_values.staus = 0; // это означает остаться в том же состоянии
			} else {
				tmp_values.staus = stoll(cells_values[2]);
			}

			new_row.push_back(tmp_values);
		}
		table.push_back(new_row);
	}
}

void put_input_to_the_tape(string &s, ll &carriage, vector<char> &tape) {
	for (int i = s.size() - 1; i >= 0; --i) {
		tape[carriage] = s[i];
		carriage--;
	}
	carriage++;
}

void print_part_tape(ll cariage, vector<char> &tape) {
	for (int i = cariage - (ANS_LENGHT / 2); i < cariage + (ANS_LENGHT / 2); i++) {
		if (tape[i - 1] == '#' && tape[i] != '#') {
			cout << "|";
		}
		if (tape[i - 1] != '#' && tape[i] == '#') {
			cout << "|";
		}
		cout << tape[i] << " ";
	}
	cout << "\n";
	return;
}

void start_turing_machine(vector<vector<cell>> &table, vector<char> &alphabet, vector<char> &tape, ll mode, ll &cariage) {
	ll stat = 1;

	ll count_iter = 0;
	while (true) {

		if (mode == 2) {
			cout << count_iter << ": ";
			print_part_tape(cariage, tape);
		}
		ll ind_of_cell = 0;
		for (;ind_of_cell < alphabet.size(); ++ind_of_cell) {
			if (alphabet[ind_of_cell] == tape[cariage]) {
				break;
			}
		}

		if (table[stat][ind_of_cell].sign != ' ') {
			tape[cariage] = table[stat][ind_of_cell].sign;
		}

		cariage += table[stat][ind_of_cell].movement; // сдвиг каретки

		if (table[stat][ind_of_cell].staus == -1) {
			break;
		} else if (table[stat][ind_of_cell].staus != 0) {
			stat = table[stat][ind_of_cell].staus;
		}

		count_iter++;
	}
	return ;
}


int main() {
	vector<char> alphabet;
	vector<vector<cell>> table;
	vector<char> tape(MAX_LENGHT, '#');
	string path_to_turing_input = "Yours path";

	get_data_of_emulator(alphabet, table, path_to_turing_input);

//	print_table(table);

	alphabet.push_back('#');

	// Теперь реализуем ввод слова
	string s;
	cin >> s;

	ll mode;
	cin >> mode;

	ll cariage = MAX_LENGHT / 2;

	put_input_to_the_tape(s, cariage, tape);

	start_turing_machine(table, alphabet, tape, mode, cariage);

	cout << "ANS: ";
	print_part_tape(cariage, tape);
	return 0;
}

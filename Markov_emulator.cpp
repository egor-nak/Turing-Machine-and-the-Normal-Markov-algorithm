#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#define ll long long

struct values_algo {
	string from, to;
	bool with_stop;
};


vector<int> prefix_function (string s) {
	int n = s.size();
	vector<int> pi (n);
	for (int i=1; i<n; ++i) {
		int j = pi[i-1];
		while (j > 0 && s[i] != s[j])
			j = pi[j-1];
		if (s[i] == s[j])  ++j;
		pi[i] = j;
	}
	return pi;
}

vector<int> find_first_occurrences(string s, string t) {
	string tmp_st = s + "@" + t;

	vector<int> pr_func = prefix_function(tmp_st);

	int n = s.size();

	vector<int> ans;

	for (int i = n; i < pr_func.size(); ++i) {
		if (pr_func[i] == n) {
			ans.push_back(i - 2 * n);
			break;
		}
	}

	return ans;
}


void read_algorithm(string path_to_file, vector<values_algo> &posl) {
	ifstream myfile(path_to_file);

	ll count_lines;
	myfile >> count_lines;

	string tmp_2;
	getline(myfile, tmp_2); // костыль, чтобы считать "\n"

	for (int h = 0; h < count_lines; ++h) {
		string tmp;
		getline(myfile, tmp);
		string tmp_with_no_space;

		for (int i = 0; i < tmp.size(); ++i) {
			if (tmp[i] != ' ') {
				tmp_with_no_space += tmp[i]; // уюираем все пробелы из слова
			}
		}

		tmp = tmp_with_no_space;

		values_algo command_tmp;

		int i = 0;
		for (; i < tmp.size(); ++i) { // формируем заменяемое значение
			if (i < tmp.size() - 1 && (tmp[i] == '=' || tmp[i] == '-') && tmp[i + 1] == '>') { // если мы наткнулись на => || ->, то это конец первой части
				break;
			}
			command_tmp.from += tmp[i];
		}

		if (tmp[i] == '=') { // устанавливаем переход
			command_tmp.with_stop = true;
		} else {
			command_tmp.with_stop = false;
		}

		i += 2;

		if (i >= tmp.size()) { // устанавливаем значение, которым заменям
			command_tmp.to = "";
		} else {
			for (; i < tmp.size(); ++i) {
				command_tmp.to += tmp[i];
			}
		}

		posl.push_back(command_tmp);
	}
}

void start_algorithm(string &s, ll mode, vector<values_algo> &algos) {
	bool flag = true;
	while (flag) {
		flag = false;
		if (mode == 2) {
			cout << s << "\n";
		}

		for (int i = 0; i < algos.size(); ++i) {
			if (algos[i].from == "") {
				s = algos[i].to + s;
				if (!algos[i].with_stop) { // если с остановой, то мы просто выполним эту опрецию и выйдем из цикла
					flag = true;
				}
				break; // после выполнения опреции, мы должны откатиться в начало алгорифма
			}
			vector<int> first_ocur = find_first_occurrences(algos[i].from, s);
			if (first_ocur.size() != 0) {
				if (!algos[i].with_stop) { // если с остановой, то мы просто выполним эту опрецию и выйдем из цикла
					flag = true;
				}
				string tmp_s = "";
				for (int ind = 0; ind < s.size(); ++ind) {
					if (ind != first_ocur[0]) {
						tmp_s += s[ind];
					} else {
						tmp_s += algos[i].to;
						ind += algos[i].from.size() - 1; // это мы учитываем что мы вырезаем кусок n-ной длины
					}
				}
				s = tmp_s; // подменяем строки
				break; // после выполнения опреции, мы должны откатиться в начало алгорифма
			}
		}
	}
}



int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	string path_to_file = "path_to_file absolute";

	vector<values_algo> posl_movements;
	read_algorithm(path_to_file, posl_movements);
//	for (auto x : posl_movements) {
//		cout << x.from << " " << x.to << " " << x.with_stop << "\n";
//	}

	string a;
	cin >> a;
			// ввод данных пользователя
	ll mode;
	cin >> mode;


	start_algorithm(a, mode, posl_movements);

	cout << a << "\n";

	return 0;
}
#include<iostream>
#include<string>
#include<sstream>
#include<cmath>
using namespace std;

#include<fstream>
using std::ifstream;
using std::ofstream;
ifstream infile("03.txt", ios::in);
//ofstream outfile("ans03_1.txt", ios::out);

string inst_s[10000];
int i_num = 0;
int i_current;
int i_next;
//store location of lebal
string label_name[500];
int label_loc[500];
int label_num = 0;
string pc[500] = {};
int entry;
int bht[1050][5] = { 0 };
string pre_value[4] = { "SN","WN","WT","ST" };
int reg_value[32] = { 0 };
string actual_value;
string predict_value;
int pc_index;

//store label_name & label_loc
void check_label(string s) {
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == ':') {
			for (int j = 0; j < i; j++) {
				if (s[j] != ' '&& s[j] != '	')label_name[label_num] += s[j];
			}
			inst_s[i_num] = "";
			int k;
			for (k = i; k < s.length(); k++) {
				if (s[k] != ' '&&s[k] != '	'&&s[k] != ':')break;
			}

			for (int j = k; j < s.length() && s[j] != ';'; j++) {
				inst_s[i_num] += s[j];
			}
			label_loc[label_num] = i_num;
			label_num++;
			break;
		}
		if (i == s.length() - 1) {
			inst_s[i_num] = "";
			int k;
			for (k = 0; k < s.length(); k++) {
				if (s[k] != ' '&&s[k] != '	')break;
			}

			for (int j = k; j < s.length() && s[j] != ';'&&s[j] != '/'; j++) {
				inst_s[i_num] += s[j];
			}
			break;
		}
	}
}

//hex to binary
string hextobi(string s) {
	string h = "";
	for (int s_i = 0; s_i < s.length(); s_i++) {
		if (s[s_i] == '0')h += "0000";
		else if (s[s_i] == '1')h += "0001";
		else if (s[s_i] == '2')h += "0010";
		else if (s[s_i] == '3')h += "0011";
		else if (s[s_i] == '4')h += "0100";
		else if (s[s_i] == '5')h += "0101";
		else if (s[s_i] == '6')h += "0110";
		else if (s[s_i] == '7')h += "0111";
		else if (s[s_i] == '8')h += "1000";
		else if (s[s_i] == '9')h += "1001";
		else if (s[s_i] == 'A')h += "1010";
		else if (s[s_i] == 'B')h += "1011";
		else if (s[s_i] == 'C')h += "1100";
		else if (s[s_i] == 'D')h += "1101";
		else if (s[s_i] == 'E')h += "1110";
		else if (s[s_i] == 'F')h += "1111";
	}
	return h;
}

//to binary
string binary(int n) {
	string r = "";
	while (n != 0) {
		r = (n % 2 == 0 ? "0" : "1") + r;
		n /= 2;
	}
	if (r == "")return "0";
	else return r;
}




//fix to correct bits
string fix(string s, int n) {
	if (s.length() < n) {
		while (s.length() != n) {
			s = "0" + s;
		}
	}
	else if (s.length() > n) {
		string f = "";
		for (int f_i = s.length() - n; f_i < s.length(); f_i++) {
			f += s[f_i];
		}
		s = f;
	}
	return s;
}

//save pc
void st_pc(string s) {
	pc[i_num] = "";
	for (int i = 0; i < s.length() - 1; i++) {
		if (s[i] == '0'&&s[i + 1] == 'x') {
			int j;
			for (j = i + 2; s[j] != ' '&&s[j] != '	'; j++) {
				if (s[j] != ' ')pc[i_num] += s[j];
			}
			inst_s[i_num] = "";
			for (int k = j; k < s.length(); k++) {
				if (k < s.length() - 1) {
					if ((s[k] != ' '&&s[k] != '	') || (s[k + 1] != ' '&&s[k + 1] != '	')) {
						if (inst_s[i_num] != "" || (s[k] != ' '&&s[k] != '	')) {
							inst_s[i_num] += s[k];
						}
					}
				}
				else {
					if (s[k] != ':' &&s[k] != ' '&&s[k] != '	')inst_s[i_num] += s[k];
				}
			}
			break;
		}
	}
	if (pc[i_num] == "") {
		pc[i_num] = fix(binary(i_num * 4), 12);
	}
	else pc[i_num] = fix(hextobi(pc[i_num]), 12);


}

//instruction act
string act[10000];
void cut(string s) {
	for (int j = 0; s[j] != ' '; j++) {
		if (s[j] != ' ') {
			act[i_num] += s[j];
		}
	}
}

//get pc index
int get_index(string s) {

	int index = 0;
	int in_bit = 0;
	int e = entry;
	while (e != 1) {
		in_bit++;
		e /= 2;
	}
	//cout << in_bit << endl;
	int p = in_bit - 1;

	for (int in_i = 10 - in_bit; in_i < 10; in_i++) {
		int ch;
		stringstream ss;
		ss << s[in_i];
		ss >> ch;
		index += ch * pow(2, p);
		p--;
	}
	return index;
}

//find label 
int find_label(string s) {
	int loc_num = i_current;
	for (int j = 0; j < label_num; j++) {
		if (label_name[j] == s) {
			loc_num = label_loc[j];
			break;
		}
	}
	return loc_num;
}

//predict
void predict(int a) {
	//cout << pc[a] << endl;
	pc_index = get_index(pc[a]);
	int pre_out = bht[pc_index][bht[pc_index][0] + 1];
	predict_value = pre_value[pre_out];
	if (predict_value == "SN" || predict_value == "WN")predict_value = "N";
	else predict_value = "T";

	//===========Output Predict value=====================
	cout << "---------------Predict---------------" << endl;
	cout << "entry¡G" << pc_index << endl;
	cout << "Predictor State¡G";
	cout << fix(binary(bht[pc_index][0]), 2) << ",";
	cout << pre_value[bht[pc_index][1]] << ",";
	cout << pre_value[bht[pc_index][2]] << ",";
	cout << pre_value[bht[pc_index][3]] << ",";
	cout << pre_value[bht[pc_index][4]] << endl;
	cout << "Prediction¡G" << predict_value << endl;
}

void actual(int a) {
	string br_label = "";
	if (act[a] == "b" || act[a] == "B") {
		for (int m = 1; m < inst_s[a].length(); m++) {
			if (inst_s[a][m] != ' '&&inst_s[a][m] != '	'&&inst_s[a][m] != ',')br_label += inst_s[a][m];
		}
		actual_value = "T";
	}

	else if (act[a].length() == 3) {
		string rs1, rs2;
		int rs1_int, rs2_int;
		int rs2_num = 0;
		int k;
		for (int j = 0; j < inst_s[a].length(); j++) {
			if (inst_s[a][j] == 'r' || inst_s[a][j] == 'R') {
				for (k = j + 1; inst_s[a][k] != ','&&k < inst_s[a].length(); k++) {
					if (rs2_num == 1)rs2 += inst_s[a][k];
					else rs1 += inst_s[a][k];
				}
				j = k;
				rs2_num++;
			}
			if (rs2_num == 2) {
				for (int m = j; m < inst_s[a].length(); m++) {
					if (inst_s[a][m] != ' '&&inst_s[a][m] != '	'&&inst_s[a][m] != ',')br_label += inst_s[a][m];
				}
				break;
			}

		}
		stringstream c;
		c << rs1;
		c >> rs1_int;
		stringstream cc;
		cc << rs2;
		cc >> rs2_int;
		if (act[a] == "beq" || act[a] == "BEQ") {
			if (reg_value[rs1_int] == reg_value[rs2_int]) {
				actual_value = "T";
			}
			else actual_value = "N";
		}
		else if (act[a] == "bge" || act[a] == "BGE") {
			if (reg_value[rs1_int] >= reg_value[rs2_int]) {
				actual_value = "T";
			}
			else actual_value = "N";
		}
		else if (act[a] == "bgt" || act[a] == "BGT") {
			if (reg_value[rs1_int] > reg_value[rs2_int]) {
				actual_value = "T";
			}
			else actual_value = "N";
		}
		else if (act[a] == "ble" || act[a] == "BLE") {
			if (reg_value[rs1_int] <= reg_value[rs2_int]) {
				actual_value = "T";
			}
			else actual_value = "N";
		}
		else if (act[a] == "blt" || act[a] == "BLT") {
			if (reg_value[rs1_int] < reg_value[rs2_int]) {
				actual_value = "T";
			}
			else actual_value = "N";
		}
		else if (act[a] == "bne" || act[a] == "BNE") {
			if (reg_value[rs1_int] != reg_value[rs2_int]) {
				actual_value = "T";
			}
			else actual_value = "N";
		}

	}

	else if (act[a].length() == 4) {
		string rs;
		int rs_num = 0;
		int k;
		int rs_int;
		for (int j = 0; j < inst_s[a].length(); j++) {
			if (inst_s[a][j] == 'r' || inst_s[a][j] == 'R') {
				for (k = j + 1; inst_s[a][k] != ','&&k < inst_s[a].length(); k++) {
					rs += inst_s[a][k];
				}
				j = k;
				rs_num++;
			}
			if (rs_num == 1) {
				for (int m = j; m < inst_s[a].length(); m++) {
					if (inst_s[a][m] != ' '&&inst_s[a][m] != '	'&&inst_s[a][m] != ',')br_label += inst_s[a][m];
				}
				break;
			}
		}
		stringstream d;
		d << rs;
		d >> rs_int;

		if (act[a] == "beqz" || act[a] == "BEQZ") {
			if (reg_value[rs_int] == 0) {
				actual_value = "T";
			}
			else actual_value = "N";
		}
		else if (act[a] == "bgez" || act[a] == "BGEZ") {
			if (reg_value[rs_int] >= 0) {
				actual_value = "T";
			}
			else actual_value = "N";
		}
		else if (act[a] == "bgtz" || act[a] == "BGTZ") {
			if (reg_value[rs_int] > 0) {
				actual_value = "T";
			}
			else actual_value = "N";
		}
		else if (act[a] == "blez" || act[a] == "BLEZ") {
			if (reg_value[rs_int] <= 0) {
				actual_value = "T";
			}
			else actual_value = "N";
		}
		else if (act[a] == "bltz" || act[a] == "BLTZ") {
			if (reg_value[rs_int] < 0) {
				actual_value = "T";
			}
			else actual_value = "N";
		}
		else if (act[a] == "bnez" || act[a] == "BNEZ") {
			if (reg_value[rs_int] != 0) {
				actual_value = "T";
			}
			else actual_value = "N";
		}


	}

	if (actual_value == "T") {
		i_next = find_label(br_label);
	}
	cout << "--------- Actual Outcome¡G" << actual_value << " ---------" << endl;
}

//update BHT
void updatebht(int a) {
	int now_state = bht[pc_index][0];
	int now_state_value = bht[pc_index][now_state + 1];
	if (actual_value == "T") {
		if (now_state_value != 3)bht[pc_index][now_state + 1]++;
		if (now_state == 0)bht[pc_index][0] = 1;
		else if (now_state == 1)bht[pc_index][0] = 3;
		else if (now_state == 2)bht[pc_index][0] = 1;
	}
	else if (actual_value == "N") {
		if (now_state_value != 0)bht[pc_index][now_state + 1]--;
		if (now_state == 1)bht[pc_index][0] = 2;
		else if (now_state == 2)bht[pc_index][0] = 0;
		else if (now_state == 3)bht[pc_index][0] = 2;
	}
}




int main() {
	while (getline(infile, inst_s[i_num]) && inst_s[i_num] != "") {
		check_label(inst_s[i_num]);
		if (inst_s[i_num] != ""&&inst_s[i_num][0] != '/') {
			st_pc(inst_s[i_num]);
			cut(inst_s[i_num]);
		}
		else i_num--;
		i_num++;
	}
	/*
	for (int i = 0; i < i_num; i++) {
		outfile << inst_s[i]<<"PC¡G"<<pc[i] << endl;
		outfile << act[i] << endl;
	}
	outfile << endl;
	for (int i = 0; i < label_num; i++) {
		outfile << label_name[i] << "  " << label_loc[i] << endl;
	}
	*/
	//=================Input Entry=================//
	cout << "Please Input Entries (Entries = 2 ªº 1~10 ¦¸¤è ) ¡G";
	cin >> entry;
	//outfile << entry << endl;
	for (i_current = 0; i_current < i_num; i_current++) {
		//cout<< i_current << endl;
		if (inst_s[i_current] == "")cout << endl;
		else {

			//============================Branch Instrction==========================================			
			if (act[i_current][0] == 'b') {
				//===========Output Instruction=======================
				cout << endl << "||||||||||||||||||||||||||||||||||||||" << endl << endl;
				cout<< "I" << i_current << "¡G" << inst_s[i_current] << endl;
				//========================Prediction==============================
				predict(i_current);
				//======================Actual Outcome===========================
				actual(i_current);
				//======================update BHT================================
				updatebht(i_current);
				//===============Miss or Not=================================
				cout << "Miss or not¡G";
				if (predict_value == actual_value)cout << "Correct" << endl;
				else cout << "Miss" << endl;
				//=================Check BHT =============================
				cout << "---------- Check Update BHT ---------" << endl;
				for (int i = 0; i < entry; i++) {
					cout << "entry_" << i << "¡G " << endl;
					cout << fix(binary(bht[i][0]), 2) << ",";
					cout << pre_value[bht[i][1]] << ",";
					cout << pre_value[bht[i][2]] << ",";
					cout << pre_value[bht[i][3]] << ",";
					cout << pre_value[bht[i][4]] << endl;
				}
				cout << endl;
				if (actual_value == "T")i_current = i_next - 1;
			}

			//============================Other Instrction==========================================
			else {
				string rd, rs1, rs2;
				int rd_int, rs1_int, rs2_int;
				int rd_num = 0;
				int k;
				for (int j = 0; j < inst_s[i_current].length(); j++) {
					if ((inst_s[i_current][j] == 'r' || inst_s[i_current][j] == 'R') && (inst_s[i_current][j - 1] != 'O' && inst_s[i_current][j - 1] != 'o')) {
						for (k = j + 1; inst_s[i_current][k] != ','&&k < inst_s[i_current].length(); k++) {
							if (rd_num == 0)rd += inst_s[i_current][k];
							else if (rd_num == 1)rs1 += inst_s[i_current][k];
							else if (rd_num == 2)rs2 += inst_s[i_current][k];
						}
						j = k;
						rd_num++;
						if (rd_num == 2 && (act[i_current] == "move" || act[i_current] == "MOVE"))break;
					}
					if (rd_num == 2 && (act[i_current] == "addi" || act[i_current] == "ADDI" || act[i_current] == "andi" || act[i_current] == "ANDI" || act[i_current] == "ori" || act[i_current] == "ORI")) {
						for (int m = j; m < inst_s[i_current].length(); m++) {
							if (inst_s[i_current][m] != ' '&&inst_s[i_current][m] != '	'&&inst_s[i_current][m] != ',') {
								rs2 += inst_s[i_current][m];
							}
						}
						break;
					}
					if (rd_num == 1 && (act[i_current] == "li" || act[i_current] == "LI")) {
						for (int m = j; m < inst_s[i_current].length(); m++) {
							if (inst_s[i_current][m] != ' '&&inst_s[i_current][m] != '	'&&inst_s[i_current][m] != ',') {
								rs1 += inst_s[i_current][m];
							}
						}
						break;
					}

				}

				stringstream othe;
				othe << rd;
				othe >> rd_int;
				stringstream ot;
				ot << rs1;
				ot >> rs1_int;
				if (rs2 != "") {
					stringstream oth;
					oth << rs2;
					oth >> rs2_int;
				}
				//cout << rd_int << " " << rs1_int << " " << rs2 << " " << endl;
				if (act[i_current] == "add" || act[i_current] == "ADD") {
					reg_value[rd_int] = reg_value[rs1_int] + reg_value[rs2_int];
				}
				else if (act[i_current] == "addi" || act[i_current] == "ADDI") {
					reg_value[rd_int] = reg_value[rs1_int] + rs2_int;
				}
				else if (act[i_current] == "mul" || act[i_current] == "MUL") {
					reg_value[rd_int] = reg_value[rs1_int] * reg_value[rs2_int];
				}
				else if (act[i_current] == "li" || act[i_current] == "LI") {
					reg_value[rd_int] = rs1_int;
				}
				else if (act[i_current] == "move" || act[i_current] == "MOVE") {
					int a = reg_value[rd_int];
					reg_value[rd_int] = reg_value[rs1_int];
					reg_value[rs1_int] = a;
				}
				else if (act[i_current] == "and" || act[i_current] == "AND" || act[i_current] == "andi" || act[i_current] == "ANDI") {
					string a;
					if (act[i_current] == "andi" || act[i_current] == "ANDI")a = binary(rs2_int);
					else a = binary(reg_value[rs2_int]);
					string b = binary(reg_value[rs1_int]);
					int l;
					if (a.length() < b.length()) {
						a = fix(a, b.length());
						l = b.length();
					}
					else {
						b = fix(b, a.length());
						l = a.length();
					}
					int p = l - 1;
					reg_value[rd_int] = 0;
					for (int r = 0; r < l; r++) {
						if (a[r] == '1'&&b[r] == '1')reg_value[rd_int] += 1 * pow(2, p);
						p--;
					}
				}
				else if (act[i_current] == "or" || act[i_current] == "OR" || act[i_current] == "ori" || act[i_current] == "ORI") {
					string a;
					if (act[i_current] == "ori" || act[i_current] == "ORI")a = binary(rs2_int);
					else a = binary(reg_value[rs2_int]);
					string b = binary(reg_value[rs1_int]);
					int l;
					if (a.length() < b.length()) {
						a = fix(a, b.length());
						l = b.length();
					}
					else {
						b = fix(b, a.length());
						l = a.length();
					}
					int p = l - 1;
					reg_value[rd_int] = 0;
					for (int r = 0; r < l; r++) {
						if (a[r] == '1' || b[r] == '1')reg_value[rd_int] += 1 * pow(2, p);
						p--;
					}
				}
				reg_value[0] = 0;//R0 is always 0
			}
		}
		/*
		for (int i = 0; i < 32; i++) {
			cout << reg_value[i] << " ";
		}
		cout << endl;
		*/
	}


	infile.close();
	//outfile.close();
}
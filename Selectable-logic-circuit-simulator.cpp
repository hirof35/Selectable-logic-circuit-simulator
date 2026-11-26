#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

// 論理演算クラス
class LogicGate {
public:
    // AND
    bool andGate(bool a, bool b) const { return a && b; }
    // OR
    bool orGate(bool a, bool b) const { return a || b; }
    // NOT
    bool notGate(bool a) const { return !a; }
    // NAND
    bool nandGate(bool a, bool b) const { return !(a && b); }
    // NOR
    bool norGate(bool a, bool b) const { return !(a || b); }
    // XOR
    bool xorGate(bool a, bool b) const { return a != b; }

    // 演算実行
    bool calculate(const string& type, bool a, bool b) {
        if (type == "AND") return andGate(a, b);
        if (type == "OR") return orGate(a, b);
        if (type == "NOT") return notGate(a);
        if (type == "NAND") return nandGate(a, b);
        if (type == "NOR") return norGate(a, b);
        if (type == "XOR") return xorGate(a, b);
        cerr << "エラー: 不明な演算 [" << type << "]" << endl;
        return false;
    }
};

// ユーティリティ関数：文字列→bool
bool strToBool(const string& s) {
    string t = s;
    transform(t.begin(), t.end(), t.begin(), ::tolower);
    return t == "1" || t == "true" || t == "yes";
}

// メニュー表示
void showMenu() {
    cout << "\n===== 選択式論理回路 メニュー =====" << endl;
    cout << "1. キーボードから入力して計算" << endl;
    cout << "2. ファイルから読み込んで計算" << endl;
    cout << "3. 終了" << endl;
    cout << "===================================" << endl;
    cout << "選択してください：";
}

// 演算タイプ選択
string selectGateType() {
    cout << "\n利用可能な演算：" << endl;
    cout << "AND  OR  NOT  NAND  NOR  XOR" << endl;
    cout << "演算名を入力：";
    string type;
    cin >> type;
    transform(type.begin(), type.end(), type.begin(), ::toupper);
    return type;
}

// 手動計算
void manualCalculate() {
    LogicGate gate;
    string type = selectGateType();
    
    bool a, b;
    cout << "入力A (0または1)："; cin >> a;
    
    if (type != "NOT") {
        cout << "入力B (0または1)："; cin >> b;
    } else {
        b = false; // NOTは1入力
    }
    
    bool result = gate.calculate(type, a, b);
    cout << "\n【計算結果】" << endl;
    cout << type << "(" << a << (type != "NOT" ? ", " + to_string(b) : "") << ") = " << result << endl;
    
    // ファイル保存
    cout << "\n結果をファイルに保存しますか？ (y/n)：";
    char ans; cin >> ans;
    if (ans == 'y' || ans == 'Y') {
        string filename = "result.txt";
        ofstream ofs(filename, ios::app);
        if (ofs) {
            ofs << type << "," << a << "," << (type != "NOT" ? to_string(b) : "-") << "," << result << endl;
            cout << filename << " に保存しました。" << endl;
        } else {
            cerr << "ファイルを開けませんでした。" << endl;
        }
    }
}

// ファイルから一括計算
void fileCalculate() {
    LogicGate gate;
    string inFilename = "input.txt";
    string outFilename = "output.txt";
    
    ifstream ifs(inFilename);
    if (!ifs.is_open()) {
        cerr << "エラー: " << inFilename << " が見つかりません。" << endl;
        cerr << "【input.txt の書式例】" << endl;
        cerr << "AND,1,0" << endl;
        cerr << "OR,1,1" << endl;
        cerr << "NOT,1" << endl;
        cerr << "XOR,1,0" << endl;
        return;
    }
    
    ofstream ofs(outFilename);
    if (!ofs.is_open()) {
        cerr << "エラー: " << outFilename << " に書き込めません。" << endl;
        return;
    }
    
    string line;
    int lineNum = 0;
    cout << "\n=== ファイル計算結果 ===" << endl;
    ofs << "演算,入力A,入力B,結果" << endl;
    
    while (getline(ifs, line)) {
        if (line.empty() || line[0] == '#') continue;
        lineNum++;
        
        vector<string> parts;
        stringstream ss(line);
        string item;
        while (getline(ss, item, ',')) {
            parts.push_back(item);
        }
        
        if (parts.size() < 2) {
            cerr << lineNum << "行目: フォーマットエラー" << endl;
            continue;
        }
        
        string type = parts[0];
        bool a = strToBool(parts[1]);
        bool b = (parts.size() >= 3) ? strToBool(parts[2]) : false;
        bool result = gate.calculate(type, a, b);
        
        string bStr = (type != "NOT") ? parts[2] : "-";
        cout << lineNum << ": " << type << "(" << a << "," << bStr << ") = " << result << endl;
        ofs << type << "," << a << "," << bStr << "," << result << endl;
    }
    
    cout << "========================" << endl;
    cout << outFilename << " に保存完了（計算" << lineNum << "件）" << endl;
    ifs.close();
    ofs.close();
}

// メイン処理
int main() {
    cout << "===== 選択式論理回路 シミュレーター =====" << endl;
    int choice;
    
    do {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1024, '\n');
            cerr << "無効な入力です。数字を入力してください。" << endl;
            continue;
        }
        
        switch (choice) {
        case 1:
            manualCalculate();
            break;
        case 2:
            fileCalculate();
            break;
        case 3:
            cout << "終了します。お疲れ様でした。" << endl;
            break;
        default:
            cout << "1～3の数字を入力してください。" << endl;
        }
    } while (choice != 3);
    
    return 0;
}

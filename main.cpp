#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <limits>
#include <array>

#include <set>
#include <random>
#include <numeric>

int Out_CinFuncs;
int Type;
int CommandCode;
int CinCode;
int CinId;
string buffer_;
string Text;

// Предварительные объявления
class PL;
class CS;

vector<PL> vecPL;
vector<CS> vecCS;

class PL {
    public:
        string Name;
        string NameFile;
        int Length;
        int Diameters;
        bool State;
        void display_info() {
            cout << Name << endl;
            cout << Length << endl;
            cout << Diameters << endl;
            cout << (State ? "true" : "false") << endl;
        }
        void download() {
            string line;
            ifstream in(NameFile);
            if (in.is_open()) {
                getline(in, line);
                Name = line;
                getline(in, line);
                Length = atoi(line.c_str());
                getline(in, line);
                Diameters =  atoi(line.c_str());
                getline(in, line);
                State = (line == "true");
            }
        }
        void save() {
            string outtxt;
            ofstream outFile(NameFile);
            if (outFile.is_open()) {
                outtxt += Name+"\n";
                outtxt += to_string(Length)+"\n";
                outtxt += to_string(Diameters)+"\n";
                outtxt += (State ? "true" : "false");
                outFile << outtxt;
                outFile.close();
            }
        }
        void Delete() {
            string outtxt;
            string line;
            ifstream in("pipeline_main.txt");
            if (in.is_open()) {
                while (getline(in, line)) {
                    if (line != NameFile) {
                        outtxt += line + "\n";
                    }
                }
                in.close();
            }
            ofstream out("pipeline_main.txt");
            if (out.is_open()) {
                out << outtxt;
                out.close();
            }
            remove(NameFile.c_str());
        }
};

string NameFile;
void GenerateName() {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(10, 99);
    
    std::set<int> numbers;
    while (numbers.size() < 5) {
        numbers.insert(dist(gen));
    }
    
    std::string result = std::accumulate(
        numbers.begin(), numbers.end(), std::string{},
        [](const std::string& acc, int num) {
            return acc + (acc.empty() ? "" : "-") + std::to_string(num);
        }
    );
    NameFile = result;
}

class CS {
    public:
        string Name;
        string NameFile;
        int Length;
        int LengthOfWork;
        int ClassStation;
        void display_info() {
            cout << Name << endl;
            cout << Length << endl;
            cout << LengthOfWork << endl;
            cout << ClassStation << endl;
        }
    void download() {
        string line;
        ifstream in(NameFile);
        if (in.is_open()) {
            getline(in, line);
            Name = line;
            getline(in, line);
            Length = atoi(line.c_str());
            getline(in, line);
            LengthOfWork =  atoi(line.c_str());
            getline(in, line);
            ClassStation = atoi(line.c_str());
        }
    }
    void save() {
        string outtxt;
        ofstream outFile(NameFile);
        if (outFile.is_open()) {
            outtxt += Name+"\n";
            outtxt += to_string(Length)+"\n";
            outtxt += to_string(LengthOfWork)+"\n";
            outtxt += to_string(ClassStation);
            outFile << outtxt;
            outFile.close();
        }
    }
    void Delete() {
        string outtxt;
        string line;
        ifstream in("compressor_stations_main.txt");
        if (in.is_open()) {
            while (getline(in, line)) {
                if (line != NameFile) {
                    outtxt += line + "\n";
                }
            }
            in.close();
        }
        ofstream out("compressor_stations_main.txt");
        if (out.is_open()) {
            out << outtxt;
            out.close();
        }
        remove(NameFile.c_str());
    }
};

void download(bool type) {
    if (type) {
        vecCS.clear();
    } else {
        vecPL.clear();
    }
    Text = "";
    
    string line;
    ifstream in(type ? "compressor_stations_main.txt" : "pipeline_main.txt");
    if (in.is_open()) {
        int i = 0;
        while (getline(in, line)) {
            if (type) {
                CS p;
                p.NameFile = line;
                p.download();
                vecCS.push_back(p);
                Text += to_string(i) + ": " + p.Name + "\n";
            } else {
                PL p;
                p.NameFile = line;
                p.download();
                vecPL.push_back(p);
                Text += to_string(i) + ": " + p.Name + "\n";
            }
            i++;
        }
        in.close();
        if (type) {
            Text += to_string(vecCS.size()) + ": Create new\n";
        } else {
            Text += to_string(vecPL.size()) + ": Create new\n";
        }
    } else {
        Text = "0: Create new\n";
    }
}

// ФУНКЦИИ ПРОВЕРКИ ВВОДА (БЕЗ ИЗМЕНЕНИЙ)
int CinFunc1(const vector<string>& list) {
    bool status = 1;
    for (size_t i = 0; i < list.size(); ++i) {
        if (buffer_ == list[i]) {
            status = 0;
        }
    }
    if (status == 1) {
        cerr << "Input error." << endl;
    }
    return status;
}

int CinFunc(int upfloor) {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cerr << "Input error." << endl;
        return 1;
    }
    if (upfloor == CommandCode) {
        cout << "exit program" << endl;
        exit(0);
    }
    if (CommandCode < 0 | CommandCode > upfloor) {
        cerr << "Input error." << endl;
        return 1;
    }
    return 0;
}

bool isIntegerStrict(const std::string& str) {
    try {
        if (str.empty()) return false;
        
        size_t pos = 0;
        stoi(str, &pos);
        return pos == str.length();
    } catch (const exception&) {
        return false;
    }
}

// НОВАЯ ФУНКЦИЯ - passed (остановить создание)
bool passed = false;

template<typename T> void ConditionPL(T& p) {
    vector<string> list = {"true", "false"};
    switch (CommandCode)
    {
        case 0:
            buffer_ = "";
            cout << "Enter new name: " << endl;
            getline(cin, buffer_);
            if (buffer_ == "passed") {
                passed = true;
                break;
            }
            p.Name = buffer_;
            break;
        case 1:
            buffer_ = "";
            cout << "Enter new Length: " << endl;
            getline(cin, buffer_);
            
            if (buffer_ == "passed") {
                passed = true;
                break;
            }

            // Проверка на валидность
            if(isIntegerStrict(buffer_)) {
                CommandCode = atoi(buffer_.c_str());
                if (CommandCode == 0) {
                    cout << "Input error" << endl;
                    passed = true;
                    break;
                }
                Out_CinFuncs = CinFunc(1000);
                if (Out_CinFuncs == 1) {
                    passed = true;
                    break;
                }
                p.Length = CommandCode;
                break;
            } else {
                cout << "Input error" << endl;
                passed = true;
                break;
            }

        case 2:
            buffer_ = "";
            cout << "Enter new Diameters: " << endl;
            getline(cin, buffer_);
            
            if (buffer_ == "passed") {
                passed = true;
                break;
            }

            // Проверка на валидность
            if(isIntegerStrict(buffer_)) {
                CommandCode = atoi(buffer_.c_str());
                if (CommandCode == 0) {
                    cout << "Input error" << endl;
                    passed = true;
                    break;
                }
                Out_CinFuncs = CinFunc(1000);
                if (Out_CinFuncs == 1) {
                    passed = true;
                    break;
                }
                p.Diameters = CommandCode;
                break;
            } else {
                cout << "Input error" << endl;
                passed = true;
                break;
            }

        case 3:
            buffer_ = "";
            cout << "Enter new State: " << endl;
            getline(cin, buffer_);
            
            if (buffer_ == "passed") {
                passed = true;
                break;
            }
            
            // Проверка на валидность
            Out_CinFuncs = CinFunc1(list);
            if (Out_CinFuncs == 1) {
                passed = true;
                break;
            }
            // Перевод в другой тип данных
            if (buffer_ == "true") {
                p.State = true;
            } else if(buffer_ == "false") {
                p.State = false;
            }
            break;
        case 4:
            p.display_info();
            break;
        default:
            break;
    }
}

template<typename T> void ConditionCS(T& p) {
    vector<string> list = {"1", "2", "3"};
    switch (CommandCode)
    {
        case 0:
            buffer_ = "";
            cout << "Enter new name: " << endl;
            getline(cin, buffer_);
            if (buffer_ == "passed") {
                passed = true;
                break;
            }
            p.Name = buffer_;
            break;
        case 1:
            buffer_ = "";
            cout << "Enter new Length: " << endl;
            getline(cin, buffer_);
            
            if (buffer_ == "passed") {
                passed = true;
                break;
            }
            
            // Проверка на валидность
            if(isIntegerStrict(buffer_)) {
                CommandCode = atoi(buffer_.c_str());
                if (CommandCode == 0) {
                    cout << "Input error" << endl;
                    passed = true;
                    break;
                }
                Out_CinFuncs = CinFunc(1000);
                if (Out_CinFuncs == 1) {
                    passed = true;
                    break;
                }
                p.Length = CommandCode;
                break;
            } else {
                cout << "Input error" << endl;
                passed = true;
                break;
            }
        case 2:
            buffer_ = "";
            cout << "Enter new LengthOfWork: " << endl;
            getline(cin, buffer_);
            
            if (buffer_ == "passed") {
                passed = true;
                break;
            }
            
            // Проверка на валидность
            if(isIntegerStrict(buffer_)) {
                CommandCode = atoi(buffer_.c_str());
                if (CommandCode == 0) {
                    cout << "Input error" << endl;
                    passed = true;
                    break;
                }
                Out_CinFuncs = CinFunc(1000);
                if (Out_CinFuncs == 1) {
                    passed = true;
                    break;
                }
                p.LengthOfWork = CommandCode;
                break;
            } else {
                cout << "Input error" << endl;
                passed = true;
                break;
            }
        case 3:
            buffer_ = "";
            cout << "Enter new ClassStation: " << endl;
            getline(cin, buffer_);
            
            if (buffer_ == "passed") {
                passed = true;
                break;
            }
            
            // Проверка на валидность
            Out_CinFuncs = CinFunc1(list);
            if (Out_CinFuncs == 1) {
                passed = true;
                break;
            }
            p.ClassStation = atoi(buffer_.c_str());
            break;
        case 4:
            p.display_info();
            break;
        default:
            break;
    }
}

int main() {
    bool status = false;
    while (true) {
        cout << "[0 - Pipeline, 1 - Compressed station, 2 - Finished program]: ";
        buffer_ = "";
        getline(cin, buffer_);

        status = isIntegerStrict(buffer_.c_str());
        if(status) {    
            CommandCode = stoi(buffer_.c_str());
            CinCode = CinFunc(2);
            if (CinCode == 1) {
                break;
            }
        } else {
            cout << "Input error" << endl;
            continue;
        }

        Type = CommandCode;
        if(Type == 0) {
            buffer_ = "";
            download(false);
        } else if (Type == 1) {
            buffer_ = "";
            download(true);
        }
        cout << Text << endl;
        
        cout << "Enter choice: ";
        getline(cin, buffer_);
        status = isIntegerStrict(buffer_.c_str());
        if(status) {    
            CommandCode = stoi(buffer_.c_str());
            if(Type == 0) {
                CinCode = CinFunc(vecPL.size()+1);
            } else {
                CinCode = CinFunc(vecCS.size()+1);
            }
            if (CinCode == 1) {
                break;
            }
        } else {
            cout << "Input error" << endl;
            continue;
        }
        
        CinId = CommandCode;
        if (Type == 0) {
            if (CinId == vecPL.size()) {
                PL p;
                passed = false; // Сбрасываем флаг перед созданием
                for (int i = 0; i <= 3; i++)
                {
                    if (passed) break; // Если была ошибка или пользователь ввел "passed", выходим
                    CommandCode = i;
                    ConditionPL(p);
                }
                
                if (!passed) { // Создаем объект только если не было ошибок и не введен "passed"
                    GenerateName();
                    p.NameFile = NameFile + ".txt";
                    vecPL.push_back(p);
                    p.save();
                    cout << "Pipeline created successfully!" << endl;
                } else {
                    cout << "Pipeline creation cancelled." << endl;
                }
                continue;
            }
            
            PL p = vecPL[CinId];
            int choice;
            do {
                cout << "[\n 0 - Name,\n 1 - Length,\n 2 - Diameters,\n 3 - State,\n 4 - print,\n 5 - save and exit,\n 6 - delete\n]: ";
                getline(cin, buffer_);
                status = isIntegerStrict(buffer_.c_str());
                if(status) {    
                    CommandCode = stoi(buffer_.c_str());
                    if (CommandCode >= 0 && CommandCode <= 6) {
                        if (CommandCode >= 0 && CommandCode <= 4) {
                            ConditionPL(p);
                        } else if (CommandCode == 5) {
                            p.save();
                            vecPL[CinId] = p;
                            break;
                        } else if (CommandCode == 6) {
                            p.Delete();
                            vecPL.erase(vecPL.begin() + CinId);
                            break;
                        }
                    } else {
                        cout << "Input error" << endl;
                    }
                } else {
                    cout << "Input error" << endl;
                }
            } while (true);
            
        } else if(Type == 1) {
            if (CinId == vecCS.size()) {
                CS p;
                passed = false; // Сбрасываем флаг перед созданием
                for (int i = 0; i <= 3; i++)
                {
                    if (passed) break; // Если была ошибка или пользователь ввел "passed", выходим
                    CommandCode = i;
                    ConditionCS(p);
                }
                
                if (!passed) { // Создаем объект только если не было ошибок и не введен "passed"
                    GenerateName();
                    p.NameFile = NameFile + ".txt";
                    vecCS.push_back(p);
                    p.save();
                    cout << "Compressor station created successfully!" << endl;
                } else {
                    cout << "Compressor station creation cancelled." << endl;
                }
                continue;
            }
            
            CS p = vecCS[CinId];
            int choice;
            do {
                cout << "[\n0 - Name,\n 1 - Length,\n 2 - Length of work,\n 3 - Class of station,\n 4 - print,\n 5 - save and exit,\n 6 - delete\n]: ";
                getline(cin, buffer_);
                status = isIntegerStrict(buffer_.c_str());
                if(status) {    
                    CommandCode = stoi(buffer_.c_str());
                    if (CommandCode >= 0 && CommandCode <= 6) {
                        if (CommandCode >= 0 && CommandCode <= 4) {
                            ConditionCS(p);
                        } else if (CommandCode == 5) {
                            p.save();
                            vecCS[CinId] = p;
                            break;
                        } else if (CommandCode == 6) {
                            p.Delete();
                            vecCS.erase(vecCS.begin() + CinId);
                            break;
                        }
                    } else {
                        cout << "Input error" << endl;
                    }
                } else {
                    cout << "Input error" << endl;
                }
            } while (true);
        } else {
            return 0;
        }
    }
    return 0;
}
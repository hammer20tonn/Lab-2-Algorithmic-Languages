#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <cstdio>



struct Pipeline {
    string Name;
    string titles[4];
    /**
    int Length;
    int Diameters;
    string State;
    string titles[4];

    int LengthFactory; // Количество цехов
    int TypeOfStation; // Тип КС
    int LengthFactoryWork; // Количество цехов в работе
    **/
    int cell1; // Количество цехов, Протяжённость(км)
    int cell2; // Количество цехов в работе, Диаметр
    int cell3; // Тип КС, Состояние(0 - on, 1 - off)
    void display_info(string name) {
        if (name == "Pipelines.txt") {
            cout << titles[0] << Name << endl;
            cout << titles[1] << cell1 << endl;
            cout << titles[2] << cell2 << endl;
            cout << titles[3] << cell3 << endl;
        } else if (name == "CompressedStation.txt") {
            cout << titles[0] << Name << endl;
            cout << titles[1] << cell1 << endl;
            cout << titles[2] << cell2 << endl;
            cout << titles[3] << cell3 << endl;
        }
    }
};

struct CompressedStation {
    string Name;
    int LengthFactory;
    int TypeOfStation;
    int LengthFactoryWork;
};

int CommandCode1;
int CommandCode2;
int CommandCode3; // Id
int CommandCode4;
int CommandCode5;
string CommandCycle;
string SubstringCommand;
int CinSpecial;

vector<string> vec(4);
vector<Pipeline> PipeLines;
Pipeline Reader(int indx, string name, string title[4]) {
    PipeLines.clear();
    string text;
    string line;
    bool flag = false;
    string Name;
    int LengthOfFactory;
    int Diameters;
    string OffOn;
    ifstream in(name);
    
    int num = 0;
    int num1 = 0;

    
    if (in.is_open()) {
        while (getline(in, line)) {
            if (line == "-------") {
                if (flag == false) {
                    flag = true;
                    num = 0;
                } else if (flag == true) {
                    flag = false;
                    Pipeline PipeLine;
                    for(int i = 0; i < 4; i++) {
                        PipeLine.titles[i] = title[i];
                    }
                    PipeLine.Name = vec.at(0);
                    try {
                        PipeLine.cell1 = stoi(vec.at(1)); // length
                        PipeLine.cell2 = stoi(vec.at(2)); // diameter
                    } catch (const exception& e) {
                        cerr << "Conversion error: " << e.what() << endl;
                        continue;
                    }
                    PipeLine.cell3 = stoi(vec.at(3));
                    PipeLines.push_back(PipeLine);
                    line = to_string(num1) + ": " + vec.at(0) + "\n";
                    text = text + line;
                    num1++;
                }
            } else if (flag == true) {
                if (num < 4) {
                    vec[num] = line;
                    num++;
                }
            }
        }
    }
    in.close();
    //cout << text;
    return PipeLines[indx];
}







void Reader_info(string name) {
    string text;
    string line;
    bool flag = false;
    string Name;
    int LengthOfFactory;
    int Diameters;
    string OffOn;
    ifstream in(name);
    
    vector<string> vec(4);
    int num = 0;
    int num1 = 0;
    
    if (in.is_open()) {
        while (getline(in, line)) {
            if (line == "-------") {
                if (flag == false) {
                    flag = true;
                    num = 0;
                } else if (flag == true) {
                    flag = false;
                    line = to_string(num1) + ": " + vec.at(0) + "\n";
                    text = text + line;
                    num1++;
                }
            } else if (flag == true) {
                if (num < 4) {
                    vec[num] = line;
                    num++;
                }
            }
        }
    }
    in.close();
    cout << text;
}







void write(string name) {
    string outtxt;
    string miniouttxt;
    for (Pipeline unit : PipeLines) {
        miniouttxt += "-------\n";
        miniouttxt += unit.Name+"\n";
        miniouttxt += to_string(unit.cell1)+"\n";
        miniouttxt += to_string(unit.cell2)+"\n";
        miniouttxt += to_string(unit.cell3)+"\n";
        miniouttxt += "-------\n";
        ofstream out;
        out.open(unit.Name+".txt");
        if (out.is_open())
        {
            out << miniouttxt;
        }   
        out.close();
        outtxt += miniouttxt;
        miniouttxt = "";
    }
    ofstream file(name);
    if (file.is_open()) {
        if (PipeLines.size() == 0) {
            outtxt += "-------\n";
            outtxt += "Zero project\n";
            outtxt += "0\n";
            outtxt += "0\n";
            outtxt += "0\n";
            outtxt += "-------\n";
        }
        file << outtxt;
        file.close();
    }
}




void Logging(const std::string& text, const std::string& name) {
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    
    char time_buffer[80];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    std::ofstream thread_("logs_" + name, std::ios::app);
    if (thread_.is_open()) {
        thread_ << "[" << time_buffer << "] " << text << "\n";
    }
}


int CinFunc(string name) {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cerr << "Input error." << endl;
        Logging("Error - Input error.", name);
        return 1;
    }
    return 0;
}


void EditFunc(string name, string title[], Pipeline *pipelines, int *CommandCode3, int size_) {
    cout << endl;
    cout << "[\n 0 - "+title[0]+",\n 1 - "+title[1]+", \n 2 - "+title[2]+",\n 3 - "+title[3]+",\n 4 - Main menu,\n 5 - Finished program,\n 6 - Check info,\n 7 - Delete \n]: ";
    cin >> CommandCode4;
    cout << endl;
    CinSpecial = CinFunc(name);
    if (CinSpecial == 1) {
        return;
    }
    if (CommandCode4 == 0) {
        string Name;
        cout << title[0] << endl;
        cin >> Name;
        CinSpecial = CinFunc(name);
        if (CinSpecial == 1) {
            return;
        }
        cout << endl;
        cout << "for (int i = 0; i < size_; i++) {" << endl;
        for (int i = 0; i < size_; i++) {
            Logging("Installation name - "+Name, name);
            cout << pipelines[i].Name << endl;
            if( remove( (pipelines[i].Name+".txt").c_str() ) != 0 ) {
                Logging("Update name Error", name);
            } else {
                Logging("Update name - "+pipelines[i].Name, name);
                pipelines[i].Name = Name;
            }
        }
    } else if (CommandCode4 == 1) {
        int Length;
        cout << title[1] << endl;
        cin >> Length;
        CinSpecial = CinFunc(name);
        if (CinSpecial == 1) {
            return;
        }
        cout << endl;
        for (int i = 0; i < size_; i++) {
            pipelines[i].cell1 = Length;
            Logging("Installation Length - "+to_string(Length), name);
        }
    } else if (CommandCode4 == 2) {
        int Diameter;
        cout << title[2] << endl;
        cin >> Diameter;
        CinSpecial = CinFunc(name);
        if (CinSpecial == 1) {
            return;
        }
        cout << endl;
        for (int i = 0; i < size_; i++) {
            pipelines[i].cell2 = Diameter;
            Logging("Installation Diameter - "+to_string(Diameter), name);
        }
    }  else if (CommandCode4 == 3) {
        int State;
        cout << title[3] << endl;
        cin >> State;
        CinSpecial = CinFunc(name);
        if (CinSpecial == 1) {
            return;
        }
        cout << endl;
        for (int i = 0; i < size_; i++) {
            pipelines[i].cell3 = State;
            Logging("Installation State - "+to_string(State), name);
        }
    } else if (CommandCode4 == 4) {
        return;
    } else if (CommandCode4 == 5) {
        exit(0);
    } else if(CommandCode4 == 6) {
        for (int i = 0; i < size_; i++) {
            pipelines[i].display_info(name);
        }
    } else if(CommandCode4 == 7) {
        for (int i = 0; i < size_; i++) {
            if( remove( (PipeLines[CommandCode3[i]].Name+".txt").c_str() ) != 0 ) {
                Logging("Delete Error", name);
            } else {
                Logging("Delete - "+PipeLines[CommandCode3[i]].Name, name);
                PipeLines.erase(PipeLines.begin() + CommandCode3[i]);
            }
        }
        write(name);
        return;
    } else {
        cerr << "Input error." << endl;
        Logging("Error - Input error.", name);
        return;
    }
    for (int i = 0; i < size_; i++) { 
        PipeLines[CommandCode3[i]] = pipelines[i];
    }
    write(name);
}



int TypesOfState[2] = {0, 1};
vector<int> ids_;
vector<Pipeline> pipelines_;
int CoreFunc(string name, string title[]) {
    int num_;
    int* ids_1;
    Pipeline* pipelines_1;
    string buffer_;
    while (true) {
        cout << "[\n 0 - Edit,\n 1 - Create new,\n 2 - Delete,\n 3 - Check info,\n 4 - Main menu,\n 5 - Finished program,\n 6 - Filter\n]: ";
        cin >> CommandCode2;
        cout << endl;
        CinSpecial = CinFunc(name);
        if (CinSpecial == 1) {
            break;
        }
        if (CommandCode2 == 0) {
            Reader_info(name);
            cin >> CommandCode3;
            cout << endl;
            CinSpecial = CinFunc(name);
            if (CinSpecial == 1) {
                break;
            }
            Pipeline pipeline;
            try {
                pipeline = Reader(CommandCode3, name, title);
                pipeline.display_info(name);
            } catch (const std::bad_alloc& e) {
                cerr << "Input error." << endl;
                Logging("Error - Input error.", name);
                break;
            }
            if (CommandCode3 == 4) {
                break;
            } else if (CommandCode3 == 5) {
                exit(0);
            } else {
                int size_ = PipeLines.size();
                if (size_ < CommandCode3 || CommandCode3 < 0) {
                    cerr << "Input error." << endl;
                    Logging("Error - Input error.", name);
                    break;
                }
            }
            Pipeline* pipelines = new Pipeline[1];
            pipelines[0] = pipeline;
            int* CommandCode3_ = new int[1];
            CommandCode3_[0] = CommandCode3;
            EditFunc(name, title, pipelines, CommandCode3_, 1);
            delete[] pipelines;
            delete[] CommandCode3_;
        } else if(CommandCode2 == 1) {
            Pipeline pipeline = Reader(0, name, title);
            string Name;
            cout << title[0] << endl;
            cin >> Name;
            CinSpecial = CinFunc(name);
            if (CinSpecial == 1) {
                break;
            }
            cout << endl;
            pipeline.Name = Name;
            int Length;
            cout << title[1] << endl;
            cin >> Length;
            CinSpecial = CinFunc(name);
            if (CinSpecial == 1) {
                break;
            }
            cout << endl;
            pipeline.cell1 = Length;
            int Diameter;
            cout << title[2] << endl;
            cin >> Diameter;
            CinSpecial = CinFunc(name);
            if (CinSpecial == 1) {
                break;
            }
            cout << endl;
            pipeline.cell2 = Diameter;
            int State;
            cout << title[3] << endl;
            cin >> State;
            CinSpecial = CinFunc(name);
            if (CinSpecial == 1) {
                break;
            }
            cout << endl;
            pipeline.cell3 = State;
            PipeLines.push_back(pipeline);
            Logging("Create project - "+Name, name);
            write(name);
        } else if(CommandCode2 == 2) {
            Reader_info(name);
            cin >> CommandCode3;
            cout << endl;
            CinSpecial = CinFunc(name);
            if (CinSpecial == 1) {
                break;
            }
            if( remove( (PipeLines[CommandCode3].Name+".txt").c_str() ) != 0 ) {
                Logging("Delete Error", name);
            } else {
                Pipeline pipeline = Reader(CommandCode3, name, title);
                Logging("Delete - "+PipeLines[CommandCode3].Name, name);
                PipeLines.erase(PipeLines.begin() + CommandCode3);
                write(name);
                Reader_info(name);
            }
        } else if(CommandCode2 == 3) {
            Reader_info(name);
            cin >> CommandCode3;
            cout << endl;
            CinSpecial = CinFunc(name);
            if (CinSpecial == 1) {
                break;
            }
            try {
                Pipeline pipeline = Reader(CommandCode3, name, title);
                pipeline.display_info(name);
            } catch (const bad_alloc& e) {
                cerr << "Input error." << endl;
                Logging("Error - Input error.", name);
                break;
            }
            break;
        } else if (CommandCode2 == 4) {
            break;
        } else if (CommandCode2 == 5) {
            exit(0);
        } else if(CommandCode2 == 6) {
            num_ = 0;
            if (name == "Pipelines.txt" | name == "CompressedStation.txt") {
                if (name == "Pipelines.txt") {
                    cout << "[\n 0 - Name,\n 1 - State,\n 2 - all\n]: ";
                } else if(name == "CompressedStation.txt") {
                    cout << "[\n 0 - Name,\n 1 - percentage of idle mini-factory,\n 2 - all\n]: ";
                }
                cin >> CommandCode3;
                CinSpecial = CinFunc(name);
                if (CinSpecial == 1) {
                    break;
                }
                cout << endl;
                if (CommandCode3 == 0) {
                    ids_.clear();
                    pipelines_.clear();
                    Pipeline pipeline = Reader(0, name, title);
                    num_ = 0;
                    cout << "[Substring]: ";
                    cin >> SubstringCommand;
                    CinSpecial = CinFunc(name);
                    if (CinSpecial == 1) {
                        return 0;
                    }
                    cout << endl;
                    for (Pipeline i : PipeLines) {
                        buffer_ = i.Name;
                        if(buffer_.find(SubstringCommand) != string::npos) {
                            cout << num_ << ": " << buffer_ << endl;
                        }
                        num_++;
                    }
                    num_ = 0;
                    cout << "To stop the transfer, enter - stop." << endl;
                    for (int i = 0; i < PipeLines.size(); i++) {
                        cout << "[Id]: ";
                        cin >> CommandCycle;
                        CinSpecial = CinFunc(name);
                        if (CinSpecial == 1) {
                            break;
                        }
                        cout << endl;
                        if (CommandCycle == "stop") {
                            break;
                        }
                        try {
                            num_ = stoi(CommandCycle);
                            ids_.push_back(num_);
                            pipelines_.push_back(PipeLines[num_]);
                            num_ = 0;
                        } catch (const invalid_argument& e) {
                            cerr << e.what() << endl;
                            break;
                        } catch (const out_of_range& e) {
                            cerr << e.what() << endl;
                            break;
                        }
                    }
                    int size_ = ids_.size();
                    if (size_ == 0) {
                        break;
                    }
                    ids_1 = new int[size_];
                    for (int i = 0; i < size_; i++) {
                        ids_1[i] = ids_[i];
                    }
                    pipelines_1 = new Pipeline[size_];
                    for (int i = 0; i < size_; i++) {
                        pipelines_1[i] = pipelines_[i];
                    }

                    EditFunc(name, title, pipelines_1, ids_1, size_);
                    delete[] ids_1;
                    delete[] pipelines_1;
                }
                if(CommandCode3 == 1 & name == "Pipelines.txt") {
                    ids_.clear();
                    pipelines_.clear();
                    Pipeline pipeline = Reader(0, name, title);
                    num_ = 0;
                    cout << "[\n 0 - On,\n 1 - Off\n]: ";
                    cin >> CommandCode4;
                    CinSpecial = CinFunc(name);
                    if (CinSpecial == 1) {
                        break;
                    }
                    cout << endl;
                    for (const auto& pipeline_ : PipeLines) {
                        if (pipeline_.cell3 == TypesOfState[CommandCode4]) {
                            ids_.push_back(num_);
                            pipelines_.push_back(pipeline_);
                            cout << num_ << ": " << pipeline_.Name << endl;
                        }
                        num_++;
                    }
                    int size_ = ids_.size();
                    ids_1 = new int[ids_.size()];
                    for (int i = 0; i < ids_.size(); i++) {
                        ids_1[i] = ids_[i];
                    }
                    pipelines_1 = new Pipeline[pipelines_.size()];
                    for (int i = 0; i < pipelines_.size(); i++) {
                        pipelines_1[i] = pipelines_[i];
                    }
                    EditFunc(name, title, pipelines_1, ids_1, size_);
                    delete[] ids_1;
                    delete[] pipelines_1;
                }
                if(CommandCode3 == 1 & name == "CompressedStation.txt") {
                    ids_.clear();
                    pipelines_.clear();
                    Pipeline pipeline = Reader(0, name, title);
                    vector<Pipeline> Pipelines = PipeLines;
                    vector<size_t> indices(Pipelines.size());
                    for (size_t i = 0; i < Pipelines.size(); ++i) {
                        indices[i] = i;
                    }
                    sort(indices.begin(), indices.end(),
                              [&Pipelines](size_t i, size_t j) {
                                  if (Pipelines[i].cell1 == 0 || Pipelines[j].cell1 == 0) {
                                    return Pipelines[i].cell1 > Pipelines[j].cell1;
                                  }
                                  return (double(Pipelines[i].cell2) / double(Pipelines[i].cell1)) * 100 < 
                                         (double(Pipelines[j].cell2) / double(Pipelines[j].cell1)) * 100;
                              });
                    for (size_t idx : indices) {
                        cout << idx << ": " << Pipelines[idx].Name << ", " << double(Pipelines[idx].cell2)/double(Pipelines[idx].cell1) * 100 << "%" << std::endl;
                    }
                    num_ = 0;
                    cout << "To stop the transfer, enter - stop." << endl;
                    for (int i = 0; i < Pipelines.size(); i++) {
                        cout << "[Id]: ";
                        cin >> CommandCycle;
                        CinSpecial = CinFunc(name);
                        if (CinSpecial == 1) {
                            break;
                        }
                        cout << endl;
                        if (CommandCycle == "stop") {
                            break;
                        }
                        try {
                            num_ = stoi(CommandCycle);
                            ids_.push_back(num_);
                            pipelines_.push_back(PipeLines[num_]);
                            num_ = 0;
                        } catch (const invalid_argument& e) {
                            cerr << e.what() << endl;
                            break;
                        } catch (const out_of_range& e) {
                            cerr << e.what() << endl;
                            break;
                        }
                    }
                    int size_ = ids_.size();
                    if (size_ == 0) {
                        break;
                    }
                    ids_1 = new int[size_];
                    for (int i = 0; i < size_; i++) {
                        ids_1[i] = ids_[i];
                    }
                    pipelines_1 = new Pipeline[size_];
                    for (int i = 0; i < size_; i++) {
                        pipelines_1[i] = pipelines_[i];
                    }

                    EditFunc(name, title, pipelines_1, ids_1, size_);
                    delete[] ids_1;
                    delete[] pipelines_1;
                    pipeline = Reader(0, name, title);
                }
                if (CommandCode3 == 2) {
                    ids_.clear();
                    pipelines_.clear();
                    Pipeline pipeline = Reader(0, name, title);
                    int i = 0;
                    for (i = 0; i < PipeLines.size(); i++) {
                        cout << i << ": " << PipeLines[i].Name << endl;
                    }
                    i++;
                    cout << i << ": " << "all objects" << endl;
                    num_ = 0;
                    cout << "To stop the transfer, enter - stop." << endl;
                    for (int j = 0; j < PipeLines.size(); j++) {
                        cout << "[Id]: ";
                        cin >> CommandCycle;
                        CinSpecial = CinFunc(name);
                        if (CinSpecial == 1) {
                            break;
                        }
                        cout << endl;
                        if (CommandCycle == "stop") {
                            break;
                        }
                        try {
                            num_ = stoi(CommandCycle);
                            if (num_ == i) {
                                for (int k = 0; k < PipeLines.size(); ++k) {
                                    ids_.push_back(k);
                                }
                                pipelines_ = PipeLines;
                                break;
                            }
                            ids_.push_back(num_);
                            pipelines_.push_back(PipeLines[num_]);
                            num_ = 0;
                        } catch (const invalid_argument& e) {
                            cerr << e.what() << endl;
                            break;
                        } catch (const out_of_range& e) {
                            cerr << e.what() << endl;
                            break;
                        }
                    }
                    int size_ = ids_.size();
                    if (size_ == 0) {
                        break;
                    }
                    ids_1 = new int[size_];
                    for (int i = 0; i < size_; i++) {
                        ids_1[i] = ids_[i];
                    }
                    pipelines_1 = new Pipeline[size_];
                    for (int i = 0; i < size_; i++) {
                        pipelines_1[i] = pipelines_[i];
                    }

                    EditFunc(name, title, pipelines_1, ids_1, size_);
                    delete[] ids_1;
                    delete[] pipelines_1;
                    pipeline = Reader(0, name, title);
                }
            }
        } else {
            cerr << "Input error." << endl;
            continue;
        }
    }
    return 0;
}








int main() {
    while (true) {
        cin.clear();
        cout << "[0 - Pipeline, 1 - Compressed station, 2 - Finished program]: ";
        cin >> CommandCode1;
        cout << endl;
        CinSpecial = CinFunc(".txt");
        if (CinSpecial == 1) {
            continue;
        }
        if (CommandCode1 == 0) {
            string arr[4] = {"Name: ", "Length(km): ", "Diameters(mm): ", "State(0 - on, 1 - off): "};
            CoreFunc("Pipelines.txt", arr);
        } else if(CommandCode1 == 1) {
            string arr[4] = {"Name: ", "Length of factory: ", "Length worked factory: ", "Type: "};
            CoreFunc("CompressedStation.txt", arr);
        } else if(CommandCode1 == 2) {
            exit(0);
        } else {
            cerr << "Input error." << endl;
        }
    }
    return 0;
}
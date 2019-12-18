#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <cctype>
#include <locale>
#include <iterator>
#include <random>
#include <cstdlib>


#define EQUALS "EQUALS"
#define NEQUALS "NEQUALS"
#define GREATER_THAN "GREATER_THAN"
#define LESS_THAN "LESS_THAN"
#define GREATER_THAN_OR_EQUAL "GREATER_THAN_OR_EQUAL"
#define LESS_THAN_OR_EQUAL "LESS_THAN_OR_EQUAL"
#define STARTIF "STARTIF"
#define ELSEIF "ELSEIF"
#define ENDIF "ENDIF"
#define STARTWHILE "STARTWHILE"
#define ENDWHILE "ENDWHILE"
#define ASSIGN_VAR "ASSIGN_VAR"
#define INPUT "INPUT"
#define OUTPUT_FROM_VAR "OUTPUT_FROM_VAR"
#define OUTPUT_STRING "OUTPUT_STRING"
#define OUTPUT_NEWLINE "OUTPUT_NEWLINE"
#define ADD "ADD"
#define SUBTRACT "SUBTRACT"
#define MULTIPLY "MULTIPLY"
#define DIVIDE "DIVIDE"
#define MODULO "MODULO"
#define INCREMENT_BY_ONE "INCREMENT_BY_ONE"
#define DECREMENT_BY_ONE "DECREMENT_BY_ONE"
#define AND "AND"
#define OR "OR"
#define XOR "XOR"
#define COMPLEMENT "COMPLEMENT"
#define SHIFT_LEFT "SHIFT_LEFT"
#define SHIFT_RIGHT "SHIFT_RIGHT"
#define ARRAY_DEFINE "ARRAY_DEFINE"
#define ARRAY_FROM_VAR_ASCII "ARRAY_FROM_VAR_ASCII"
#define ARRAY_PUSH "ARRAY_PUSH"
#define ARRAY_GET_SIZE "ARRAY_GET_SIZE"
#define ARRAY_ACCESS "ARRAY_ACCESS"
#define ARRAY_MODIFY "ARRAY_MODIFY"
#define RANDOM_SET_SEED "RANDOM_SET_SEED"
#define RANDOM_GEN "RANDOM_GEN"
#define EXIT "EXIT"


using namespace std;


/**
 * Hold program codes
 */
vector<string> program_data;


// initialize random object
mt19937 mt_rand;


// credit: https://stackoverflow.com/a/37454181/1768052
vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

bool is_number(const std::string &s)
{
    try {
        stol(s, nullptr, 0);
        return true;
    } catch (const std::exception &e) {
        return false;
    }
}


/**
 * Trim functionalities
 * credit: https://stackoverflow.com/a/217605/1768052
 */

// trim from left
static inline std::string ltrim_copy(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    return s;
}

bool comparison(int left, const string &op, int right)
{
    if (op == EQUALS)
        return left == right;
    else if (op == NEQUALS)
        return left != right;
    else if (op == GREATER_THAN)
        return left > right;
    else if (op == LESS_THAN)
        return left < right;
    else if (op == GREATER_THAN_OR_EQUAL)
        return left >= right;
    else if (op == LESS_THAN_OR_EQUAL)
        return left <= right;
    else
        return false;
}


int count_line_indentation(const string &line)
{
    int indent = 0;
    if (line[indent] == '\t') {
        for (; line[indent] == '\t'; indent++) {}
        return indent;
    } else {
        for (; line[indent] == ' '; indent++) {}
        return indent;
    }
}


int scan_line(int start_pc, const string &search_for, int indent_num)
{
    while (start_pc < program_data.size()) {
        string &line_scan = program_data[start_pc];
        if (line_scan.find(search_for) != string::npos) {
            if (count_line_indentation(line_scan) == indent_num) { // found matching indent
                return start_pc;
            }
        }
        start_pc++;
    }
    return -1;
}


#ifdef DEBUG

int break_line = -1;

#endif



void run_program(int start_pc, int end_pc,
                 map<string, string> variable, map<string, vector<int>> array_val,
                 map<string, string> &prev_variable, map<string, vector<int>> &prev_array_val) {


    for ( ; start_pc <= end_pc; start_pc++) {

        string line = program_data[start_pc];
        string line_trimmed = ltrim_copy(line);

        if (line_trimmed.empty())
            continue;

        string op = split(line_trimmed, " ")[0];

#ifdef DEBUG

        if (break_line == -1 || start_pc == break_line) {

            if (start_pc == break_line)
                break_line = -1;

            cout << endl;
            cout << "-----------------------------" << endl;
            cout << endl;
            cout << "Commands:" << endl;
            cout << "\tn   = next line" << endl;
            cout << "\tpv  = print all variables" << endl;
            cout << "\tpa  = print all arrays" << endl;
            cout << "\tb N = break at line N" << endl;
            cout << "\tx N = display N (int or variable) as hexadecimal" << endl;
            cout << endl;

            while (true) {
                
                cout << "Statement: " << line_trimmed << endl;
                cout << "Current PC: " << start_pc << endl;
                cout << ">> ";
                string debug_cmd;
                getline(cin, debug_cmd);

                if (debug_cmd == "n") {

                    break;

                } else if (debug_cmd == "pv") {

                    cout << endl;
                    cout << "----> VARIABLES" << endl;
                    cout << endl;

                    for (auto const& x : variable) {
                        cout << '\t' << x.first << " = " << x.second << endl;
                    }

                    cout << endl;
                    cout << endl;

                } else if (debug_cmd == "pa") {

                    cout << endl;
                    cout << "----> ARRAY" << endl;
                    cout << endl;

                    for (auto const& x : array_val) {
                        cout << "\t" << x.first << " = {" << endl;
                        cout << "\t    ";
                        for (auto &ele : x.second) cout << ele << ", ";
                        cout << endl;
                        cout << "\t}" << endl;
                        cout << endl;
                    }

                    cout << endl;
                    cout << endl;

                } else if (debug_cmd[0] == 'b') {

                    int line = stol(split(debug_cmd, " ")[1], nullptr, 0) - 1;

                    if (line <= start_pc) {
                        cout << endl;
                        cout << "Error! Break line must be after current start_pc" << endl;
                        cout << endl;
                        continue;
                    }

                    break_line = line;

                    break;
                
                } else if (debug_cmd[0] == 'x') {

                    string val = split(debug_cmd, " ")[1];
                    int int_val = stol(is_number(val) ? val : variable[val], nullptr, 0);
                    
                    cout << endl;
                    cout << " --> 0x" << hex << int_val << endl;
                    cout << endl;

                } else {

                    cout << "Error! Command not found!" << endl;
                }
            }
        }

        


#endif

        /**
         * if-else statement
         */
        if (op == STARTIF) {

            int indent_num = count_line_indentation(line);

            string comp_statement = line_trimmed.substr(line_trimmed.find(" ") + 1, line_trimmed.length()-1);

            vector<string> comp_data = split(comp_statement, " ");
            int left = stol(is_number(comp_data[0]) ? comp_data[0] : variable[comp_data[0]], nullptr, 0);
            int right = stol(is_number(comp_data[2]) ? comp_data[2] : variable[comp_data[2]], nullptr, 0);
            bool comp_result = comparison(left, comp_data[1], right);

            // search for line that match
            int pc_afterif_scan = scan_line(start_pc, ELSEIF, indent_num);
            int pc_endif_scan = scan_line(start_pc, ENDIF, indent_num);

            // if else statement exist
            if (pc_afterif_scan != -1) {

                int if_start_pc = comp_result ? start_pc+1 : pc_afterif_scan+1;
                int if_end_pc = comp_result ? pc_afterif_scan-1 : pc_endif_scan-1;

                // recursively run sub-program
                run_program(if_start_pc, if_end_pc, variable, array_val, variable, array_val);

            // if no else statement and condition is true
            } else if (comp_result) {

                int if_start_pc = start_pc+1;
                int if_end_pc = pc_endif_scan-1;
                
                // recursively run sub-program
                run_program(if_start_pc, if_end_pc, variable, array_val, variable, array_val);
            }

            // adjust pc after running if-statement
            start_pc = pc_endif_scan;

        /**
         * while statement
         */
        } else if (op == STARTWHILE) {

            int indent_num = count_line_indentation(line);

            string comp_statement = line_trimmed.substr(line_trimmed.find(" ") + 1, line_trimmed.length()-1);
            
            vector<string> comp_data = split(comp_statement, " ");
            int left = stol(is_number(comp_data[0]) ? comp_data[0] : variable[comp_data[0]], nullptr, 0);
            int right = stol(is_number(comp_data[2]) ? comp_data[2] : variable[comp_data[2]], nullptr, 0);
            bool comp_result = comparison(left, comp_data[1], right);

            // search for line that match ENDIF
            int pc_endwhile_scan = scan_line(start_pc, ENDWHILE, indent_num);

            if (comp_result) {

                int if_start_pc = start_pc+1;
                int if_end_pc = pc_endwhile_scan-1;
                
                // recursively run sub-program
                run_program(if_start_pc, if_end_pc, variable, array_val, variable, array_val);
            }

            // adjust pc after running if-statement
            start_pc = comp_result ? start_pc-1 : pc_endwhile_scan;
        

        /**
         * Normal statements
         */
        } else {

            line = line_trimmed;

            // assign var with number
            if (op == ASSIGN_VAR) {

                string var1 = split(line, " ")[1];
                string var2 = line.substr(line.find(" ", line.find(" ") + 1) + 1, line.length()-1);

                // check if target is var or constant|string
                variable[var1] = (is_number(var2) || variable.find(var2) == variable.end()) ? var2 : variable[var2];
            }

            /**
             * INPUT/OUTPUT OPERATIONS
             */

            // input (string)
            else if (op == INPUT) {

                // get variable name
                string var_name = line.substr(line.find(" ") + 1, line.length()-1);

                string in;
                getline(cin, in);

                variable[var_name] = in;
            }

            // output
            else if (op == OUTPUT_FROM_VAR) {

                // get variable name
                string var_name = split(line, " ")[1];

                cout << variable[var_name];
            }

            // output (string)
            else if (op == OUTPUT_STRING) {

                // get variable name
                string var_name = line.substr(line.find(" ") + 1, line.length()-1);

                cout << var_name;
            }

            // output (newline)
            else if (op == OUTPUT_NEWLINE) {
                cout << endl;
            }

            /**
             * MATHEMATIC OPERATIONS
             */

            // addition
            else if (op == ADD) {

                vector<string> str_split = split(line, " ");
                
                string &var1 = str_split[1];
                string &var2 = str_split[2];

                int old_value = stol(variable[var1], nullptr, 0);
                old_value += stol(is_number(var2) ? var2 : variable[var2], nullptr, 0); // check if var2 is var or constant
                variable[var1] = to_string(old_value);
            }

            // subtraction
            else if (op == SUBTRACT) {

                vector<string> str_split = split(line, " ");
                
                string &var1 = str_split[1];
                string &var2 = str_split[2];

                int old_value = stol(variable[var1], nullptr, 0);
                old_value -= stol(is_number(var2) ? var2 : variable[var2], nullptr, 0); // check if var2 is var or constant
                variable[var1] = to_string(old_value);
            }

            // multiplication
            else if (op == MULTIPLY) {

                vector<string> str_split = split(line, " ");
                
                string &var1 = str_split[1];
                string &var2 = str_split[2];

                int old_value = stol(variable[var1], nullptr, 0);
                old_value *= stol(is_number(var2) ? var2 : variable[var2], nullptr, 0); // check if var2 is var or constant
                variable[var1] = to_string(old_value);
            }

            // division
            else if (op == DIVIDE) {

                vector<string> str_split = split(line, " ");
                
                string &var1 = str_split[1];
                string &var2 = str_split[2];

                int old_value = stol(variable[var1], nullptr, 0);
                old_value /= stol(is_number(var2) ? var2 : variable[var2], nullptr, 0); // check if var2 is var or constant
                variable[var1] = to_string(old_value);
            }

            // modulo
            else if (op == MODULO) {

                vector<string> str_split = split(line, " ");
                
                string &var1 = str_split[1];
                string &var2 = str_split[2];

                int old_value = stol(variable[var1], nullptr, 0);
                old_value %= stol(is_number(var2) ? var2 : variable[var2], nullptr, 0); // check if var2 is var or constant
                variable[var1] = to_string(old_value);
            }

            // increment by one
            else if (op == INCREMENT_BY_ONE) {
            
                // there is a bug,
                // if ran obfuscator, this split will throw garbage
                //string &var = split(line, " ")[1];
                //variable[var] = to_string( stol(variable[var], nullptr, 0) + 1 );
            
                string var = split(line, " ")[1];
                variable[var] = to_string( stol(variable[var], nullptr, 0) + 1 );
            
            }

            // decrement by one
            else if (op == DECREMENT_BY_ONE) {
                string &var = split(line, " ")[1];
                variable[var] = to_string( stol(variable[var], nullptr, 0) - 1 );
            }


            /**
             * BITWISE OPERATIONS
             */

            // and
            else if (op == AND) {

                vector<string> str_split = split(line, " ");
                
                string &var1 = str_split[1];
                string &var2 = str_split[2];

                int old_value = stol(variable[var1], nullptr, 0);
                old_value &= stol(is_number(var2) ? var2 : variable[var2], nullptr, 0); // check if var2 is var or constant
                variable[var1] = to_string(old_value);
            }

            // or
            else if (op == OR) {

                vector<string> str_split = split(line, " ");
                
                string &var1 = str_split[1];
                string &var2 = str_split[2];

                int old_value = stol(variable[var1], nullptr, 0);
                old_value |= stol(is_number(var2) ? var2 : variable[var2], nullptr, 0); // check if var2 is var or constant
                variable[var1] = to_string(old_value);
            }

            // xor
            else if (op == XOR) {

                vector<string> str_split = split(line, " ");
                
                string &var1 = str_split[1];
                string &var2 = str_split[2];

                int old_value = stol(variable[var1], nullptr, 0);
                old_value ^= stol(is_number(var2) ? var2 : variable[var2], nullptr, 0); // check if var2 is var or constant
                variable[var1] = to_string(old_value);
            }

            // complement
            else if (op == COMPLEMENT) {
                string &var = split(line, " ")[1];
                variable[var] = to_string( ~stol(variable[var], nullptr, 0) );
            }

            // shift_left
            else if (op == SHIFT_LEFT) {

                vector<string> str_split = split(line, " ");
                
                string &var1 = str_split[1];
                string &var2 = str_split[2];

                int old_value = stol(variable[var1], nullptr, 0);
                old_value <<= stol(is_number(var2) ? var2 : variable[var2], nullptr, 0); // check if var2 is var or constant
                variable[var1] = to_string(old_value);
            }

            // shift right
            else if (op == SHIFT_RIGHT) {

                vector<string> str_split = split(line, " ");
                
                string &var1 = str_split[1];
                string &var2 = str_split[2];

                int old_value = stol(variable[var1], nullptr, 0);
                old_value >>= stol(is_number(var2) ? var2 : variable[var2], nullptr, 0); // check if var2 is var or constant
                variable[var1] = to_string(old_value);
            }

            /**
             * ARRAY OPERATIONS
             */

            // define array
            else if (op == ARRAY_DEFINE) {

                // get array name
                string arr_name = split(line, " ")[1];

                vector<int> vec;
                array_val[arr_name] = vec;
            }

            // initialize array from ascii string
            else if (op == ARRAY_FROM_VAR_ASCII) {

                vector<string> data = split(line, " ");

                string &arr_name = data[1];
                string &from_var_ascii = data[2];

                for (int i = 0; i < variable[from_var_ascii].size(); i++) 
                    array_val[arr_name].push_back((int)variable[from_var_ascii][i]);
            }

            // push into array
            else if (op == ARRAY_PUSH) {

                // get array name
                vector<string> data = split(line, " ");

                string &arr_name = data[1];
                string &target = data[2];

                // check if target is constant
                array_val[arr_name].push_back(stol(is_number(target) ? target : variable[target], nullptr, 0));
            }

            // get size of array
            else if (op == ARRAY_GET_SIZE) {

                vector<string> data = split(line, " ");

                string &arr_name = data[1];
                string &target_variable = data[2];

                variable[target_variable] = to_string(array_val[arr_name].size());
            }

            // fetch data from array
            else if (op == ARRAY_ACCESS) {

                // get array name
                vector<string> data = split(line, " ");

                string &arr_name = data[1];
                int index_element = stol(is_number(data[2]) ? data[2] : variable[data[2]], nullptr, 0);
                string &target_variable = data[3];

                int data_fetched = array_val[arr_name][index_element];

                variable[target_variable] = to_string(data_fetched);
            }

            // modify array at i'th index
            else if (op == ARRAY_MODIFY) {

                vector<string> data = split(line, " ");

                string &arr_name = data[1];
                int index_element = stol(is_number(data[2]) ? data[2] : variable[data[2]], nullptr, 0);
                string &target = data[3];

                // check if target is var or constant
                array_val[arr_name][index_element] = stol(is_number(target) ? target : variable[target], nullptr, 0);

            }

            /**
             * RANDOM OPERATIONS
             */

            // set seed to mt19937
            else if (op == RANDOM_SET_SEED) {
                string var = split(line, " ")[1];
                int seed_val = stol(is_number(var) ? var : variable[var], nullptr, 0); // check if var2 is var or constant
                mt_rand.seed(seed_val);
            }

            // gen next random number
            else if (op == RANDOM_GEN) {
                
                string arr_name = split(line, " ")[1];

                int rand_val = mt_rand();

                array_val[arr_name].push_back((rand_val >> 24) & 0xff);
                array_val[arr_name].push_back((rand_val >> 16) & 0xff);
                array_val[arr_name].push_back((rand_val >>  8) & 0xff);
                array_val[arr_name].push_back((rand_val >>  0) & 0xff);
            }

            /**
             * MISC OPERATIONS
             */

            else if (op == EXIT) {
                exit(stol(split(line, " ")[1], nullptr, 0));
            }

        }
    }

    // reflect changes in current context to previous context

    for (auto &x : prev_variable)
        if (variable.find(x.first) != variable.end())
            prev_variable[x.first] = variable[x.first];

    for (auto &x : prev_array_val)
        if (array_val.find(x.first) != array_val.end())
            prev_array_val[x.first] = array_val[x.first];


}


int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << endl;
        cout << "ERROR!: No script found!" << endl;
        cout << endl << "Example:" << endl;
        cout << argv[0] << " script_file" << endl;
        cout << endl;
        return 0;
    }

#ifdef DEBUG

    cout << endl;
    cout << "---- DEBUG MODE ----" << endl;
    cout << endl;
    cout << endl;
    cout << "Good luck!" << endl;
    cout << endl;

#endif

    // read program
    ifstream program_codes(argv[1]);
    
    string line;
    while (getline(program_codes, line)) {
        program_data.push_back(line);
    }

    // hold variable name=>value
    map<string, string> variable;

    // hold array arrayname=>array
    map<string, vector<int>> array_val;

    // run our program
    run_program(0, program_data.size()-1, variable, array_val, variable, array_val);

    return 0;
}
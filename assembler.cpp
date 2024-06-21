#include <bits/stdc++.h>
#include "tables.hpp"
#define sz(x) int(x.size())
using namespace std;

ifstream fin;
ofstream fout, fout2;
string label, opcode, operand, line, current_block = "DEFAULT",error="";
int line_number, LOCCTR, save_LOCCTR, current_block_number, current_number_of_blocks = 1, program_length;
vector<pair<int, string>> block_mapping;

int precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}
int applyOp(int a, int b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    }
    return -1;
}
int get_value(string s)
{
    int i;
    stack<int> values;
    stack<char> ops;
    for (i = 0; i < s.length(); i++)
    {
        if (s[i] == ' ')
            continue;
        else if (s[i] == '(')
        {
            ops.push(s[i]);
        }
        else if (isdigit(s[i]))
        {
            int val = 0;
            while (i < s.length() &&
                   isdigit(s[i]))
            {
                val = (val * 10) + (s[i] - '0');
                i++;
            }

            values.push(val);
            i--;
        }
        else if (s[i] == ')')
        {
            while (!ops.empty() && ops.top() != '(')
            {
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }
            if (!ops.empty())
                ops.pop();
        }

        else
        {
            while (!ops.empty() && precedence(ops.top()) >= precedence(s[i]))
            {
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }
            ops.push(s[i]);
        }
    }
    while (!ops.empty())
    {
        int val2 = values.top();
        values.pop();

        int val1 = values.top();
        values.pop();

        char op = ops.top();
        ops.pop();

        values.push(applyOp(val1, val2, op));
    }
    return values.top();
}
int evaluate_expression(string s, bool chk = 0)
{
    if (s == "*")
        return LOCCTR;

    vector<string> s1;
    vector<char> s2;

    if (sz(s) <= 1)
        return -1;

    if (s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/')
    {
        s2.push_back(s[0]);
        s = s.substr(1);
    }

    int n = sz(s);
    string operand = "";
    for (int i = 0; i < n; i++)
    {
        if (!(s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/'))
        {
            operand += s[i];
        }
        else
        {
            s1.push_back(operand);
            operand = "";

            s2.push_back(s[i]);
        }
    }
    if (int(operand.size()))
        s1.push_back(operand);
    int value;
    if (sz(s1) == sz(s2))
    {
        int plus = 0, minus = 0;
        if (SYMTAB.find(s1[0]) != SYMTAB.end() && SYMTAB[s1[0]].relative == 1)
            if (s2[0] == '+')
                plus++;
            else if (s2[0] == '-')
                minus++;
            else
                return -1;
        for (int i = 1; i < sz(s2); i++)
        {
            if (s2[i] == '+')
            {
                if (SYMTAB.find(s1[i]) != SYMTAB.end() && SYMTAB[s1[i]].relative == 1)
                    plus++;
            }
            else if (s2[i] == '-')
            {
                if (SYMTAB.find(s1[i]) != SYMTAB.end() && SYMTAB[s1[i]].relative == 1)
                    minus++;
            }
            else if (s2[i] == '*' || s2[i] == '/')
            {
                if (SYMTAB.find(s1[i]) != SYMTAB.end() && SYMTAB[s1[i]].relative == 1 || SYMTAB.find(s1[i - 1]) != SYMTAB.end() && SYMTAB[s1[i - 1]].relative == 1)
                {
                    return -1;
                }
            }
        }
        if (plus - minus >= 2 || plus - minus < 0)
            return -1;
        if (plus - minus == 0)
            SYMTAB[label].relative = 0;
        vector<string> x;
        for (int i = 0; i < sz(s1); i++)
        {
            string ans;
            if (SYMTAB.find(s1[i]) != SYMTAB.end())
            {
                int temp = SYMTAB[s1[i]].address;
                if (chk == 1)
                    temp += BLOCKTAB[block_mapping[SYMTAB[s1[i]].block].second].start;
                ans = to_string(temp);
            }
            else
                ans = s1[i];
            x.push_back(ans);
        }
        string f = "0";
        for (int i = 0; i < sz(x); i++)
            f += s2[i] + x[i];
        value = get_value(f);
    }
    else
    {
        int plus = 0, minus = 0;
        if (SYMTAB.find(s1[0]) != SYMTAB.end() && SYMTAB[s1[0]].relative == 1)
            plus++;
        for (int i = 0; i < sz(s2); i++)
        {
            if (s2[i] == '+')
            {
                if (SYMTAB.find(s1[i + 1]) != SYMTAB.end() && SYMTAB[s1[i + 1]].relative == 1)
                    plus++;
            }

            else if (s2[i] == '-')
            {
                if (SYMTAB.find(s1[i + 1]) != SYMTAB.end() && SYMTAB[s1[i + 1]].relative == 1)
                    minus++;
            }

            else if (s2[i] == '*' || s2[i] == '/')
            {
                if (SYMTAB.find(s1[i]) != SYMTAB.end() && SYMTAB[s1[i]].relative == 1 || SYMTAB.find(s1[i + 1]) != SYMTAB.end() && SYMTAB[s1[i + 1]].relative == 1)
                {
                    return -1;
                }
            }
        }

        if (plus - minus >= 2 || plus - minus < 0)
            return -1;

        if (plus == minus)
            SYMTAB[label].relative = 0;

        vector<string> x;
        for (int i = 0; i < sz(s1); i++)
        {
            string ans;
            if (SYMTAB.find(s1[i]) != SYMTAB.end())
            {
                int temp = SYMTAB[s1[i]].address;
                if (chk == 1)
                    temp += BLOCKTAB[block_mapping[SYMTAB[s1[i]].block].second].start;
                ans = to_string(temp);
            }
            else
                ans = s1[i];
            x.push_back(ans);
        }

        string f = "";
        for (int i = 0; i < sz(s2); i++)
            f += x[i] + s2[i];
        f += x.back();
        value = get_value(f);
    }
    return value;
}
string intToHex(int num, int x = 3)
{
    string hex = "0123456789ABCDEF";
    string result = "";
    for (int i = sizeof(num) * 2 - 1; i >= 0; i--)
    {
        int x = (num >> (i * 4)) & 0xF;
        result += hex[x];
    }
    return result.substr(x);
}
string modified(string s, char c = ' ', int x = 6)
{
    int len = sz(s);
    string temp = s + string(x - len, c);
    return temp;
}
void print_line(int l1 = LOCCTR, int l2 = current_block_number, string l3 = label, string l4 = opcode, string l5 = operand)
{

    if (l3 == "")
        l3 = "-";

    if (l5 == "")
        l5 = "-";

    if (l1 != -1)
        fout<<modified(intToHex(l1)) << " " << l2 << " " << modified(l3) << " " << modified(l4) << " " << l5 << endl;
    else
        fout<<modified("-") << " " << l2 << " " << modified(l3) << " " << modified(l4) << " " << l5 << endl;
}
bool is_comment_line(string &s)
{
    if (sz(s) == 0)
        return 1;

    int i = 0;
    while (i < sz(s))
    {
        if (s[i] != ' ')
            break;
        i++;
    }
    if (s[i] == '.' || i == sz(s))
        return 1;
    return 0;
}
void capitalize(string &s)
{
    for (auto &c : s)
        if (c >= 'a' && c <= 'z')
            c += 'A' - 'a';
}
void initialize_segments(string &s)
{
    string temp = "";
    operand = "", label = "", opcode = "";
    int len = sz(s), i = 0;
    vector<string> x;
    while (i < len)
    {
        if (s[i] != ' ')
            temp += s[i];
        else if (sz(temp))
        {
            x.push_back(temp);
            temp = "";
        }
        i++;
    }
    if (temp != "")
        x.push_back(temp);
    for (auto &sx : x)
        capitalize(sx);

    if (OPTAB.find(x[0]) != OPTAB.end() || ASMDIR.find(x[0]) != ASMDIR.end())
    {
        opcode = x[0];
        string p = "";
        for (int i = 1; i < sz(x); i++)
            p += x[i] + " ";
        if (p != "")
        {
            p.pop_back();
            operand = p;
        }
        else
            operand = "";
        label = "";
    }
    else
    {
        label = x[0];
        if (OPTAB.find(x[1]) != OPTAB.end() || ASMDIR.find(x[1]) != ASMDIR.end())
        {
            opcode = x[1];
            string p = "";
            for (int i = 2; i < sz(x); i++)
                p += x[i] + " ";
            p.pop_back();
            operand = p;
        } 
        else
        {
            error+="Line Number "+to_string(line_number)+": Invalid Instruction\n";
        }
    }
}
int stringhex_to_int(string &s)
{
    int ans = stoi(s, 0, 16);
    return ans;
}
bool check_literal_format(string &s)
{
    if(s=="=*")
        return 1;

    char c1 = s[0], c2 = s[1], c3 = s[2], c4 = s.back();

    if (c1 == '=' && (c2 == 'C' || c2 == 'X') && c3 == '\'' && c4 == '\'')
        return 1;
    return 0;
}
bool is_valid_byte_sequence(string &s)
{
    char c = s[0];
    if (sz(s) <= 3)
        return 0;

    for (int i = 2; i < sz(s) - 1; i++)
    {
        if (c == 'C' && (!(s[i] >= 0 && s[i] <= 255)))
            return 0;

        if (c == 'X' && (!(s[i] >= '0' && s[i] <= '9' || s[i] >= 'A' && s[i] <= 'F')))
            return 0;
    }
    if (c == 'X' && (sz(s) - 3) % 2 != 0)
        return 0;
    return 1;
}
int evaluate_byte_sequence(string &s)
{
    if (s == "*")
        return LOCCTR;
    char c = s[0];
    int value = 0;
    int len = sz(s);
    if (c == 'C')
    {
        int x = 1;
        for (int i = len - 2; i >= 2; i--)
        {
            value += int(s[i]) * x;
            x *= 256;
        }
    }
    else if (c == 'X')
    {
        int x = 1;
        for (int i = len - 2; i >= 2; i--)
        {
            int current = (s[i] >= '0' && s[i] <= '9') ? s[i] - '0' : s[i] - 'A';
            value += current * x;
            x *= 16;
        }
    }
    return value;
}
void handle_ltorg()
{
    for (auto &literal : LITTAB)
    {
        if (literal.second.unassigned == 0)
            continue;

        literal.second.address.insert(LOCCTR);
        literal.second.blocknumber = current_block_number;
        literal.second.unassigned = 0;

        print_line(LOCCTR, current_block_number, "", "*", literal.first);
        LOCCTR += literal.second.length;
        line_number++;
    }
}
void modify_blocktab()
{
    for (auto block : BLOCKTAB)
        block_mapping.push_back({block.second.id, block.first});
    sort(block_mapping.begin(), block_mapping.end());

    for (auto &x : block_mapping)
    {
        if (x.first == 0)
            continue;

        BLOCKTAB[x.second].start = BLOCKTAB[block_mapping[x.first - 1].second].start + BLOCKTAB[block_mapping[x.first - 1].second].length;
    }

    program_length = BLOCKTAB[(block_mapping.back()).second].start + BLOCKTAB[(block_mapping.back()).second].length;
}
int get_length_for_LOCCTR()
{
    string x = intToHex(LOCCTR);
    reverse(x.begin(),x.end());
    int len = sz(x);
    int i;
    for (i = 0; i < len; i++)
    {
        if (x[i] == '0')
            break;
    }
    return (i + 2) / 2;
}
class TextRecord
{
    vector<string> v;
    int curr_len, curr_start;
    string curr_string;

public:
    TextRecord()
    {
        curr_len = 0;
        curr_start = -1;
        curr_string = "";
    }

    void insert(string s, int address)
    {
        if (curr_len + int(s.size()) <= 60)
        {
            curr_len += int(s.size());
            curr_string += s + "^";

            if (curr_start == -1)
                curr_start = address;
        }
        else
        {
            v.push_back("T^" + intToHex(curr_start, 2) + "^" + intToHex(curr_len / 2, 6) + "^" + curr_string.substr(0, int(curr_string.size()) - 1));
            curr_len = int(s.size());
            curr_string = s + "^";
            curr_start = address;
        }
    }
    void break_text_record()
    {
        if (curr_len == 0)
            return;

        if (int(curr_string.size()))
        {
            v.push_back("T^" + intToHex(curr_start, 2) + "^" + intToHex(curr_len / 2, 6) + "^" + curr_string.substr(0, int(curr_string.size()) - 1));
            curr_len = 0;
            curr_start = -1;
            curr_string = "";
        }
    }
    void print_text_records(ofstream &fout)
    {
        if (curr_len)
        {
            v.push_back("T^" + intToHex(curr_start, 2) + "^" + intToHex(curr_len / 2, 6) + "^" + curr_string.substr(0, int(curr_string.size()) - 1));
        }

        for (auto &s : v)
            fout << s << endl;
    }
};

/*PASS1*/
void pass1(string ifile, string ofile)
{

    fin.open(ifile);
    if (!fin.is_open())
    {
        cout << "Error opening the file:" << ifile << endl;
        return;
    }

    fout.open(ofile);
    if (!fout.is_open())
    {
        cout << "Error opening the file:" << ofile << endl;
        return;
    }

    line_number=1;
    getline(fin, line);
    while (is_comment_line(line))
        getline(fin, line);

    error+="PASS1 Errors:\n";
    initialize_segments(line);
    if (opcode == "START")
    {
        int start_address = stringhex_to_int(operand);
        LOCCTR = start_address;
    }
    print_line();

    line_number++;
    while (opcode != "END")
    {
        getline(fin, line);
        if (is_comment_line(line))
            continue;

        initialize_segments(line);
        if (sz(label) != 0)
        {
            if (SYMTAB.find(label) != SYMTAB.end())
                SYMTAB[label].error = 1;
            else
                SYMTAB[label].error = 0;
            SYMTAB[label].block = current_block_number;
            SYMTAB[label].address = LOCCTR;
        }

        if (opcode == "LTORG" || opcode == "ORG" || opcode == "EQU" || opcode == "USE" || opcode == "BASE" || opcode == "NOBASE")
            print_line(-1);
        else
            print_line();

        if (check_literal_format(operand))
        {
            string name = operand.substr(1);
            int len;
            if (!(is_valid_byte_sequence(name)||name=="*"))
                error+="Line Number "+to_string(line_number)+": Invalid Byte-Sequence\n";
            else if (name[0] == 'X')
                len = (sz(name) - 3) / 2;
            else if (name[0] == 'C')
                len = sz(name) - 3;
            else if (name == "*")
            {
                len = get_length_for_LOCCTR();
                name += intToHex(LOCCTR);
            }
            if (LITTAB.find(name) != LITTAB.end() && LITTAB[name].blocknumber == current_block_number)
                LITTAB[name].error = 1;
            else
                LITTAB[name].error = 0;
            LITTAB[name].length = len;
            LITTAB[name].unassigned = 1;
            LITTAB[name].operand_value = evaluate_byte_sequence(name);
            LITTAB[name].blocknumber = -1;
        }

        if (opcode == "RESB")
            LOCCTR += stoi(operand);
        else if (opcode == "RESW")
            LOCCTR += 3 * stoi(operand);
        else if (opcode == "WORD")
            LOCCTR += 3;
        else if (opcode == "BYTE")
        {
            char c = operand[0];
            if (!is_valid_byte_sequence(operand))
                error+="Line Number "+to_string(line_number)+": Invalid Byte-Sequence\n";
            else if (c == 'X')
                LOCCTR += (sz(operand) - 3) / 2;
            else if (c == 'C')
                LOCCTR += sz(operand) - 3;
        }

        else if (OPTAB.find(opcode) != OPTAB.end())
            LOCCTR += OPTAB[opcode].format;

        else if (opcode == "LTORG")
            handle_ltorg();
        else if (opcode == "EQU")
        {
            int val = evaluate_expression(operand, 0);
            if (val == -1)
                error+="Line Number "+to_string(line_number)+": EQU is used, but all required labels were not found.\n";
            
            else
                SYMTAB[label].address = val;
        }
        else if (opcode == "ORG")
        {
            if (sz(operand) != 0)
            {
                int val = evaluate_expression(operand);
                if (val == -1)
                    error+="Line Number "+to_string(line_number)+": EQU is used, but all required labels were not found.\n";
                else
                {
                    save_LOCCTR = LOCCTR;
                    LOCCTR = val;
                }
            }
            else
                LOCCTR = save_LOCCTR;
        }
        else if (opcode == "USE")
        {
            BLOCKTAB[current_block].length = LOCCTR;
            if (sz(operand) == 0)
            {
                current_block = "DEFAULT";
                current_block_number = BLOCKTAB["DEFAULT"].id;
                LOCCTR = BLOCKTAB["DEFAULT"].length;
            }
            else
            {
                current_block = operand;
                if (BLOCKTAB.find(operand) == BLOCKTAB.end())
                {
                    current_block_number = current_number_of_blocks;
                    BLOCKTAB[operand].id = current_number_of_blocks++;
                    LOCCTR = 0;
                }
                else
                {
                    current_block_number = BLOCKTAB[operand].id;
                    LOCCTR = BLOCKTAB[operand].length;
                }
            }
        }
        line_number++;
    }
    handle_ltorg();
    BLOCKTAB[current_block].length = LOCCTR;
    modify_blocktab();
    fin.close();
    fout.close();
}

void print_line_v2(int l1 = LOCCTR, int l2 = current_block_number, string l3 = label, string l4 = opcode, string l5 = operand, string l6 = "")
{

    if (l3 == "-")
        l3 = "";

    if (l5 == "-")
        l5 = "";

    if (l1 != -1)
        fout2 << modified(intToHex(l1)) << "\t" << l2 << "\t" << modified(l3) << "\t" << modified(l4) << "\t" << modified(l5, ' ', 40) << "\t" << l6 << endl;
    else
        fout2 << modified("") << "\t" << l2 << "\t" << modified(l3) << "\t" << modified(l4) << "\t" << modified(l5, ' ', 40) << l6 << endl;
}
void initialize_segments_v2(string &s)
{
    string temp = "";
    operand = "", label = "", opcode = "";
    int len = sz(s), i = 0;
    vector<string> x;
    while (i < len)
    {
        if (s[i] != ' ')
            temp += s[i];
        else if (sz(temp))
        {
            x.push_back(temp);
            temp = "";
        }
        i++;
    }
    if (temp != "")
        x.push_back(temp);

    bool f = 1;
    for (auto temp : x[0])
        if (!(temp >= '0' && temp <= '9' || temp >= 'A' && temp <= 'F'))
            f = 0;

    if (f == 1)
        LOCCTR = stringhex_to_int(x[0]);
    else
        LOCCTR = -1;
    current_block_number = stoi(x[1]);
    label = x[2];
    opcode = x[3];
    operand = "";
    for (i = 4; i < sz(x); i++)
        operand += x[i];
}
pair<int, int> are_valid_registers(string s)
{
    string r1 = "", r2 = "";

    r1 += s[0];
    if (REGTAB.find(r1) == REGTAB.end())
        return {-1, -1};

    if (sz(s) >= 3)
    {
        r2 += s[2];
        if (REGTAB.find(r2) == REGTAB.end())
            return {-1, -1};
    }
    else
        r2 = "A";

    return {REGTAB[r1], REGTAB[r2]};
}
string validate_memory_reference(string s)
{
    char c = s[0];
    // debug(c);
    if (c == '#' || c == '@')
    {
        s = s.substr(1);
        // debug(s);
        if (SYMTAB.find(s) != SYMTAB.end())
        {
            if (c == '#')
                return s + "I";
            return s + "D";
        }
        return "-1";
    }
    else
    {
        int comma = 0;
        int len = sz(s);
        for (; comma < len; comma++)
            if (s[comma] == ',')
                break;
        if (comma == len)
            if (SYMTAB.find(s) != SYMTAB.end())
                return s + "S";
            else
                return "-1";

        string s1 = s.substr(0, comma);
        string s2 = s.substr(comma + 1);
        if (SYMTAB.find(s1) != SYMTAB.end())
            if (s2 == "X")
                return s1 + "X";
        return "-1";
    }
}
string validate_literal_reference(string s)
{
    char c = s[0];
    if (c != '=')
        return "-1";

    if(sz(s)<=1)
        return "-1";

    if (s[0] == '=' && s[1] == '*' && sz(s)==2)
        return "*";

    s = s.substr(1);
    c = s[0];
    if (sz(s) <= 3)
        return "-1";
    if (!(s[1] == '\'' && s.back() == '\''))
        return "-1";
    if (!(s[0] == 'X' || s[0] == 'C'))
        return "-1";

    if (c == 'X')
    {
        for (int i = 2; i < sz(s) - 1; i++)
            if (!(s[i] >= '0' && s[i] <= '9' || s[i] >= 'A' && s[i] <= 'F'))
                return "-1";
    }

    return s;
}
string validate_immediate_usage(string s)
{
    char c = s[0];
    if (c == '#')
    {
        bool f = 1;
        for (int i = 1; i < sz(s); i++)
            if (!(s[i] >= '0' && s[i] <= '9'))
                f = 0;

        if (f)
            return s.substr(1);
    }
    return "-1";
}

/*PASS2*/
void pass2(string inter, string out, string out2)
{
    string h, e;
    TextRecord t;
    vector<string> m;

    fin.open(inter);
    if (!fin.is_open())
    {
        cout << "Error opening the file:" << inter << endl;
        return;
    }

    fout.open(out);
    if (!fout.is_open())
    {
        cout << "Error opening the file:" << out << endl;
        return;
    }

    fout2.open(out2);
    if (!fout2.is_open())
    {
        cout << "Error opening the file:" << out2 << endl;
        return;
    }

    line_number=1;
    error+="\nPASS2 Errors:\n";
    getline(fin, line);
    initialize_segments_v2(line);
    int start = LOCCTR;
    if (opcode == "START")
        h = "H^" + modified(label, '_') + "^" + intToHex(stringhex_to_int(operand), 2) + "^" + intToHex(program_length - start, 2);
    print_line_v2(LOCCTR, current_block_number, label, opcode, operand, "");
    line_number++;start=-1;
    while (getline(fin, line))
    {
        initialize_segments_v2(line);

        if(start==-1&&OPTAB.find(opcode)!=OPTAB.end())
            start=LOCCTR;
        
        if (OPTAB.find(opcode) != OPTAB.end())
        {
            string temp = "";
            if (OPTAB[opcode].format == 1)
                temp = OPTAB[opcode].opcode;
            else if (OPTAB[opcode].format == 2)
            {
                temp = OPTAB[opcode].opcode;
                auto check = are_valid_registers(operand);
                if (check == make_pair(-1, -1))
                    error+="Line Number "+to_string(line_number)+": Invalid Registers\n";
                string s1 = to_string(check.first);
                string s2 = to_string(check.second);
                temp += s1 + s2;
            }
            else if (OPTAB[opcode].format == 3)
            {
                if (opcode == "RSUB")
                    temp = "4F0000";

                else
                {
                    auto check1 = validate_memory_reference(operand);
                    auto check2 = validate_immediate_usage(operand);
                    auto check3 = validate_literal_reference(operand);

                    if (check1 != "-1")
                    {
                        char c = check1.back();
                        int len = sz(check1);
                        int n, i, x = 0, e = 0, p, b;
                        if (c == 'S')
                            n = 1, i = 1;
                        else if (c == 'I')
                            n = 0, i = 1;
                        else if (c == 'D')
                            n = 1, i = 0;
                        else
                            n = 1, i = 1, x = 1;

                        string f = check1.substr(0, len - 1);
                        int location = SYMTAB[f].address + BLOCKTAB[block_mapping[SYMTAB[f].block].second].start;
                        int disp = location - (LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start + OPTAB[opcode].format);
                        if (disp >= -2048 && disp <= 2047)
                            p = 1, b = 0;
                        else
                        {
                            if (!used_BASE)
                                error+="Line Number "+to_string(line_number)+": BASE is not used when required\n";
                            else
                            {
                                disp = disp + LOCCTR - base + OPTAB[opcode].format;
                                if (disp >= 0 && disp <= 4095)
                                    b = 1, p = 0;
                                else
                                    error+="Line Number "+to_string(line_number)+": Invalid Displacement\n";
                            }
                        }
                        int third = x * 8 + b * 4 + p * 2 + e;
                        int first_plus_second = stringhex_to_int(OPTAB[opcode].opcode) + 2 * n + i;
                        temp = intToHex(first_plus_second, 6) + intToHex(third, 7) + intToHex(disp, 5);
                    }
                    else if (check2 != "-1")
                    {
                        int n = 0, i = 1, x = 0, b = 0, p = 0, e = 0;
                        int disp = stoi(check2);

                        if(!(disp>=-2048&&disp<=2047))
                            error+="Line Number "+to_string(line_number)+": Displacement field for Format 3 is not in the range\n";
                        int third = x * 8 + b * 4 + p * 2 + e;
                        int first_plus_second = stringhex_to_int(OPTAB[opcode].opcode) + 2 * n + i;
                        temp = intToHex(first_plus_second, 6) + intToHex(third, 7) + intToHex(disp, 5);
                    }
                    else if (check3 != "-1")
                    {
                        int n = 1, i = 1, x = 0, e = 0, p, b;
                        int disp;
                        if (check3[0] == '*')
                            check3 += intToHex(LOCCTR);

                        string f = check3;
                        int location = *((LITTAB[f].address).lower_bound(LOCCTR)) + BLOCKTAB[block_mapping[LITTAB[f].blocknumber].second].start;
                        disp = location - (LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start + OPTAB[opcode].format);
                        if (disp >= -2048 && disp <= 2047)
                            p = 1, b = 0;
                        else
                        {
                            if (!used_BASE)
                                error+="Line Number "+to_string(line_number)+": BASE is not used when required\n";
                            else
                            {
                                disp = disp + LOCCTR - base + OPTAB[opcode].format;
                                if (disp >= 0 && disp <= 4095)
                                    b = 1, p = 0;
                                else
                                    error+="Line Number "+to_string(line_number)+": Invalid Displacement\n";
                            }
                        }
                        int third = x * 8 + b * 4 + p * 2 + e;
                        int first_plus_second = stringhex_to_int(OPTAB[opcode].opcode) + 2 * n + i;
                        temp = intToHex(first_plus_second, 6) + intToHex(third, 7) + intToHex(disp, 5);
                    }

                    else
                       error+="Line Number "+to_string(line_number)+": Invalid operand for format 3\n";
                }
            }

            else
            {
                auto check1 = validate_memory_reference(operand);
                auto check2 = validate_immediate_usage(operand);
                auto check3 = validate_literal_reference(operand);
                if (check1 != "-1")
                {
                    char c = check1.back();
                    int len = sz(check1);
                    int n, i, x = 0, e = 1, p = 0, b = 0;
                    if (c == 'S')
                        n = 1, i = 1;
                    else if (c == 'I')
                        n = 0, i = 1;
                    else if (c == 'D')
                        n = 1, i = 0;
                    else
                        n = 1, i = 1, x = 1;
                    string f = check1.substr(0, len - 1);
                    int value = SYMTAB[f].address;
                    int third = x * 8 + b * 4 + p * 2 + e;
                    int first_plus_second = stringhex_to_int(OPTAB[opcode].opcode) + 2 * n + i;

                    if (SYMTAB[f].relative == 0)
                        temp = intToHex(first_plus_second, 6) + intToHex(third, 7) + intToHex(value);
                    else
                    {
                        temp = intToHex(first_plus_second, 6) + intToHex(third, 7) + intToHex(value + BLOCKTAB[block_mapping[SYMTAB[f].block].second].start);
                        m.push_back("M^" + intToHex(LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start + 1, 2) + "^05");
                    }
                }
                else if (check2 != "-1")
                {
                    int n = 0, i = 1, x = 0, b = 0, p = 0, e = 1;
                    int disp = stoi(check2);
                    if(!(disp>=0&&disp<(1<<20)))
                            error+="Line Number "+to_string(line_number)+": Address field for Format 4 is not in the range\n";
                    int third = x * 8 + b * 4 + p * 2 + e;
                    int first_plus_second = stringhex_to_int(OPTAB[opcode].opcode) + 2 * n + i;
                    temp = intToHex(first_plus_second, 6) + intToHex(third, 7) + intToHex(disp, 3);
                }

                else if (check3 != "-1")
                {
                    int n = 1, i = 1, x = 0, e = 1, p, b;
                    if (check3 == "*")
                        check3 += to_string(LOCCTR);

                    string f = check3;
                    int location = *((LITTAB[f].address).lower_bound(LOCCTR)) + BLOCKTAB[block_mapping[LITTAB[f].blocknumber].second].start;
                    int third = x * 8 + b * 4 + p * 2 + e;
                    int first_plus_second = stringhex_to_int(OPTAB[opcode].opcode) + 2 * n + i;
                    temp = intToHex(first_plus_second, 6) + intToHex(third, 7) + intToHex(location, 3);
                    m.push_back("M^" + intToHex(LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start + 1, 2) + "^05");
                }
            }
            t.insert(temp, LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start);
            print_line_v2(LOCCTR, current_block_number, label, opcode, operand, temp);
        }

        else if (opcode == "RESB" || opcode == "RESW")
        {
            t.break_text_record();
            print_line_v2(LOCCTR, current_block_number, label, opcode, operand, "");
        }

        else if (opcode == "WORD")
        {
            string temp = intToHex(stoi(operand), 2);
            t.insert(temp, LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start);
            print_line_v2(LOCCTR, current_block_number, label, opcode, operand, temp);
        }

        else if (opcode == "BYTE")
        {
            char c = operand[0];
            string temp;
            if (c == 'X')
                temp = operand.substr(2, sz(operand) - 3);
            else if (c == 'C')
            {
                string temp2 = operand.substr(2, sz(operand) - 3);
                for (auto &c : temp2)
                {
                    int x = c;
                    temp += intToHex(x, 6);
                }
            }
            t.insert(temp, LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start);
            print_line_v2(LOCCTR, current_block_number, label, opcode, operand, temp);
        }

        else if (opcode == "LTORG")
            print_line_v2(LOCCTR, current_block_number, label, opcode, operand, "");

        else if (opcode == "EQU")
        {
            int val = evaluate_expression(operand, 1);
            if(val!=-1){
            SYMTAB[label].address = val;
            print_line_v2(LOCCTR, current_block_number, label, opcode, operand, "");
            }
        }

        else if (opcode == "ORG" || opcode == "USE")
        {
            print_line_v2(LOCCTR, current_block_number, label, opcode, operand, "");
            t.break_text_record();
        }

        else if (opcode == "BASE")
        {
            used_BASE = 1;
            base = SYMTAB[operand].address;
            print_line_v2(-1, current_block_number, label, opcode, operand, "");
        }

        else if (opcode == "NOBASE")
        {
            used_BASE = 0;
            print_line_v2(-1, current_block_number, label, opcode, operand, "");
        }

        else if (opcode == "END")
            print_line_v2(-1, current_block_number, label, opcode, operand, "");

        else if (opcode == "*")
        {
            int cnt;
            string temp;
            if(operand[0]=='C')
                cnt = 2*sz(operand) - 6;
            if (operand[0] == 'X')
                cnt = sz(operand) - 3;
            if(operand[0] == '*'){
                int i;
                for(i=1;i<sz(operand);i++)
                    if(operand[i]!='0')
                    break;
                cnt=max(1,(6-i)/2);
                if(cnt%2!=0)
                cnt++;
            }
            temp = intToHex(evaluate_byte_sequence(operand), 8 - cnt);
            print_line_v2(LOCCTR, current_block_number, label, opcode, operand, temp);
            t.insert(temp, LOCCTR + BLOCKTAB[block_mapping[current_block_number].second].start);
        }
        line_number++;
    }
    e = "E^" + intToHex(start, 2);
    fout << h << endl;
    t.print_text_records(fout);
    for (auto &md : m)
        fout << md << endl;
    fout << e << endl;

    fout2<<"\n/**********************************************************************************/\nERRORS in the SIC/XE Program:\n"<<endl;
    fout2<<error<<endl;
}

void assemble(string input="program.txt"){
    load_BLOCKS();
    load_OPTAB();
    pass1(input, "intermediate_file.txt");
    pass2("intermediate_file.txt", "obj_program.txt", "assembly_listing.txt");
}

int main()
{
    assemble();
    return 0;
}
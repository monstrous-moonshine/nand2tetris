#include <cctype>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>
struct Assem {
    void operator()(FILE *in) {
        char *buf = NULL;
        size_t sz = 0;
        while (getline(&buf, &sz, in) != -1) {
            do_line(buf);
        }
        free(buf);
        if (!pending.empty()) {
            fprintf(stderr, "Undefined label\n");
            exit(1);
        }
        for (size_t i = 0; i < out.size(); i++) {
            hexprint(out[i] >> 8);
            hexprint(out[i] & 0xff);
            fputs("\n", stdout);
        }
    }
private:
    void do_line(char *buf) {
        char *tok = strtok(buf, " \t\n");
        if (tok[0] == '@') {
            tok++;
            if (isdigit(tok[0])) {
                push(atoi(tok));
            } else if (islower(tok[0])) {
                auto it = var_map.find(tok);
                if (it == var_map.end()) {
                    var_map.emplace(tok, var_off);
                    push(var_off);
                    var_off++;
                } else {
                    push(it->second);
                }
            } else if (isupper(tok[0])) {
                auto it = label_map.find(tok);
                if (it == label_map.end()) {
                    pending[tok].push_back(out.size());
                    push(0);
                } else {
                    push(it->second);
                }
            } else {
                fprintf(stderr, "Invalid instruction\n");
                exit(1);
            }
        } else if (tok[0] == '(') {
            tok = strtok(tok + 1, ")");
            label_map.emplace(tok, out.size());
            auto it = pending.find(tok);
            if (it != pending.end()) {
                for (auto pos : it->second) {
                    insert(out.size(), pos);
                }
                pending.erase(it);
            }
        } else if (tok[0] == '/') {
            // comment
        } else {
            char *eq = strchr(tok, '=');
            char *sc = strchr(tok, ';');
            char *dst = eq ? tok : NULL;
            char *cmp = eq ? eq + 1 : tok;
            char *jmp = sc ? sc + 1 : NULL;
            if (eq) *eq = 0;
            if (sc) *sc = 0;
            int code = 0xe000;
            if (dst) {
                if (strchr(dst, 'A')) code |= 1 << 5;
                if (strchr(dst, 'D')) code |= 1 << 4;
                if (strchr(dst, 'M')) code |= 1 << 3;
            }
            if (jmp) {
                const char *const jinstr[] = {
                    "", "JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP",
                };
                bool found = false;
                for (int i = 1; i< 8; i++) {
                    if (strncmp(jmp, jinstr[i], 3) == 0) {
                        code |= i;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    fprintf(stderr, "Invalid instruction\n");
                    exit(1);
                }
            }
            if (char *is_mem = strchr(cmp, 'M')) {
                *is_mem = 'A';
                code |= 1 << 12;
            }
            const char *const cinstr[] = {
                "0", "1", "-1", "D", "A", "!D", "!A", "-D", "-A",
                "D+1", "A+1", "D-1", "A-1", "D+A", "D-A", "A-D",
                "D&A", "D|A",
            };
            int ccode[] = {
                0b101010, 0b111111, 0b111010, 0b001100, 0b110000,
                0b001101, 0b110001, 0b001111, 0b110011, 0b011111,
                0b110111, 0b001110, 0b110010, 0b000010, 0b010011,
                0b000111, 0b000000, 0b010101,
            };
            bool found = false;
            for (int i = 0; i < sizeof cinstr / sizeof cinstr[0]; i++) {
                if (strcmp(cmp, cinstr[i]) == 0) {
                    code |= ccode[i] << 6;
                    found = true;
                    break;
                }
            }
            if (!found) {
                fprintf(stderr, "Invalid instruction\n");
                exit(1);
            }
            push(code);
        }
    }
    void push(int n) {
        out.push_back(n);
    }
    void insert(int n, int pos) {
        out[pos] = n;
    }
    void hexprint(unsigned char c) {
        for (int i = 0; i < 8; i++)
            putc('0' + ((c >> (7 - i)) & 1), stdout);
    }
    std::vector<unsigned short> out;
    int var_off{};
    std::unordered_map<std::string, int> var_map;
    std::unordered_map<std::string, int> label_map;
    std::unordered_map<std::string, std::vector<int>> pending;
};

int main() {
    Assem assem;
    assem(stdin);
}

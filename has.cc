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
        int count = 0;
        while (getline(&buf, &sz, in) != -1) {
            do_line(buf, ++count);
        }
        free(buf);
        if (!pending.empty()) {
            fprintf(stderr, "Undefined labels:");
            for (auto &&[k, _] : pending)
                fprintf(stderr, " %s", k.c_str());
            fprintf(stderr, "\n");
            exit(1);
        }
        for (size_t i = 0; i < out.size(); i++) {
            hexprint(out[i]);
            fputs("\n", stdout);
        }
    }
private:
    void do_a_instr(int count, const char *tok) {
        if (isdigit(tok[0])) {
            out.push_back(atoi(tok) & 0x7fff);
        } else if (islower(tok[0])) {
            auto it = var_map.find(tok);
            if (it == var_map.end()) {
                var_map.emplace(tok, var_off);
                out.push_back(var_off);
                var_off++;
            } else {
                out.push_back(it->second);
            }
        } else if (isupper(tok[0])) {
            auto it = label_map.find(tok);
            if (it == label_map.end()) {
                pending[tok].push_back(out.size());
                out.push_back(0);
            } else {
                out.push_back(it->second);
            }
        } else {
            fprintf(stderr, "%d: %s: invalid instruction\n", count, tok-1);
            exit(1);
        }
    }
    int jmp_code(const char *jmp) {
        static const char *const jinstr[] = {
            "", "JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP",
        };
        for (int i = 1; i < 8; i++)
            if (strncmp(jmp, jinstr[i], 3) == 0)
                return i;
        return -1;
    }
    int cmp_code(const char *cmp) {
        static const char *const cinstr[] = {
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
        for (int i = 0; i < sizeof cinstr / sizeof cinstr[0]; i++)
            if (strcmp(cmp, cinstr[i]) == 0)
                return ccode[i] << 6;
        return -1;
    }
    void do_c_instr(int count, char *tok) {
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
            int jcode = jmp_code(jmp);
            if (jcode < 0) {
                fprintf(stderr, "%d: %s: invalid jump spec\n", count, jmp);
                exit(1);
            }
            code |= jcode;
        }
        if (char *is_mem = strchr(cmp, 'M')) {
            *is_mem = 'A';
            code |= 1 << 12;
        }
        // cmp is always present
        if (1) {
            int ccode = cmp_code(cmp);
            if (ccode < 0) {
                fprintf(stderr, "%d: %s: invalid comp spec\n", count, cmp);
                exit(1);
            }
            code |= ccode;
        }
        out.push_back(code);
    }
    void do_label(int count, char *tok) {
        if (!isupper(tok[0])) {
            fprintf(stderr, "%d: %s: invalid label\n", count, tok-1);
            exit(1);
        }
        char *c = strchr(tok, ')');
        if (!c) {
            fprintf(stderr, "%d: %s: unterminated label\n", count, tok-1);
            exit(1);
        }
        *c = 0;
        if (label_map.count(tok) != 0) {
            fprintf(stderr, "%d: %s: duplicate label\n", count, tok-1);
            exit(1);
        }
        label_map.emplace(tok, out.size());
        auto it = pending.find(tok);
        if (it != pending.end()) {
            for (auto pos : it->second)
                out[pos] = out.size();
            pending.erase(it);
        }
    }
    void do_line(char *buf, int count) {
        char *tok = strtok(buf, " \t\n\r");
        if (!tok) return;
        switch (tok[0]) {
        case '@':
            do_a_instr(count, tok + 1);
            break;
        case '(':
            do_label(count, tok + 1);
            break;
        case '/':
            // comment
            break;
        default:
            do_c_instr(count, tok);
        }
    }
    void hexprint(unsigned short c) {
        for (int i = 0; i < 16; i++)
            putc('0' + ((c >> (15 - i)) & 1), stdout);
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

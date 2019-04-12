#include <iostream>
#include <string>

using namespace std;

int main()
{
  string mem_a = "PLRAM[0]";
  string mem_b = "PLRAM[1]";
  string mem_c = "PLRAM[2]";
  string mem_d = "PLRAM[";
  string mem_e = "DDR[0]";
  string mem_f = "ddr1";
  string mem_g = "DDR[1]";
  string concat = "";
  auto concat1 = {mem_a, mem_b, mem_c, mem_d, mem_e, mem_f, mem_g};
  for (auto &mem : concat1) {
    cout << mem << " ";
  }
  cout << endl;

  string index = "[";
  string sep = ":";
  for (auto &mem : concat1) {
    auto found = mem.find(index);
    if (found != string::npos && found < mem.length()-1 && concat != "") {

      cout << "found :" << found << " in :" << mem.length() << endl;
      cout << "ID :" << stoi(mem.substr(found + 1, 1)) << endl;
    } else {
      concat += mem;
    }
  }
  cout << concat << endl;
}
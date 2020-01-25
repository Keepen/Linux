#include <iostream>
#include <string>
using namespace std;
class Solution {
  public:
    int StrToInt(string str) {
      if (!str.size()) {
        return 0;

      }
      int ret = 0;
      int j = 1;
      string::iterator it = str.end() - 1;
      while (it != str.begin()) {
        if (!(*it >= '0' && *it <= '9')) {
          return 0;

        }
        ret += (*it - '0') * j;
        j *= 10;
        --it;

      }
      if (*it >= '0' && *it <= '9') {
        ret += (*it - '0') * j;
        if (ret < 0) {
          return 0;

        }

      }
      else if (*it == '-') {
        if (ret < 0) {
          if (ret ==  -2147483648) {
            return ret;

          }
          return 0;

        }
        ret *= -1;

      }
      else {
        if (ret < 0) {
          return 0;

        }

      }
      return ret;

    }

};

void test() {
  string s;
  while (cin >> s) {
    Solution sl;
    cout << sl.StrToInt(s);
  }
}


int main() {
  test();
  return 0;

}

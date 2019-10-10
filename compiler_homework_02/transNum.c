
unsigned long long int transNum(char* token, int count) {
  unsigned long long int temp = 0;
  for (int i = 0; i < count; temp = temp * 10 + (token[i++] - '0')) {
  }
  return temp;
}
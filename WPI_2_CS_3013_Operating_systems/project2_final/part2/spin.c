
void spin() {
  int i;
  for (i = 0; i < 1000000000; i++);
  return;
}

int main() {
  spin();
  return 0;
}

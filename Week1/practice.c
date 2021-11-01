int foo(int i);
void var(int val);

int main(void)
{
  foo(5);
  return(0);
}

int foo(int i)
{
  var(i);
  return(0);
}

void var(int val)
{
  int i = 0;
  for(; val > 0; val--)
  {
    i += val;
  }
}

/*
    b (break) function|file.c:line
    s (step)(into function)
    n (next)(step over function)
    p (print) expression
    bt (backtrace)
*/
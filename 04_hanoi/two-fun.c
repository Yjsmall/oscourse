#include <assert.h>
#include <stdio.h>

struct Frame {
  // Each frame has a program counter to keep track its next
  // to-be-executed statement.
  int pc;
  char name;

  // The internal state of the frame. This state includes
  // both arguments and local variables (if any).
  //
  // Arguments:
  int n;

  // Local variables:
  int c1, c2;
};

typedef struct Frame Frame;

/**
 *
 * int f(int n) { return (n<=1) ? 1 : f(n-1) + g(n-2); }
 * int g(int n) { return (n<=1) ? 1 : f(n+1) + g(n-1); }
 */

int f(int n) {
  Frame stk[64];
  Frame *top = stk - 1;

// Function call: push a new frame (PC=0) onto the stack
#define call(...) ({ *(++top) = (Frame){.pc = 0, __VA_ARGS__}; })

// Function return: pop the top-most frame
#define ret(val)                                                               \
  ({                                                                           \
    top--;                                                                     \
    retval = (val);                                                            \
  })

  // The last function-return's value. It is not obvious
  // that we only need one retval.
  int retval = 0;

  // The initial call to the recursive function
  call('f', n);

  while (1) {
    // Fetch the top-most frame.
    Frame *f = top;
    if (top < stk) {
      // No top-most frame any more; we're done.
      break;
    }

    // Jumps may change this default next pc.
    int next_pc = f->pc + 1;

    // Single step execution.

    // Extract the parameters from the current frame. (It's
    // generally a bad idea to reuse variable names in
    // practice; but we did it here for readability.)
    char name = f->name;
    int n = f->n;

    switch (f->name) {
    case 'f':
      switch (f->pc) {
      case 0:
        if (n <= 1) {
          ret(1);
        }
        break;
      case 1:
        call('f', n - 1);
        break;
      case 2:
        f->c1 = retval;
        break;
      case 3:
        call('g', n - 2);
        break;
      case 4:
        f->c2 = retval;
        break;
      case 5:
        ret(f->c1 + f->c2);
        break;
      default:
        assert(0);
      }
      break;
    case 'g':
      switch (f->pc) {
      case 0:
        if (n <= 1) {
          ret(1);
        }
        break;
      case 1:
        call('f', n + 1);
        break;
      case 2:
        f->c1 = retval;
        break;
      case 3:
        call('g', n - 1);
        break;
      case 4:
        f->c2 = retval;
        break;
      case 5:
        ret(f->c1 + f->c2);
        break;
      default:
        assert(0);
      }
      break;
    }

    f->pc = next_pc;
  }

  return retval;
}

int g(int n) {
  Frame stk[64];
  Frame *top = stk - 1;

#define call(...) ({ *(++top) = (Frame){.pc = 0, __VA_ARGS__}; })

// Function return: pop the top-most frame
#define ret(val)                                                               \
  ({                                                                           \
    top--;                                                                     \
    retval = (val);                                                            \
  })

  // The last function-return's value. It is not obvious
  // that we only need one retval.
  int retval = 0;

  call('g', n);

  while (1) {
    Frame *f = top;
    if (top < stk) {
      break;
    }

    int next_pc = f->pc + 1;

    // Single step execution.

    // Extract the parameters from the current frame. (It's
    // generally a bad idea to reuse variable names in
    // practice; but we did it here for readability.)
    char name = f->name;
    int n = f->n;

    switch (f->name) {
    case 'f':
      switch (f->pc) {
      case 0:
        if (n <= 1) {
          ret(1);
        }
        break;
      case 1:
        call('f', n - 1);
        break;
      case 2:
        f->c1 = retval;
        break;
      case 3:
        call('g', n - 2);
        break;
      case 4:
        f->c2 = retval;
        break;
      case 5:
        ret(f->c1 + f->c2);
        break;
      default:
        assert(0);
      }
      break;
    case 'g':
      switch (f->pc) {
      case 0:
        if (n <= 1) {
          ret(1);
        }
        break;
      case 1:
        call('f', n + 1);
        break;
      case 2:
        f->c1 = retval;
        break;
      case 3:
        call('g', n - 1);
        break;
      case 4:
        f->c2 = retval;
        break;
      case 5:
        ret(f->c1 + f->c2);
        break;
      default:
        assert(0);
      }
      break;
    }

    f->pc = next_pc;
  }

  return retval;
}

int f_recursive(int n);
int g_recursive(int n);
int f_recursive(int n) { return (n<=1) ? 1 : f_recursive(n-1) + g_recursive(n-2); }
int g_recursive(int n) { return (n<=1) ? 1 : f_recursive(n+1) + g_recursive(n-1); }

int main() {
  printf("f-non-recursive: %d\n", f(5));
  printf("g-non-recursive: %d\n", g(5));
  printf("f-recursive: %d\n", f_recursive(5));
  printf("g-recursive: %d\n", g_recursive(5));
}
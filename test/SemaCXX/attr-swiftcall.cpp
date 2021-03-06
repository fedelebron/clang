// RUN: %clang_cc1 -triple x86_64-apple-darwin10 -fsyntax-only -verify %s

#define SWIFTCALL __attribute__((swiftcall))
#define INDIRECT_RESULT __attribute__((swift_indirect_result))
#define ERROR_RESULT __attribute__((swift_error_result))
#define CONTEXT __attribute__((swift_context))

int notAFunction SWIFTCALL; // expected-warning {{'swiftcall' only applies to function types; type here is 'int'}}
void variadic(int x, ...) SWIFTCALL; // expected-error {{variadic function cannot use swiftcall calling convention}}
void multiple_ccs(int x) SWIFTCALL __attribute__((vectorcall)); // expected-error {{vectorcall and swiftcall attributes are not compatible}}
void (*functionPointer)(void) SWIFTCALL;

void indirect_result_nonswift(INDIRECT_RESULT void *out); // expected-error {{'swift_indirect_result' parameter can only be used with swiftcall calling convention}}
void indirect_result_bad_position(int first, INDIRECT_RESULT void *out) SWIFTCALL; // expected-error {{'swift_indirect_result' parameters must be first parameters of function}}
void indirect_result_bad_type(INDIRECT_RESULT int out) SWIFTCALL; // expected-error {{'swift_indirect_result' parameter must have pointer type; type here is 'int'}}
void indirect_result_single(INDIRECT_RESULT void *out) SWIFTCALL;
void indirect_result_multiple(INDIRECT_RESULT void *out1, INDIRECT_RESULT void *out2) SWIFTCALL;

void error_result_nonswift(ERROR_RESULT void **error); // expected-error {{'swift_error_result' parameter can only be used with swiftcall calling convention}} expected-error{{'swift_error_result' parameter must follow 'swift_context' parameter}}
void error_result_bad_position(ERROR_RESULT void **error, int last) SWIFTCALL; // expected-error {{'swift_error_result' parameter must be last parameter of function}}
void error_result_bad_position2(int first, ERROR_RESULT void **error) SWIFTCALL; // expected-error {{'swift_error_result' parameter must follow 'swift_context' parameter}}
void error_result_bad_type(CONTEXT void *context, ERROR_RESULT int error) SWIFTCALL; // expected-error {{'swift_error_result' parameter must have pointer to unqualified pointer type; type here is 'int'}}
void error_result_bad_type2(CONTEXT void *context, ERROR_RESULT int *error) SWIFTCALL; // expected-error {{'swift_error_result' parameter must have pointer to unqualified pointer type; type here is 'int *'}}
void error_result_okay(int a, int b, CONTEXT void *context, ERROR_RESULT void **error) SWIFTCALL;

void context_nonswift(CONTEXT void *context); // expected-error {{'swift_context' parameter can only be used with swiftcall calling convention}}
void context_bad_position(CONTEXT void *context, int x) SWIFTCALL; // expected-error {{'swift_context' parameter can only be followed by 'swift_error_result' parameter}}
void context_bad_type(CONTEXT int context) SWIFTCALL; // expected-error {{'swift_context' parameter must have pointer type; type here is 'int'}}
void context_okay(CONTEXT void *context) SWIFTCALL;

template <class T> void indirect_result_temp_okay1(INDIRECT_RESULT T *out) SWIFTCALL;
template <class T> void indirect_result_temp_okay2(INDIRECT_RESULT T out) SWIFTCALL; // expected-note {{candidate template ignored: substitution failure [with T = int]: 'swift_indirect_result' parameter must have pointer type; type here is 'int'}}
void test_indirect_result_temp(void *out) {
  indirect_result_temp_okay1(out);
  indirect_result_temp_okay2(out);
  indirect_result_temp_okay2(1); // expected-error {{no matching function for call to 'indirect_result_temp_okay2'}}
}

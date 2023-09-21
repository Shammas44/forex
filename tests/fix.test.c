#include <criterion/criterion.h>
#include <stdio.h>
#include <string.h>
#include "fix.h"

int truc(char *str){
  size_t length = 0;
  while (str[length] != '\0') {
    length++;
  }
  return length;
}

Test(get_checksum, is_defined){
  get_checksum("");
}

Test(get_checksum, return_ascii_code_from_A){
  int res = get_checksum("A");
  cr_assert_eq(65,res);
}

Test(get_checksum, return_ascii_code_from_empty_string){
  int res = get_checksum("");
  cr_assert_eq(0,res);
}

Test(get_checksum, return_ascii_code_from_space){
  int res = get_checksum(" ");
  cr_assert_eq(32,res);
}

Test(get_checksum, return_ascii_code_from_AAA){
  int res = get_checksum("AAA");
  cr_assert_eq(65*3,res);
}

Test(get_checksum, return_modulo_256_from_AAAA_ascii_code){
  int res = get_checksum("AAAA");
  cr_assert_eq((65*4)%256,res);
}

#define  EXCEPTIONS {8, 9, 10}
#define  EXCEPTIONS_SIZE 3

Test(in_exception, is_an_exception){
  int exceptions[] = EXCEPTIONS;
  int is_defined = in_exceptions(8, exceptions, EXCEPTIONS_SIZE);
  cr_assert_eq(true,is_defined);
}

Test(in_exception, is_not_exception){
  int exceptions[] = EXCEPTIONS;
  int is_defined = in_exceptions(7, exceptions, EXCEPTIONS_SIZE);
  cr_assert_neq(true,is_defined);
}

Test(in_exception, is_not_exception_0){
  int exceptions[] = EXCEPTIONS;
  int is_defined = in_exceptions(0, exceptions, EXCEPTIONS_SIZE);
  cr_assert_eq(false,is_defined);
}

Test(in_exception, is_exception_0){
  int exceptions[] = {0, 8, 9, 10};
  int is_defined = in_exceptions(0, exceptions, EXCEPTIONS_SIZE);
  cr_assert_eq(true,is_defined);
}

Test(compose_message, except_message){
  const int exceptions[] = EXCEPTIONS;
  PairKeyValue dic[] = {
    {msg_type, "logon"}
  };
  char* msg = compose_message(dic,1,exceptions, 3);
  cr_assert_str_eq("8=FIX.4.4\0019=9\00110=209\00135=logon",msg);
  free(msg);
}

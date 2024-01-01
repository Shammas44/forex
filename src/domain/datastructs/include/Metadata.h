#ifndef METADATA_H
#define METADATA_H
#include <pthread.h>
#include <stdio.h>
#define T Metadata

typedef struct T T;

typedef void (Metadata_destructor)(T *self);
typedef void (Metadata_set_equity)(T *self, double current_price);
typedef double (Metadata_get_equity)(T *self);
typedef void (Metadata_set_leverage)(T *self, int leverage);
typedef int (Metadata_get_leverage)(T *self);
typedef double (Metadata_get_available_funds)(T *self);
typedef void (Metadata_set_capital)(T *self, double capital);
typedef double (Metadata_get_capital)(T *self);
typedef double (Metadata_get_market_position)(T *self);
typedef void (Metadata_set_market_position)(T *self, int order_size);
typedef void (Metadata_set_number_of_trades)(T *self, int number);
typedef double (Metadata_get_number_of_trades)(T *self);
typedef void (Metadata_set_last_price)(T *self, double last_price);
typedef double (Metadata_get_last_price)(T *self);
typedef void (Metadata_set_spread)(T *self, double spread);
typedef double (Metadata_get_spread)(T *self);
typedef void (Metadata_set_broker_commision_dpm)(T *self, double dpm);
typedef double (Metadata_get_broker_commision_dpm)(T *self);

typedef struct T {
Metadata_destructor *destructor;
Metadata_set_equity *set_equity;
Metadata_get_equity *get_equity;
Metadata_set_leverage *set_leverage;
Metadata_get_leverage *get_leverage;
Metadata_get_available_funds *get_available_funds;
Metadata_set_capital *set_capital;
Metadata_get_capital *get_capital;
Metadata_get_market_position *get_market_position;
Metadata_set_market_position *set_market_position;
Metadata_set_number_of_trades *set_number_of_trades;
Metadata_get_number_of_trades *get_number_of_trades;
Metadata_set_last_price *set_last_price;
Metadata_get_last_price *get_last_price;
Metadata_set_spread *set_spread;
Metadata_get_spread *get_spread;
Metadata_set_broker_commision_dpm *set_broker_commision_dpm;
Metadata_get_broker_commision_dpm *get_broker_commision_dpm;
void *__private;
} T;

T* metadata_constructor();

#undef T
#endif
